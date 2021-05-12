#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-23 17:46:08
 # @LastEditTime: 2021-03-23 17:51:30
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/11_03_until.sh
### 
END_CONDITION=end


until [ "$var1" = "$END_CONDITION" ]
# 在循环顶部测试条件。
do
    echo "Input variable #1 "
    echo "($END_CONDITION to exit)"
    read var1
    echo "variable #1 = $var1"
    echo
done


#                ---                   #
#  就像 "for" 和 "while" 循环一样，
#+ "until" 循环也可以写的像C语言一样。
LIMIT=10
var=0

until (( var > LIMIT ))
do  # ^^ ^     ^     ^^   没有方括号，没有 $ 前缀。
    echo -n "$var "
    (( var++))
done    # 0 1 2 3 4 5 6 7 8 9 10
