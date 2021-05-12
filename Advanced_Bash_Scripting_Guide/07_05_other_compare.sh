#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-19 15:18:05
 # @LastEditTime: 2021-03-19 15:54:03
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/07_05_other_compare.sh
### 
a=4
b=5

# 这里的 "a" 和 "b" 可以是整数也可以是字符串。
# 因为 Bash 的变量是弱类型的，因此字符串和整数比较有很多相同之处。
# 在 Bash 中可以用处理整数的方式来处理全是数字的字符串。
# 但是谨慎使用。


#-n
#字符串非空（null）。
#caution 使用 -n 时字符串必须是在括号中且被引用的。使用 ! -z 判断未引用的字符串或者直接判断（样例 7-6）通常可行，但是非常危险。判断字符串时一定要引用[^1]。
#
#-n -z 一定加上双引号 否则非常危险

echo

if [ "$a" -ne "$b" ]
then
  echo "$a is not equal to $b"
  echo "(arithmetic comparison)"
fi




if [ "$a" != "$b" ]
then
  echo "$a is not equal to $b."
  echo "(string comparison)"
  #     "4"  != "5"
  # ASCII 52 != ASCIII 53
fi


#-z 字符串为空，即字符串长度为0 k 空时为真
String=''   # 长度为0的字符串变量。
if [ -z "$String" ]
then
    echo "\$String is null.-z"
else
     echo "\$String is NOT null. -z"
fi     # $String is null.

#-n 字符串非空（null）。 非空为真
# 使用 if [ ... ] 结构
# 如果字符串未被初始化，则其值是未定义的。
# 这种状态就是空 "null"（并不是 0）。
if [ -n $string1 ]    # 并未声明或是初始化 string1。
then
    echo "String \"string1\" is not null."
else
    echo "String \"string1\" is null."
fi

echo
# 再试一次。
if [ -n "$string1" ]   # 这次引用了 $string1。
then
    echo "String \"string1\" is not null."
else
    echo "String \"string1\" is null."
fi                    # 在测试括号内引用字符串得到了正确的结果。


if [ $string1 ]       # 这次只有一个 $string1。
then
  echo "String \"string1\" is not null."
else
  echo "String \"string1\" is null."
fi                    # 结果正确。

# 独立的 [ ... ] 测试操作符可以用来检测字符串是否为空。
# 最好将字符串进行引用（if [ "$string1" ]）。
#
# Stephane Chazelas 指出：
#    if [ $string1 ]    只有一个参数 "]"
#    if [ "$string1" ]  则有两个参数，空的 "$string1" 和 "]"



echo

string1=initialized

if [ $string1 ]       # $string1 这次仍然没有被引用。
then
    echo "String \"string1\" is not null."
else
    echo "String \"string1\" is null."
fi                    # 这次的结果仍然是正确的。
# 最好将字符串引用（"$string1"）


string1="a = b"
if [ $string1 ]       # $string1 这次仍然没有被引用。
then
  echo "String \"string1\" is not null."
else
  echo "String \"string1\" is null."
fi                    # 这次没有引用就错了。

exit 0   # 同时感谢 Florian Wisser 的提示。



