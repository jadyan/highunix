#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-24 09:14:46
 # @LastEditTime: 2021-03-24 09:15:10
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/11_04_for_inner.sh
### 
# nested-loop.sh: 嵌套 "for" 循环。
outer=1             # 设置外层循环计数器。
# 外层循环。
for a in 1 2 3 4 5 
do
    echo "Pass $outer in outer loop."
    echo "---------------------"
    inner=1           # 重设内层循环计数器。
    # =====================================
    # 内层循环。
    for b in 1 2 3 4 5
    do
        echo "Pass $inner in inner loop."
        let "inner+=1"  # 增加内层循环计数器。
    done
    # 内层循环结束。
    # =====================================
    let "outer+=1"    # 增加外层循环计数器。
    echo              # 在每次外层循环输出中加入空行。
done
# 外层循环结束。
exit 0