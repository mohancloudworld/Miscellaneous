#!/bin/bash

# This script reschedules itself every 6 minutes and if the time is 
# past 5PM restarts, the machine after 5 mins. This makes the machine
# auto-restart every 5 minutes, till the local time is past 12 in the 
# midnight.
# This script should be executed with sudo user or root privilages.

echo `which $0`|at now + 6 minutes
# restart only after 5PM, till midnight
if [[ `date +"%H"` -gt "17" ]]; then
	echo 'init 6'|at now + 5 minutes
fi
