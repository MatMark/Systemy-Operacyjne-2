#!/bin/bash

find ./testy4 -maxdepth 2 \( -name "*.txt" -or -name "*.dat" \) | while read plik
do
   echo `realpath $plik` | tee -a ./testy4/result
done
