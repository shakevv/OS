#!/bin/bash

[ $# -eq 2 ] || exit 1

dir=$1
string=$2

find $dir -mindepth 1 -maxdepth 1 -type f -regextype egrep -regex ".*vmlinuz-[0-9]+\.[0-9]+\.[0-9]+-$string" -printf "%f\n" | sort | tail -n 1

exit 0
