#!/bin/bash

BASE="guiao-2"
current_dir=$(basename "`pwd`")
if [ "$current_dir" != "$BASE" ]; then
    echo "Pasta base diferente de \"$BASE\""
    exit 1
fi

DIRS="docs entrada include obj src"
for d in $DIRS; do
    if ! test -d "$d"; then
        echo "Falta a pasta \"$d\""
        exit 1
    fi
done

FILES="Makefile";
for f in $FILES; do
    if ! test -f "$f"; then
        echo "Falta o ficheiro \"$f\""
        exit 1
    fi
done

if test -z `find . -name relatorio.pdf -o -name relatorio-2.pdf`; then
    echo "Falta o ficheiro \"relatorio-2.pdf\""
    exit 1
fi
