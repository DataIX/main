#!/usr/bin/env -iS -P /bin sh
#
# Copyright (c) 2006-2011 J. Hellenthal <jhell@DataIX.net>,
# All rights reserved.
#
# $Id$
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# see also sh(1), stat(1), echo(1), md5(1), sha256(1)
#
PATH=/sbin:/bin:/usr/sbin:/usr/bin ;readonly PATH

renice 5 -p $$ 2>/dev/null
OURUSERID="`id -u`" 
FILESPACE="$@" 

readonly OURUSERID FILESPACE

if [ -z "$FILESPACE" ]; then
	echo "Usage: distattr file1 [file2 file3 ...] [/path/to/files*]"
	exit 1
else
	for file in $FILESPACE; do
		if [ "$OURUSERID" != "0" -a ! -O $file -a ! -G $file -o ! -w $file ]; then
			NOOPFILES="$NOOPFILES $file"
		else
			FILESLIST="$FILESLIST $file"
		fi
	done
	readonly NOOPFILES FILESLIST
fi

case $OURUSERID in
	0) NAMESPACE="system"
	   MESGSPACE="SYSTEM: ";;
	*) NAMESPACE="user"
	   MESGSPACE="USER:   "
esac; readonly NAMESPACE MESGSPACE

echo -e "\nUsing namespace \"$NAMESPACE\" to store extattr(9)"
SETATTR="setextattr ${NAMESPACE}" ;readonly SETATTR

for file in $FILESLIST; do
	if [ -f $file -a -r $file ]; then
		export `stat -s $file`

		echo -e "FILE:\t$file"
		echo -n "${MESGSPACE}Setting "

		echo -n "NAME"
		$SETATTR NAME "$file" $file 2>/dev/null ||\
			export NAERR="$NAERR $file"

		echo -n ", MD5"
		$SETATTR MD5 `md5 -q $file` $file 2>/dev/null ||\
			export MDERR="$MDERR $file"

		echo -n ", SHA256"
		$SETATTR SHA256 `sha256 -q $file` $file 2>/dev/null ||\
			export SHERR="$SHERR $file"

		if [ -n "$st_size" ]; then
			echo -n ", SIZE"
			$SETATTR SIZE $st_size $file 2>/dev/null ||\
				export SIERR="$SIERR $file"
		fi

		if [ -n "$st_birthtime" ]; then 
			echo -n ", CREATED"
			$SETATTR CREATED "`date -j -r $st_birthtime`" $file 2>/dev/null ||\
				export CRERR="$CRERR $file"
		fi

		echo -n ", TIMESTAMP"
		$SETATTR TIMESTAMP "`date`" $file 2>/dev/null ||\
			export TSERR="$TSERR $file"

		echo "... [DONE]"
	fi
done; echo

if [ -n "$NAERR" ]; then
	echo "Namespace errors setting attribute: NAME"
	for file in $NAERR; do
		echo $file
	done |column -x -c80
fi

if [ -n "$MDERR" ]; then
	echo "Namespace errors setting attribute: MD5"
	for file in $MDERR; do
		echo $file
	done |column -x -c80
fi

if [ -n "$SHERR" ]; then
	echo "Namespace errors setting attribute: SHA256"
	for file in $SHERR; do
		echo $file
	done |column -x -c80
fi

if [ -n "$SIERR" ]; then
	echo "Namespace errors setting attribute: SIZE"
	for file in $SIERR; do
		echo $file
	done |column -x -c80
fi

if [ -n "$CRERR" ]; then
	echo "Namespace errors setting attribute: CREATED"
	for file in $CRERR; do
		echo $file
	done |column -x -c80
fi

if [ -n "$TSERR" ]; then
	echo "Namespace errors setting attribute: TIMESTAMP"
	for file in $TSERR; do
		echo $file
	done |column -x -c80
fi
