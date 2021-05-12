#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-18 16:37:10
 # @LastEditTime: 2021-03-18 16:42:10
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/0407_variable_shift.sh

 #使用 shift 命令可以将全体位置参数向左移一位, 重新赋值。

#$1 <--- $2, $2 <--- $3, $3 <--- $4，以此类推。

#原先的 $1 将会消失，而 $0（脚本名称）不会有任何改变。如果你在脚本中使用了大量的位置参数，shift 可以让你不使用{大括号}助记法也可以访问超过10个的位置参数。
### 
# shft.sh: 使用 `shift` 命令步进访问所有的位置参数。
# 将这个脚本命名为 shft.sh，然后在调用时跟上一些参数。
# 例如：
#    sh shft.sh a b c def 83 barndoor

until [ -z "$1" ]  # 直到访问完所有的参数
do
  echo  "$1 "
  shift
done

echo               # 换行。

echo               # 换行。
# 那些被访问完的参数又会怎样呢？
echo "$2"
# 什么都不会被打印出来。
# 当 $2 被移动到 $1 且没有 $3 时，$2 将会保持空。
# 因此 shift 是移动参数而非复制参数。


exit
#  可以参考 echo-params.sh 脚本，在不使用 shift 命令的情况下，
#+ 步进访问所有位置参数。
