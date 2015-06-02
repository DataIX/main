From the blog of realneal: http://blogs.sun.com/realneel/entry/zfs_arc_statistics

ZFS ARC Statistics
ARC Statistics are exported via kstat(1M) (CR 6510807) in Solaris 10 U4 and Solaris nevada (build 57+). A simple way to see them is to use kstat -m zfs.

I wrote a simple perl script to print out the ARC statistics. Sample output is shown below. You can either print the set of default fields, or specify what fields you want printed (see arcstat -v for details)


In later blogs, I will describe how to make sense of these numbers. Sample output below

Cached random read
```
thumper1[bash]$ ~/arcstat.pl
    Time  read  miss  miss%  dmis  dm%  pmis  pm%  mmis  mm%  arcsz     c
18:15:21  985M  100M     10   47M   14   53M    8    1M    3    11G   11G
18:15:22  137K     0      0     0    0     0    0     0    0    11G   11G
18:15:23  138K     0      0     0    0     0    0     0    0    11G   11G
18:15:24  138K     0      0     0    0     0    0     0    0    11G   11G
18:15:25  138K     0      0     0    0     0    0     0    0    11G   11G
18:15:26  138K     0      0     0    0     0    0     0    0    11G   11G
18:15:27  139K     0      0     0    0     0    0     0    0    11G   11G
18:15:28  140K     0      0     0    0     0    0     0    0    11G   11G
18:15:29  139K     0      0     0    0     0    0     0    0    11G   11G
18:15:30  140K     0      0     0    0     0    0     0    0    11G   11G
18:15:31  139K     0      0     0    0     0    0     0    0    11G   11G
18:15:32   33K     0      0     0    0     0    0     0    0    11G   11G
```

Uncached sequential write
```
    Time  read  miss  miss%  dmis  dm%  pmis  pm%  mmis  mm%  arcsz     c
18:17:48     0     0      0     0    0     0    0     0    0    11G   11G
18:17:49     0     0      0     0    0     0    0     0    0    11G   11G
18:17:50   664   372     56     7    2   365  100     5    2    11G   11G
18:17:51    1K   534     36     5    0   529  100     5    0    11G   11G
18:17:52    2K   774     33     6    0   768  100     6    0    10G   10G
18:17:53    2K   645     31     5    0   640  100     5    0    10G   10G
18:17:54    1K   645     35     5    0   640  100     5    0    10G   10G
18:17:55    2K   645     31     5    0   640  100     5    0    10G   10G
18:17:56    1K   646     35     6    0   640  100     6    0    10G   10G
18:17:57    2K   645     31     5    0   640  100     5    0    10G   10G
18:17:58    2K   774     33     6    0   768  100     6    0    10G   10G
```
