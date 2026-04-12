# High Performance Computing

Repositório para o trabalho prático da disciplina de Computação de Alto Desempenho.

## Estrutura do projeto

```text
high-performance-computing/
├── .vscode/
├── docs/
│   ├── apendice_a.pdf
│   ├── apendice_b_modelo_relatorio.pdf
│   ├── enunciado.pdf
│   └── exemplo_entrada_0.txt
├── scripts/
│   └── package_submission.sh
├── src/
│   ├── studentspar.c
│   └── studentsseq.c
├── submission/
├── tests/
├── .gitignore
└── README.md
```

## Descrição das pastas

- `src/`: contém os arquivos-fonte em C do trabalho
  - `studentsseq.c`: implementação sequencial
  - `studentspar.c`: implementação paralela

- `docs/`: contém os materiais de apoio e referência do trabalho
  - `enunciado.pdf`: enunciado principal
  - `apendice_a.pdf`: material complementar do Apêndice A
  - `apendice_b_modelo_relatorio.pdf`: modelo esperado para o relatório
  - `exemplo_entrada_0.txt`: arquivo de exemplo de entrada

- `scripts/`: contém scripts auxiliares do projeto
  - `package_submission.sh`: script para organizar os arquivos exigidos na entrega final e gerar o `.zip`

- `submission/`: contém os arquivos finais preparados para submissão no e-disciplinas

- `tests/`: espaço reservado para testes adicionais, arquivos de entrada e validações

## Sobre o arquivo `package_submission.sh`

O arquivo `package_submission.sh` é um script de shell usado para automatizar a preparação da entrega final do trabalho.

O nome significa:

- `package`: empacotar ou organizar os arquivos
- `submission`: submissão ou entrega
- `.sh`: script de shell executado no terminal

Na prática, esse script:

1. copia os arquivos exigidos pelo professor para a pasta de entrega
2. organiza esses arquivos com os nomes esperados
3. gera um arquivo `.zip` pronto para submissão

Isso ajuda a evitar erros como:

- esquecer arquivos na entrega
- usar nomes incorretos
- zipar arquivos desnecessários
- montar a submissão manualmente toda vez

## Arquivos esperados para a entrega

De acordo com o enunciado, a entrega final deve conter:

- `studentsseq.c`
- `studentspar.c`
- `respostas.pdf`

Esses arquivos devem ser compactados em um único arquivo `.zip`.

## Como usar o script de entrega

Primeiro, certifique-se de que o relatório final esteja salvo como:

`docs/respostas.pdf`

Depois, no terminal, a partir da raiz do repositório, execute:

```bash
chmod +x scripts/package_submission.sh
./scripts/package_submission.sh