#!/bin/bah
###
 # @Author: your name
 # @Date: 2021-03-17 10:49:39
 # @LastEditTime: 2021-03-17 10:56:02
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/03_ctrl_M.sh
### 
read -n 1 -s -p \
$'Control-M leaves cursor at beginning of this line. Press Enter. \x0d'
           # '0d' 是 Control-M 的十六进制的值
echo >&2   # '-s' 参数禁用了回显，所以需要显式的另起一行。


read -n 1 -s -p $'Control-J leaves cursor on next line. \x0a'
           # '0a' 是 Control-J 换行符的十六进制的值
echo >&2

read -n 1 -s -p $'And Control-K\x0bgoes straight down.'
echo >&2   # Control-K 是垂直制表符。

var=$'\x0aThis is the bottom line\x0bThis is the top line\x0a'
echo "$var"

echo "$var" | col


#  这却会使得右侧行高于左侧行。
#  这也解释了为什么我们需要在行首和行尾加上换行符
#  来避免显示的混乱。

# Lee Maschmeyer 的解释：
# --------------------------
#  在第一个垂直制表符的例子中，垂直制表符使其
#  在没有回车的情况下向下打印。
#  这在那些不能回退的设备上，例如 Linux 的终端才可以。
#  而垂直制表符的真正目的是向上而非向下。
#  它可以用来在打印机中用来打印上标。
#  col 工具可以用来模拟真实的垂直制表符行为。
exit 0
