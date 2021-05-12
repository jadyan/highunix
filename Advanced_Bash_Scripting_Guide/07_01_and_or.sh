#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-19 13:38:02
 # @LastEditTime: 2021-03-19 14:08:03
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/07_01_if_else.sh
### 
#注意，双括号算术扩展表达式的退出状态码不是一个错误的值。算术表达式为0，返回1；算术表达式不为0，返回0。

((0&&1))# 逻辑与
echo $? 


let "num = ((0 && 1))"
echo $num
    
let "num = ((0 && 1 ))"
echo $?

echo "逻辑或"
((200||11))# 逻辑或
echo $?

let "num = (( 200 || 11 ))"
echo  $num

let "num = (( 200 || 11 ))"
echo $?


echo  "按位或"
((200|11))# 按位或
echo $?

# ...
let "num = (( 200 | 11 ))"
echo $num                    # 203


let "num = (( 200 | 11 ))"
echo $? # 0     **


echo "var=-2&&(( var+=2))"
var=-2&&(( var+=2))
echo $?

echo "var=-2&&(( var+=2))&& echo $var"
var=-2&&(( var+=2))&& echo $var
echo $?