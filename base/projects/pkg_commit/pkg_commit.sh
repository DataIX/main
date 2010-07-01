#!/bin/sh

# $Id$

trap 'exit 1' 2

cd /var/db/pkg

readonly ADDENDUM=$(mktemp /tmp/.pkg_commit.XXXXXX)
readonly COMMITLOG=$(mktemp /tmp/.pkg_commit.XXXXXX)
readonly DEPRECATED=$(mktemp /tmp/.pkg_commit.XXXXXX)
readonly MODIFIED=$(mktemp /tmp/.pkg_commit.XXXXXX)
readonly SVNSTATUS=$(mktemp /tmp/.pkg_commit.XXXXXX)
readonly WRITEAREA=$(mktemp /tmp/.pkg_commit.XXXXXX)

_tclean(){
	rm -f $SVNSTATUS $WRITEAREA $ADDENDUM $DEPRECATED $MODIFIED \
	$COMMITLOG
}

_tstatus(){
	svn status >$SVNSTATUS
	if [ -z "$(cat $SVNSTATUS)" ]; then
		printf "Nothing to be done, Cleaning up...\n"
		_tclean		||printf "Error in: _tclean,$?\n"
		exit 0
	fi
}

_taddendum(){
	grep "? " $SVNSTATUS |awk '{print $2}' |xargs svn add |\
		awk '{print $2}' |cut -f1 -d/ |sort | uniq >$WRITEAREA

        if [ -n "$(cat $WRITEAREA)" ]; then
		printf "Addendum:\n"    > $ADDENDUM
		cat $WRITEAREA          >>$ADDENDUM
		printf "\n"             >>$ADDENDUM
		printf ""               > $WRITEAREA
        fi
}


_tdeprecated(){
	grep "! " $SVNSTATUS |awk '{print $2}' |xargs svn del |\
		awk '{print $2}' |cut -f1 -d/ |sort | uniq >$WRITEAREA

        if [ -n "$(cat $WRITEAREA)" ]; then
		printf "Deprecated:\n"	> $DEPRECATED
		cat $WRITEAREA		>>$DEPRECATED
		printf "\n"		>>$DEPRECATED
		printf ""		> $WRITEAREA
	fi
}


_tmodified(){
	grep "M " $SVNSTATUS |awk '{print $2}' |cut -f1 -d/ |sort |\
		uniq >$WRITEAREA

        if [ -n "$(cat $WRITEAREA)" ]; then
		printf "Modified:\n"	> $MODIFIED
		cat $WRITEAREA		>>$MODIFIED
		printf "\n"		>>$MODIFIED
		printf ""		> $WRITEAREA
	fi
}

_tcombine(){
	for tmpfile in $ADDENDUM $DEPRECATED $MODIFIED; do
		if [ -n "$(cat $tmpfile)" ]; then 
			cat $tmpfile >>$COMMITLOG
		fi
	done
}

_tcommit(){
	if [ -n "$(cat $COMMITLOG)" ]; then
		svn commit -F $COMMITLOG	||exit 1
		svn update			||exit 2
	fi
}

_tmail(){
	cat $COMMITLOG |mail -s "Package Commit Report: $(date)" root
}

_tmain(){
	_tstatus	||printf "Error in: _tstatus,$?\n"
	_taddendum	||printf "Error in: _taddendum,$?\n"
	_tdeprecated	||printf "Error in: _tdeprecated,$?\n"
	_tmodified	||printf "Error in: _tmodified,$?\n"
	_tcombine	||printf "Error in: _tcombine,$?\n"
	_tcommit	||printf "Error in: _tcommit,$?\n"
	_tmail		||printf "Error in: _tmail,$?\n"
	_tclean		||printf "Error in: _tclean,$?\n"
	exit 0
}

_tmain
exit 0
