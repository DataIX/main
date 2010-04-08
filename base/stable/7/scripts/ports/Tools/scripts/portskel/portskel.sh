#!/bin/sh
#
# Copyright (c) 2009-2010 Jason J. Hellenthal <jhell@dataix.net>,
# All rights reserved.
#
# $Id$
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistribution's of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistribution's in binary form must reproduce the above copyright
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

: ${PORTS:=/usr/ports}
: ${PKGDIR:=/var/db/pkg}

_PSVER='$Id$'

trap 'exit 1' 2

_add_Header(){
	RUNTIME="`date -u`"
	echo "#"
	echo "# Created:	$RUNTIME"
	echo "#     Tag:	$TAG"
	echo "#"
	echo "# \$Id\$"
	echo "#"
}

_echo_rootMakefile(){
	TAG="Master Makefile"
	_add_Header
	echo ".include <$PORTS/Makefile>"
}

_echo_subcMakefile(){
	TAG="$category"
	_add_Header
	echo ".include <$PORTS/$category/Makefile>"
}

_error_Cases(){
	ERRORLEVEL=4
	[ ! -z "$PORTSKEL" ] && ERRORLEVEL=$(($ERRORLEVEL-1))
	[ "$PORTSKEL" != "/" ] && ERRORLEVEL=$(($ERRORLEVEL-1))
	[ "$PORTSKEL" != "$PORTS" ] && ERRORLEVEL=$(($ERRORLEVEL-1))
	[ "$PORTSKEL" != "$PORTS/" ] && ERRORLEVEL=$(($ERRORLEVEL-1))
	if [ "$ERRORLEVEL" -ne "0" ]; then
		echo " 1) Specify a path for create/clone/auto."
		echo " 2) Make sure you have write access to [1]."
		echo " 3) The directory specified is not $PORTS."
		echo " 4) The directory specified is not /."
		echo
		_pskel_Usage
		exit 1
	fi
}

PROGNAME=$(basename $0)
PORTSKEL="$2"

echo $PORTSKEL |grep "\.\." >>/dev/null 2>&1 && echo "\
Portskel detects but allows the use of \"..\" If you did
not intend to use these then hit ^C now or this process
will continue in ~10 seconds." && sleep 20

_create_Structure(){
	: ${OPERATION:=Creating}
	_error_Cases
	echo "This will not overwrite existing categories or Makefiles."
	echo "This only adds/updates new categories or missing Makefiles."
	echo
	if [ ! -r $PORTSKEL ]; then
		echo -e ">>> $OPERATION structure...\t[$PORTSKEL]"
		mkdir -p $PORTSKEL
		else
		OPERATION="Updating"
		echo -e ">>> $OPERATION structure...\t[$PORTSKEL]"
	fi

	if [ -d $PORTSKEL ]; then

		PORTMGMT=$(ls -d $PORTS/[A-Z]* |xargs basename |sed 's/Makefile//')
		CATEGORY=$(ls -d $PORTS/[a-z]* |xargs basename)
		CATEGORY=$(echo $CATEGORY |sed -e 's/packages//' -e 's/distfiles//')

		for portmgmt in $PORTMGMT; do
			if [ ! -r $PORTSKEL/$portmgmt ]; then
				echo -e ">>> $OPERATION top-level...\t$portmgmt"
				ln -s $PORTS/$portmgmt $PORTSKEL
			fi
		done

		if [ ! -r $PORTSKEL/Makefile ]; then
			echo -e ">>> $OPERATION top-level...\tMakefile"
			_echo_rootMakefile >$PORTSKEL/Makefile
		fi

		for category in $CATEGORY; do
			mkdir -p $PORTSKEL/$category
			if [ ! -f $PORTSKEL/$category/Makefile ]; then
				echo -e ">>> $OPERATION sub-level...\t$category"
				_echo_subcMakefile >$PORTSKEL/$category/Makefile
			fi
		done
	fi
}

_update_Structure(){
	: ${OPERATION=Updating}
	_create_Structure
	_clone_Origins
}

_pskel_Usage(){
	echo "Usage: $PROGNAME [version]|[help]|[create|clone|auto] /path/to/yourskel"
}

_get_Origins(){
	grep "@comment ORIGIN:" $PKGDIR/*/+CONTENTS |sed 's/.*ORIGIN://' |sort
}

_clone_Origins(){
	_error_Cases
	for pkgorigin in `_get_Origins`; do
		if [ -d "$PORTSKEL" -a ! -d "$PORTSKEL/$pkgorigin" ]; then
			echo -e ">>> Linking clone...\t\t$pkgorigin"
			ln -s $PORTS/$pkgorigin $PORTSKEL/$pkgorigin
		fi
	done
}

_pskel_Help(){
    echo
    _pskel_Usage
    echo
    echo "General Commands"
    echo "
  create
    This will create a skeleton of your ports structure without the actual
    ports. You can use this command to create and update just the structure
    of the skeleton from time to time."
    echo "
  clone
    This will create clones or symlinks of already installed ports on your
    system and link them to their ORIGINS in the ports tree. You can use
    this command from time to time to automagically add new clones to the
    ports skeleton that have been added to your system since the last time
    portskel was ran."
    echo "
  auto
    This is a ease of use command that calls create & clone. This is meant
    for use with cron or other automation procedures and can be used in
    place of the previous two commands."
    echo "
  version
    Obviously the running version of portskel!."
    echo "
  help
    This help screen."
    echo
    echo "Environment Variables"
    echo "
  PKGDIR
    This can be used to specify a path to your package database.
    Default: /var/db/pkg"
    echo "
  PORTS
    This can be used to specify a path to your ports tree.
    Default: /usr/ports"
    echo
}

case $1 in
	create) _create_Structure ;;
	clone) _clone_Origins ;;
	auto) _update_Structure ;;
	help) _pskel_Help ;;
	version) echo "Version 2010d, $_PSVER" ;;
	*) _pskel_Usage
esac
