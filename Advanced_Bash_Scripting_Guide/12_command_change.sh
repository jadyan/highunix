#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-24 10:15:31
 # @LastEditTime: 2021-03-24 10:20:16
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/12_command_change.sh
### 
# csubloop.sh: 将循环的输出结果赋值给变量。
variable1=`for i in 1 2 3 4 5
do
    echo -n "$i"                 #  在这里，'echo' 命令非常关键。
done`
echo "variable1 = $variable1"  # variable1 = 12345
i=0

variable2=`while [ "$i" -lt 10 ]
do
    echo -n "$i"                 # 很关键的 'echo'。
    let "i += 1"                 # i 自增。
done`
echo "variable2 = $variable2"  # variable2 = 0123456789
# 这个例子表明可以在变量声明时嵌入循环。


#greeting=`./myctest`
greeting=$(./myctest)
echo $greeting


#$() `` 一样
variable2=$(while [ "$i" -lt 10 ]
do
    echo -n "$i"                 # 很关键的 'echo'。
    let "i += 1"                 # i 自增。
done)
echo "variable2 = $variable2"  # variable2 = 0123456789
# 这个例子表明可以在变量声明时嵌入循环。

exit 0