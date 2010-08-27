#!/bin/sh

# $Id$

PATH="/bin:/usr/bin:/usr/local/bin"

trap 'exit 1' 2

cd /var/db/pkg

if [ -d .hg ]; then
	hg addremove
else
	echo "No Mercurial repo. Exiting..."
	exit 1
fi

readonly ADDENDUM=$(mktemp /tmp/.pkg_commit.XXXXXX)
readonly COMMITLOG=$(mktemp /tmp/.pkg_commit.XXXXXX)
readonly DEPRECATED=$(mktemp /tmp/.pkg_commit.XXXXXX)
readonly MODIFIED=$(mktemp /tmp/.pkg_commit.XXXXXX)
readonly HGSTATUS=$(mktemp /tmp/.pkg_commit.XXXXXX)
readonly WRITEAREA=$(mktemp /tmp/.pkg_commit.XXXXXX)

_tclean(){
	rm -f $HGSTATUS $WRITEAREA $ADDENDUM $DEPRECATED $MODIFIED \
	$COMMITLOG
}

_tstatus(){
	hg status >$HGSTATUS
	if [ -z "$(cat $HGSTATUS)" ]; then
		printf "Nothing to be done, Cleaning up...\n"
		_tclean		||printf "Error in: _tclean,$?\n"
		exit 0
	fi
}

_taddendum(){
	grep "A " $HGSTATUS |awk '{print $2}' |\
		cut -f1 -d/ |sort | uniq >$WRITEAREA

        if [ -n "$(cat $WRITEAREA)" ]; then
		printf "Addendum:\n"    > $ADDENDUM
		cat $WRITEAREA          >>$ADDENDUM
		printf "\n"             >>$ADDENDUM
		printf ""               > $WRITEAREA
        fi
}


_tdeprecated(){
	grep "R " $HGSTATUS |awk '{print $2}' |cut -f1 -d/ |sort |\
	uniq >$WRITEAREA

        if [ -n "$(cat $WRITEAREA)" ]; then
		printf "Deprecated:\n"	> $DEPRECATED
		cat $WRITEAREA		>>$DEPRECATED
		printf "\n"		>>$DEPRECATED
		printf ""		> $WRITEAREA
	fi
}


_tmodified(){
	grep "M " $HGSTATUS |awk '{print $2}' |cut -f1 -d/ |sort |\
	uniq >$WRITEAREA

        if [ -n "$(cat $WRITEAREA)" ]; then
		printf "Modified:\n"	> $MODIFIED
		cat $WRITEAREA		>>$MODIFIED
		printf "\n"		>>$MODIFIED
		printf ""		> $WRITEAREA
	fi
}

_tcombine(){
	printf "Package database update by pkg_commit(1)\n\n" >$COMMITLOG
	for tmpfile in $ADDENDUM $DEPRECATED $MODIFIED; do
		if [ -n "$(cat $tmpfile)" ]; then 
			cat $tmpfile >>$COMMITLOG
		fi
	done
}

_tcommit(){
	if [ -n "$(cat $COMMITLOG)" ]; then
		hg commit -l $COMMITLOG	||exit 1
	fi
}

_tmail(){
	hg tip
	echo
	cat $COMMITLOG
}

_tmain(){
	_tstatus	||printf "Error in: _tstatus,$?\n"
	_taddendum	||printf "Error in: _taddendum,$?\n"
	_tdeprecated	||printf "Error in: _tdeprecated,$?\n"
	_tmodified	||printf "Error in: _tmodified,$?\n"
	_tcombine	||printf "Error in: _tcombine,$?\n"
	_tcommit	||printf "Error in: _tcommit,$?\n"
	_tmail		|mail -s "Package Database Update: $(hg tip --template '{rev}:{node|short}')" root
	_tclean		||printf "Error in: _tclean,$?\n"
	exit 0
}

_tmain
exit 0
