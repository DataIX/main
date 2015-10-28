#!/usr/bin/perl
#
# Copyright (c) 2006-2015 Jason J. Hellenthal <jhellenthal@DataIX.net>,
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

use File::Copy;

@V=(
	0x64, 0x73, 0x66, 0x64, 0x3b, 0x6b, 0x66, 0x6f, 0x41, 0x2c, 0x2e,
	0x69, 0x79, 0x65, 0x77, 0x72, 0x6b, 0x6c, 0x64, 0x4a, 0x4b, 0x44,
	0x48, 0x53, 0x55, 0x42, 0x73, 0x67, 0x76, 0x63, 0x61, 0x36, 0x39,
	0x38, 0x33, 0x34, 0x6e, 0x63, 0x78, 0x76, 0x39, 0x38, 0x37, 0x33,
	0x32, 0x35, 0x34, 0x6b, 0x3b, 0x66, 0x67, 0x38, 0x37
);

if ($ARGV[0] eq ""){
	print "cisco Vigenere M7 Password Decryption Utility.\n";
	print "Usage:\n";
	print " cdecrypt 04480E051A33490E	# Reveals a single password\n";
	print " cdecrypt switch-confg		# Changes all passwords in switch-confg\n";
}

if(open(F,"<$ARGV[0]")){
	open(FO,">cdcout.rcf") || die("Cannot open 'cdcout.rcf' for writing ($!)\n");
		while(<F>){
			if (/(.*password\s)(7\s)([0-9a-fA-F]{4,})/){
				my $d=Decrypt($3);
				s/(.*password\s)(7\s)([0-9a-fA-F]{4,})/$1$d/;
			}
		print FO $_;
	}
	close(F);
	close(FO);
	copy($ARGV[0],"$ARGV[0].orig")||die("Cannot copy '$ARGV[0]' to '$ARGV[0].orig'");
	copy("cdcout.rcf",$ARGV[0])||die("Cannot copy '$ARGV[0]' to '$ARGV[0].orig'");
	unlink "cdcout.rcf";
	} else {
		print Decrypt($ARGV[0]) . "\n";
}

sub Decrypt{
	my $pw=shift(@_);
	my $i=substr($pw,0,2);
	my $c=2;
	my $r="";
	while ($c<length($pw)){
		$r.=chr(hex(substr($pw,$c,2))^$V[$i++]);
		$c+=2;
		$i%=53;
	}
	return $r;
}
