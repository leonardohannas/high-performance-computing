# Comparacao de Implementacao: Sequencial vs Paralela

Este documento resume as decisoes de engenharia adotadas no projeto, com foco em desempenho, diferencas de abordagem e ganhos praticos.

## Objetivo

Avaliar uma pipeline de agregacao estatistica em quatro niveis:

1. Aluno
2. Cidade
3. Regiao
4. Brasil

A medicao de tempo segue o requisito de considerar apenas a fase de computacao (sem E/S).

## Base Comum Entre as Versoes

Ambas as versoes utilizam:

1. O mesmo modulo de estatistica em [src/stats.c](../src/stats.c)
2. O mesmo tipo `Stats` em [src/stats.h](../src/stats.h)
3. O mesmo modulo de tempo/impressao em [src/utils.c](../src/utils.c)
4. A mesma saida tabular formatada

## Evolucao Tecnica Implementada

### 1. Buffer temporario controlado pelo chamador

O buffer passou a ser alocado fora de `calculate_stats` para evitar custo repetido de alocacao durante cada calculo e para deixar o fluxo de memoria previsivel.

Com isso:

1. Cada thread pode reutilizar o mesmo buffer ao longo de varias chamadas.
2. Evita disputa no alocador global em execucao paralela.
3. Melhora localidade e reduz overhead de gestao de heap.

Assinatura atual:

`void calculate_stats(const float *restrict data, int n, Stats *restrict out_stats, float *restrict temp_buffer);`

### 2. Melhorias de vetorizacao (SIMD)

Foi adotada a estrategia de facilitar auto-vetorizacao do compilador:

1. `restrict` na API de estatistica para reduzir aliasing ambiguo.
2. `#pragma GCC ivdep` no loop de copia para `temp_buffer`.
3. Build com `-O3` (configuravel no Makefile).

Importante:

1. Nao foram usados intrinsics manuais (AVX/SSE/NEON explicitos).
2. A abordagem usa auto-vetorizacao orientada por compilador.

### 3. Paralelizacao com OpenMP em [src/studentspar.c](../src/studentspar.c)

A versao paralela agora usa OpenMP em fases independentes:

1. Preenchimento da matriz
2. Agregacao por aluno
3. Agregacao por cidade
4. Agregacao por regiao

A agregacao Brasil permanece sequencial por custo/beneficio (overhead de fork/join nao compensa).

Se uma alocacao de buffer local falhar dentro de uma regiao paralela, o programa aborta imediatamente. Isso evita violar a semantica do `omp for` e elimina risco de deadlock por threads que entram em caminhos diferentes dentro da mesma equipe.

### 4. Geração de notas thread-safe

Na versao paralela:

1. `rand_r` substitui `rand` para evitar lock global.
2. Cada iteracao usa seed local derivada (`seed + i`).

Consequencia:

1. A sequencia numerica pode diferir da versao sequencial, mas sem race conditions.

### 5. Layout de memoria otimizado na versao paralela

A matriz de notas da versao paralela foi convertida para bloco 1D contiguo:

1. Melhor localidade de cache
2. Menos indirecoes de ponteiro
3. Melhor oportunidade de prefetch e vetorizacao do compilador

### 6. Buffers locais por thread

Nas regioes paralelas de agregacao:

1. Cada thread aloca buffer temporario apenas uma vez por fase.
2. Evita `malloc/free` por elemento processado.
3. Reduz pressao no heap em cargas altas.
4. Se a alocacao falhar, o processo aborta para manter a execucao consistente.

### 7. Build portavel e orientado a desempenho

No [src/Makefile](../src/Makefile):

1. `OPTFLAGS ?= -O3`
2. `NATIVEFLAGS ?=` para tuning opcional (ex.: `-march=native`)
3. `-lrt` aplicado apenas em Linux
4. `-fopenmp` aplicado no alvo paralelo

## Diferencas Principais Entre Sequencial e Paralelo

## Sequencial ([src/studentsseq.c](../src/studentsseq.c))

1. Fluxo linear (um nucleo)
2. Matriz alocada como bloco 1D contiguo, igual a versao paralela
3. Buffers de mediana reutilizados por fase
4. Geração de notas sequencial sem concorrencia

## Paralelo ([src/studentspar.c](../src/studentspar.c))

1. OpenMP com paralelismo por fase
2. Matriz contigua 1D
3. `rand_r` com estado local por iteracao
4. Buffer temporario por thread em cada fase
5. Falha imediata em caso de erro de alocacao dentro das regioes paralelas

## Impacto Esperado em Desempenho

Para tamanhos grandes de entrada (N alto):

1. Reducao de tempo total na versao paralela
2. Menor overhead de alocacao por uso de buffers reutilizaveis no chamador
3. Melhor uso de cache no caminho paralelo
4. Ganho adicional por auto-vetorizacao no compilador

Como o baseline sequencial tambem usa matriz 1D contigua, a comparacao de speedup fica mais justa e reflete melhor o ganho do paralelismo, nao de um layout de memoria diferente.

## Como Reproduzir Benchmark

No diretorio [src](../src):

1. `make clean`
2. `make build`
3. `make run-seq`
4. `make run-par`

Comparar a ultima linha de cada execucao:

`Tempo de resposta em segundos, sem considerar E/S: ...`

## Tuning Opcional

Para teste local em maquina alvo, habilitar arquitetura nativa:

1. `make clean`
2. `make NATIVEFLAGS='-march=native' build`

Isso pode aumentar desempenho, mas reduz portabilidade binaria entre CPUs diferentes.
