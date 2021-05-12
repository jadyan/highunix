#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-18 16:20:22
 # @LastEditTime: 2021-03-18 16:25:49
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/0404_variable_spacial.sh
### 
# 调用脚本时使用至少10个参数，例如
# ./scriptname 1 2 3 4 5 6 7 8 9 10
MINPARAMS=10

echo
echo "The name of this script is \"$0\"."
# 附带 ./ 代表当前目录
echo "The name of this script is \"`basename $0`\"."
# 除去路径信息（查看 'basename'）
echo

if [ -n "$1" ]              # 测试变量是否存在
then
    echo "Parameter #1 is $1"  # 使用引号转义#
fi


if [ -n "$2" ]
then
 echo "Parameter #2 is $2"
fi
if [ -n "$3" ]
then
 echo "Parameter #3 is $3"
fi
# ...
if [ -n "${10}" ]  # 大于 $9 的参数必须被放在大括号中
then
 echo "Parameter #10 is ${10}"
fi
echo "-----------------------------------"
echo "All the command-line parameters are: "$*"" #所有的参数
echo

args=$#           # 传入参数的个数
echo ${args}

#传入参数个数
lastarg=${!args}
# 这是 $args 的一种间接引用方式
echo ${lastarg}


 lastarg2=${!#} 
 echo ${lastarg2} 
#这是 $# 的一种间接引用方式。
# 注意 lastarg=${!$#} 是无效的。