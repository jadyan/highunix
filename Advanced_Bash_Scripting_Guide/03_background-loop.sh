#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-12 14:54:33
 # @LastEditTime: 2021-03-12 14:54:34
 # @LastEditors: your name
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/03_background-loop.sh
### 
# background-loop.sh
for i in 1 2 3 4 5 6 7 8 9 10            # 第一个循环
do
  echo -n "$i "
done & # 这个循环在后台运行。
       # 有时会在第二个循环结之后才执行此后台循环。
echo   # 此'echo' 有时不显示
for i in 11 12 13 14 15 16 17 18 19 20   # 第二个循环
do
  echo -n "$i "
done
echo   # 此'echo' 有时不显示
# ======================================================
# 脚本期望输出结果：
# 1 2 3 4 5 6 7 8 9 10
# 11 12 13 14 15 16 17 18 19 20
# 一些情况下可能会输出：
# 11 12 13 14 15 16 17 18 19 20
# 1 2 3 4 5 6 7 8 9 10 bozo $
# 第二个 'echo' 没有被执行，为什么？
# 另外一些情况下可能会输出：
# 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
# 第一个 'echo' 没有被执行，为什么？
# 非常罕见的情况下，可能会输出：
# 11 12 13 1 2 3 4 5 6 7 8 9 10 14 15 16 17 18 19 20
# 前台循环抢占（preempt）了后台循环。
exit 0
#  Nasimuddin Ansari 建议：在第6行和第14行的
#  echo -n "$i " 后增加 sleep 1，
#  会得到许多有趣的结果。