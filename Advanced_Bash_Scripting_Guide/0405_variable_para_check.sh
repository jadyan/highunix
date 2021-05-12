#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-18 16:29:47
 # @LastEditTime: 2021-03-18 16:33:13
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/0405_variable_para_check.sh
### 

#
# #如果一个脚本需要一个命令行参数但是在调用的时候却没有传入，那么这将会造成一个空变量赋值。
#这通常不是我们想要的。一种避免的方法是，在使用期望的位置参数时候，在赋值语句两侧添加一个额外的字符。

variable1_=$1_  # 而不是 variable1=$1
# 使用这种方法可以在没有位置参数的情况下避免产生错误。

critical_argument01=$variable1_

# 多余的字符可以被去掉，就像下面这样：
variable1=${variable1_/_//}
# 仅仅当 $variable1_ 是以下划线开头时候才会有一些副作用。
# 这里使用了我们稍后会介绍的参数替换模板中的一种。
# （将替换模式设为空等价于删除。）

# 更直接的处理方法就是先检测预期的位置参数是否被传入。
if [ -z $1 ]
then
    echo "没有参数"
    exit $E_MISSING_POS_PARAM
fi

#  但是，正如 Fabin Kreutz 指出的，
#+ 上面的方法会有一些意想不到的副作用。
#  更好的方法是使用参数替换：
#         ${1:-$DefaultVal}
#  详情查看第十章“操作变量”的第二节“变量替换”。