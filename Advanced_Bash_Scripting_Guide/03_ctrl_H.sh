#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-17 10:25:20
 # @LastEditTime: 2021-03-17 10:34:33
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/03_ctrl_sign.sh
### 


a=""                  # 两个退格符 Ctrl-H
                          # 在 vi/vim 中使用 Ctrl-V Ctrl-H 来键入
echo "abcdef"             # abcdef
echo
echo -n "abcdef$a "       # abcd f  ^ 末尾有空格退格两次的结果

#
echo  "没有空格"
echo
echo -n "abcdef$a"        # abcdef (我的一样有效)
#                                ^ 末尾没有空格时为什么退格无效了？
                          # 并不是我们期望的结果。
