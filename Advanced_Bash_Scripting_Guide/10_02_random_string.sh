#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-22 14:16:09
 # @LastEditTime: 2021-03-22 14:18:41
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/10_02_random_string.sh
### 
# rand-string.sh
# 产生一个8个字符的随机字符串。
if [ -n "$1" ]  #  如果在命令行中已经传入了参数，
then            #+ 那么就以它作为起始字符串。
    str0="$1"
else            #  否则，就将脚本的进程标识符PID作为起始字符串。
    str0="$$"
fi

POS=2  # 从字符串的第二位开始。
LEN=8  # 截取八个字符。

str1=$( echo "$str0" | md5sum | md5sum )
echo $str1

randstring="${str1:$POS:$LEN}"

echo "$randstring"