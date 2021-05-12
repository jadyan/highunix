#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-19 15:40:35
 # @LastEditTime: 2021-03-19 15:52:57
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/07_06_zmore.sh
### 
# zmore
# 使用筛选器 'more' 查看 gzipped 文件。


E_NOARGS=85
E_NOTFOUND=86
E_NOTGZIP=87

echo "$#"
if [ $# -eq 0 ] # 作用和 if [ -z "$1" ] 相同。
# $1 可以为空： zmore "" arg2 arg3
then
    echo "Usage: `basename $0` filename" >&2
    # 将错误信息通过标准错误 stderr 进行输出。
    exit $E_NOARGS
    # 脚本的退出状态为 85.
fi


filename=$1
if [ ! -f "$filename" ]   # 引用字符串以防字符串中带有空格。
then
    echo "File $filename not found!" >&2   # 通过标准错误 stderr 进行输出。
    exit $E_NOTFOUND
fi

str=""

#-n -z 一定加上双引号 否则非常危险
#如果${str} 为空
if [ -z "${str}" ]; then
    echo "ok"
else
    echo "err"
fi

#若果${str}不为空
if [ -n "${str}" ]; then
    echo "ok"
else
    echo "err"
fi