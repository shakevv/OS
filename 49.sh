#!/bin/bash
	COMMAND="sqlplus -SL \"/ as SYSDBA\" @foo.sql"
echo $COMMAND
[ $# -eq 1 ] || exit 3

USER=$( whoami )

if [ "$USER" != "oracle" ] && [ "$USER" != "grid" ]; then 
	exit 2
fi

[ ! -z "$ORACLE_BASE" ] || exit 1
[ ! -z "$ORACLE_HOME" ] || exit 1
[ ! -z "$ORACLE_SID" ] || exit 1

TMP=$( mktemp )
FILE="$ORACLE_HOME"/bin/sqlplus
CONTENT=$( cat $FILE )
if [ "$CONTENT" != "SELECT value FROM v\$parameter WHERE name = 'diagnostic_dest';\nEXIT;" ]; then
	rm -- $TMP
	exit 5
fi
if [ "$USER" == "oracle" ]; then
	COMMAND="sqlplus -SL \"/ as SYSDBA\" @foo.sql"
	$COMMAND > $TMP
	if [ $? -ne 0 ]; then
		rm -- $TMP
		exit 4
	fi
	DIAG_DEST=$( cat $TMP | awk 'NR == 4')
	rm -- $TMP
	DIAG_BASE=$ORACLE_BASE
	if [ ! -z "DIAG_DEST" ]; then
		DIAG_BASE=$DIAG_DEST
	fi
	echo "rdbms: $( find $DIAG_BASE/diag/rdbms -mindepth 2 -maxdepth 2 -type f -name "*_[0-9]+(trm|trc)$" -mtime +$1 -printf "%k\n" | awk 'BEGIN {sum=0} {sum=sum+$1} END {print sum}' )"
fi
