#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 14:22:50
 # @LastEditTime: 2021-03-26 14:22:50
 # @LastEditors: your name
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/27_fibo.sh
### 
# fibo.sh : 斐波那契序列 (递归)
# 作者: M. Cooper
# License: GPL3
# ----------算法--------------
# Fibo(0) = 0
# Fibo(1) = 1
# else
#   Fibo(j) = Fibo(j-1) + Fibo(j-2)
# ---------------------------------
MAXTERM=15       # 要产生的计算次数。
MINIDX=2         # 如果下标小于2，那么 Fibo(idx) = idx.
Fibonacci ()
{
    idx=$1   # 不需要是局部变量，为什么？
    if [ "$idx" -lt "$MINIDX" ]
    then
        echo "$idx"  # 前两个下标是0和1 ... 从上面的算法可以看出来。
    else
        (( --idx ))  # j-1
        term1=$( Fibonacci $idx )   #  Fibo(j-1)
        (( --idx ))  # j-2
        term2=$( Fibonacci $idx )   #  Fibo(j-2)
        echo $(( term1 + term2 ))
    fi
    #  一个丑陋的实现
    #  C语言里，一个更加优雅的斐波那契递归实现
    #+ 是一个简单的只需要7-10代码的算法翻译。
}
for i in $(seq 0 $MAXTERM)
do  # 计算 $MAXTERM+1 次.
    FIBO=$(Fibonacci $i)
    echo -n "$FIBO "
done
# 0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 610
# 要花费一段时间，不是么？ 一个递归脚本是有些慢的。
echo
exit 0