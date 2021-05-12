#!/bin/bash


logname=tell.log
cat /dev/null>tell.log

while true
do
	./tellwait1 >> tell.log
done
