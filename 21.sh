#! /usr/bin/bash

[ $# -eq 3 ] || exit 1
[ -f $1 ] || exit 2
[ -n $2 ] || exit 3
[ -n $3 ] || exit 4

FILE=$1
KEY1=$2
KEY2=$3

VAL1=$( grep "^$KEY1=" $FILE | cut -d "=" -f 2 )
VAL2=$( grep "^$KEY2=" $FILE | cut -d "=" -f 2 )

CHECK=$( grep "$KEY2=" $FILE) 

if [ -z "$CHECK" ]; then 
	echo "$KEY2=" >> $FILE
	echo grep works
	exit 0
fi

NEWLINE=$( grep -v -f \
	<(echo $VAL1 | xargs | tr " " "\n" | sort) \
	<(echo $VAL2 | xargs | tr " " "\n" | sort) \
	| xargs )

echo $VAL1
echo "$VAL2"

#$( comm -13 \
#		<(echo $VAL1 | xargs | tr " " "\n" | sort) \
#		<(echo $VAL2 | xargs | tr " " "\n" | sort) \
#		| xargs ) 

sed -i -e "s/^$KEY2=$VAL2/$KEY2=$NEWLINE/" $FILE


