#!/bin/sh

printf " <->\tUID \tLOCAL:PORT \t\tREMOTE:PORT\n"
printf " ===\t======  ======================  ======================\n"
lsof -Pnl -iTCP -sTCP:ESTABLISHED 2>/dev/null |\
	awk '{print $3,$(NF - 1)}' |\
	grep "[0-9]" |sed 's/->/ /' |\
	awk '{print " \t"$1" \t"$2" \t"$3}' |\
	sort -n |uniq -c |sort -nr
