#!/usr/bin/bash

[ $# -eq 1 ] || exit 1

[ -d $1 ] || exit 2

find -L $1 -type l -printf "%p\n"  
#find $1 -type l -printf "%p %Y\n" | awk '$2 == "N" {print $1}'
