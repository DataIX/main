#!/bin/sh
#
# Copyright (c) 2006-2010 J. Hellenthal <jhell@DataIX.net>,
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

case $(id -u) in
        0) NAMESPACE="system"
           MESGSPACE="SYSTEM: ";;
        *) NAMESPACE="user"
           MESGSPACE="USER:   "
esac

echo -e "Using namespace \"$NAMESPACE\" to store extattr(9)"
SETATTR="setextattr ${NAMESPACE}"
NOW="`date`"

for file in $@; do
  if [ -r $file -a -f $file ]; then
      [ -x /usr/bin/stat ] && export `/usr/bin/stat -s $file`
    echo -e "FILE:   $file"
    echo -n "${MESGSPACE}Setting "
    echo -n "NAME"
      [ x"$file" != x"" ] && $SETATTR NAME "$file" $file
    echo -n ", MD5"
      [ -x /sbin/md5 ] && $SETATTR MD5 `/sbin/md5 -q $file` $file
    echo -n ", SHA256"
      [ -x /sbin/sha256 ] && $SETATTR SHA256 `/sbin/sha256 -q $file` $file
    echo -n ", SIZE"
      [ x"$st_size" != x"" ] && $SETATTR SIZE $st_size $file
    echo -n ", CREATED"
      [ x"$st_birthtime" != x"" ] && $SETATTR CREATED "`date -j -r $st_birthtime`" $file
    echo -n ", TIMESTAMP"
      [ x"$NOW" != x"" ] && $SETATTR TIMESTAMP "`date`" $file
    echo "... [DONE]"
  fi
done
