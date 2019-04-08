#!/bin/bash
if [ $# -ne 2 ]
then
	echo "Błędna liczba parametrów"
	echo "$0 katalog rozszerzenie"
	exit 1
fi

if [ -d $1 ]
then
	cd $1
	lista=`pwd`
	for file in `ls $lista`
	do
		if [[ $file == *.$2 ]]
		then
			echo "$file: `cat $file`" >> wynik
		fi
	done
else
	echo "Parametr 1 nie jest katalogiem"
fi
