#!/bin/sh
#
# $Id$
#
# Copyright (c) 2009-2012 Jason J. Hellenthal <jhell@DataIX.net>,
# All rights reserved.
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
# THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#

TABLE="blacklist"
BLIST="/etc/pf/tables/blacklist"
PFLOG="/var/log/pflog"

trap 'exit 1' 2

umask 077

readonly TABLE PFLOG BLIST

pfctl -t $TABLE -T show >>/dev/null 2>&1
if [ $? -ne 0 ]; then
        echo "$0: Table $TABLE was not found, exiting..."
        exit 1
fi

if [ ! -f $BLIST ]; then
        echo "$0: File $BLIST was not found, exiting..."
        exit 1
fi

TMPFILE1=$(mktemp /tmp/.pftbl_fil.XXXXXX)
TMPFILE2=$(mktemp /tmp/.pftbl_fil.XXXXXX)
if [ $? -ne 0 ]; then
	echo "$0: Can't create temp file(s), exiting..."
	exit 1
fi

readonly TMPFILE1 TMPFILE2

_pcap_fil(){
	for logfile in $PFLOG; do
	  if [ -f $logfile ]; then
	  	tcpdump -tnr $logfile 2>/dev/null |\
	  	awk '{print $2}' |\
	  	cut -f1-4 -d. |\
	  	sed -e 's/192\.168\.31.*//' \
	  	    -e 's/1\.1\.1\.1//' \
	  	    -e 's/2\.2\.2\.2//' \
	  	    -e 's/0\.0\.0\.0//' \
	  	    -e 's/.*:.*//' |\
		sort -n |uniq
	  else
		echo "$0: No log file(s) found, exiting..."
		exit 1
	  fi
	done
}

#
# Uncomment this if you wish to parse IP's from your pflog.
#
#_pcap_fil |sort -n |uniq - $TMPFILE1

cat $BLIST |awk '{print $1}' >>$TMPFILE1
pfctl -t $TABLE -T show |awk '{print $1}' >>$TMPFILE1

for ipaddr in $(cat $TMPFILE1); do
        echo $ipaddr >>$TMPFILE2
done

echo "Statistics:"
echo " - Old count in table store:" ;wc -l $BLIST ;echo
sort -n $TMPFILE2 |uniq - $BLIST
echo " - New count in table store:" ;wc -l $BLIST ;echo
echo " - Changes to running table:"
echo -n "       "
pfctl -t $TABLE -T add `cat $BLIST`

rm -f $TMPFILE1 $TMPFILE2
