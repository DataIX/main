#!/bin/sh
#
# $Id$
#

# PROVIDE: netwait
# REQUIRE: NETWORKING
# BEFORE: mountcritremote
# KEYWORD: nojail

. /etc/rc.subr

name="netwait"
rc_var=`set_rcvar`

start_cmd="${name}_start"
stop_cmd=":"

: ${netwait_enable:="NO"}		# Wait for network availability.
: ${netwait_address:="127.0.0.1"}	# IPv4 or IPv6 address to ping.
: ${netwait_flags:="-c 6"}		# see ping(8) & ping6(8).
: ${netwait_ping:=/sbin/ping}		# Which ping to use. 4 or 6
: ${netwait_sleep:=0}			# Time to sleep after ping[6](8)

netwait_start()
{
	local rc

	echo "Waiting for ${netwait_address} to respond..."

	if [ -x ${netwait_ping} ]; then
		${netwait_ping} ${netwait_flags} ${netwait_address} >>/dev/null 2>&1
	else
		warn "${netwait_ping} is not available."
		return
	fi
	rc=$?

	if [ ${netwait_sleep} -gt 0 ]; then
		sleep ${netwait_sleep}
	fi

	if [ $rc -eq 0 ]; then
		echo "Host ${netwait_address} responded; Network is available."
	else
		echo "No response from ${netwait_address}, Continuing..."
	fi
}

load_rc_config $name
run_rc_command "$1"
