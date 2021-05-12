#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-23 09:06:33
 # @LastEditTime: 2021-03-23 10:11:06
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/10_05_string_change.sh
### 

var1=1
var2=2
unset var3
#var3="" unset var3是不一样的，从这里就能看出
# 没有设置 var3。
echo ${var1-$var2}   # 1
echo ${var3-$var2}   # 2
#           ^          注意前面的 $ 前缀。
echo ${var3} # 从这里可以看出不是真正的替换掉了var3
echo ${username-`whoami`}
# 如果变量 $username 没有被设置，输出 `whoami` 的结果。

echo "------------------------------------------------"
# 无论变量的值是否为空，其是否已被声明决定了缺省设置的触发。
username0=
echo "username0 has been declared, but is set to null."
echo "username0 = ${username0-`whoami`}"
# 将不会输出 `whoami` 的结果。

echo "------------------------------------------------"
echo "未定义username1"
echo username1 has not been declared.
echo "username1 = ${username1-`whoami`}"
# 将会输出 `whoami` 的结果。

echo "-----------------------------------------------"
#username2=""
username2=
echo "username2 has been declared, but is set to null."
echo "username2 = ${username2:-`whoami`}"
#                            ^
# 因为这里是 :- 而不是 -，所以将会输出 `whoami` 的结果。
# 与上面的 username0 比较。



echo "---------------------------------------------------------------"
echo ":-  - 区别"
# 
# 再来一次：
variable=
# 变量已被声明，但其值为空。
echo "${varibale-0}"    # 没有输出。
echo "${variable:-1}"   # 1
#        



unset variable
echo "${variable-2}"    # 2  #没有定义可以替换
echo "${variable:-3}"   # 3



#DEFAULT_FILENAME=generic.data
#filename=${1:-$DEFAULT_FILENAME}
# 如果没有其他特殊情况，下面的代码块将会操作文件 "generic.data"。
# 代码块开始
# ...
# ...
# ...
# 代码块结束
# 摘自样例 "hanoi2.bash"：
#DISKS=${1:-E_NOPARAM}   # 必须指定碟子的个数。
#  将 $DISKS 设置为传入的第一个命令行参数，
#+ 如果没有传入第一个参数，则设置为 $E_NOPARAM。



echo "\${parameter+alt_value}, \${parameter:+alt_value} 测试"

echo "###### \${parameter+alt_value} ########
        当变量parameter没有定义或者被unset时，+ 不能替换字符"
echo

a=${param1+xyz}
echo "a = $a"      #a=

param2=
a=${param2+xyz}
echo "a = $a"      # a = xyz

param3=123
a=${param3+xyz}
echo "a = $a"      # a = xyz


echo
echo "###### \${parameter:+alt_value} ########"
echo "只要为空 或者被 没有定义 都不能替换"
echo

a=${param4:+xyz}   # a=
echo "a = $a"


param5=
a=${param5:+xyz}
echo "a = $a"      # a =
# 不同于 a=${param5+xyz}

param6=123
a=${param6:+xyz}
echo "a = $a"      # a = xyz


echo "\${parameter?err_msg}, \${parameter:?err_msg}"
# 检查系统环境变量。
# 这是一种良好的预防性维护措施。
# 如果控制台用户的名称 $USER 没有被设置，那么主机将不能够识别用户。

: ${HOSTNAME?} ${USER?} ${HOME?} ${MAIL?}

  echo
  echo "Name of the machine is $HOSTNAME."
  echo "You are $USER."
  echo "Your home directory is $HOME."
  echo "Your mail INBOX is located in $MAIL."
  echo
  echo "If you are reading this message,"
  echo "critcial environmental variables have been set."
  echo
  echo

  # ------------------------------------------------------
# ${variablename?} 结构统一可以检查脚本中的变量是否被设置。

ThisVariable=Value-of-ThisVariable
# 顺带一提，这个字符串的值可以被设置成名称中不可以使用的禁用字符。
: ${ThisVariable?}                               #如果没有设置 会报错
echo "Value of ThisVariable is $ThisVariable." 
echo; echo


#相当于异常信息
#//: ${ZZXy23AB?"ZZXy23AB has not been set."}
# 因为 ZZXy23AB 没有被设置，所以脚本会终止同时显示错误消息。

#你可以指定错误消息。
# : ${variablename?"ERROR MESSAGE"}
# 与这些结果相同:  dummy_variable=${ZZXy23AB?}
#                 dummy_variable=${ZZXy23AB?"ZZXy23AB has not been set."}
#
#                 echo ${ZZXy23AB?} >/dev/null
# 将上面这些检查变量是否被设置的方法同 "set -u" 作比较。

echo "You will not see this message, because script already terminated."
#HERE=0
#exit $HERE   # 将不会从这里退出。



echo "------------------------------------------------------------------"

#!/bin/bash
# usage-message.sh
: ${1?"Usage: $0 ARGUMENT"}
# 如果命令行参数缺失，脚本将会在这里结束，并且返回下面的错误信息。
#    usage-message.sh: 1: Usage: usage-message.sh ARGUMENT
echo "These two lines echo only if command-line parameter given."
echo "command-line parameter = \"$1\""
exit 0  # 仅当命令行参数存在是才会从这里退出。
# 在传入和未传入命令行参数的情况下查看退出状态。
# 如果传入了命令行参数，那么 "$?" 的结果是0。
# 如果没有，那么 "$?" 的结果是1。