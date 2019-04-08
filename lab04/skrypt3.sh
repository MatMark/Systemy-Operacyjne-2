#!/bin/bash

find ./testy3/A/ -type f -mmin +1 | while read plik
do
    cp $plik ./testy3/B/
done
