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

trap 'exit 1' 2

PKG_DIST="/exports/packages"
PKG_DATA="/var/db/pkg"

cd $PKG_DIST/ ||exit
ls $PKG_DATA/ |sed -e 's/^pkgdb\.db//' -e 's/^\.hg//' >.pkg_list

for _pkg_name in `sort .pkg_list`; do
	[ ! -f $_pkg_name.tbz ] && pkg_create -v -b $_pkg_name
done

ls $PKG_DIST/ |sed 's/\.tbz//' |fgrep -vf .pkg_list >.pkg_remove
for _pkg_name in `sort .pkg_remove`; do
	rm -if $_pkg_name.tbz ||echo "Failed to remove $_pkg_name.tbz"
done

rm -f .pkg_list .pkg_remove
