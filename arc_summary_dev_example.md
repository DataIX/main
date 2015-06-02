
```
------------------------------------------------------------------------
ZFS Subsystem Report				Thu Oct 21 11:20:06 2010
------------------------------------------------------------------------

FreeBSD 8.1-STABLE				801500
Hardware Platform:				i386
Processor Architecture:				i386

11:20AM  up 47 mins, 4 users, load averages: 4.25, 4.51, 4.15

------------------------------------------------------------------------

Physical Memory:				1009.91	MiB

Kernel Memory:					286.55	MiB
DATA:					96.65%	276.94	MiB
TEXT:					3.35%	9.61	MiB

------------------------------------------------------------------------

ARC Summary: (HEALTHY)
	Storage pool Version:			15
	Filesystem Version:			4
	Memory Throttle Count:			0

ARC Misc:
	Deleted:				45407
	Recycle Misses:				38344
	Mutex Misses:				12
	Evict Skips:				12

ARC Size:				51.89%	257.38	MiB
	Target Size: (Adaptive)		51.89%	257.36	MiB
	Min Size (Hard Limit):		12.50%	62.00	MiB
	Max Size (High Water):		~8:1	496.00	MiB

ARC Size Breakdown:
	Recently Used Cache Size:	29.93%	77.04	MiB
	Frequently Used Cache Size:	70.07%	180.34	MiB

ARC Hash Breakdown:
	Elements Max:				56453
	Elements Current:		99.57%	56212
	Collisions:				100597
	Chain Max:				13
	Chains:					13923

ARC Efficiency:					1510841
	Cache Hit Ratio:		87.63%	1323915
	Cache Miss Ratio:		12.37%	186926
	Actual Hit Ratio:		71.79%	1084699

	Data Demand Efficiency:		88.81%	644681
	Data Prefetch Efficiency:	45.07%	4928

	CACHE HITS BY CACHE LIST:
	  Anonymously Used:		8.69%	115050
	  Most Recently Used:		8.78%	116301
	  Most Frequently Used:		73.15%	968398
	  Most Recently Used Ghost:	2.21%	29224
	  Most Frequently Used Ghost:	7.17%	94942

	CACHE HITS BY DATA TYPE:
	  Demand Data:			43.25%	572538
	  Prefetch Data:		0.17%	2221
	  Demand Metadata:		38.37%	508037
	  Prefetch Metadata:		18.21%	241119

	CACHE MISSES BY DATA TYPE:
	  Demand Data:			38.59%	72143
	  Prefetch Data:		1.45%	2707
	  Demand Metadata:		39.77%	74346
	  Prefetch Metadata:		20.18%	37730

------------------------------------------------------------------------

L2 ARC Summary: (HEALTHY)
	Passed Headroom:			0
	Tried Lock Failures:			681
	IO In Progress:				1423
	Low Memory Aborts:			5
	Free on Write:				28
	Writes While Full:			14
	R/W Clashes:				0
	Bad Checksums:				0
	IO Errors:				0
	SPA Mismatch:				0

L2 ARC Size: (Adaptive)				581.98	MiB
	Header Size:			0.30%	1.74	MiB

L2 ARC Breakdown:				186910
	Hit Ratio:			38.60%	72146
	Miss Ratio:			61.40%	114764
	Feeds:					2850

L2 ARC Buffer:
	Bytes Scanned:				382.66	GiB
	Buffer Iterations:			2.850k
	List Iterations:			182.305k
	NULL List Iterations:			13.126k

L2 ARC Writes:
	Writes Sent:			100.00%	1619

------------------------------------------------------------------------

File-Level Prefetch: (HEALTHY)

DMU Efficiency:					13977692
	Hit Ratio:			91.75%	12824983
	Miss Ratio:			8.25%	1152709

	Colinear:				1152709
	  Hit Ratio:			0.05%	596
	  Miss Ratio:			99.95%	1152113

	Stride:					12826205
	  Hit Ratio:			99.68%	12784770
	  Miss Ratio:			0.32%	41435

DMU Misc:
	Reclaim:				1152113
	  Successes:			4.12%	47439
	  Failures:			95.88%	1104674

	Streams:				40342
	  +Resets:			0.31%	127
	  -Resets:			99.69%	40215
	  Bogus:			0.00%	0

------------------------------------------------------------------------

VDEV Cache Summary:				39769
	Hit Ratio:			56.29%	22385
	Miss Ratio:			43.71%	17384
	Delegations:				9403

------------------------------------------------------------------------

ZFS Tunable (sysctl):
	hw.availpages=258537
	hw.physmem=1058967552
	kern.maxusers=504
	vfs.zfs.l2c_only_size=285809664
	vfs.zfs.mfu_ghost_data_lsize=23675904
	vfs.zfs.mfu_ghost_metadata_lsize=43420160
	vfs.zfs.mfu_ghost_size=67096064
	vfs.zfs.mfu_data_lsize=132452352
	vfs.zfs.mfu_metadata_lsize=1156608
	vfs.zfs.mfu_size=169181184
	vfs.zfs.mru_ghost_data_lsize=42126336
	vfs.zfs.mru_ghost_metadata_lsize=160320000
	vfs.zfs.mru_ghost_size=202446336
	vfs.zfs.mru_data_lsize=4985856
	vfs.zfs.mru_metadata_lsize=27998720
	vfs.zfs.mru_size=57425408
	vfs.zfs.anon_data_lsize=0
	vfs.zfs.anon_metadata_lsize=0
	vfs.zfs.anon_size=2711040
	vfs.zfs.l2arc_norw=1
	vfs.zfs.l2arc_feed_again=1
	vfs.zfs.l2arc_noprefetch=0
	vfs.zfs.l2arc_feed_min_ms=200
	vfs.zfs.l2arc_feed_secs=1
	vfs.zfs.l2arc_headroom=2
	vfs.zfs.l2arc_write_boost=8388608
	vfs.zfs.l2arc_write_max=8388608
	vfs.zfs.arc_meta_limit=130023424
	vfs.zfs.arc_meta_used=129959576
	vfs.zfs.mdcomp_disable=0
	vfs.zfs.arc_min=65011712
	vfs.zfs.arc_max=520093696
	vfs.zfs.zfetch.array_rd_sz=1048576
	vfs.zfs.zfetch.block_cap=256
	vfs.zfs.zfetch.min_sec_reap=2
	vfs.zfs.zfetch.max_streams=8
	vfs.zfs.prefetch_disable=0
	vfs.zfs.check_hostid=1
	vfs.zfs.recover=0
	vfs.zfs.txg.write_limit_override=0
	vfs.zfs.txg.synctime=5
	vfs.zfs.txg.timeout=30
	vfs.zfs.scrub_limit=10
	vfs.zfs.vdev.cache.bshift=16
	vfs.zfs.vdev.cache.size=10485760
	vfs.zfs.vdev.cache.max=16384
	vfs.zfs.vdev.aggregation_limit=131072
	vfs.zfs.vdev.ramp_rate=2
	vfs.zfs.vdev.time_shift=6
	vfs.zfs.vdev.min_pending=4
	vfs.zfs.vdev.max_pending=10
	vfs.zfs.cache_flush_disable=0
	vfs.zfs.zil_disable=0
	vfs.zfs.zio.use_uma=0
	vfs.zfs.version.zpl=4
	vfs.zfs.version.spa=15
	vfs.zfs.version.dmu_backup_stream=1
	vfs.zfs.version.dmu_backup_header=2
	vfs.zfs.version.acl=1
	vfs.zfs.debug=0
	vfs.zfs.super_owner=0
	vm.kmem_map_free=90943488
	vm.kmem_map_size=393977856
	vm.kmem_size=536870912
	vm.kmem_size_scale=1
	vm.kmem_size_min=134217728
	vm.kmem_size_max=805306368

------------------------------------------------------------------------
```