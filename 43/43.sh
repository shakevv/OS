#!/bin/bash

if [ $# -eq 0 ]; then
	echo "Invalid number of args"
	exit 1
fi

olddate=""
date=$( date -I | tr -d '-' )
file=""

while [ $# -ne 0 ]; do
	file=$1
	echo "filename: ${file}"
	if egrep -q '^.*SOA.*\(' $file; then
		olddate=$( cat $file \
				| egrep -A1 'SOA' \
				| tail -n 1 \
				| xargs \
				| awk '{print $1}' )
	else
		echo "After grep if else"
		olddate=$( cat $file \
				| egrep 'SOA' \
				| xargs \
				| rev \
				| awk '{print $5}' \
				| rev \
				| awk '{print $0}' )
	fi


	if egrep -q -v "$date" <( echo "$olddate" ); then
		date="${date}00"
		echo "Date is: ${date}"
	else
		if echo $olddate | egrep -q '[0-9]{8}99'; then
			exit 3
		fi
		date=$( expr $olddate + 1 )
	fi
	echo "before sed"
	sed -i -e "s/$olddate/$date/g" $file
echo "after sed"
shift
echo "after shift"
done

exit 0
