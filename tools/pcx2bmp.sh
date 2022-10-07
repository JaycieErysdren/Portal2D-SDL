#!/bin/bash
for FILE in *.pcx
do
    convert ${FILE%%.*}.pcx ${FILE%%.*}.bmp
done
