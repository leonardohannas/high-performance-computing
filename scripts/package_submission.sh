
## `scripts/package_submission.sh`

```bash
#!/bin/bash

set -e

REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SUBMISSION_DIR="$REPO_ROOT/submission"
ZIP_NAME="tb1_entrega.zip"

SEQ_FILE="$REPO_ROOT/src/studentsseq.c"
PAR_FILE="$REPO_ROOT/src/studentspar.c"
REPORT_FILE="$REPO_ROOT/docs/respostas.pdf"

echo "Preparando arquivos para submissão..."

if [ ! -f "$SEQ_FILE" ]; then
    echo "Erro: arquivo não encontrado: $SEQ_FILE"
    exit 1
fi

if [ ! -f "$PAR_FILE" ]; then
    echo "Erro: arquivo não encontrado: $PAR_FILE"
    exit 1
fi

if [ ! -f "$REPORT_FILE" ]; then
    echo "Erro: arquivo não encontrado: $REPORT_FILE"
    echo "Certifique-se de que o relatório final exista com o nome docs/respostas.pdf"
    exit 1
fi

mkdir -p "$SUBMISSION_DIR"

rm -f "$SUBMISSION_DIR/studentsseq.c"
rm -f "$SUBMISSION_DIR/studentspar.c"
rm -f "$SUBMISSION_DIR/respostas.pdf"
rm -f "$SUBMISSION_DIR/$ZIP_NAME"

cp "$SEQ_FILE" "$SUBMISSION_DIR/studentsseq.c"
cp "$PAR_FILE" "$SUBMISSION_DIR/studentspar.c"
cp "$REPORT_FILE" "$SUBMISSION_DIR/respostas.pdf"

cd "$SUBMISSION_DIR"
zip -r "$ZIP_NAME" studentsseq.c studentspar.c respostas.pdf

echo "Submissão gerada com sucesso em:"
echo "$SUBMISSION_DIR/$ZIP_NAME"