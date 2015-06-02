# News #

---


_Mon Jan  9 05:17:44 UTC 2012_
> For convenience I have recently began uploading files to the download area in a version format 'RELENG\_X\_X' which usually coincides to release done in FreeBSD and a branch that it is known to run on. Upon each new release the old version will be deprecated but left intact for future reference whether there has been a change or not.

_Fri Sep  9 03:58:30 UTC 2011_
> Recent searches uncovered the following URL for Nexenta & IllumOS ~4 months ago thanks to Scott Roberts

> http://www.nexenta.org/boards/1/topics/2308

> I will see if some of these changes can be incorporated back into the version here as soon as I get some free time in an effort to materialize a version that can be used cross platform. Patches and suggestions are always welcome in my =Inbox or an Issue Report.

_Sat Sep  3 18:22:49 UTC 2011_
> As of this date the downloads section version of arc\_summary.pl has been updated to the current repository version. If there are any issues with the operation of this script on older systems, please file a issue report and I will address that ASAP. The previous version of this script was 59 & uploaded 4-20-2010 which can also be checked out of the repository. There were **2805** downloads of the previous version 59.

_Mon Jul  4 03:40:39 UTC 2011_
> Fairly close to this date there has been more than **2500** downloads of the download version of arc\_summary.pl and uncounted and undocumented downloads of the repository development version.

> With ZFS v28 being merged to FreeBSD 8.2-STABLE before the upcoming release of 8.3 I will be deprecating the download version of arc\_summary.pl and putting out dated release versions that coincide with the releases of FreeBSD. Please be patient as this happens and if there are any concerns or suggestions please let me know <jhell at DataIX dot net>

> Thank you.

_Sun Mar 20 05:02:10 UTC 2011_
> As of this date there have been more than **2100** downloads of arc\_summary.pl and still counting!. Thank you for your usage!

_Mon Nov 22 19:51:24 UTC 2010_
> There have been a total of **~1500** downloads as of this date of the version of arc\_summary.pl that was written for FreeBSD 7.1-RELEASE and later systems.

> As of this time work has began toward adjusting arc\_summary.pl to work on multiple versions of the OS without having to worry about what is and is not supported. It is currently scheduled to be completed by ~~Jan 1 2011~~ and at which time the downloads section will be updated with the latest working version available. **This has been slated for a later time. TBA**

_Mon Oct 18 02:06:50 UTC 2010_
> arc\_summary.pl has undergone some visual changes and enhancements. The repository version of of the script now has a function ``fBytes()'' that formats as you have guessed it "bytes" into K,M,G,T,P,E,Z & Y representations. Simple as this might seem, this is a space saver for your output with the newly added L2 ARC Buffer section that really needed the fBytes() function.

> Also due to the above, two other functions have been implemented. fHits() and fPerc(). These functions humanize the output you see from the script ultimately making it more readable. fHits() formats the output with the suffixes "k,m,b,t,q,Q,s,S". with (k) being the lowest of one thousand and (S) being the largest of one Septtillion

_Fri Aug 27 14:55:13 UTC 2010_
> Branch stable/7 has been removed in favor of the stable/7 tag. [here](http://bit.ly/91bL6C)

_Fri Aug 27 09:27:54 UTC 2010_
> As of now the repository has been switched over to Mercurial for development. If there is a problem please email me directly or file a issue in the above tab. The old subversion repository should still be available by svn(1) client and is ~4.1MB.

  * svn co http://jhell.googlecode.com/svn

_Sun Aug 22 02:45:33 UTC 2010_
> Today there has been **1000** downloads of the arc\_summary.pl script for FreeBSD.

_Mon Jul 12 22:50:39 UTC 2010_
> As of this date there has been more than **800** downloads of the arc\_summary.pl script for FreeBSD and a large number of unaccounted downloads from the repository.

_Sat Apr 24 21:10:57 UTC 2010_
> At some point today there has been a total of **500** downloads of arc\_summary.pl.

# Updates #

---

_Sun Mar 20 05:02:10 UTC 2011_
> It is important that if you are using **FreeBSD >= 8.1** that you give the repository version of arc\_summary a shot. It contains new formatting for human readable output as well a switch to display a specific page that is shown in the bottom right hand corner of each page of output.

> Page numbers for the flag (-p) can be determined by the output shown on your screen. Each page is labeled with a page number that designates its number to (-p). This is an undocumented feature as of right now as it is not needed for operation and currently only supports displaying full output or just one single page. Only the first (-p) flag is honored.

> Example: ( Displaying Page 1 )
> arc\_summary.pl -p1

> Current Repository Version: 1. http://bit.ly/iag9wx

> Current Raw File: 1. http://bit.ly/arc_summary


_Mon Nov 22 19:51:24 UTC 2010_
> arc\_summary.pl has been updated to allow printing of specific sections by number. This can be done by calling arc\_summary.pl with an argument of 1-6 that will print in order of appearance from the whole output. Currently this only takes one argument but I am working that over to accept multiple arguments.


_Mon Jul 02 22:45:30 UTC 2010_
> stable/7 branch in the repository has been populated. No more updates should effect this branch except for slight bug fixes. As you can guess this branch is for the FreeBSD 7.X stable/7 or otherwise known by RELENG\_7.


_Tue Apr 20 22:27:55 UTC 2010_
> arc\_summary.pl has been updated to [revision 59](https://code.google.com/p/jhell/source/detail?r=59) in the file download section. New stats and calculations have been added that makes this a added value for those running ZFS on their FreeBSD systems.


_Sun Feb 21 05:52:34 UTC 2010_
> Subversion Repository organized and set for development. Current files in the download section are now developed from this repository and will be updated once a month unless a critical bug fix is needed.


## Public Keys ##

---

| **Key** | **Name** | **@** |
|:--------|:---------|:------|
| [0x89D8547E](http://bit.ly/cEoLXH) | jhell    | DataIX.net |