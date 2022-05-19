#!/bin/bash
for f in $@;
do
    iconv -f utf-16 -t utf-8 -o out $f && mv out $f
done
