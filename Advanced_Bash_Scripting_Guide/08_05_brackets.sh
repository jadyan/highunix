#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-19 17:20:59
 # @LastEditTime: 2021-03-19 17:25:29
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/08_05_brackets.sh
### 
# c-vars.sh
# 以C风格的方式操作变量，使用(( ... ))结构
echo
(( a = 23 ))  #  C风格的变量赋值，注意"="等号前后都有空格


echo "a (initial value) = $a"   # 23

(( a++ ))     #  后缀自增'a'，C-style.
echo "a (after a++) = $a"       # 24


(( a-- ))     #  后缀自减'a', C-style.
echo "a (after a--) = $a"       # 23


(( ++a ))     #  前缀自增'a', C-style.
echo "a (after ++a) = $a"       # 24

(( --a ))     #  前缀自减'a', C-style.
echo "a (after --a) = $a"       # 23
echo



########################################################
#  注意，C风格的++，--运算符，前缀形式与后缀形式有不同的
#+ 副作用。
n=1; let --n && echo "True" || echo "False"  # False
n=1; let n-- && echo "True" || echo "False"  # True
#  感谢 Jeroen Domburg。
########################################################




echo
(( t = a<45 ? 7:11 ))   # C风格三目运算符。
#       ^  ^ ^
echo "If a < 45, then t = 7, else t = 11."  # a = 23
echo "t = $t "                              # t = 7
echo


# -----------
# 复活节彩蛋!
# -----------
#  Chet Ramey 偷偷往Bash里加入了C风格的语句结构，
#  还没写文档说明 (实际上很多是从ksh中继承过来的)。
#  在Bash 文档中，Ramey把 (( ... ))结构称为shell 算术运算，
#  但是这种表述并不准确...
#  抱歉啊，Chet，把你的秘密抖出来了。
#  参看 "for" 和 "while" 循环章节关于 (( ... )) 结构的部分。
#  (( ... )) 结构在Bash 2.04版本之后才能正常工作。
exit