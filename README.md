# High Performance Computing

Repositório para trabalhos e projetos da disciplina de Computação de Alto Desempenho.

## Estrutura

- `tb1/`: arquivos do Primeiro Trabalho Prático
- `src/`: códigos fonte em C
- `docs/`: enunciado, apêndices e relatório em PDF
- `tests/`: arquivos de teste
- `scripts/`: scripts auxiliares
- `submission/`: arquivos finais para entrega

## Sobre o arquivo `package_submission.sh`

O arquivo `package_submission.sh` é um script de shell usado para automatizar a preparação da entrega final do trabalho.

O nome significa:

- `package`: empacotar / organizar os arquivos
- `submission`: submissão / entrega
- `.sh`: script de shell executado no terminal

Na prática, esse script pode:

1. copiar os arquivos exigidos pelo professor, como `studentsseq.c`, `studentspar.c` e `respostas.pdf`
2. colocá-los em uma pasta final de entrega
3. gerar um arquivo `.zip` pronto para submissão no e-disciplinas

Isso ajuda a evitar erros, como esquecer arquivos, usar nomes incorretos ou gerar um `.zip` com conteúdo desnecessário.

## Exemplo

Se os arquivos do trabalho estiverem organizados assim:

- `src/studentsseq.c`
- `src/studentspar.c`
- `docs/respostas.pdf`

o script pode copiá-los para a pasta `submission/` e gerar algo como:

- `submission/studentsseq.c`
- `submission/studentspar.c`
- `submission/respostas.pdf`
- `submission/tb1_entrega.zip`

## Observação

O uso desse script não é obrigatório. Ele existe apenas para facilitar a organização e a geração da entrega final.