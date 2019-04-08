#!/bin/bash

find ./testy2 \( -name "*jpg" -or -name "*png" \) -size +1k | while read plik
do
    echo `realpath $plik` | tr "\/" "\\\\"
done
