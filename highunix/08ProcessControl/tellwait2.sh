#!/bin/bash


logname=tell.log
cat /dev/null>tell.log

while true
do
	./tellwait2 >> tell2.log
done
