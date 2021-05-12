#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 13:41:41
 # @LastEditTime: 2021-03-26 13:41:57
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/25_function.sh
### 
# 函数和参数
clear;
DEFAULT=default                 # 默认参数值。D
func2 () {
    if [ -z "$1" ]              # 第一个参数长度是否为零？
    then
        echo "-Parameter #1 is zero length.-"  # 或者没有参数传递进来。
    else
        echo "-Parameter #1 is \"$1\".-"
    fi
    variable=${1-$DEFAULT}
    echo "variable = $variable"     #  这里的参数替换
                                    #+ 表示什么？
                                    #  ---------------------------
                                    #  为了区分没有参数的情况
                                    #+ 和只有一个null参数的情况。
    if [ "$2" ]
    then
        echo "-Parameter #2 is \"$2\".-"
    fi
    return 0
}
echo
echo "Nothing passed."
func2                          # 不带参数调用
echo
echo "Zero-length parameter passed."
func2 ""                        # 使用0长度的参数进行调用
echo
echo "Null parameter passed."
func2 "$uninitialized_param"    # 使用未初始化的参数进行调用
echo
echo "One parameter passed."
func2 first           # 带一个参数的调用
echo
echo "Two parameters passed."
func2 first second    # 带两个参数的调用
echo
echo "\"\" \"second\" passed."
func2 "" second       # 第一个调用参数为0长度参数，
echo                  # 第二个是ASCII码的字符串参数。
exit 0