#!/bin/bash

if [ $# -ne 2 ]; then
	echo "invalid number of args"
	exit 1
fi

file=$1
dir=$2
dictfile="${dir}/dict.txt"
touch $dictfile

if [ $? -ne 0 ]; then
	echo "err"
	exit 2
fi

cat $file \
	| tr -s ' ' \
	| sort -k 1,2 \
	| awk '{print $1" "$2}' \
	| uniq \
	| awk '{print NR" "$0}' \
	> $dictfile

while read number fname lname; do
	newfile="${dir}/${number}"
	touch $newfile
	egrep "^${fname}(\s)+${lname}" $file > $newfile
done < <( cat $dictfile )

exit 0
