#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-23 10:13:35
 # @LastEditTime: 2021-03-23 10:31:55
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/10_05_02_string_change.sh
### 
# length.sh

#${#*} 和 ${#@} 返回位置参数的个数。
#任意数组 array，${#array[*]} 和 ${#array[@]} 返回数组中元素的个数。

#:${1? "use $0 ARGUMENT"}

E_NO_ARGS=65
if [ $# -eq 0 ]  # 脚本必须传入参数。
then
  echo "Please invoke this script with one or more command-line arguments."
  exit $E_NO_ARGS
fi


var01=abcdEFGH28ij
echo "var01 = ${var01}"
echo "Length of var01 = ${#var01}"

# 现在我们尝试加入空格。
var02="abcd EFGH28ij"
echo "var02 = ${var02}"
echo "Length of var02 = ${#var02}"

echo "Number of command-line arguments passed to script = ${#@}"
echo "Number of command-line arguments passed to script = ${#*}"


echo "-------------------------------------------------------------------"

echo "
\${var#Pattern}, \${var##Pattern}
\${var#Pattern} 删除 \$var 前缀部分匹配到的最短长度的 \$Pattern。
\${var##Pattern} 删除 \$var 前缀部分匹配到的最长长度的 \$Pattern。"

strip_leading_zero () #  删除传入参数中可能存在的
{                     #+ 前缀0。
  return=${1#0}       #  "1" 代表 "$1"，即传入的参数。
  echo ${return}
}                     #  从 "$1" 中删除 "0"。

strip_leading_zero 0aaahhh


strip_leading_zero2 () # 删除前缀0，
{                      # 否则 Bash 会将其解释为8进制数。
  shopt -s extglob     # 启用扩展通配特性。
  local val=${1##+(0)} # 使用本地变量，匹配前缀中所有的0。
  shopt -u extglob     # 禁用扩展通配特性。
  _strip_leading_zero2=${val:-0}
                       # 如果输入的为0，那么返回 0 而不是 ""。
  echo $val
  echo ""
}


strip_leading_zero2   0000bbbaaahhh


echo `basename $PWD`        # 当前工作目录的目录名。
echo "${PWD##*/}"           # 当前工作目录的目录名。
echo
echo `basename $0`          # 脚本名。
echo $0                     # 脚本名。
echo "${0##*/}"             # 脚本名。

echo
filename=test.data
echo "${filename##*.}"      # data
                            # 文件扩展名


echo "-----------------------------------------------------------------
\${var%Pattern}, \${var%%Pattern}
\${var%Pattern} 删除 \$var 后缀部分匹配到的最短长度的 \$Pattern。

\${var%%Pattern} 删除 \$var 后缀部分匹配到的最长长度的 \$Pattern。
-----------------------------------------------------------------
"

# 使用 # ## % %% 参数替换操作符进行模式匹配
var1=abcd12345abc6789
pattern1=a*c  # 通配符 * 可以匹配 a 与 c 之间的任意字符


echo
echo "var1 = $var1"           # abcd12345abc6789
echo "var1 = ${var1}"         # abcd12345abc6789
echo "Number of characters in ${var1} = ${#var1}"

echo
echo "pattern1 = $pattern1"   # a*c  (匹配 'a' 与 'c' 之间的一切)
echo "--------------"

echo '${var1#$pattern1}  =' "${var1#$pattern1}"    #         d12345abc6789
# 匹配到首部最短的3个字符                                   abcd12345abc6789
#             ^                                           |-|


echo '${var1##$pattern1} =' "${var1##$pattern1}"   #                  6789
# 匹配到首部最长的12个字符                                  abcd12345abc6789
#             ^                                           |----------|
echo; echo; echo


pattern2=b*9            # 匹配 'b' 与 '9' 之间的任意字符
echo "var1 = $var1"     # 仍旧是 abcd12345abc6789
echo
echo "pattern2 = $pattern2"
echo "--------------"
echo '${var1%pattern2}  =' "${var1%$pattern2}"     #     abcd12345a
# 匹配到尾部最短的6个字符                                  abcd12345abc6789
#             ^                                                    |----|

echo '${var1%%pattern2} =' "${var1%%$pattern2}"    #     a
# 匹配到尾部最长的12个字符                                 abcd12345abc6789
#             ^                                           |-------------|
# 牢记 # 与 ## 是从字符串左侧开始，
#      % 与 %% 是从右侧开始。
echo