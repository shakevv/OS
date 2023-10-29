#!/bin/bash

if [ $# -gt 2 ] || [ $# -eq 0 ]; then
	echo "invalid number of args"
	exit 1
fi

state=1

if [ $# -eq 2 ]; then 
	state=2
fi

broken=0
while read file isbroken; do
line=" -> "
dest=""
	if [ $isbroken == 'N' ]; then
		broken=$( expr $broken + 1 )
		continue
	fi
	dest=$( readlink $file )
	line="${file}${line}${dest}"
	if [ $state -eq 2 ]; then 
		echo $line >> $2
	else
		echo $line
	fi

done< <( find $1 -type l -printf "%f %Y\n" )

if [ $state -eq 2 ]; then
	echo "Broken symlinks: ${broken}" >> $2
else
	echo "Broken symlinks: ${broken}"
fi

exit 0
