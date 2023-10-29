#!/usr/bin/bash

[ $# -ge 2 ] || exit 1
S=0
CMD=$( mktemp )
NAME=""
OPTS=$( mktemp )
ARGS=$( mktemp )
DPROPS=$( mktemp )

while $# -gt 0; do
	if [ "$1" == "-jar" ]; then
		S=1
		shift
		continue
	fi

	if [ -f $1 ] && [ $S -eq 1 ]; then
		S=2
		NAME=$1
		shift
		continue
	fi

	if [ -f $1 ] && [ $S -eq 0 ]; then
		echo ERR
		exit 2
	fi

	STATUS=$(echo $1 | egrep -v "^-D+" )
	if [ S -le 1 ] && [ -n STATUS ]; then
		"$1 " >> OPTS
	fi

	STATUS2=$( echo $1 | egrep "^-D+" )
	if [ S -eq 1 ] && [ -n STATUS2 ]; then
		"$1 " >> DPROPS
	fi

	if [ S -eq 2 ]; then
		"$1 " >> ARGS
	fi
		shift
done

[ S -eq 2 ] || exit 2


echo "java " >> CMD
cat OPTS | xargs >> CMD
echo " " >> CMD
cat DPROPS | xargs >> CMD
echo " -jar " >> CMD
echo "$NAME " >> CMD
cat ARGS | xargs >> CMD

ENDCOMMAND=$( cat CMD | xargs )
$ENDCOMMAND

rm -- ARGS
rm -- DPROPS
rm -- CMD
rm -- OPTS
