#!/bin/bash
if [ $# -ne 2 ]
then
	echo "Błedna liczba parametrów"
	echo "$0 katalog1 katalog2"
	exit 1
fi

if [ ! -d $1 ]
then
	echo "$1 nie jest katalogiem"
	exit 1
fi

if [ ! -d $2 ]
then
	echo "$2 nie jest katalogiem"
	exit 1
fi

cd $1
list1=`ls $1`
list2=`ls $2`

for file1 in $list1
do
	for file2 in $list2
	do
		if [[ $file1 == $file2 ]]
		then
			echo "$file1 jest taki sam"
			rm $file1
		fi
	done

done



