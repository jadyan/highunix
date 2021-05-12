#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-18 16:43:38
 # @LastEditTime: 2021-03-18 16:45:19
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/040702_variable_shift.sh
### 


#./040702_variable_shift.sh  12  34 56  78
#78


# shift-past.sh
shift 3    # 移动3位。
# 与 n=3; shift $n 效果相同。
echo "$1"
exit 0
# ======================== #




#  但是就像 Eleni Fragkiadaki 指出的那样，
#  如果尝试将位置参数（$#）传给 'shift'，
#  将会导致脚本错误的结束，同时位置参数也不会发送改变。
#  这也许是因为陷入了一个死循环...
#  比如：
#      until [ -z "$1" ]
#      do
#         echo -n "$1 "
#         shift 20    #  如果少于20个位置参数，
#      done           #+ 那么循环将永远不会结束。
#
#  当你不确定是否有这么多的参数时，你可以加入一个测试：
#      shift 20 || break
#               ^^^^^^^^