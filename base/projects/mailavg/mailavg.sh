#!/bin/sh
#
# Copyright (c) 2006-2011 Jason J. Hellenthal <jhell@DataIX.net>,
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
# mailavg - average size of files in /var/mail
#
# Written by Wes Morgan, morgan@engr.uky.edu, 2 Feb 90
# Rewritten by Jason Hellenthal, jasonh@myrealbox.com, 9 Jun 06
#

ls -1Fs /var/mail | awk '
   { if(NR != 0) {
       total += $1; 
       count += 1;
       size = $1 + 0; 
       if(size == 0) zerocount+=1;
       if(size > 0 && size <= 4) over0+=1;
       if(size > 4 && size <= 8) over4+=1;
       if(size > 8 && size <= 16) over8+=1;
       if(size > 16 && size <= 32) over16+=1;
       if(size > 32 && size <= 64) over32+=1;
       if(size > 64 && size <= 128) over64+=1;
       if(size > 128 && size <= 256) over128+=1;
       if(size > 256 && size <= 512) over256+=1;
       if(size > 512 && size <= 1024) over512+=1;
       if(size > 1024 && size <= 2048) over1024+=1;
       if(size > 2048 && size <= 3072) over2048+=1;
       if(size > 3072 && size <= 4096) over3072+=1;
       if(size > 4096 && size <= 5120) over4096+=1;
       if(size > 5120 && size <= 10240) over5120+=1;
       if(size > 10240 && size <= 15360) over10240+=1;
       if(size > 15360 && size <= 20480) over15360+=1;
       if(size > 20480 && size <= 25600) over20480+=1;
       if(size > 25600 && size <= 51200) over25600+=1;
       if(size > 51200 && size <= 102400) over51200+=1;
       if(size > 102400 && size <= 204800) over102400+=1;
       if(size > 204800 && size <= 307200) over204800+=1;
       if(size > 307200 && size <= 409600) over307200+=1;
       if(size > 409600 && size <= 512000) over409600+=1;
       if(size > 512000 && size <= 1024000) over512000+=1;
       if(size > 1024000) over1024000+=1;
       }
   }
   END { printf("\nSpool:     /var/mail\n")
         printf("Mboxes:    %d\n", count)
         printf("Empty:     %d\n", zerocount)
         printf("Blocks:    %d\n", total)
         printf("Average: %6.2f blocks\n", total/count)
         printf("\n            Distribution\n")
         printf("Blocks:                        Mboxes:\n")
	 printf("--------------------------------------\n")
         printf(" 1      <-> 4 ................. %d\n",over0)
         printf(" 5      <-> 8 ................. %d\n",over4)
         printf(" 9      <-> 16 ................ %d\n",over8)
         printf(" 17     <-> 32 ................ %d\n",over16)
         printf(" 33     <-> 64 ................ %d\n",over32)
         printf(" 65     <-> 128 ............... %d\n",over64)
         printf(" 129    <-> 256 ............... %d\n",over128)
         printf(" 257    <-> 512 ............... %d\n",over256)
         printf(" 513    <-> 1024 .............. %d\n",over512)
         printf(" 1025   <-> 1536 .............. %d\n",over1024)
         printf(" 1537   <-> 2048 .............. %d\n",over1536)
         printf(" 2049   <-> 3072 .............. %d\n",over2048)
         printf(" 3073   <-> 4096 .............. %d\n",over3072)
         printf(" 4097   <-> 5120 .............. %d\n",over4096)
         printf(" 5121   <-> 10240 ............. %d\n",over5120)
         printf(" 10241  <-> 15360 ............. %d\n",over10240)
         printf(" 15361  <-> 20480 ............. %d\n",over15360)
         printf(" 20481  <-> 25600 ............. %d\n",over20480)
         printf(" 25601  <-> 51200 ............. %d\n",over25600)
         printf(" 51201  <-> 102400 ............ %d\n",over51200)
         printf(" 102401 <-> 204800 ............ %d\n",over102400)
         printf(" 204801 <-> 307200 ............ %d\n",over204800)
         printf(" 307201 <-> 409600 ............ %d\n",over307200)
         printf(" 409601 <-> 512000 ............ %d\n",over409600)
         printf(" 512001 <-> 1024000 ........... %d\n",over512000)
         printf(" Over:      1024000 ........... %d\n",over1024000)
	 printf("--------------------------------------\n")
       }'
