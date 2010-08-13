#!/usr/bin/perl
#
# $Id$
#
# Copyright (c) 2008, Ben Rockwood <benr@cuddletech.com>,
# Copyright (c) 2010 Jason J. Hellenthal  <jhell@dataix.net>,
# Copyright (c) 2010 Martin Matuska <mm@FreeBSD.org>,
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

# If you are having troubles when using this script from cron(8) please try
# adjusting your PATH before reporting problems.
#
# This script as of this version of FreeBSD that coincides with the date and
# revision at the top of this file uses the binaries from:
#
# /bin /usr/bin & /sbin
#
# Binaries used are:
#
# dc(1), kldstat(8), sed(1), sysctl(8), uname(1), vmstat(8), & xargs(1)
#
# Binaries that I am working on phasing out are:
#
# dc(1), sed(1) & xargs(1)

use strict;

my $useheader = 0;	# Change to 1 to enable FreeBSD header.
my $usetunable = 1;	# Change to 0 to disable sysctl MIB spill.

sub hline(){
	print "\n------------------------------------------------------------------------\n\n";
}

my $Kstat;
my @arcstats = `sysctl 'kstat.zfs.misc.arcstats'`;
foreach my $arcstats (@arcstats) {
	chomp $arcstats;
	my ($name,$value) = split /:/, $arcstats;
	my @z = split /\./, $name;
	my $n = pop @z;
	${Kstat}->{zfs}->{0}->{arcstats}->{$n} = $value;
}

### System Information / FreeBSD ###
my $daydate = localtime;
my $zpl = `sysctl -n 'vfs.zfs.version.zpl'`;
my $spa = `sysctl -n 'vfs.zfs.version.spa'`;
my $phys_memory = `sysctl -n 'hw.physmem'`;
my $phys_memory_MiB = ($phys_memory / 1048576);
my $ktext = `kldstat |awk \'BEGIN {print "16i 0";} NR>1 {print toupper(\$4) "+"} END {print "p"}\' |dc`;
my $kdata = `vmstat -m |sed -Ee '1s/.*/0/;s/.* ([0-9]+)K.*/\\1+/;\$s/\$/1024*p/' |dc`;
my $kmem = ( $ktext + $kdata );
my $kmem_MiB = ( $kmem / 1048576 );
my $ktext_perc = ( 100 * ( $ktext / $kmem ));
my $kdata_perc = ( 100 * ( $kdata / $kmem ));
my $kdata_MiB = ( $kdata / 1048576 );
my $ktext_MiB = ( $ktext / 1048576 );
my $throttle = ${Kstat}->{zfs}->{0}->{arcstats}->{memory_throttle_count};

print "\n------------------------------------------------------------------------\n";
printf("ZFS Subsystem Report\t\t\t\t%s", $daydate);
if ($useheader != 0) {
	my $unamev = `uname -v |sed 's/@.*//' |xargs`;
	my $unamem = `sysctl -n 'hw.machine'`;
	my $unamep = `sysctl -n 'hw.machine_arch'`;
	my $osreldate = `sysctl -n 'kern.osreldate'`;
	my $sysuptime = `uptime`;
	hline();
	printf("%s", $unamev);
	print "\n";
	printf("Kernel Version:\t\t\t\t\t%d (osreldate)\n", $osreldate);
	print "\n";
	printf("Hardware Platform:\t\t\t\t%s", $unamem);
	printf("Processor Architecture:\t\t\t\t%s", $unamep);
	print "\n";
	printf("%s", $sysuptime);
}
hline();
printf("Physical Memory:\t\t\t\t%0.2fM\n", $phys_memory_MiB);
print "\n";
printf("Kernel Memory:\t\t\t\t\t%0.2fM\n", $kmem_MiB);
printf("DATA:\t\t\t\t\t%0.2f%%\t%0.2fM\n", $kdata_perc, $kdata_MiB);
printf("TEXT:\t\t\t\t\t%0.2f%%\t%0.2fM\n", $ktext_perc, $ktext_MiB);

hline();
print "ARC Summary:\n";
printf("\tStorage pool Version:\t\t\t%d\t(spa)\n", $spa);
printf("\tFilesystem Version:\t\t\t%d\t(zpl)\n", $zpl);
printf("\tMemory Throttle Count:\t\t\t%d\n", $throttle);
print "\n";

### ARC Misc. ###
my $deleted = ${Kstat}->{zfs}->{0}->{arcstats}->{deleted};
my $recycle_miss = ${Kstat}->{zfs}->{0}->{arcstats}->{recycle_miss};
my $mutex_miss = ${Kstat}->{zfs}->{0}->{arcstats}->{mutex_miss};
my $evict_skip = ${Kstat}->{zfs}->{0}->{arcstats}->{evict_skip};

print "ARC Misc:\n";
printf("\tDeleted:\t\t\t\t%d\n", $deleted);
printf("\tRecycle Misses:\t\t\t\t%d\n", $recycle_miss);
printf("\tMutex Misses:\t\t\t\t%d\n", $mutex_miss);
printf("\tEvict Skips:\t\t\t\t%d\n", $mutex_miss);
print "\n";

### ARC Sizing ###
my $mru_size = ${Kstat}->{zfs}->{0}->{arcstats}->{p};
my $mru_size_MiB = ($mru_size / 1048576);

my $target_size = ${Kstat}->{zfs}->{0}->{arcstats}->{c};
my $target_size_MiB = ($target_size / 1048576);
my $target_min_size = ${Kstat}->{zfs}->{0}->{arcstats}->{c_min};
my $target_min_size_MiB = ($target_min_size / 1048576);
my $target_max_size = ${Kstat}->{zfs}->{0}->{arcstats}->{c_max};
my $target_max_size_MiB = ($target_max_size / 1048576);
my $target_size_ratio = ($target_max_size / $target_min_size);
my $target_size_perc = 100*($target_size / $target_max_size);
my $target_size_min_perc = 100*($target_min_size / $target_max_size);

my $arc_size = ${Kstat}->{zfs}->{0}->{arcstats}->{size};
my $arc_size_MiB = ($arc_size / 1048576);
my $arc_size_perc = 100*($arc_size / $target_max_size);

print "ARC Size:\n";
printf("\tCurrent Size:\t\t\t%0.2f%%\t%0.2fM\t(arcsize)\n",
	$arc_size_perc,  $arc_size_MiB);
printf("\tTarget Size: (Adaptive)\t\t%0.2f%%\t%0.2fM\t(c)\n",
	$target_size_perc, $target_size_MiB);
printf("\tMin Size (Hard Limit):\t\t%0.2f%%\t%0.2fM\t(c_min)\n",
	$target_size_min_perc, $target_min_size_MiB);
printf("\tMax Size (High Water):\t\t~%d:1\t%0.2fM\t(c_max)\n",
	$target_size_ratio, $target_max_size_MiB);

print "\nARC Size Breakdown:\n";
if ($arc_size > $target_size) {
	my $mfu_size = ($arc_size - $mru_size);
	my $mfu_size_MiB = ($mfu_size / 1048576);
	my $mru_perc = 100*($mru_size / $arc_size);
	my $mfu_perc = 100*($mfu_size / $arc_size);
	printf("\tRecently Used Cache Size:\t%0.2f%%\t%0.2fM\t(p)\n",
		$mru_perc, $mru_size_MiB);
	printf("\tFrequently Used Cache Size:\t%0.2f%%\t%0.2fM\t(arcsize-p)\n",
		$mfu_perc, $mfu_size_MiB);
}

if ($arc_size < $target_size) {
	my $mfu_size = ($target_size - $mru_size);
	my $mfu_size_MiB = ($mfu_size / 1048576);
	my $mru_perc = 100*($mru_size / $target_size);
	my $mfu_perc = 100*($mfu_size / $target_size);
	printf("\tRecently Used Cache Size:\t%0.2f%%\t%0.2fM\t(p)\n",
		$mru_perc, $mru_size_MiB);
	printf("\tFrequently Used Cache Size:\t%0.2f%%\t%0.2fM\t(c-p)\n",
		$mfu_perc, $mfu_size_MiB);
}
print "\n";

### ARC Hash ###
my $hash_elements = ${Kstat}->{zfs}->{0}->{arcstats}->{hash_elements};
my $hash_elements_max = ${Kstat}->{zfs}->{0}->{arcstats}->{hash_elements_max};
my $hash_collisions = ${Kstat}->{zfs}->{0}->{arcstats}->{hash_collisions};
my $hash_chains = ${Kstat}->{zfs}->{0}->{arcstats}->{hash_chains};
my $hash_chain_max = ${Kstat}->{zfs}->{0}->{arcstats}->{hash_chain_max};
my $hash_elements_perc = 100*($hash_elements / $hash_elements_max);

print "ARC Hash Breakdown:\n";
printf("\tElements Max:\t\t\t\t%d\n",
	$hash_elements_max);
printf("\tElements Current:\t\t%0.2f%%\t%d\n",
	$hash_elements_perc, $hash_elements);
printf("\tCollisions:\t\t\t\t%d\n",
	$hash_collisions);
printf("\tChain Max:\t\t\t\t%d\n",
	$hash_chain_max);
printf("\tChains:\t\t\t\t\t%d\n",
	$hash_chains);
print "\n";

### ARC Efficency ###
my $arc_hits = ${Kstat}->{zfs}->{0}->{arcstats}->{hits};
my $arc_misses = ${Kstat}->{zfs}->{0}->{arcstats}->{misses};
my $arc_accesses_total = ($arc_hits + $arc_misses);

my $arc_hit_perc = 100*($arc_hits / $arc_accesses_total);
my $arc_miss_perc = 100*($arc_misses / $arc_accesses_total);

my $mfu_hits = ${Kstat}->{zfs}->{0}->{arcstats}->{mfu_hits};
my $mru_hits = ${Kstat}->{zfs}->{0}->{arcstats}->{mru_hits};
my $mfu_ghost_hits = ${Kstat}->{zfs}->{0}->{arcstats}->{mfu_ghost_hits};
my $mru_ghost_hits = ${Kstat}->{zfs}->{0}->{arcstats}->{mru_ghost_hits};
my $anon_hits = $arc_hits - ($mfu_hits + $mru_hits + $mfu_ghost_hits + $mru_ghost_hits);

my $real_hits = ($mfu_hits + $mru_hits);
my $real_hits_perc = ( 100 * ( $real_hits / $arc_accesses_total ));

### These should be based on TOTAL HITS ($arc_hits) ###
my $anon_hits_perc = ( 100 * ( $anon_hits / $arc_hits ));
my $mfu_hits_perc = ( 100 * ( $mfu_hits / $arc_hits ));
my $mru_hits_perc = ( 100 * ( $mru_hits / $arc_hits ));
my $mfu_ghost_hits_perc = ( 100 * ( $mfu_ghost_hits / $arc_hits ));
my $mru_ghost_hits_perc = ( 100 * ( $mru_ghost_hits / $arc_hits ));

my $demand_data_hits = ${Kstat}->{zfs}->{0}->{arcstats}->{demand_data_hits};
my $demand_metadata_hits = ${Kstat}->{zfs}->{0}->{arcstats}->{demand_metadata_hits};
my $prefetch_data_hits = ${Kstat}->{zfs}->{0}->{arcstats}->{prefetch_data_hits};
my $prefetch_metadata_hits = ${Kstat}->{zfs}->{0}->{arcstats}->{prefetch_metadata_hits};

my $demand_data_hits_perc = ( 100 * ( $demand_data_hits / $arc_hits ));
my $demand_metadata_hits_perc = ( 100 * ( $demand_metadata_hits / $arc_hits ));
my $prefetch_data_hits_perc = ( 100 * ( $prefetch_data_hits / $arc_hits ));
my $prefetch_metadata_hits_perc = ( 100 * ( $prefetch_metadata_hits / $arc_hits ));

my $demand_data_misses = ${Kstat}->{zfs}->{0}->{arcstats}->{demand_data_misses};
my $demand_metadata_misses = ${Kstat}->{zfs}->{0}->{arcstats}->{demand_metadata_misses};
my $prefetch_data_misses = ${Kstat}->{zfs}->{0}->{arcstats}->{prefetch_data_misses};

my $prefetch_metadata_misses = ${Kstat}->{zfs}->{0}->{arcstats}->{prefetch_metadata_misses};

my $demand_data_misses_perc = ( 100 * ( $demand_data_misses / $arc_misses ));
my $demand_metadata_misses_perc = ( 100 * ($demand_metadata_misses / $arc_misses ));
my $prefetch_data_misses_perc = ( 100 * ( $prefetch_data_misses / $arc_misses ));
my $prefetch_metadata_misses_perc = ( 100 * ( $prefetch_metadata_misses / $arc_misses ));

my $prefetch_data_total = ( $prefetch_data_hits + $prefetch_data_misses );
my $prefetch_data_perc = "00";
if ($prefetch_data_total > 0) {
        $prefetch_data_perc = ( 100 * ( $prefetch_data_hits / $prefetch_data_total ));
}

my $demand_data_total = ( $demand_data_hits + $demand_data_misses );
my $demand_data_perc = ( 100 * ( $demand_data_hits / $demand_data_total ));

print "ARC Efficiency:\n";
printf("\tCache Access Total:\t\t\t%d\n",
	$arc_accesses_total);
printf("\tCache Hit Ratio:\t\t%0.2f%%\t%d\n",
	$arc_hit_perc, $arc_hits);
printf("\tCache Miss Ratio:\t\t%0.2f%%\t%d\n",
	$arc_miss_perc, $arc_misses);
printf("\tActual Hit Ratio:\t\t%0.2f%%\t%d\n",
	$real_hits_perc, $real_hits);
print "\n";
printf("\tData Demand Efficiency:\t\t%0.2f%%\n",
	$demand_data_perc);

if ($prefetch_data_total > 0){ 
	printf("\tData Prefetch Efficiency:\t%0.2f%%\n",
		$prefetch_data_perc);
}
print "\n";

print "\tCACHE HITS BY CACHE LIST:\n";
if ( $anon_hits > 0 ){
	printf("\t  Anonymously Used:\t\t%0.2f%%\t%d\n",
		$anon_hits_perc, $anon_hits);
}

printf("\t  Most Recently Used:\t\t%0.2f%%\t%d\t(mru)\n",
	$mru_hits_perc, $mru_hits);
printf("\t  Most Frequently Used:\t\t%0.2f%%\t%d\t(mfu)\n",
	$mfu_hits_perc, $mfu_hits);
printf("\t  Most Recently Used Ghost:\t%0.2f%%\t%d\t(mru_ghost)\n",
	$mru_ghost_hits_perc, $mru_ghost_hits);
printf("\t  Most Frequently Used Ghost:\t%0.2f%%\t%d\t(mfu_ghost)\n",
	$mfu_ghost_hits_perc, $mfu_ghost_hits);

print "\n\tCACHE HITS BY DATA TYPE:\n";
printf("\t  Demand Data:\t\t\t%0.2f%%\t%d\n",
	$demand_data_hits_perc, $demand_data_hits);
printf("\t  Prefetch Data:\t\t%0.2f%%\t%d\n",
	$prefetch_data_hits_perc, $prefetch_data_hits);
printf("\t  Demand Metadata:\t\t%0.2f%%\t%d\n",
	$demand_metadata_hits_perc, $demand_metadata_hits);
printf("\t  Prefetch Metadata:\t\t%0.2f%%\t%d\n",
	$prefetch_metadata_hits_perc, $prefetch_metadata_hits);

print "\n\tCACHE MISSES BY DATA TYPE:\n";
printf("\t  Demand Data:\t\t\t%0.2f%%\t%d\n",
	$demand_data_misses_perc, $demand_data_misses);
printf("\t  Prefetch Data:\t\t%0.2f%%\t%d\n",
	$prefetch_data_misses_perc, $prefetch_data_misses);
printf("\t  Demand Metadata:\t\t%0.2f%%\t%d\n",
	$demand_metadata_misses_perc, $demand_metadata_misses);
printf("\t  Prefetch Metadata:\t\t%0.2f%%\t%d\n",
	$prefetch_metadata_misses_perc, $prefetch_metadata_misses);

### L2 ARC Stats Sysctl's ###
my $l2_hits = ${Kstat}->{zfs}->{0}->{arcstats}->{l2_hits};
my $l2_misses = ${Kstat}->{zfs}->{0}->{arcstats}->{l2_misses};
my $l2_feeds = ${Kstat}->{zfs}->{0}->{arcstats}->{l2_feeds};
my $l2_rw_clash = ${Kstat}->{zfs}->{0}->{arcstats}->{l2_rw_clash};
my $l2_writes_sent = ${Kstat}->{zfs}->{0}->{arcstats}->{l2_writes_sent};
my $l2_writes_done = ${Kstat}->{zfs}->{0}->{arcstats}->{l2_writes_done};
my $l2_writes_error = ${Kstat}->{zfs}->{0}->{arcstats}->{l2_writes_error};
my $l2_writes_hdr_miss = ${Kstat}->{zfs}->{0}->{arcstats}->{l2_writes_hdr_miss};
my $l2_evict_lock_retry = ${Kstat}->{zfs}->{0}->{arcstats}->{l2_evict_lock_retry};
my $l2_evict_reading = ${Kstat}->{zfs}->{0}->{arcstats}->{l2_evict_reading};
my $l2_free_on_write = ${Kstat}->{zfs}->{0}->{arcstats}->{l2_free_on_write};
my $l2_abort_lowmem = ${Kstat}->{zfs}->{0}->{arcstats}->{l2_abort_lowmem};
my $l2_cksum_bad = ${Kstat}->{zfs}->{0}->{arcstats}->{l2_cksum_bad};
my $l2_io_error = ${Kstat}->{zfs}->{0}->{arcstats}->{l2_io_error};
my $l2_size = ${Kstat}->{zfs}->{0}->{arcstats}->{l2_size};
my $l2_hdr_size = ${Kstat}->{zfs}->{0}->{arcstats}->{l2_hdr_size};
my $l2_access_total = ( $l2_hits + $l2_misses );
my $l2_health_count = ( $l2_writes_error + $l2_cksum_bad + $l2_io_error );

### L2 ARC ###
if ($l2_size > 0 & $l2_access_total > 0) {
	### L2 ARC Stats Calculations ###
	my $l2_hdr_size_perc = ( 100 * ( $l2_hdr_size / $l2_size ));
	my $l2_hits_perc = ( 100 * ( $l2_hits / ( $l2_access_total )));
	my $l2_misses_perc = ( 100 * ( $l2_misses / ( $l2_access_total )));
	my $l2_writes_done_perc = ( 100 * ( $l2_writes_done / $l2_writes_sent ));
	my $l2_writes_error_perc = ( 100 * ( $l2_writes_error / $l2_writes_sent ));
	my $l2_size_MiB = ( $l2_size / 1048576 );
	my $l2_hdr_size_MiB = ( $l2_hdr_size / 1048576 );

	hline();

	if ($l2_health_count > 0) {
		printf("L2 ARC Summary: (%s)\n", "DEGRADED");
	} else {
		printf("L2 ARC Summary: (%s)\n", "HEALTHY");
	}
	printf("\tLow Memory Aborts:\t\t\t%d\n",
		$l2_abort_lowmem);
	printf("\tFree on Write:\t\t\t\t%d\n",
		$l2_free_on_write);
	printf("\tR/W Clashes:\t\t\t\t%d\n",
		$l2_rw_clash);
	printf("\tBad Checksums:\t\t\t\t%d\n",
		$l2_cksum_bad);
	printf("\tIO Errors:\t\t\t\t%d\n",
		$l2_io_error);
	print "\n";
	
	print "L2 ARC Size:\n";
	printf("\tCurrent Size: (Adaptive)\t\t%0.2fM\n",
		$l2_size_MiB);
	printf("\tHeader Size:\t\t\t%0.2f%%\t%0.2fM\n",
		$l2_hdr_size_perc, $l2_hdr_size_MiB);
	print "\n";
	
	if (($l2_evict_lock_retry + $l2_evict_reading) > 0) {
		print "L2 ARC Evicts:\n";
		printf("\tLock Retries:\t\t\t\t%d\n",
			$l2_evict_lock_retry);
		printf("\tUpon Reading:\t\t\t\t%d\n",
			$l2_evict_reading);
		print "\n";
	}
	print "L2 ARC Breakdown:\n";
	printf("\tAccess Total:\t\t\t\t%d\n",
		$l2_access_total);
	printf("\tHit Ratio:\t\t\t%0.2f%%\t%d\n",
		$l2_hits_perc, $l2_hits);
	printf("\tMiss Ratio:\t\t\t%0.2f%%\t%d\n",
		$l2_misses_perc, $l2_misses);
	printf("\tFeeds:\t\t\t\t\t%d\n",
		$l2_feeds);
	print "\n";
	
	if ($l2_writes_done != $l2_writes_sent) {
		printf("\tWRITES: (%s)\n", "FAULTED");
		printf("\t  Sent Total:\t\t\t\t%d\n",
			$l2_writes_sent);
		printf("\t  Done Ratio:\t\t\t%0.2f%%\t%d\n",
			$l2_writes_done_perc, $l2_writes_done);
		printf("\t  Error Ratio:\t\t\t%0.2f%%\t%d\n",
			$l2_writes_error_perc, $l2_writes_error);
	} else {
		print "\tWRITES:\n";
		printf("\t  Sent Total:\t\t\t%0.2f%\t%d\n",
			100, $l2_writes_sent);
	}
}

### DMU Stats ###
my @zfetch_stats = `sysctl 'kstat.zfs.misc.zfetchstats'`;
foreach my $zfetch_stats (@zfetch_stats) {
	chomp $zfetch_stats;
	my ($name,$value) = split /:/, $zfetch_stats;
	my @z = split /\./, $name;
	my $n = pop @z;
	${Kstat}->{zfs}->{0}->{zfetch_stats}->{$n} = $value;
}

### DMU Stats Sysctl's ###
my $zfetch_hits = ${Kstat}->{zfs}->{0}->{zfetch_stats}->{hits};
my $zfetch_misses = ${Kstat}->{zfs}->{0}->{zfetch_stats}->{misses};
my $zfetch_colinear_hits = ${Kstat}->{zfs}->{0}->{zfetch_stats}->{colinear_hits};
my $zfetch_colinear_misses = ${Kstat}->{zfs}->{0}->{zfetch_stats}->{colinear_misses};
my $zfetch_stride_hits = ${Kstat}->{zfs}->{0}->{zfetch_stats}->{stride_hits};
my $zfetch_stride_misses = ${Kstat}->{zfs}->{0}->{zfetch_stats}->{stride_misses};
my $zfetch_reclaim_successes = ${Kstat}->{zfs}->{0}->{zfetch_stats}->{reclaim_successes};
my $zfetch_reclaim_failures = ${Kstat}->{zfs}->{0}->{zfetch_stats}->{reclaim_failures};
my $zfetch_streams_resets = ${Kstat}->{zfs}->{0}->{zfetch_stats}->{streams_resets};
my $zfetch_streams_noresets = ${Kstat}->{zfs}->{0}->{zfetch_stats}->{streams_noresets};
my $zfetch_bogus_streams = ${Kstat}->{zfs}->{0}->{zfetch_stats}->{bogus_streams};
my $zfetch_access_total = ( $zfetch_hits + $zfetch_misses );
my $zfetch_colinear_total = ( $zfetch_colinear_hits + $zfetch_colinear_misses );
my $zfetch_stride_total = ( $zfetch_stride_hits + $zfetch_stride_misses );
my $zfetch_health_count = ( $zfetch_bogus_streams );

if ($zfetch_access_total > 0) {
	my $zfetch_hits_perc = ( 100 * ( $zfetch_hits / ( $zfetch_access_total )));
	my $zfetch_misses_perc = ( 100 * ( $zfetch_misses / ( $zfetch_access_total )));
	my $zfetch_colinear_hits_perc = ( 100 * ( $zfetch_colinear_hits / ( $zfetch_colinear_total )));
	my $zfetch_colinear_misses_perc = ( 100 * ( $zfetch_colinear_misses / ( $zfetch_colinear_total )));
	my $zfetch_stride_hits_perc = ( 100 * ( $zfetch_stride_hits / ( $zfetch_stride_total )));
	my $zfetch_stride_misses_perc = ( 100 * ( $zfetch_stride_misses / ( $zfetch_stride_total )));

	hline();
	if ($zfetch_health_count > 0) {
		printf("File-Level Prefetch: (%s)\n\n", "DEGRADED");
	} else {
		printf("File-Level Prefetch: (%s)\n\n", "HEALTHY");
	};
	print "DMU Efficiency:\n";
	printf("\tAccess Total:\t\t\t\t%d\n",
		$zfetch_access_total);
	printf("\tHit Ratio:\t\t\t%0.2f%%\t%d\n",
		$zfetch_hits_perc, $zfetch_hits);
	printf("\tMiss Ratio:\t\t\t%0.2f%%\t%d\n",
		$zfetch_misses_perc, $zfetch_misses);
	print "\n";

	printf("\tColinear Access Total:\t\t\t%d\n",
		$zfetch_colinear_total);
	printf("\tColinear Hit Ratio:\t\t%0.2f%%\t%d\n",
		$zfetch_colinear_hits_perc, $zfetch_colinear_hits);
	printf("\tColinear Miss Ratio:\t\t%0.2f%%\t%d\n",
		$zfetch_colinear_misses_perc, $zfetch_colinear_misses);
	print "\n";

	printf("\tStride Access Total:\t\t\t%d\n",
		$zfetch_stride_total);
	printf("\tStride Hit Ratio:\t\t%0.2f%%\t%d\n",
		$zfetch_stride_hits_perc, $zfetch_stride_hits);
	printf("\tStride Miss Ratio:\t\t%0.2f%%\t%d\n",
		$zfetch_stride_misses_perc, $zfetch_stride_misses);
	print "\n";

        if ($zfetch_health_count > 0) {
                printf("DMU Misc: (%s)\n", "FAULTED");
        } else {
                print "DMU Misc:\n";
        };
	printf("\tReclaim Successes:\t\t\t%d\n",
		$zfetch_reclaim_successes);
	printf("\tReclaim Failures:\t\t\t%d\n",
		$zfetch_reclaim_failures);
	printf("\tStream Resets:\t\t\t\t%d\n",
		$zfetch_streams_resets);
	printf("\tStream NoResets:\t\t\t%d\n",
		$zfetch_streams_noresets);
	printf("\tBogus Streams:\t\t\t\t%d\n",
		$zfetch_bogus_streams);
}

### VDEV Cache Stats ###
my @vdev_cache_stats = `sysctl 'kstat.zfs.misc.vdev_cache_stats'`;
foreach my $vdev_cache_stats (@vdev_cache_stats) {
	chomp $vdev_cache_stats;
	my ($name,$value) = split /:/, $vdev_cache_stats;
	my @z = split /\./, $name;
	my $n = pop @z;
	${Kstat}->{zfs}->{0}->{vdev_cache_stats}->{$n} = $value;
}

my $vdev_cache_delegations = ${Kstat}->{zfs}->{0}->{vdev_cache_stats}->{delegations};
my $vdev_cache_misses = ${Kstat}->{zfs}->{0}->{vdev_cache_stats}->{misses};
my $vdev_cache_hits = ${Kstat}->{zfs}->{0}->{vdev_cache_stats}->{hits};
my $vdev_cache_total = ( $vdev_cache_misses + $vdev_cache_hits );
my $vdev_cache_hits_perc = ( 100 * ( $vdev_cache_hits / $vdev_cache_total ));
my $vdev_cache_misses_perc = ( 100 * ( $vdev_cache_misses / $vdev_cache_total ));
my $vdev_cache_delegations_perc = ( 100 * ( $vdev_cache_delegations / $vdev_cache_total ));

hline();
print "VDEV Cache Summary:\n";
printf("\tAccess Total:\t\t\t\t%d\n",
	$vdev_cache_total);
printf("\tHit Ratio:\t\t\t%0.2f%%\t%d\n",
	$vdev_cache_hits_perc, $vdev_cache_hits);
printf("\tMiss Ratio:\t\t\t%0.2f%%\t%d\n",
	$vdev_cache_misses_perc, $vdev_cache_misses);
printf("\tDelegations:\t\t\t\t%d\n",
	$vdev_cache_delegations);

if ($usetunable != 0) {
	### Tunables FreeBSD  ###
	my @Tunable = qw(
		kern.maxusers
		vfs.zfs
		vm.kmem_size
		vm.kmem_size_scale
		vm.kmem_size_min
		vm.kmem_size_max
	);
	my @tunable = `sysctl -e @Tunable`;
	hline();
	print "ZFS Tunable (sysctl):\n";
	foreach my $tunable (@tunable){
		chomp($tunable);
		print "\t$tunable\n";
	}
}
print "\n------------------------------------------------------------------------\n";
