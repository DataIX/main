#!/usr/bin/env -iS perl
#
# $Id$
#
# Copyright (c) 2008 Ben Rockwood <benr@cuddletech.com>,
# Copyright (c) 2010 Martin Matuska <mm@FreeBSD.org>,
# Copyright (c) 2010-2011 Jason J. Hellenthal <jhell@DataIX.net>,
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
# /usr/bin & /sbin
#
# Binaries used are:
#
# dc(1), kldstat(8), sed(1), sysctl(8) & vmstat(8)
#
# Binaries that I am working on phasing out are:
#
# dc(1) & sed(1)

use strict;
use Getopt::Std;
use Switch 'Perl5', 'Perl6';

my $usetunable = 1;	# Change to 0 to disable sysctl MIB spill.

sub hline {
	print "\n------------------------------------------------------------------------\n\n";
}

sub fBytes {
	my $kbytes = (1024);
	my $mbytes = ($kbytes * $kbytes);
	my $gbytes = ($mbytes * $kbytes);
	my $tbytes = ($gbytes * $kbytes);
	my $pbytes = ($tbytes * $kbytes);
	my $ebytes = ($pbytes * $kbytes);
	my $zbytes = ($ebytes * $kbytes);
	my $ybytes = ($zbytes * $kbytes);

	my $Bytes = $_[0] || 0;
	defined($Bytes) or $Bytes = 0;

	my $Decimal = $_[1] || 2;
	defined($Decimal) or $Decimal = 2;

	if ($Bytes >= $ybytes) {
		return sprintf('%0.' . $Decimal . 'f', ($Bytes / $ybytes)) . "\tYiB";
	} elsif ($Bytes >= $zbytes) {
		return sprintf('%0.' . $Decimal . 'f', ($Bytes / $zbytes)) . "\tZiB";
	} elsif ($Bytes >= $ebytes) {
		return sprintf('%0.' . $Decimal . 'f', ($Bytes / $ebytes)) . "\tEiB";
	} elsif ($Bytes >= $pbytes) {
		return sprintf('%0.' . $Decimal . 'f', ($Bytes / $pbytes)) . "\tPiB";
	} elsif ($Bytes >= $tbytes) {
		return sprintf('%0.' . $Decimal . 'f', ($Bytes / $tbytes)) . "\tTiB";
	} elsif ($Bytes >= $gbytes) {
		return sprintf('%0.' . $Decimal . 'f', ($Bytes / $gbytes)) . "\tGiB";
	} elsif ($Bytes >= $mbytes) {
		return sprintf('%0.' . $Decimal . 'f', ($Bytes / $mbytes)) . "\tMiB";
	} elsif ($Bytes >= $kbytes) {
		return sprintf( '%0.' . $Decimal . 'f', ($Bytes / $kbytes)) . "\tKiB";
	} elsif ($Bytes == 0) { return sprintf('%d', 0 . "\tBytes");
	} else { return sprintf('%d', $Bytes) . "\tBytes"; }
}

sub fHits {
	my $khits = (1000);
	my $mhits = ($khits * $khits);
	my $bhits = ($mhits * $khits);
	my $thits = ($bhits * $khits);
	my $qhits = ($thits * $khits);
	my $Qhits = ($qhits * $khits);
	my $shits = ($Qhits * $khits);
	my $Shits = ($shits * $khits);

	my $Hits = $_[0] || 0;
	defined($Hits) or $Hits = 0;

	my $Decimal = $_[1] || 2;
	defined($Decimal) or $Decimal = 2;

	if ($Hits >= $Shits) {
		return sprintf('%0.' . $Decimal . 'f', ($Hits / $Shits)) . "S";
	} elsif ($Hits >= $shits) {
		return sprintf('%0.' . $Decimal . 'f', ($Hits / $shits)) . "s";
	} elsif ($Hits >= $Qhits) {
		return sprintf('%0.' . $Decimal . 'f', ($Hits / $Qhits)) . "Q";
	} elsif ($Hits >= $qhits) {
		return sprintf('%0.' . $Decimal . 'f', ($Hits / $qhits)) . "q";
	} elsif ($Hits >= $thits) {
		return sprintf('%0.' . $Decimal . 'f', ($Hits / $thits)) . "t";
	} elsif ($Hits >= $bhits) {
		return sprintf('%0.' . $Decimal . 'f', ($Hits / $bhits)) . "b";
	} elsif ($Hits >= $mhits) {
		return sprintf('%0.' . $Decimal . 'f', ($Hits / $mhits)) . "m";
	} elsif ($Hits >= $khits) {
		return sprintf('%0.' . $Decimal . 'f', ($Hits / $khits)) . "k";
	} elsif ($Hits == 0) { return sprintf("%d", 0);
	} else { return sprintf("%d", $Hits); }
}

sub fPerc {
	my $lVal = $_[0] || 0;
	defined($lVal) or $lVal = 0;

	my $rVal = $_[1] || 0;
	defined($rVal) or $rVal = 0;

	my $Decimal = $_[2] || 2;
	defined($Decimal) or $Decimal = 2;

	if ($rVal > 0) {
		return sprintf('%0.' . $Decimal . 'f', 100*($lVal / $rVal)) . "%";
	} else { return sprintf('%0.' . $Decimal . 'f', 100) . "%"; }
}

my @Kstats = qw(
	hw.pagesize
	hw.physmem
	kern.maxusers
	vm.kmem_map_free
	vm.kmem_map_size
	vm.kmem_size
	vm.kmem_size_max
	vm.kmem_size_min
	vm.kmem_size_scale
	vm.stats
	kstat.zfs
	vfs.zfs
);

my $Kstat;
my @Kstat_pull = `/sbin/sysctl -q @Kstats`;
foreach my $kstat (@Kstat_pull) {
	chomp $kstat;
	if ($kstat =~ m/^([^:]+):\s+(.+)\s*$/s) {
		$Kstat->{$1} = $2;
	};
};

sub _system_memory {
	sub mem_rounded {
		my ($mem_size) = @_;
		my $chip_size = 1;
		my $chip_guess = ($mem_size / 8) - 1;
		while ($chip_guess != 0) {
			$chip_guess >>= 1;
			$chip_size <<= 1;
		}
		my $mem_round = (int($mem_size / $chip_size) + 1) * $chip_size;
		return $mem_round;
	}

	my $pagesize = $Kstat->{"hw.pagesize"};
	my $mem_hw = &mem_rounded($Kstat->{"hw.physmem"});
	my $mem_phys = $Kstat->{"hw.physmem"};
	my $mem_all = $Kstat->{"vm.stats.vm.v_page_count"} * $pagesize;
	my $mem_wire = $Kstat->{"vm.stats.vm.v_wire_count"} * $pagesize;
	my $mem_active = $Kstat->{"vm.stats.vm.v_active_count"} * $pagesize;
	my $mem_inactive = $Kstat->{"vm.stats.vm.v_inactive_count"} * $pagesize;
	my $mem_cache = $Kstat->{"vm.stats.vm.v_cache_count"} * $pagesize;
	my $mem_free = $Kstat->{"vm.stats.vm.v_free_count"} * $pagesize;

	my $mem_gap_vm = $mem_all - ($mem_wire + $mem_active + $mem_inactive + $mem_cache + $mem_free);

	my $mem_total = $mem_hw;
	my $mem_avail = $mem_inactive + $mem_cache + $mem_free;
	my $mem_used = $mem_total - $mem_avail;

	print "System Memory:\n";
	print "\t+-------------------------------------------------+\n";
	printf("\t%s\t%s Active,\t", fPerc($mem_active, $mem_all), fBytes($mem_active));
	printf("%s\t%s Inact\n", fPerc($mem_inactive, $mem_all), fBytes($mem_inactive));
	printf("\t%s\t%s Wired,\t", fPerc($mem_wire, $mem_all), fBytes($mem_wire));
	printf("%s\t%s Cache\n", fPerc($mem_cache, $mem_all), fBytes($mem_cache));
	printf("\t%s\t%s Free,\t", fPerc($mem_free, $mem_all), fBytes($mem_free));
	printf("%s\t%s Gap\n", fPerc($mem_gap_vm, $mem_all), fBytes($mem_gap_vm));
	print "\t+-------------------------------------------------+\n";
	printf("\tReal Installed:\t\t\t\t%s\n", fBytes($mem_hw));
	printf("\tReal Available:\t\t\t%s\t%s\n", fPerc($mem_phys, $mem_hw), fBytes($mem_phys));
	printf("\tReal Managed:\t\t\t%s\t%s\n", fPerc($mem_all, $mem_phys), fBytes($mem_all));
	print "\t+-------------------------------------------------+\n";
	printf("\tLogical Total:\t\t\t\t%s\n", fBytes($mem_total));
	printf("\tLogical Used:\t\t\t%s\t%s\n", fPerc($mem_used, $mem_total), fBytes($mem_used));
	printf("\tLogical Free:\t\t\t%s\t%s\n", fPerc($mem_avail, $mem_total), fBytes($mem_avail));
	print "\t+-------------------------------------------------+\n";
	print "\n";

	my $ktext = `/sbin/kldstat |/usr/bin/awk \'BEGIN {print "16i 0";} NR>1 {print toupper(\$4) "+"} END {print "p"}\' |/usr/bin/dc`;
	my $kdata = `/usr/bin/vmstat -m |/usr/bin/sed -Ee '1s/.*/0/;s/.* ([0-9]+)K.*/\\1+/;\$s/\$/1024*p/' |/usr/bin/dc`;
	my $kmem = ($ktext + $kdata);
	my $kmem_map_size = $Kstat->{"vm.kmem_map_size"};
	my $kmem_map_free = $Kstat->{"vm.kmem_map_free"};

	printf("Kernel Memory:\t\t\t\t\t%s\n", fBytes($kmem));
	printf("\tDATA:\t\t\t\t%s\t%s\n", fPerc($kdata, $kmem), fBytes($kdata));
	printf("\tTEXT:\t\t\t\t%s\t%s\n\n", fPerc($ktext, $kmem), fBytes($ktext));
	printf("\tKMAP:\t\t\t\t\t%s\n", fBytes($kmem_map_size));
	printf("\tFREE:\t\t\t\t%s\t%s\n", fPerc($kmem_map_free, $kmem_map_size), fBytes($kmem_map_free));
}

my $daydate = localtime; chomp $daydate;
print "\n------------------------------------------------------------------------\n";
printf("ZFS Subsystem Report\t\t\t\t%s", $daydate);
hline();

sub _arc_summary {
	if (!$Kstat->{"vfs.zfs.version.spa"}) { return };
	my $spa = $Kstat->{"vfs.zfs.version.spa"};
	my $zpl = $Kstat->{"vfs.zfs.version.zpl"};
	my $memory_throttle_count = $Kstat->{"kstat.zfs.misc.arcstats.memory_throttle_count"};

	print "ARC Summary: ";
	if ($memory_throttle_count > 0) {
		print "(THROTTLED)\n";
	} else { print "(HEALTHY)\n"; }
	printf("\tStorage pool Version:\t\t\t%d\n", $spa);
	printf("\tFilesystem Version:\t\t\t%d\n", $zpl);
	printf("\tMemory Throttle Count:\t\t\t%s\n", fHits($memory_throttle_count));
	print "\n";

	### ARC Misc. ###
	my $deleted = $Kstat->{"kstat.zfs.misc.arcstats.deleted"};
	my $evict_skip = $Kstat->{"kstat.zfs.misc.arcstats.evict_skip"};
	my $mutex_miss = $Kstat->{"kstat.zfs.misc.arcstats.mutex_miss"};
	my $recycle_miss = $Kstat->{"kstat.zfs.misc.arcstats.recycle_miss"};

	print "ARC Misc:\n";
	printf("\tDeleted:\t\t\t\t%s\n", fHits($deleted));
	printf("\tRecycle Misses:\t\t\t\t%s\n", fHits($recycle_miss));
	printf("\tMutex Misses:\t\t\t\t%s\n", fHits($mutex_miss));
	printf("\tEvict Skips:\t\t\t\t%s\n", fHits($mutex_miss));
	print "\n";

	### ARC Sizing ###
	my $arc_size = $Kstat->{"kstat.zfs.misc.arcstats.size"};
	my $mru_size = $Kstat->{"kstat.zfs.misc.arcstats.p"};
	my $target_max_size = $Kstat->{"kstat.zfs.misc.arcstats.c_max"};
	my $target_min_size = $Kstat->{"kstat.zfs.misc.arcstats.c_min"};
	my $target_size = $Kstat->{"kstat.zfs.misc.arcstats.c"};

	my $target_size_ratio = ($target_max_size / $target_min_size);

	printf("ARC Size:\t\t\t\t%s\t%s\n",
		fPerc($arc_size, $target_max_size),  fBytes($arc_size));
	printf("\tTarget Size: (Adaptive)\t\t%s\t%s\n",
		fPerc($target_size, $target_max_size), fBytes($target_size));
	printf("\tMin Size (Hard Limit):\t\t%s\t%s\n",
		fPerc($target_min_size, $target_max_size), fBytes($target_min_size));
	printf("\tMax Size (High Water):\t\t%d:1\t%s\n",
		$target_size_ratio, fBytes($target_max_size));

	print "\nARC Size Breakdown:\n";
	if ($arc_size > $target_size) {
		my $mfu_size = ($arc_size - $mru_size);
		printf("\tRecently Used Cache Size:\t%s\t%s\n",
			fPerc($mru_size, $arc_size), fBytes($mru_size));
		printf("\tFrequently Used Cache Size:\t%s\t%s\n",
			fPerc($mfu_size, $arc_size), fBytes($mfu_size));
	}

	if ($arc_size < $target_size) {
		my $mfu_size = ($target_size - $mru_size);
		printf("\tRecently Used Cache Size:\t%s\t%s\n",
			fPerc($mru_size, $target_size), fBytes($mru_size));
		printf("\tFrequently Used Cache Size:\t%s\t%s\n",
			fPerc($mfu_size, $target_size), fBytes($mfu_size));
	}
	print "\n";

	### ARC Hash Breakdown ###
	my $hash_chain_max = $Kstat->{"kstat.zfs.misc.arcstats.hash_chain_max"};
	my $hash_chains = $Kstat->{"kstat.zfs.misc.arcstats.hash_chains"};
	my $hash_collisions = $Kstat->{"kstat.zfs.misc.arcstats.hash_collisions"};
	my $hash_elements = $Kstat->{"kstat.zfs.misc.arcstats.hash_elements"};
	my $hash_elements_max = $Kstat->{"kstat.zfs.misc.arcstats.hash_elements_max"};

	print "ARC Hash Breakdown:\n";
	printf("\tElements Max:\t\t\t\t%s\n", fHits($hash_elements_max));
	printf("\tElements Current:\t\t%s\t%s\n",
		fPerc($hash_elements, $hash_elements_max), fHits($hash_elements));
	printf("\tCollisions:\t\t\t\t%s\n", fHits($hash_collisions));
	printf("\tChain Max:\t\t\t\t%s\n", fHits($hash_chain_max));
	printf("\tChains:\t\t\t\t\t%s\n", fHits($hash_chains));
}

sub _arc_efficiency {
	if (!$Kstat->{"vfs.zfs.version.spa"}) { return };
	my $arc_hits = $Kstat->{"kstat.zfs.misc.arcstats.hits"};
	my $arc_misses = $Kstat->{"kstat.zfs.misc.arcstats.misses"};
	my $demand_data_hits = $Kstat->{"kstat.zfs.misc.arcstats.demand_data_hits"};
	my $demand_data_misses = $Kstat->{"kstat.zfs.misc.arcstats.demand_data_misses"};
	my $demand_metadata_hits = $Kstat->{"kstat.zfs.misc.arcstats.demand_metadata_hits"};
	my $demand_metadata_misses = $Kstat->{"kstat.zfs.misc.arcstats.demand_metadata_misses"};
	my $mfu_ghost_hits = $Kstat->{"kstat.zfs.misc.arcstats.mfu_ghost_hits"};
	my $mfu_hits = $Kstat->{"kstat.zfs.misc.arcstats.mfu_hits"};
	my $mru_ghost_hits = $Kstat->{"kstat.zfs.misc.arcstats.mru_ghost_hits"};
	my $mru_hits = $Kstat->{"kstat.zfs.misc.arcstats.mru_hits"};
	my $prefetch_data_hits = $Kstat->{"kstat.zfs.misc.arcstats.prefetch_data_hits"};
	my $prefetch_data_misses = $Kstat->{"kstat.zfs.misc.arcstats.prefetch_data_misses"};
	my $prefetch_metadata_hits = $Kstat->{"kstat.zfs.misc.arcstats.prefetch_metadata_hits"};
	my $prefetch_metadata_misses = $Kstat->{"kstat.zfs.misc.arcstats.prefetch_metadata_misses"};

	my $anon_hits = $arc_hits - ($mfu_hits + $mru_hits + $mfu_ghost_hits + $mru_ghost_hits);
	my $arc_accesses_total = ($arc_hits + $arc_misses);
	my $demand_data_total = ($demand_data_hits + $demand_data_misses);
	my $prefetch_data_total = ($prefetch_data_hits + $prefetch_data_misses);
	my $real_hits = ($mfu_hits + $mru_hits);

	printf("ARC Efficiency:\t\t\t\t\t%s\n", fHits($arc_accesses_total));
	printf("\tCache Hit Ratio:\t\t%s\t%s\n",
		fPerc($arc_hits, $arc_accesses_total), fHits($arc_hits));
	printf("\tCache Miss Ratio:\t\t%s\t%s\n",
		fPerc($arc_misses, $arc_accesses_total), fHits($arc_misses));
	printf("\tActual Hit Ratio:\t\t%s\t%s\n",
		fPerc($real_hits, $arc_accesses_total), fHits($real_hits));
	print "\n";
	printf("\tData Demand Efficiency:\t\t%s\t%s\n",
		fPerc($demand_data_hits, $demand_data_total), fHits($demand_data_total));

	if ($prefetch_data_total > 0){ 
		printf("\tData Prefetch Efficiency:\t%s\t%s\n",
			fPerc($prefetch_data_hits, $prefetch_data_total), fHits($prefetch_data_total));
	}
	print "\n";

	print "\tCACHE HITS BY CACHE LIST:\n";
	if ( $anon_hits > 0 ){
		printf("\t  Anonymously Used:\t\t%s\t%s\n",
			fPerc($anon_hits, $arc_hits), fHits($anon_hits));
	}

	printf("\t  Most Recently Used:\t\t%s\t%s\n",
		fPerc($mru_hits, $arc_hits), fHits($mru_hits));
	printf("\t  Most Frequently Used:\t\t%s\t%s\n",
		fPerc($mfu_hits, $arc_hits), fHits($mfu_hits));
	printf("\t  Most Recently Used Ghost:\t%s\t%s\n",
		fPerc($mru_ghost_hits, $arc_hits), fHits($mru_ghost_hits));
	printf("\t  Most Frequently Used Ghost:\t%s\t%s\n",
		fPerc($mfu_ghost_hits, $arc_hits), fHits($mfu_ghost_hits));

	print "\n\tCACHE HITS BY DATA TYPE:\n";
	printf("\t  Demand Data:\t\t\t%s\t%s\n",
		fPerc($demand_data_hits, $arc_hits), fHits($demand_data_hits));
	printf("\t  Prefetch Data:\t\t%s\t%s\n",
		fPerc($prefetch_data_hits, $arc_hits), fHits($prefetch_data_hits));
	printf("\t  Demand Metadata:\t\t%s\t%s\n",
		fPerc($demand_metadata_hits, $arc_hits), fHits($demand_metadata_hits));
	printf("\t  Prefetch Metadata:\t\t%s\t%s\n",
		fPerc($prefetch_metadata_hits, $arc_hits), fHits($prefetch_metadata_hits));

	print "\n\tCACHE MISSES BY DATA TYPE:\n";
	printf("\t  Demand Data:\t\t\t%s\t%s\n",
		fPerc($demand_data_misses, $arc_misses), fHits($demand_data_misses));
	printf("\t  Prefetch Data:\t\t%s\t%s\n",
		fPerc($prefetch_data_misses, $arc_misses), fHits($prefetch_data_misses));
	printf("\t  Demand Metadata:\t\t%s\t%s\n",
		fPerc($demand_metadata_misses, $arc_misses), fHits($demand_metadata_misses));
	printf("\t  Prefetch Metadata:\t\t%s\t%s\n",
		fPerc($prefetch_metadata_misses, $arc_misses), fHits($prefetch_metadata_misses));
}

sub _l2arc_summary {
	if (!$Kstat->{"vfs.zfs.version.spa"}) { return };
	my $l2_abort_lowmem = $Kstat->{"kstat.zfs.misc.arcstats.l2_abort_lowmem"};
	my $l2_cksum_bad = $Kstat->{"kstat.zfs.misc.arcstats.l2_cksum_bad"};
	my $l2_evict_lock_retry = $Kstat->{"kstat.zfs.misc.arcstats.l2_evict_lock_retry"};
	my $l2_evict_reading = $Kstat->{"kstat.zfs.misc.arcstats.l2_evict_reading"};
	my $l2_feeds = $Kstat->{"kstat.zfs.misc.arcstats.l2_feeds"};
	my $l2_free_on_write = $Kstat->{"kstat.zfs.misc.arcstats.l2_free_on_write"};
	my $l2_hdr_size = $Kstat->{"kstat.zfs.misc.arcstats.l2_hdr_size"};
	my $l2_hits = $Kstat->{"kstat.zfs.misc.arcstats.l2_hits"};
	my $l2_io_error = $Kstat->{"kstat.zfs.misc.arcstats.l2_io_error"};
	my $l2_misses = $Kstat->{"kstat.zfs.misc.arcstats.l2_misses"};
	my $l2_rw_clash = $Kstat->{"kstat.zfs.misc.arcstats.l2_rw_clash"};
	my $l2_size = $Kstat->{"kstat.zfs.misc.arcstats.l2_size"};
	my $l2_write_buffer_bytes_scanned = $Kstat->{"kstat.zfs.misc.arcstats.l2_write_buffer_bytes_scanned"};
	my $l2_write_buffer_iter = $Kstat->{"kstat.zfs.misc.arcstats.l2_write_buffer_iter"};
	my $l2_write_buffer_list_iter = $Kstat->{"kstat.zfs.misc.arcstats.l2_write_buffer_list_iter"};
	my $l2_write_buffer_list_null_iter = $Kstat->{"kstat.zfs.misc.arcstats.l2_write_buffer_list_null_iter"};
	my $l2_write_bytes = $Kstat->{"kstat.zfs.misc.arcstats.l2_write_bytes"};
	my $l2_write_full = $Kstat->{"kstat.zfs.misc.arcstats.l2_write_full"};
	my $l2_write_in_l2 = $Kstat->{"kstat.zfs.misc.arcstats.l2_write_in_l2"};
	my $l2_write_io_in_progress = $Kstat->{"kstat.zfs.misc.arcstats.l2_write_io_in_progress"};
	my $l2_write_not_cacheable = $Kstat->{"kstat.zfs.misc.arcstats.l2_write_not_cacheable"};
	my $l2_write_passed_headroom = $Kstat->{"kstat.zfs.misc.arcstats.l2_write_passed_headroom"};
	my $l2_write_pios = $Kstat->{"kstat.zfs.misc.arcstats.l2_write_pios"};
	my $l2_write_spa_mismatch = $Kstat->{"kstat.zfs.misc.arcstats.l2_write_spa_mismatch"};
	my $l2_write_trylock_fail = $Kstat->{"kstat.zfs.misc.arcstats.l2_write_trylock_fail"};
	my $l2_writes_done = $Kstat->{"kstat.zfs.misc.arcstats.l2_writes_done"};
	my $l2_writes_error = $Kstat->{"kstat.zfs.misc.arcstats.l2_writes_error"};
	my $l2_writes_hdr_miss = $Kstat->{"kstat.zfs.misc.arcstats.l2_writes_hdr_miss"};
	my $l2_writes_sent = $Kstat->{"kstat.zfs.misc.arcstats.l2_writes_sent"};

	my $l2_access_total = ($l2_hits + $l2_misses);
	my $l2_health_count = ($l2_writes_error + $l2_cksum_bad + $l2_io_error);

	if ($l2_size > 0 && $l2_access_total > 0) {
		print "L2 ARC Summary: ";
		if ($l2_health_count > 0) {
			print "(DEGRADED)\n";
		} else { print "(HEALTHY)\n"; }
		printf("\tPassed Headroom:\t\t\t%s\n", fHits($l2_write_passed_headroom));
		printf("\tTried Lock Failures:\t\t\t%s\n", fHits($l2_write_trylock_fail));
		printf("\tIO In Progress:\t\t\t\t%s\n", fHits($l2_write_io_in_progress));
		printf("\tLow Memory Aborts:\t\t\t%s\n", fHits($l2_abort_lowmem));
		printf("\tFree on Write:\t\t\t\t%s\n", fHits($l2_free_on_write));
		printf("\tWrites While Full:\t\t\t%s\n", fHits($l2_write_full));
		printf("\tR/W Clashes:\t\t\t\t%s\n", fHits($l2_rw_clash));
		printf("\tBad Checksums:\t\t\t\t%s\n", fHits($l2_cksum_bad));
		printf("\tIO Errors:\t\t\t\t%s\n", fHits($l2_io_error));
		printf("\tSPA Mismatch:\t\t\t\t%s\n", fHits($l2_write_spa_mismatch));
		print "\n";

		printf("L2 ARC Size: (Adaptive)\t\t\t\t%s\n", fBytes($l2_size));
		printf("\tHeader Size:\t\t\t%s\t%s\n",
			fPerc($l2_hdr_size, $l2_size), fBytes($l2_hdr_size));
		print "\n";

		if (($l2_evict_lock_retry + $l2_evict_reading) > 0) {
			print "L2 ARC Evicts:\n";
			printf("\tLock Retries:\t\t\t\t%s\n", fHits($l2_evict_lock_retry));
			printf("\tUpon Reading:\t\t\t\t%s\n", fHits($l2_evict_reading));
			print "\n";
		}
		printf("L2 ARC Breakdown:\t\t\t\t%s\n", fHits($l2_access_total));
		printf("\tHit Ratio:\t\t\t%s\t%s\n",
			fPerc($l2_hits, $l2_access_total), fHits($l2_hits));
		printf("\tMiss Ratio:\t\t\t%s\t%s\n",
			fPerc($l2_misses, $l2_access_total), fHits($l2_misses));
		printf("\tFeeds:\t\t\t\t\t%s\n", fHits($l2_feeds));
		print "\n";

		print "L2 ARC Buffer:\n";
		printf("\tBytes Scanned:\t\t\t\t%s\n", fBytes($l2_write_buffer_bytes_scanned));
		printf("\tBuffer Iterations:\t\t\t%s\n", fHits($l2_write_buffer_iter));
		printf("\tList Iterations:\t\t\t%s\n", fHits($l2_write_buffer_list_iter));
		printf("\tNULL List Iterations:\t\t\t%s\n", fHits($l2_write_buffer_list_null_iter));
		print "\n";

		print "L2 ARC Writes:\n";
		if ($l2_writes_done != $l2_writes_sent) {
			printf("\tWrites Sent: (%s)\t\t\t\t%s\n",
				"FAULTED", fHits($l2_writes_sent));
			printf("\t  Done Ratio:\t\t\t%s\t%s\n",
				fPerc($l2_writes_done, $l2_writes_sent), fHits($l2_writes_done));
			printf("\t  Error Ratio:\t\t\t%s\t%s\n",
				fPerc($l2_writes_error, $l2_writes_sent), fHits($l2_writes_error));
		} else { printf("\tWrites Sent:\t\t\t%s\t%s\n", fPerc(100), fHits($l2_writes_sent)); }
	}
}

sub _dmu_summary {
	if (!$Kstat->{"vfs.zfs.version.spa"}) { return };
	my $zfetch_bogus_streams = $Kstat->{"kstat.zfs.misc.zfetchstats.bogus_streams"};
	my $zfetch_colinear_hits = $Kstat->{"kstat.zfs.misc.zfetchstats.colinear_hits"};
	my $zfetch_colinear_misses = $Kstat->{"kstat.zfs.misc.zfetchstats.colinear_misses"};
	my $zfetch_hits = $Kstat->{"kstat.zfs.misc.zfetchstats.hits"};
	my $zfetch_misses = $Kstat->{"kstat.zfs.misc.zfetchstats.misses"};
	my $zfetch_reclaim_failures = $Kstat->{"kstat.zfs.misc.zfetchstats.reclaim_failures"};
	my $zfetch_reclaim_successes = $Kstat->{"kstat.zfs.misc.zfetchstats.reclaim_successes"};
	my $zfetch_streams_noresets = $Kstat->{"kstat.zfs.misc.zfetchstats.streams_noresets"};
	my $zfetch_streams_resets = $Kstat->{"kstat.zfs.misc.zfetchstats.streams_resets"};
	my $zfetch_stride_hits = $Kstat->{"kstat.zfs.misc.zfetchstats.stride_hits"};
	my $zfetch_stride_misses = $Kstat->{"kstat.zfs.misc.zfetchstats.stride_misses"};

	my $zfetch_access_total = ($zfetch_hits + $zfetch_misses);
	my $zfetch_colinear_total = ($zfetch_colinear_hits + $zfetch_colinear_misses);
	my $zfetch_health_count = ($zfetch_bogus_streams);
	my $zfetch_reclaim_total = ($zfetch_reclaim_successes + $zfetch_reclaim_failures);
	my $zfetch_streams_total = ($zfetch_streams_resets + $zfetch_streams_noresets + $zfetch_bogus_streams);
	my $zfetch_stride_total = ($zfetch_stride_hits + $zfetch_stride_misses);

	if ($zfetch_access_total > 0) {
		print "File-Level Prefetch: ";
		if ($zfetch_health_count > 0) {
			print "(DEGRADED)\n\n";
		} else { print "(HEALTHY)\n\n"; }
		printf("DMU Efficiency:\t\t\t\t\t%s\n", fHits($zfetch_access_total));
		printf("\tHit Ratio:\t\t\t%s\t%s\n",
			fPerc($zfetch_hits, $zfetch_access_total),
			fHits($zfetch_hits));
		printf("\tMiss Ratio:\t\t\t%s\t%s\n",
			fPerc($zfetch_misses, $zfetch_access_total),
			fHits($zfetch_misses));
		print "\n";

		printf("\tColinear:\t\t\t\t%s\n", fHits($zfetch_colinear_total));
		printf("\t  Hit Ratio:\t\t\t%s\t%s\n",
			fPerc($zfetch_colinear_hits, $zfetch_colinear_total),
			fHits($zfetch_colinear_hits));
		printf("\t  Miss Ratio:\t\t\t%s\t%s\n",
			fPerc($zfetch_colinear_misses, $zfetch_colinear_total),
			fHits($zfetch_colinear_misses));
		print "\n";

		printf("\tStride:\t\t\t\t\t%s\n", fHits($zfetch_stride_total));
		printf("\t  Hit Ratio:\t\t\t%s\t%s\n",
			fPerc($zfetch_stride_hits, $zfetch_stride_total),
			fHits($zfetch_stride_hits));
		printf("\t  Miss Ratio:\t\t\t%s\t%s\n",
			fPerc($zfetch_stride_misses, $zfetch_stride_total),
			fHits($zfetch_stride_misses));
		print "\n";

		if ($zfetch_health_count > 0) {
			printf("DMU Misc: (%s)\n", "FAULTED");
		} else { print "DMU Misc:\n"; }
		printf("\tReclaim:\t\t\t\t%s\n", fHits($zfetch_reclaim_total));
		printf("\t  Successes:\t\t\t%s\t%s\n",
			fPerc($zfetch_reclaim_successes, $zfetch_reclaim_total),
			fHits($zfetch_reclaim_successes));
		printf("\t  Failures:\t\t\t%s\t%s\n",
			fPerc($zfetch_reclaim_failures, $zfetch_reclaim_total),
			fHits($zfetch_reclaim_failures));
		printf("\n\tStreams:\t\t\t\t%s\n", fHits($zfetch_streams_total));
		printf("\t  +Resets:\t\t\t%s\t%s\n",
			fPerc($zfetch_streams_resets, $zfetch_streams_total),
			fHits($zfetch_streams_resets));
		printf("\t  -Resets:\t\t\t%s\t%s\n",
			fPerc($zfetch_streams_noresets, $zfetch_streams_total),
			fHits($zfetch_streams_noresets));
		printf("\t  Bogus:\t\t\t\t%s\n", fHits($zfetch_bogus_streams));
	}
}

sub _vdev_summary {
	if (!$Kstat->{"vfs.zfs.version.spa"}) { return };
	my $vdev_cache_delegations = $Kstat->{"kstat.zfs.misc.vdev_cache_stats.delegations"};
	my $vdev_cache_misses = $Kstat->{"kstat.zfs.misc.vdev_cache_stats.misses"};
	my $vdev_cache_hits = $Kstat->{"kstat.zfs.misc.vdev_cache_stats.hits"};
	my $vdev_cache_total = ($vdev_cache_misses + $vdev_cache_hits + $vdev_cache_delegations);

	if ($vdev_cache_total > 0) {
		printf("VDEV Cache Summary:\t\t\t\t%s\n", fHits($vdev_cache_total));
		printf("\tHit Ratio:\t\t\t%s\t%s\n",
			fPerc($vdev_cache_hits, $vdev_cache_total), fHits($vdev_cache_hits));
		printf("\tMiss Ratio:\t\t\t%s\t%s\n",
			fPerc($vdev_cache_misses, $vdev_cache_total), fHits($vdev_cache_misses));
		printf("\tDelegations:\t\t\t%s\t%s\n",
			fPerc($vdev_cache_delegations, $vdev_cache_total), fHits($vdev_cache_delegations));
	}
}

sub _sysctl_summary {
	if ($usetunable != 0) {
		my @Tunable = qw(
			kern.maxusers
			vfs.zfs
			vm.kmem_size
			vm.kmem_size_scale
			vm.kmem_size_min
			vm.kmem_size_max
		);
		my @tunable = `/sbin/sysctl -qe @Tunable`;
		print "ZFS Tunable (sysctl):\n";
		foreach my $tunable (@tunable){
			chomp($tunable);
			print "\t$tunable\n";
		}
	}
}

my @unSub = qw(
	_system_memory
	_arc_summary
	_arc_efficiency
	_l2arc_summary
	_dmu_summary
	_vdev_summary
	_sysctl_summary
);

sub _call_all {
	my $page = 1;
	foreach my $unsub (@unSub) {
		eval $unsub &&
		printf("\t\t\t\t\t\t\t\tPage: %2d", $page) &&
		hline; ++$page;
	}
	printf("\t\t\t\t\t\t\t\tPage: %2d", --$page);
	hline;
}

my %opt;
getopt("p:", \%opt);
if (%opt) {
	switch($opt{p}) {
		case 1 { eval $unSub[0]; hline; }
		case 2 { eval $unSub[1]; hline; }
		case 3 { eval $unSub[2]; hline; }
		case 4 { eval $unSub[3]; hline; }
		case 5 { eval $unSub[4]; hline; }
		case 6 { eval $unSub[5]; hline; }
		case 7 { eval $unSub[6]; hline; }
		else {
			_call_all;
		}
	}
} else {
	_call_all;
}

__END__
