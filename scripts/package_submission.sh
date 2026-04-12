#!/bin/bash

set -e

REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SUBMISSION_DIR="$REPO_ROOT/submission"
ZIP_NAME="tb1_entrega.zip"

mkdir -p "$SUBMISSION_DIR"
rm -f "$SUBMISSION_DIR"/*

cp "$REPO_ROOT/src/studentsseq.c" "$SUBMISSION_DIR/studentsseq.c"
cp "$REPO_ROOT/src/studentspar.c" "$SUBMISSION_DIR/studentspar.c"
cp "$REPO_ROOT/docs/respostas.pdf" "$SUBMISSION_DIR/respostas.pdf"

cd "$SUBMISSION_DIR"
zip -r "$ZIP_NAME" studentsseq.c studentspar.c respostas.pdf

echo "Entrega gerada em: $SUBMISSION_DIR/$ZIP_NAME"