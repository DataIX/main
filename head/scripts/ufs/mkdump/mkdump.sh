#!/bin/sh
#
# Copyright (c) 2006-2012 Jason J. Hellenthal <jhell@DataIX.net>,
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
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

readonly PATH="/sbin:/usr/sbin:/bin:/usr/bin"

#
# Set this to the devices that you wish to dump.
#
# These must consist of only the device name seperated by
# spaces with no prefixed path. This includes geli(8) and
# gbde(8) suffixed devices. *Warning* if you backup a
# cryptographic filesystem into a file or tape device the
# encryption for that filesystem does not get exported
# to the file or tape and your media in the file or tape
# will be clearly readble by anyone that has access to it.
# Another better aproach for encrypted filesystems backup
# is to use a mdconfig(8) file-backed disk and then just
# copy the whole file to your backup using something like
# rsync(1), ssh(1), ftp(1). 
#
dump_devs="ada0p3 ada0p4 ada0p6"

#
# Set this to the path where you wish to save your dumps.
#
# If you have a mounted filesystem on /mnt/smb/winbox/exports
# for example, you should create a directory in the root
# directory of the mount so that the script can test if the
# directory exists. Also if you plan to use this on multiple
# machines, it is a good idea to keep each machines dumps
# under a directory of the same name of the machine that is
# going to be dumped to avoid confusion or possible overwrite.
#
dump_path="/exports/Tapes"

#
# Set this only if you know what you are doing. Live filesystem
# flags will be added to this variable automaticly if the system
# supports the feature (FreeBSD 5, 6, 7 / UFS2).
#
dump_flags="-h0 -B 4194304"

#
# Set this to the uid of the user that you usually do dumps as.
#
# Only change this if you know what you are doing and have
# taken the correct steps and chmod'd and/or chown'd the right
# devices to give that user permission to do the dumps.
#
dump_uid="0"

#
# Comment out this next line after you have made the proper
# modifications to this script that include editing the
# variables dump_devs & dump_path above.
#
trip="Please make the proper modifications to this script"




###############################################################
#   Source below this point should not need to be changed.    #
###############################################################

umask 077
idprio 10 -$$

_chkid_f(){
	if [ -n "$trip" ]; then
		echo "$trip before usage."
		exit 1
	fi
	if [ "`id -u`" != "$dump_uid" ]; then
		echo "mkdump may only be run by uid/$dump_uid."
		exit 2
	fi
}

if [ `sysctl -n kern.osreldate` -ge "500000" ]; then
	dump_flags="${dump_flags} -L"
fi

_mkdump_purge(){
	for _purgefile in $dump_path/$_dumpdev-level$dump_lvl-vol*; do
		if [ -f $_purgefile ]; then
			echo -n "  DUMP: Removing stale dump file: $(basename $_purgefile)"
			rm $_purgefile 2>/dev/null && echo -e "\t[DONE]" ||echo -e "\t[FAILED]"
		fi
	done
	if [ "$dump_level" -le "8" ]; then 
		for _stalefile in $dump_path/$_dumpdev-lvl$dump_str-vol*; do
			if [ -f $_stalefile ]; then
				echo -n "  DUMP: Removing stale dump file: $(basename $_stalefile)"
				rm $_stalefile 2>/dev/null && echo -e "\t[DONE]" ||echo -e "\t[FAILED]"
			fi
		done
        fi
}

_mkdump_f(){
	for _dumpdev in $dump_devs; do
		VOLUME='$DUMP_VOLUME'
		PIPESTR="cat >$dump_path/$_dumpdev-lvl$dump_level"
		#PIPESTR="dd of=$dump_path/$_dumpdev-lvl$dump_level"
		if [ -c /dev/$_dumpdev -a -d $dump_path ]; then
			_mkdump_purge
			echo "  DUMP: Performing level $dump_level dump for: $_dumpdev"
			dump $dump_flags -${dump_level}u -P "''$PIPESTR-vol$VOLUME''" /dev/$_dumpdev
			echo
		else
			echo -e "\nError on device $_dumpdev at level $dump_level."
			echo "Dump device or the dump path was specified incorrectly."
			exit 3
		fi
	done
	cp -f /etc/dumpdates $dump_path
}

_mkdump_read_f(){
	unset dump_devs
	unset dump_path
	unset dump_level
	unset dump_str
	clear
	echo -e "\nEnter the path you wish to save dumps to."
	echo -n "Dump path: "
	read dump_path ;readonly dump_path
	echo -e "\nEnter a list of devices to dump."
	echo -n "Device list: "
	read dump_devs ;readonly dump_devs
	echo -e "\nEnter dump level [0-9]"
	echo -n "Dump level: "
	read dump_level
	if [ -n "$dump_path" -a -n "$dump_devs" -a -n "$dump_level" ] ; then
		case $dump_level in
			0) readonly dump_level="0"
			   dump_str="[1-9]" ;_mkdump_f ;;
			1) readonly dump_level="1"
			   dump_str="[2-9]" ;_mkdump_f ;;
			2) readonly dump_level="2"
			   dump_str="[3-9]" ;_mkdump_f ;;
			3) readonly dump_level="3"
			   dump_str="[4-9]" ;_mkdump_f ;;
			4) readonly dump_level="4"
			   dump_str="[5-9]" ;_mkdump_f ;;
			5) readonly dump_level="5"
			   dump_str="[6-9]" ;_mkdump_f ;;
			6) readonly dump_level="6"
			   dump_str="[7-9]" ;_mkdump_f ;;
			7) readonly dump_level="7"
			   dump_str="[8-9]" ;_mkdump_f ;;
			8) readonly dump_level="8"
			   dump_str="9" ;_mkdump_f ;;
			9) readonly dump_level="9" ; _mkdump_f ;;
			*) echo "Enter a valid dump level [0-9]."
			   exit 4
		esac
	else
		echo "Path, level & devices need to be set correctly."
		exit 5
	fi
}

_chkid_f

case $1 in
	0) readonly dump_level="0"
	   dump_str="[1-9]" ;_mkdump_f ;;
	1) readonly dump_level="1"
	   dump_str="[2-9]" ;_mkdump_f ;;
	2) readonly dump_level="2"
	   dump_str="[3-9]" ;_mkdump_f ;;
	3) readonly dump_level="3"
	   dump_str="[4-9]" ;_mkdump_f ;;
	4) readonly dump_level="4"
	   dump_str="[5-9]" ;_mkdump_f ;;
	5) readonly dump_level="5"
	   dump_str="[6-9]" ;_mkdump_f ;;
	6) readonly dump_level="6"
	   dump_str="[7-9]" ;_mkdump_f ;;
	7) readonly dump_level="7"
	   dump_str="[8-9]" ;_mkdump_f ;;
	8) readonly dump_level="8"
	   dump_str="9" ;_mkdump_f ;;
	9) readonly dump_level="9"
	   dump_str="" ; _mkdump_f ;;
	i) _mkdump_read_f ;;
	*) echo "Usage: mkdump [0-9,i]"
esac
