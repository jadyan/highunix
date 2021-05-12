#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-24 09:43:00
 # @LastEditTime: 2021-03-24 09:44:04
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/11_06_circle_break.sh
### 
# break-levels.sh: 跳出循环.
# "break N" 跳出 N 层循环。
for outerloop in 1 2 3 4 5
do
  echo -n "Group $outerloop:   "
  # ------------------------------------------
  for innerloop in 1 2 3 4 5
  do
    echo -n "$innerloop "
    if [ "$innerloop" -eq 3 ]
    then
      break  # 尝试一下 break 2 看看会发生什么。
             # （它同时中止了内层和外层循环。）
    fi
  done
  # ------------------------------------------
  echo
done
echo
exit 0