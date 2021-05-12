#!/bin/bash
###
# @Author: your name
# @Date: 2021-03-24 09:23:25
 # @LastEditTime: 2021-03-24 09:42:09
 # @LastEditors: Please set LastEditors
# @Description: In User Settings Edit
# @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/11_05_circle_control.sh
### 
LIMIT=19  # 循环上界
echo
echo "Printing Numbers 1 through 20 (but not 3 and 11)."
a=0
while [ $a -le "$LIMIT" ]
do
    a=$(($a+1))
	if [ "$a" -eq 3 ] || [ "$a" -eq 11 ]  # 除了 3 和 11。
	then
	    continue      # 略过本次循环的剩余部分。
	fi
	    echo -n "$a "   # 当 a 等于 3 和 11 时，将不会执行这条语句。
done
# 思考：
# 为什么循环不会输出到20？
echo; echo
echo Printing Numbers 1 through 20, but something happens after 2.
##################################################################
# 用 'break' 代替了 'continue'。
a=0
while [ "$a" -le "$LIMIT" ]
do
    a=$(($a+1))
    if [ "$a" -gt 2 ]
    then
        break  # 中止循环。
    fi
    echo -n "$a "
done
echo; echo; echo
exit 0
