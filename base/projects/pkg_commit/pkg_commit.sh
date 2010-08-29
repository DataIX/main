#!/bin/sh

PATH="/bin:/usr/bin:/usr/local/bin"

trap 'exit 1' 2

cd /var/db/pkg

if [ -d .hg ]; then
	hg addremove
else
	printf "No Mercurial repo. Exiting...\n"
	printf "Please ( cd /var/db/pkg && hg init . ) before using this.\n"
	exit 1
fi

readonly ADDENDUM=$(mktemp /tmp/.pkg_commit.XXXXXX)
readonly COMMITLOG=$(mktemp /tmp/.pkg_commit.XXXXXX)
readonly DEPRECATED=$(mktemp /tmp/.pkg_commit.XXXXXX)
readonly HGSTATUS=$(mktemp /tmp/.pkg_commit.XXXXXX)
readonly MODIFIED=$(mktemp /tmp/.pkg_commit.XXXXXX)
readonly WRITEAREA=$(mktemp /tmp/.pkg_commit.XXXXXX)

_tclean(){
	rm -f $HGSTATUS $WRITEAREA $ADDENDUM $DEPRECATED $MODIFIED $COMMITLOG
	for tmpfile in $HGSTATUS $WRITEAREA $ADDENDUM $DEPRECATED $MODIFIED $COMMITLOG; do
		if [ -f $tmpfile ]; then
			LEFTOVER="${LEFTOVER} $tmpfile"
		fi
	done ;unset tmpfile

	if [ -n "$LEFTOVER" ]; then
		printf "Problem discovered while removing files.\n\n"
		printf "Please remove the folling files manually:\n"
		for tmpfile in $LEFTOVER; do
			printf "$tmpfile\n"
		done ;unset tmpfile
	fi
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
	grep "A " $HGSTATUS |awk '{print $2}' |cut -f1 -d/ |sort |\
	uniq >$WRITEAREA

        if [ -n "$(cat $WRITEAREA)" ]; then
		printf "Addendum:\n"
		cat $WRITEAREA ;printf "" >$WRITEAREA
		printf "\n"
        fi
}


_tdeprecated(){
	grep "R " $HGSTATUS |awk '{print $2}' |cut -f1 -d/ |sort |\
	uniq >$WRITEAREA

        if [ -n "$(cat $WRITEAREA)" ]; then
		printf "Deprecated:\n"
		cat $WRITEAREA ;printf "" >$WRITEAREA
		printf "\n"
	fi
}


_tmodified(){
	grep "M " $HGSTATUS |awk '{print $2}' |cut -f1 -d/ |sort |\
	uniq >$WRITEAREA

        if [ -n "$(cat $WRITEAREA)" ]; then
		printf "Modified:\n"
		cat $WRITEAREA ;printf "" >$WRITEAREA
		printf "\n"
	fi
}

_tcombine(){
	printf "Package database update by pkg_commit(1)\n\n"
	for tmpfile in $ADDENDUM $DEPRECATED $MODIFIED; do
		if [ -n "$(cat $tmpfile)" ]; then 
			cat $tmpfile
		fi
	done ;unset tmpfile
}

_tcommit(){
	if [ -n "$(cat $COMMITLOG)" ]; then
		hg commit -l $COMMITLOG	||exit 1
	fi
	HGREV="$(hg tip --template '{rev}:{node|short}')"
}

_tmail(){
	hg tip
	for tmpfile in $ADDENDUM $DEPRECATED $MODIFIED; do
		if [ -n "$(cat $tmpfile)" ]; then
			cat $tmpfile
		fi
	done ;unset tmpfile
	SUBJECT="Package Database Commit: ${HGTIP}"
}

_tmain(){
	_tstatus
	_taddendum	>$ADDENDUM
	_tdeprecated	>$DEPRECATED
	_tmodified	>$MODIFIED
	_tcombine	>$COMMITLOG
	_tcommit
	_tmail		|mail -s "${SUBJECT}" $(id -un)
	_tclean
	exit 0
}

_tmain
