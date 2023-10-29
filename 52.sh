#!/bin/bash

[ $# -eq 1 ] || exit 1

[ -f $1 ] || exit 2

CFG_FILE=$1
FILE="/proc/acpi/wakeup"

while read device status; do
	if [ -z "$device" ];then 
		continue
	fi

	LINE=$( cat $FILE | awk -v d=$device '$1==d' )
	if [ -z "$LINE" ]; then
		echo "WARN: Unexisting device - $device"
		continue
	fi

	NEW_LINE=$( echo $LINE | \
		awk -v s=$status '{print $1" "$2" "s" "$4}' ) 

	sed -i -e "s/$LINE/$NEW_LINE/" $FILE

done < <( cat $CFG_FILE | awk -F '#' '{print $1}' )
