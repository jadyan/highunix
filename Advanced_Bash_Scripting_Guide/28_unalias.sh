#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 14:28:44
 # @LastEditTime: 2021-03-26 14:28:45
 # @LastEditors: your name
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/28_unalias.sh
### 
# unalias.sh
shopt -s expand_aliases  # 开启别名扩展.
alias llm='ls -al | more'
llm
echo
unalias llm              # 取消别名.
llm
# 'llm' 不再被识别后的报错信息.