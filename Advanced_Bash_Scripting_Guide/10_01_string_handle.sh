#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-22 13:47:36
 # @LastEditTime: 2021-03-22 17:59:16
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/10_01_string_handle.sh
### 

stringZ=abcABC123ABCabc

echo ${#stringZ}

echo `expr length $stringZ`
#上面两个表达式等价于C语言中的 strlen() 函数

echo `expr "$stringZ" : '.*'`



# paragraph-space.sh
# 版本 2.1，发布日期 2012年7月29日
# 在无空行的文本文件的段落之间插入空行。
# 像这样使用: $0 <FILENAME

MINLEN=60        # 可以试试修改这个值。它用来做判断。
#  假设一行的字符数小于 $MINLEN，并且以句点结束段落。
#+ 结尾部分有练习！

while read line  # 当文件有许多行的时候
do
  echo "$line"   # 输出行本身。
  len=${#line}
  if [[ "$len" -lt "$MINLEN" && "$line" =~ [*{\.}]$ ]]
# if [[ "$len" -lt "$MINLEN" && "$line" =~ \[*\.\] ]]
# 新版Bash将不能正常运行前一个版本的脚本。Ouch！
# 感谢 Halim Srama 指出这点，并且给出了修正版本。
    then echo    #  在该行以句点结束时，
  fi             #+ 增加一行空行。
done




echo "expr match  操作（匹配字符串长度）"
stringZ=abcABC123ABCabc
echo `expr match "$stringZ" 'abc[A-Z]*.2'` 
echo `expr "$stringZ" : 'abc[A-Z]*.2'`       # 8



echo "expr index 操作 （返回在 $string 中第一个出现的 $substring 字符所在的位置。）"
echo "几乎等价于C语言中的 strchr()。"
stringZ=abcABC123ABCabc
echo `expr index "$stringZ" C12`
echo `expr index "$stringZ" 1c`


echo "截取字符串（字符串分片）"
echo "在 $string 中截取自 $position 起的字符串。"
stringZ=abcABC123ABCabc
#       0123456789.....
#       索引位置从0开始。

echo ${stringZ:0}   #abcABC123ABCabc
echo ${stringZ:1}   #bcABC123ABCabc
echo ${stringZ:7}   #23ABCabc
echo ${stringZ:7:3} #23A #三个字符的子字符串。

echo "从右到左截取"
# 从右至左进行截取可行么？
echo ${stringZ:-4}   
# ${parameter:-default} 将会得到整个字符串。

## 括号或者增加空格都可以"转义"位置参数
echo ${stringZ:(-4)}
echo ${stringZ: -4} 



echo "expr substr \$string \$position \$length, 在 $string 中截取自 $position 起，长度为 $length 的字符串"
#position 从1开始

stringZ=abcABC123ABCabc

echo `expr substr $stringZ 1 2`              # ab
echo `expr substr $stringZ 4 3`              # ABC

echo "----------------------------------------------------------------------"

echo "expr match "$string" '\($substring\)' \
    在 $string 中截取自 $position 起的字符串，其中 $substring 是正则表达式。
expr "$string" : '\($substring\)' \
    在 $string 中截取自 $position 起的字符串，其中 $substring 是正则表达式。\
    "
stringZ=abcABC123ABCabc
#       =======
echo `expr match "$stringZ" '\(.[b-c]*[A-Z]..[0-9]\)'`   # abcABC1
echo `expr "$stringZ" : '\(.[b-c]*[A-Z]..[0-9]\)'`       # abcABC1
echo `expr "$stringZ" : '\(........\)'`                   # abcABC12




echo "----------------------------------------------------------------------"



echo "
\${string#substring}
删除从 \$string 起始部分起，匹配到的最短的 \$substring。

\${string##substring}
删除从 \$string 起始部分起，匹配到的最长的 \$substring。 "


stringZ=abcABC123ABCabc
#       |----|          最长
#       |----------|    最短

echo ${stringZ#a*C}      # 123ABCabc
# 删除 'a' 与 'c' 之间最短的匹配。
echo ${stringZ}

# 删除 'a' 与 'c' 之间最长的匹配。
echo ${stringZ##a*C}


# 你可以使用变量代替 substring。


X='a*C'

echo ${stringZ#${X}}      # 123ABCabc
echo ${stringZ##$X}     # abc



echo "
\${string%substring}
删除从 \$string 结尾部分起，匹配到的最短的 \$substring。
"

stringZ=abcABC123ABCabc
#                    ||     最短
#        |------------|     最长

echo ${stringZ%b*c}
echo ${stringZ%%b*c}



# 将当前目录下所有后缀名为 "TXT" 的文件改为 "txt" 后缀。
# 例如 "file1.TXT" 改为 "file1.txt"。
#SUFF=TXT
#suff=txt
#for i in $(ls *.$SUFF)
#do
#  mv -f $i $(i%.$SUFF).$suff
#  #  除了从变量 $i 右侧匹配到的最短的字符串之外，
#  #+ 其他一切都保持不变。
#done ### 如果需要，循环可以压缩成一行的形式。
# 感谢 Rory Winston。




echo "
子串替换
\${string/substring/replacement}
替换匹配到的第一个 \$substring 为 \$replacement。[^2]

\${string//substring/replacement}
替换匹配到的所有 \$substring 为 \$replacement。
"

# # 将匹配到的第一个 'abc' 替换为 'xyz'。
stringZ=abcABC123ABCabc
echo ${stringZ/abc/xyz}   

# 将匹配到的所有 'abc' 替换为 'xyz'。
echo ${stringZ//abc/xyz}   



echo  ---------------
echo "$stringZ"               # abcABC123ABCabc
echo  ---------------
                              # 字符串本身并不会被修改！


# 匹配以及替换的字符串可以是参数么？
match=abc
repl=000
echo ${stringZ/$match/$repl}  # 000ABC123ABCabc
#              ^      ^         ^^^
echo ${stringZ//$match/$repl} # 000ABC123ABC000
# Yes!      


# 如果没有给定 $replacement 字符串会怎样？
echo ${stringZ/abc}           # ABC123ABCabc
echo ${stringZ//abc}          # ABC123ABC

echo "
\${string/#substring/replacement}
替换 \$string 中最前端匹配到的 \$substring 为 \$replacement。

\${string/%substring/replacement}
替换 \$string 中最末端匹配到的 \$substring 为 \$replacement。
"
stringZ=abcABC123ABCabc
echo ${stringZ/#abc/XYZ}          # XYZABC123ABCabc
                                  # 将前端的 'abc' 替换为 'XYZ'

echo ${stringZ/%abc/XYZ}          # abcABC123ABCXYZ
                                  # 将末端的 'abc' 替换为 'XYZ'

exit