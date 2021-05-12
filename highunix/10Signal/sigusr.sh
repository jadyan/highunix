#!/bin/bash


ps -aef|grep sigusr|grep -v "grep"|awk '{print $2}'|xargs kill -SIGUSR1
