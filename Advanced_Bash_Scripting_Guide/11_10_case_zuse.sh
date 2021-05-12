#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-24 09:59:28
 # @LastEditTime: 2021-03-24 10:01:45
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/11_10_case_zuse.sh
### 
# isaplpha.sh: 使用 "case" 结构检查输入。
SUCCESS=0
FAILURE=1   #  以前是FAILURE=-1,
            #+ 但现在 Bash 不允许返回负值。
isalpha ()  # 测试字符串的第一个字符是否是字母。
{
if [ -z "$1" ]                # 检测是否传入参数。
then
  return $FAILURE
fi
case "$1" in
  [a-zA-Z]*) return $SUCCESS;;  # 是否以字母形式开始？
  *        ) return $FAILURE;;
esac
}             # 可以与 C 语言中的函数 "isalpha ()" 作比较。
isalpha2 ()   # 测试整个字符串是否都是字母。
{

  [ $# -eq 1 ] || return $FAILURE
  case $1 in
  *[!a-zA-Z]*|"") return $FAILURE;;
               *) return $SUCCESS;;
  esac
}
isdigit ()    # 测试整个字符串是否都是数字。
{             # 换句话说，也就是测试是否是一个整型变量。
  [ $# -eq 1 ] || return $FAILURE
  case $1 in
    *[!0-9]*|"") return $FAILURE;;
              *) return $SUCCESS;;
  esac
}
check_var ()  # 包装后的 isalpha ()。
{
if isalpha "$@"
then
  echo "\"$*\" begins with an alpha character."
  if isalpha2 "$@"
  then        # 其实没必要检查第一个字符是不是字母。
    echo "\"$*\" contains only alpha characters."
  else
    echo "\"$*\" contains at least one non-alpha character."
  fi
else
  echo "\"$*\" begins with a non-alpha character."
              # 如果没有传入参数同样同样返回“存在非字母”。
fi
echo
}
digit_check ()  # 包装后的 isdigit ()。
{
if isdigit "$@"
then
  echo "\"$*\" contains only digits [0 - 9]."
else
  echo "\"$*\" has at least one non-digit character."
fi
echo
}
a=23skidoo
b=H3llo
c=-What?
d=What?
e=$(echo $b)   # 命令替换。
f=AbcDef
g=27234
h=27a34
i=27.34
check_var $a
check_var $b
check_var $c
check_var $d
check_var $e
check_var $f
check_var     # 如果不传入参数会发送什么？
#
digit_check $g
digit_check $h
digit_check $i
exit 0        # S.C. 改进了本脚本。
# 练习：
# -----
# 写一个函数 'isfloat ()' 来检测输入值是否是浮点数。
# 提示：可以参考函数 'isdigit ()'，在其中加入检测合法的小数点即可。