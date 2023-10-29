#! /usr/bin/bash

#[ "$(whoami)" != "root" ] || exit 1

while read U H; do
	if [ -n "${H}" ]; then
		echo "${U}"
		continue
	fi
done<  <( cat /etc/passwd | awk -F ':' '{print $1 " " $6}' )


