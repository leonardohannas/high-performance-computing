# Relatório de Execução e Otimização - Avaliação de Estudantes (TB1)

## 1. Configuração do Experimento

Os testes de desempenho foram conduzidos para avaliar o impacto da paralelização via OpenMP e otimizações de arquitetura no tempo de execução da pipeline de agregação estatística. A medição de tempo isolou estritamente o processamento matemático (excluindo E/S).

**Carga de Trabalho (Entrada):**
* **Regiões (R):** 3
* **Cidades por Região (C):** 4
* **Alunos por Cidade (A):** 6
* **Notas por Aluno (N):** 1.000.000
* **Total de Notas Processadas:** 72.000.000 de pontos de dados de ponto flutuante.
* **Número de Threads (T):** 4

**Otimizações de Compilação Aplicadas:**
O código foi compilado utilizando o GCC (v11.4.0) com as seguintes *flags* agressivas de otimização:
`gcc -Wall -O3 -march=native -ffast-math`
* O `-O3` ativou a auto-vetorização (SIMD).
* O `-march=native` e `-ffast-math` permitiram à CPU utilizar instruções de hardware dedicadas, como FMA (*Fused Multiply-Add*), reduzindo ciclos de clock nas operações da variância de Welford.

---

## 2. Resultados de Desempenho

Foram executados diferentes cenários de escalonamento para a versão paralela, a fim de comparar o gerenciador dinâmico do sistema operacional contra a fixação estática de *threads* (Thread Affinity) do OpenMP.

| Implementação | Parâmetros de Escalonamento (OpenMP) | Tempo de Resposta (s) |
| :--- | :--- | :--- |
| **Sequencial** | N/A (Média de 2 execuções: 20.599s e 20.983s) | **20.791 s** |
| **Paralela** | Padrão (Escalonador do Sistema Operacional) | **8.323 s** |
| **Paralela** | `OMP_PROC_BIND=true OMP_PLACES=cores` | **11.063 s** |
| **Paralela** | `OMP_PROC_BIND=spread OMP_PLACES=threads`| **11.158 s** |

---

## 3. Análise de Métricas (Speedup e Eficiência)

As métricas foram calculadas utilizando o melhor tempo da versão paralela (8.323s) contra a média da versão sequencial altamente otimizada (20.791s).

* **Speedup ($S$):** $S = \frac{T_{seq}}{T_{par}} = \frac{20.791}{8.323} \approx$ **2,50x**
* **Eficiência ($E$):** $E = \frac{S}{T} = \frac{2,50}{4} \approx$ **0,625 (62,5%)**

**Interpretação:** Com 4 *threads*, a aplicação rodou 2,5 vezes mais rápido. Uma eficiência de 62,5% em um problema de 72 milhões de *floats* é um resultado altamente positivo. A eficiência não atinge 100% (Speedup de 4x) devido à Lei de Amdahl: há *overheads* inerentes à criação das *threads*, sincronização implícita nas barreiras dos laços `for` e, primariamente, a saturação da largura de banda da memória (Memory Wall), visto que as Unidades Lógicas Aritméticas (ALUs) processam os dados mais rápido do que a RAM consegue fornecê-los.

---

## 4. Discussão Arquitetural e o Paradoxo da Afinidade de Threads

Uma observação contraintuitiva revelada por este experimento é que a **aplicação de regras rígidas de afinidade de threads (Pinning) degradou o desempenho**, aumentando o tempo de resposta de ~8.3s para ~11.1s. 

Teoricamente, `OMP_PLACES=cores` deveria melhorar a localidade do cache L1/L2, evitando a migração de contexto. No entanto, na prática, o escalonador dinâmico do Sistema Operacional (Linux) superou a configuração estática. Isso ocorreu por dois motivos arquiteturais:

1. **Gestão do "Memory Wall":** Com as diretivas SIMD e o layout 1D contíguo aplicado no código em C, os núcleos da CPU processam blocos de memória de forma muito rápida. Quando forçamos o *pinning* estático em 4 núcleos físicos distintos, todas as *threads* podem colidir no controlador de memória simultaneamente exigindo novos blocos da RAM. O escalonamento dinâmico do SO introduz micro-atrasos e flutuações naturais que acabam dessincronizando levemente as *threads*, aliviando o tráfego no barramento de memória (Memory Bus).
2. **Topologia Heterogênea Modernizada:** Em CPUs modernas (com P-Cores de alta performance e E-Cores de eficiência, ou SMT/Hyperthreading complexos), o OpenMP pode mapear estaticamente fatias grandes do vetor para *threads* lógicas ou núcleos de menor clock. O escalonador do kernel Linux possui heurísticas mais avançadas em tempo real para alocar cargas pesadas baseadas em instruções SIMD para os núcleos físicos mais rápidos e ociosos no exato momento da execução.

## 5. Conclusão

A implementação atingiu o seu objetivo com sucesso. A combinação de otimizações sequenciais profundas (matrizes em layout 1D contíguo, uso de buffers reutilizáveis e redução de ramificações no cálculo de Mínimo/Máximo) e paralelizáveis (Welford *thread-safe* de passagem única, buffers locais no *stack* em vez de disputas no *heap*) gerou uma base sólida. 

A paralelização em OpenMP provou ser extremamente eficaz (Speedup de 2,5x), sendo demonstrado que, para este perfil de acesso à memória altamente contíguo e agressivo, delegar o escalonamento ao sistema operacional rende resultados superiores à gestão manual de afinidade do OpenMP.