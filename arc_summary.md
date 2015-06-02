# News #

_Sat Sep  3 18:22:49 UTC 2011_
> As of this date the downloads section version of arc\_summary.pl has been updated to the current repository version. If there are any issues with the operation of this script on older systems, please file a issue report and I will address that ASAP. The previous version of this script was 59 & uploaded 4-20-2010 which can also be checked out of the repository. There were **2805** downloads of the previous version 59.

_Mon Jul  4 03:40:39 UTC 2011_
> Fairly close to this date there has been more than 2500 downloads of the download version of arc\_summary.pl and uncounted and undocumented downloads of the repository development version.

> With ZFS v28 being merged to FreeBSD 8.2-STABLE before the upcoming release of 8.3 I will be deprecating the download version of arc\_summary.pl and putting out dated release versions that coincide with the releases of FreeBSD. Please be patient as this happens and if there are any concerns or suggestions please let me know <jhell at DataIX dot net>

> Thank you.

Sun Mar 20 05:02:10 UTC 2011
> As of this date there have been more than **2100** downloads of arc\_summary.pl and still counting!. Thank you for your usage!

Mon Nov 22 19:51:24 UTC 2010
> There have been a total of ~1500 downloads as of this date of the version of arc\_summary.pl that was written for FreeBSD 7.1-RELEASE and later systems.

> As of this time work has began toward adjusting arc\_summary.pl to work on multiple versions of the OS without having to worry about what is and is not supported. It is currently scheduled to be completed by ~~Jan 1 2011~~ and at which time the downloads section will be updated with the latest working version available. **This has been slated for a later time. TBA**

Mon Oct 18 02:06:50 UTC 2010
> arc\_summary.pl has undergone some visual changes and enhancements. The repository version of of the script now has a function ``fBytes()'' that formats as you have guessed it "bytes" into K,M,G,T,P,E,Z & Y representations. Simple as this might seem, this is a space saver for your output with the newly added L2 ARC Buffer section that really needed the fBytes() function.

> Also due to the above, two other functions have been implemented. fHits() and fPerc(). These functions humanize the output you see from the script ultimately making it more readable. fHits() formats the output with the suffixes "k,m,b,t,q,Q,s,S". with k being the lowest of ```one thousand'' and S being the largest of ```one Septtillion''

Sun Aug 22 02:45:33 UTC 2010
> Today there has been **1000** downloads of the arc\_summary.pl script for FreeBSD.

Mon Jul 12 22:50:39 UTC 2010
> As of this date there has been more than **800** downloads of the arc\_summary.pl script for FreeBSD and a large number of unaccounted downloads from the repository.

Sat Apr 24 21:10:57 UTC 2010
> At some point today there has been a total of **500** downloads of arc\_summary.pl.

# Introduction #

This perl script is used to prep a report for user review with the contents being that of the ZFS ARC Statistics contained in sysctl kstats MIBs.

> Quoting the original author of the script for Sun based systems:

> While ARC is an amazing achievement its memory consumption raises doubts and questions in many administrators and users who view memory as a precious commodity. The Kstat facility provides access to a variety of ARC counters, but interpretting these properly is complex and non-intuative in their raw form, without spending several hours in the arc.c code. The purpose of arc\_summary.pl is to provide a historical analysis of the ZFS ARC.

This script is still work in progress.
  * Currently [arc\_summary.pl](http://jhell.googlecode.com/files/arc_summary.pl) tries to make use of all kstats that are available.
  * It includes some specific system information such as uname, uptime & date in a header.
  * Spills relevant sysctl MIBs at the bottom of the report.
  * Includes kernel memory usage.
  * Has L2 ARC kstats.
  * Current with sysctl MIBs as of ZFS v14. ( Please report new sysctls [here](http://bit.ly/94ORw0))

## Download ##

| **Name** | **Example** | **Description** |
|:---------|:------------|:----------------|
| [Stable](http://bit.ly/dtk50s) | [Output](http://bit.ly/bhXCGV) | 8.1-STABLE or lower with ZFSv14 |
| [Development ](http://bit.ly/djkYe2) | [Output](http://bit.ly/bmQNrT) | 8.1-STABLE or greater with ZFSv15 patch |
| [Stable 7 ](http://bit.ly/agwLYl) | [Output](http://bit.ly/bz9zb1) | 7.X users with ZFSv13 |

## Links ##

  1. [FreeBSD.org Website](http://bit.ly/aihrpc)
  1. [FreeBSD.org WiKi](http://bit.ly/cCMdOM)
  1. [Sun Microsystems](http://bit.ly/94eRA3)
  1. [OpenSolaris](http://bit.ly/93rGf5)

### Updates ###

Sun Mar 20 05:02:10 UTC 2011
> It is important that if you are using **FreeBSD >= 8.1** that you give the repository version of arc\_summary a shot. It contains new formatting for human readable output as well a switch to display a specific page that is shown in the bottom right hand corner of each page of output.

> Page numbers for the flag (-p) can be determined by the output shown on your screen. Each page is labeled with a page number that designates its number to (-p). This is an undocumented feature as of right now as it is not needed for operation and currently only supports displaying full output or just one single page. Only the first (-p) flag is honored.

> Example: ( Displaying Page 1 )
> arc\_summary.pl -p1

> Current Repository Version: 1. http://bit.ly/iag9wx

> Current Raw File: 1. http://bit.ly/arc_summary
