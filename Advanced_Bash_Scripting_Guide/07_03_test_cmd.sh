#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-19 14:29:05
 # @LastEditTime: 2021-03-19 14:56:54
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/07_03_test_cmd.sh
### 
echo

str=$1

#-z 空为true
#-n 非空为true
if test -z $str 
then
     echo "No command-line arguments."
else
    echo "First command-line argument is $1."
fi


if /usr/bin/test -z "$1"  # 等价于内建命令 "test"
#  ^^^^^^^^^^^^^              # 指定全路径
then
  echo "No command-line arguments."
else
  echo "First command-line argument is $1."
fi


if [ -z "$1" ] # 功能和上面的代码相同。
#if [ -z "$1"                #理论上可行，但是 Bash 会提示缺失右括号
then
  echo "No command-line arguments."
else
  echo "First command-line argument is $1."
fi
echo

if /usr/bin/[ -z "$1" ]
#if /usr/bin/[ -z "$1"       # 理论上可行，但是会报错
#                             # 已经在 Bash 3.x 版本被修复了
then
    echo "No command-line arguments."   
else
    echo "First command-line argument is $1."
fi


#在 Bash 里，[[ ]] 是比 [ ] 更加通用的写法。其作为扩展test 命令从 ksh88 中被继承了过来。
#在 [[ 和 ]] 中不会进行文件名扩展或字符串分割，但是可以进行参数扩展和命令替换。

#使用 [[...]] 代替 [...]可以避免很多逻辑错误。比如可以在 [[]] 中使用 &&，||，< 和 > 操作符，而在 [] 中使用则会报错。

file=/etc/passwd

if [[ -e $file ]]
then
  echo "Password file exists."
fi

#在 [[]] 中会自动执行八进制和十六进制的进制转换操作。

decimal=15
octal=017       # = 15 (十进制)
hex=0x0f        # = 15 (十进制)


if [ $decimal -eq $octal ];then
    echo "$decimal equals $octal"
else
    # 在单括号 [ ] 之间不会进行进制转换。
    echo "$decimal is not equal to $octal"# 15 不等于 017
fi

if [[ $decimal -eq $octal ]];then
    echo "$decimal equals $octal"
else
    # # 在双括号 [[ ]] 之间会进行进制转换。
    echo "$decimal is not equal to $octal"# 15 不等于 017
fi

if [[ "$decimal" -eq "$hex" ]]
then
    echo "$decimal equals $hex"     # 15 等于 0x0f
else
    echo "$decimal is not equal to $hex"
fi 
# 十六进制也可以进行转换。


#语法上并不严格要求在 if 之后一定要写 test 命令或者测试结构（[] 或 [[]]）。
#if COMMAND 的退出状态就是COMMAND 的退出状态。
dir=/home/bozo

if cd $dir 2>/dev/null ; then  # "2>/dev/null" 重定向消去错误输出。
    echo "Now in $dir."
else
    echo "Can't change to $dir."
fi


#同样的，测试括号也不一定需要与 if 一起使用。其可以同 列表结构 结合而不需要 if。

var1=20
var2=22
[ "$var1" -ne "$var2" ] && echo "$var1 is not equal to $var2"


#(( )) 结构 扩展和执行算术表达式。如果执行结果为0，其返回的 退出状态码 为1（假）。非0表达式返回的退出状态为0（真）。这与上述所使用的 test 和 [ ] 结构形成鲜明的对比。

((0))
echo "Exit status of \"(( 0 ))\" is $?." # 1

((1))
echo "Exit status of \"(( 1 ))\" is $?."# 0


((5>4))# 真
echo "Exit status of \"(( 5 > 4 ))\" is $?."# 0


((5>9))# 假
echo "Exit status of \"(( 5 > 9 ))\" is $?."# 1

((5==5))# 真
echo "Exit status of \"(( 5 == 5 ))\" is $?."# 0
# (( 5 = 5 )) 会报错。

((5-5))# 0
echo "Exit status of \"(( 5 - 5 ))\" is $?."# 1

((5/4))# 合法
echo "Exit status of \"(( 5 / 4 ))\" is $?."# 0 


((1/2))# 结果小于1
echo "Exit status of \"(( 1 / 2 ))\" is $?."# 舍入至0。
# 1



((1/0))2>/dev/null                          # 除0，非法
#           ^^^^^^^^^^^
echo "Exit status of \"(( 1 / 0 ))\" is $?."# 1

# "2>/dev/null" 的作用是什么？
# 如果将其移除会发生什么？
# 尝试移除这条语句并重新执行脚本。
# ======================================= #
# (( ... )) 在 if-then 



var1=5
var2=4
if (( var1 > var2 ))
then #^      ^      注意不是 $var1 和 $var2，为什么？
  echo "$var1 is greater then $var2"
fi # 5 大于 4







