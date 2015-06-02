# Introduction #

Most of these aliases are commands that work or have worked on a FreeBSD machine that I have kept around for a while. This is a very mixed set of aliases that most of which I still use from time to time and may be helpful for your everyday use.

To use these just copy them into a file "I prefer ~/.sh\_aliasrc" and add a line like the following to your shell startup rcfile.
  * `[ -r ~/.sh_aliasrc ] && . ~/.sh_aliasrc`

# Details #

```
[ -x /usr/local/bin/scp ] && alias scp=/usr/local/bin/scp
[ -x /usr/local/bin/sftp ] && alias sftp=/usr/local/bin/sftp
[ -x /usr/local/bin/ssh ] && alias ssh=/usr/local/bin/ssh
[ -x /usr/local/bin/ssh-add ] && alias ssh-add="/usr/local/bin/ssh-add -t $((3600*72))"
[ -x /usr/local/bin/ssh-agent ] && alias ssh-agent=/usr/local/bin/ssh-agent
[ -x /usr/local/bin/ssh-keygen ] && alias ssh-keygen=/usr/local/bin/ssh-keygen
alias atime='stat -f "Access: %Sa %N"'
alias bogovac="sqlite3 ~/.bogofilter/wordlist.db 'VACUUM;'"
alias btime='stat -f "Create: %SB %N"'
alias cp='cp -i'
alias cp='cp -ip'
alias ctime='stat -f "Change: %Sc %N"'
alias dayserial='date "+%Y%m%d%H"'
alias dir="ls -sSixhF"
alias emacs='emacs -nw'
alias extrconf='strings -n3 /boot/kernel/kernel |sed -n 's/^___//p''
alias ffpr='firefox -ProfileManager'
alias go='screen startx'
alias h=history
alias helo='env |mail -s "HELO TEST MESSAGE $$::$RANDOM" $LOGNAME'
alias hup='kill -s HUP'
alias j="jobs -l"
alias la="ls -A"
alias lf="ls -FA"
alias ll="ls -lA"
alias lpq='/usr/local/bin/lpq'
alias lpr='/usr/local/bin/lpr'
alias lprm='/usr/local/bin/lprm'
alias lsdev='stat -f "%nName:%t%N%nType:%t%HT%nMajor:%t%Hr%nMinor:%t%Lr%n%n"'
alias maintainer="make -C /usr/ports/\$1 maintainer"
alias mdate='date -u ;date ;udate'
alias mtime='stat -f "Modify: %Sm %N"'
alias mv='mv -i'
alias myip='curl -s https://www.changeip.com/ip.asp |head -n1 |tr -d '\r''
alias pd2u="perl -i.orig -pne 's/\r\n/\n/g'"
alias permof='stat -f "%p %Sp %Su %Sg %N"'
alias ping8='ping -D -s 0'
alias pkg_ver="pkg_version -Ivl'<'"
alias pkg_which="pkg_info -qW"
alias qs="make -C /usr/ports quicksearch"
alias rm='rm -I'
alias slrn='slrn --kill-log $HOME/News/kill.log'
alias stop='kill -s STOP'
alias stopkill="kill \`ps a -o stat,pid |awk '/^T /{print \$2}'\`"
alias stoplist="ps -p \`ps a -o stat,pid |awk '/^T /{print \$2}'\` 2>/dev/null"
alias suspend='kill -s STOP'
alias svncleanup="svn propdel svn:sync-lock --revprop -r 0"
alias sweep='ping -D -G 56 -h 7'
alias term='kill -s TERM'
alias time="/usr/bin/time -h"
alias tpsys='xterm -fn 12x24 -geometry 80x30+0+0 -bw 0 -T XTerm'
alias trace='traceroute -P icmp'
alias udate='date "+%s"'
alias vim='vim -X'
alias xemacs='/usr/local/bin/emacs'
alias zomkill="kill \`ps a -o stat,pid |awk '/^Z /{print \$2}'\`"
alias zomlist="ps -p \`ps a -o stat,pid |awk '/^Z /{print \$2}'\` 2>/dev/null"
```