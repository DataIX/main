#!/bin/sh -
#
# Copyright (c) 2015 J. Hellenthal <jhellenthal@DataIX.net>
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
# THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#

# Provide dynamic hostid calculation while allowing System Administrators
# to set the system hostid from a trackable uuid value in /etc/hostid file
# for all intended purposes of auditing and network related tasks.

# The hostid is generated using calculation of kernel & session uuid's first
# 8 characters from a sha512 sum of both kern.uuid kern.bootsessionuid to hex
# then converted to an integer that is a modulas of ((2^31)-1).

hostid_file="${hostid_file:-/etc/hostid}"
tr='/usr/bin/tr [:lower:] [:upper:]'
sha512='/usr/bin/openssl sha -sha512'
uuidgen='/usr/bin/uuidgen'
sysctl='/usr/sbin/sysctl -q'
head='/usr/bin/head -c 8'
cat='/usr/bin/cat'

_hostid_set() {
	uuidpair="$1"
	if [ -n "$2" ]; then
		uuidpair="${uuidpair}:$2"
	fi
	id="0x$(printf ${uuidpair} |${sha512} |${head} |${tr})"
	id="$(printf '%d' $((${id} % $(((2**31)-1)))))"

	printf "Setting hostid: %d\n" ${id}
	${sysctl} kern.hostid=${id} >/dev/null
	return
}

_hostid_get() {
	kuuid="$(${sysctl} -n kern.uuid)"
	buuid="$(${sysctl} -n kern.bootsessionuuid)"
	printf "%s %s" ${kuuid} ${buuid}
	return
}

_hostid_gen() {
	uuid="$(_hostid_get)"
	if [ -z "${uuid}" ]; then
		uuid="$(${uuidgen})"
	fi
	_hostid_set ${uuid}
	return
}

_hostid_rst() {
	_hostid_gen
	printf "%s" ${uuid} >${hostid_file}
	if [ $? -ne 0 ]; then
		printf "WARN[%d] Could not store uuid: %s\n" $? ${hostid_file}
	fi
	return
}

if [ -r "${hostid_file}" ]; then
	_hostid_set "$(${cat} ${hostid_file})"
else
	_hostid_gen
fi
