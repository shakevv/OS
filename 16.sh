#!/usr/bin/bash

[ $# -eq 1 ] || exit 1

if [ "$(whoami)" != "root" ]; then
	echo "Not root"
	exit 2
fi

file=$( mktemp )

for user in $( ps -e -o user= | sort | uniq ); do
	R=$( ps -u $user -o rss= | awk '{s+=$1} END {print s}' )
	echo "$user $R"

	if [ $R -le $1 ]; then
		continue
	fi

	P=$( ps -u $user -o rss,pid | awk 'NR != 1' | sort -n -k 1,1 | tail -n 1 | cut -d " " -f 2 )

	kill -TERM $P
	sleep 2
	kill -KILL $P
done


