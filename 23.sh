#!/usr/bin/bash

T2=$( mktemp )

while read d n; do
		find $d \
			-maxdepth 1 \
			-mindepth 1 \
			-type f \
			-printf "%T@ %f $n\n" 2>/dev/null \
			| sort -n -k 1,1 | tail -n 1 >> $T2
done < <(cat /etc/passwd \
		| awk -F ':' '{print $6" "$5}' \
		| cut -d "," -f 1)

#for dir in $( cat T | awk '{print $1}' ); do
#		find $dir \
#			-maxdepth 1 \
#			-mindepth 1 \
#			-type f \
#			-printf "%T@ %p %u\n" 2>/dev/null \
#			| sort -n -k 1,1 | tail -n 1
#done >> $T2

cat $T2 | sort -n -k 1,1 | tail -n 1 | cut -d " " -f 2,3,4 

rm -- $T2
