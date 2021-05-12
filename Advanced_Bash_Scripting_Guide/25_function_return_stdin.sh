#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 14:10:55
 # @LastEditTime: 2021-03-26 14:14:24
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/25_function_return_stdin.sh
### 
# realname.sh
#
# 依靠username，从/etc/passwd 中获得“真名”.
ARGCOUNT=1       # 需要一个参数.
E_WRONGARGS=85
file=/etc/passwd
pattern=$1
if [ $# -ne "$ARGCOUNT" ]
then
    echo "Usage: `basename $0` USERNAME"
    exit $E_WRONGARGS
fi
file_excerpt ()    # 按照要求的模式来扫描文件,
{                  #+ 然后打印文件的相关部分。
    while read line  # "while" 并不一定非得有 [ 条件 ] 不可。
    do
        echo "$line" | grep $1 | awk -F":" '{ print $5 }'
        # awk用":" 作为界定符。
    done
    } <$file  # 重定向到函数的stdin。
file_excerpt $pattern
# 是的，整个脚本其实可以被缩减为
#       grep PATTERN /etc/passwd | awk -F":" '{ print $5 }'
# or
#       awk -F: '/PATTERN/ {print $5}'
# or
#       awk -F: '($1 == "username") { print $5 }' # 从username中获取真名
# 但是，这些起不到示例的作用。



: <<"EOF"
还有一个办法，或许能够更好的理解重定向函数的stdin。 它在函数内添加了一对大括号，并且将重定向stdin的行为放在这对添加的大括号上。

# 用下面的方法来代替它:
Function ()
{
    ...
} < file

# 试试这个:
Function ()
{
    { 
        ...
    } < file 
}

# 同样的,
Function ()  # 没问题.
{
    {
        echo $*
    } | tr a b 
}
Function ()  # 不行.
{
    echo $*
} | tr a b   # 这儿的内嵌代码块是被强制的。
# 感谢, S.C.
EOF