#!/bin/bash
ps -ef|grep mask|grep -v "grep"|awk '{print $2}'|xargs kill -10
