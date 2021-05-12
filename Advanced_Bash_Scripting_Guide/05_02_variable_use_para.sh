#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-18 17:05:17
 # @LastEditTime: 2021-03-18 17:13:41
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/05_02_variable_use_para.sh
### 
# weirdvars.sh: 输出一些奇怪的变量
echo
var="'(]\\{}\$\""
echo $var        # '(]\{}$"
echo "$var"      # '(]\{}$"     没有任何区别。
echo


IFS='\'
echo $var        # '(] {}$"     \ 被转换成了空格，为什么？
echo "$var"      # '(]\{}$"



echo
var2="\\\\\""
echo $var2       #   "
echo "$var2"     # \\"
echo
# 但是...var2="\\\\"" 不是合法的语句，为什么？
var3='\\\\'
echo "$var3"     # \\\\
# 强引用是可以的。




# ************************************************************ #
# 就像第一个例子展示的那样，嵌套引用是允许的。

echo "$(echo '"')"           # "

# 在有些时候这种方法非常有用。


var1="Two bits"
echo "\$var1 = "$var1""      # $var1 = Two bits



#单引号（’ ‘）与双引号类似，但是在单引号中不能引用变量，因为 $ 不再具有特殊含义。在单引号中，
#除'之外的所有特殊字符都将会被直接按照字面意思解释。可以认为单引号（“全引用”）是双引号（“部分引用”）的一种更严格的形式。

echo "Why can't I write 's between single quotes"
echo

# 可以采取迂回的方式。
echo 'Why can'\''t I write '"'"'s between single quotes'
#    |-------|  |----------|   |-----------------------|
# 由三个单引号引用的字符串，再加上转义以及双引号包住的单引号组成。