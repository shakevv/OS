#!/bin/bash

if [ $# -lt 1 ] || [ $# -gt 2 ]; then
	exit 1
fi

DIR=$1
N=""

if [ ! -z $2 ]; then
	N=$2
fi

if [ ! -z "$N" ]; then 
	while read name hard; do
		if [ $hard -ge $N ]; 
			echo $name
		fi
	done < <( find $DIR -type f -printf "%p %n\n" ) 
else
	while read name;do
		echo $name
	done < <( find -L $DIR -type l -printf "%p\n" )

