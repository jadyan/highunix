#! /bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 15:31:30
 # @LastEditTime: 2021-03-26 15:34:52
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/29_array_strcat.sh
### 
# CopyArray.sh
#拷贝和连接数组
clear
CpArray_Mac() {
    # 建立赋值命令
    echo -n 'eval '
    echo -n "$2"                    # 目的名字
    echo -n '=( ${'
    echo -n "$1"                    # 源名字
    echo -n '[@]} )'
# 上边这些语句会构成一条命令。
# 这仅仅是形式上的问题。
}


declare -f CopyArray

CopyArray=CpArray_Mac


Hype() {
# "Pointer"函数
# 状态产生器
# 需要连接的数组名为$1.
# (把这个数组与字符串"Really Rocks"结合起来，形成一个新数组.)
# 并将结果从数组$2中返回.
    local -a TMP
    local -a hype=( Really Rocks )
    $($CopyArray $1 TMP)
    TMP=( ${TMP[@]} ${hype[@]} )
    $($CopyArray TMP $2)
}


declare -a before=( Advanced Bash Scripting )
declare -a after

echo "Array Before = ${before[@]}"

Hype before after
echo "Array After = ${after[@]}"


echo "What ${after[@]:3:2}?"
declare -a modest=( ${after[@]:2:1} ${after[@]:3:2} )
#                    ---- 子串提取 ----
echo "Array Modest = ${modest[@]}"
# 'before' 发生了什么变化 ?
echo "Array Before = ${before[@]}"
exit 0