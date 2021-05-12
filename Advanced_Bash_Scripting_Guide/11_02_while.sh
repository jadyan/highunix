#!/bin/bash


###
 # @Author: your name
 # @Date: 2021-03-23 17:21:13
 # @LastEditTime: 2021-03-23 17:29:45
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/11_02_for.sh
### 
var0=0
LIMIT=10
while [ "$var0" -lt "$LIMIT" ]
#      ^                    ^
# 必须有空格，因为这是测试结构
do
    echo -n "$var0 " # -n 不会另起一行
#             ^           空格用来分开输出的数字。
    #var0=`expr $var0 + 1` 
    # var0=$(($var0+1))  效果相同。
    # var0=$((var0 + 1)) 效果相同。
    # let "var0 += 1"    效果相同。
    ((var0++))
done    # 还有许多其他的方法也可以达到相同的效果。
echo ; echo ;


echo
# 等价于：
while [ "$var1" != "end" ] # while test "$var1" != "end"
do
    echo "Input variable #1 (end to exit) "
    read var1                    # 不是 'read $var1' （为什么？）。
    echo "variable #1 = $var1"   # 因为存在 "#"，所以需要使用引号。
    # 如果输入的是 "end"，也将会在这里输出。
    # 在结束本轮循环之前都不会再测试循环条件了。
    echo
done


echo ; echo ;

var1=unset
previous=$var1
while echo "previous-variable = $previous"
      echo
      previous=$var1
[ "$var1" != end ]
# 记录下 $var1 之前的值。
# 在 while 循环中有4个条件，但只有最后的那个控制循环。
# 最后一个条件的退出状态才会被记录。
do
    echo "Input variable #1 (end to exit) "
    read var1
    echo "variable #1 = $var1"
done