#!/bin/sh
#
# $Id$
#

_get_ident(){
	ident /boot/kernel/kernel |grep \$FreeBSD |\
		awk '{print $3, $4, $5, $2, $6}' |\
		sort -n |uniq
}

_get_commits(){
	_get_ident |awk '{print $5}' |sort |uniq -c |\
		awk '{print $2"["$1"]"}' |column -x -c80
}

_get_commits_top15(){
	_get_ident |awk '{print $5}' |sort |uniq -c |\
		awk '{print $1, $2}' |sort -nr |head -n15
}

echo
echo "Top 15 commits to the running kernel:"
echo "==============================================================================="
_get_commits_top15 |awk '{print $2"["$1"]"}' |column -x -c80
echo
echo "Number of contributions by committer:"
echo "==============================================================================="
_get_commits
echo
