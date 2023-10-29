#!/bin/bash

[ $# -eq 3 ] || exit 1

[ "$(whoami)" == "root" ] || exit 2

SRC=$1
DEST=$2
STR=$3
while read dir name; do
	echo "$name"
	if [ -f $dir/$name ]; then
		P=$( echo $dir | sed -e "s,$SRC,$DEST," )
		echo "P is $P"
		if [ ! -d $P ]; then
			mkdir $P
		fi
		cp $dir/$name $P/$name
	fi
done < <(find $SRC -type f -printf "%h %f\n" | egrep "$STR" ) 
