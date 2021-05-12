#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-19 10:18:20
 # @LastEditTime: 2021-03-19 10:30:25
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/05_03_variable_Transferred_meaning.sh
### 
# escaped.sh: 转义字符
##############################################
### 首先让我们先看一下转义字符的基本用法。 ###
##############################################
# 转义新的一行。
# ------------



echo ""
echo "This will print
as two lines."
# This will print
# as two lines.

echo "This will print \
as one line."
# This will print as one line.


echo; echo
echo "============="


echo "\v\v\v\v"      # 按字面意思打印 \v\v\v\v
#使用 echo 命令的 -e 选项来打印转义字符。
echo "============="
echo "VERTICAL TABS"
echo -e "\v\v\v\v"   # 打印四个垂直制表符。
echo "=============="


echo "QUOTATION MARK"
echo -e "\042"       # 打印 " （引号，八进制ASCII码为42）。
echo "=============="


# 使用 $'\X' 这样的形式后可以不需要加 -e 选项。
echo; echo "NEWLINE and (maybe) BEEP"

echo $'\n' # 新的一行。
echo $'\a' ## 警报（响铃）。 # 根据不同的终端版本，也可能是闪屏。


# 我们之前介绍了 $'\nnn' 字符串扩展，而现在我们要看到的是...
# ============================================ #
# 自 Bash 第二个版本开始的 $'\nnn' 字符串扩展结构。
# ============================================ #


echo "Introducing the \$\' ... \' string-expansion construct . . . "
echo ". . . featuring more quotation marks."

echo $'\t \042 \t'   # 在制表符之间的引号。
# 需要注意的是 '\nnn' 是一个八进制的值。


# 字符串扩展同样适用于十六进制的值，格式是 $'\xhhh'。
echo $'\t \x22 \t'  # 在制表符之间的引号。
# 感谢 Greg Keraunen 指出这些。
# 在早期的 Bash 版本中允许使用 '\x022' 这样的形式。
echo


# 将 ASCII 码字符赋值给变量。
# -----------------------
quote=$'\042'        # 将 " 赋值给变量。
echo "$quote Quoted string $quote and this lies outside the quotes."



# 连接多个 ASCII 码字符给变量。
triple_underline=$'\137\137\137'  # 137是 '_' ASCII码的八进制形式
echo "$triple_underline UNDERLINE $triple_underline"

echo
ABC=$'\101\102\103\010'           # 101，102，103是 A, B, C 
                                  # ASCII码的八进制形式。
echo $ABC

echo


escape=$'\033'                    # 033 是 ESC 的八进制形式
echo "\"escape\" echoes an $escape"
                                  # 没有可见输出

echo
exit 0