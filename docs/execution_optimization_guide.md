# Guia de Execucao e Otimizacao

Este documento descreve como executar cada variante do projeto e quando usar cada uma, com foco em comparacao justa entre sequencial e paralelo.

## Objetivo do guia

1. Explicar qual versao executar em cada contexto.
2. Explicar por que uma configuracao pode ser melhor que a outra.
3. Evitar excesso de boilerplate de benchmark.

## Versoes disponiveis

## 1) Versao Sequencial

Arquivo principal: [src/studentsseq.c](../src/studentsseq.c)

Quando usar:

1. Validacao funcional e depuracao.
2. Baseline de desempenho para calculo de speedup.
3. Execucao em ambientes sem OpenMP.

Por que ela e importante:

1. Representa o custo de computacao em um unico nucleo.
2. Usa o mesmo layout 1D contiguo da versao paralela, mantendo a comparacao justa.
3. Evita distorcer speedup com diferencas de estrutura de dados.
4. Mantem legibilidade alta com alocacao simples por `malloc`.

## 2) Versao Paralela (OpenMP)

Arquivo principal: [src/studentspar.c](../src/studentspar.c)

Quando usar:

1. Execucao de producao para entradas grandes.
2. Medicao de ganho real com multiplos nucleos.
3. Avaliacao de escalabilidade por numero de threads (`T` do arquivo de entrada).

Por que ela tende a ser melhor:

1. Divide as fases de agregacao entre threads.
2. Usa matriz 1D contigua para melhor localidade de cache.
3. Usa `rand_r` para geracao thread-safe.
4. Reutiliza buffers temporarios por thread para reduzir overhead de heap.
5. Preserva legibilidade mesmo com otimizações (sem aritmetica de padding em alocacao).

## Modos de otimizacao (casos de execucao)

## Comandos essenciais

Executar a partir de [src](../src).

1. Build padrao (configuracao atual do Makefile):
`make clean && make build`
2. Rodar apenas sequencial:
`make run-seq`
3. Rodar apenas paralelo:
`make run-par`

## Comandos por caso

### Caso A: Portavel

1. Compilar sem otimização nativa especifica da CPU:
`make clean && make NATIVEFLAGS= build`
2. Rodar sequencial (baseline):
`./studentsseq`
3. Rodar paralelo:
`./studentspar`

### Caso B: Nativo da maquina

1. Compilar com tuning nativo:
`make clean && make NATIVEFLAGS='-march=native -ffast-math' build`
2. Rodar sequencial:
`./studentsseq`
3. Rodar paralelo:
`./studentspar`

### Caso C: Paralelo com afinidade de threads

1. Compilar (portavel ou nativo):
`make clean && make build`
2. Rodar paralelo com afinidade em nucleos:
`OMP_PROC_BIND=true OMP_PLACES=cores ./studentspar`
3. Opcionalmente forcar numero de threads em runtime (sobrescreve configuracao de ambiente quando aplicavel):
`OMP_NUM_THREADS=4 OMP_PROC_BIND=true OMP_PLACES=cores ./studentspar`

## Caso A: Portavel

Perfil:

1. Compilacao otimizada sem depender de recursos especificos de uma CPU.
2. Melhor para reprodutibilidade entre maquinas diferentes.

Quando priorizar:

1. Entrega academica em ambientes heterogeneos.
2. Comparacoes entre computadores diferentes.

Trade-off:

1. Menor desempenho absoluto que o modo nativo.

## Caso B: Nativo da maquina

Perfil:

1. Compilacao com otimizações especificas da CPU local.
2. Pode ativar vetorizacao mais agressiva e instrucoes de hardware (por exemplo FMA).

Quando priorizar:

1. Benchmark final em uma maquina fixa.
2. Busca por menor tempo de resposta absoluto.

Trade-off:

1. Binario menos portavel para outras arquiteturas.

## Caso C: Paralelo com afinidade de threads

Perfil:

1. Mantem threads presas a nucleos para reduzir migracao entre cores.
2. Ajuda a preservar cache local durante a execucao paralela.

Quando priorizar:

1. Entradas muito grandes.
2. Benchmark de menor variacao entre repeticoes.

Trade-off:

1. Pode reduzir flexibilidade do agendador do sistema operacional.

## Qual e melhor: sequencial ou paralelo?

Depende do objetivo:

1. Corretude e baseline: sequencial.
2. Menor tempo para cargas grandes: paralelo.
3. Comparacao de speedup confiavel: usar as duas versoes com o mesmo conjunto de entrada e o mesmo criterio de medicao (sem E/S).

## Leitura correta dos resultados

1. Compare sempre o tempo reportado na ultima linha de saida.
2. Use a mesma entrada (`R`, `C`, `A`, `N`, `T`, `seed`) para comparar cenarios.
3. Para relatorio, reporte pelo menos mediana de varias repeticoes por caso.

## Resumo pratico

1. Para validar logica: execute a versao sequencial.
2. Para desempenho: execute a versao paralela.
3. Para maior desempenho local: habilite modo nativo e afinidade de threads.
4. Para comparacao academica justa: mantenha mesma entrada e mesma metodologia de medicao em todos os casos.
