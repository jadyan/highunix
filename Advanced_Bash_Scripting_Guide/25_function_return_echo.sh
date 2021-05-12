#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 13:54:50
 # @LastEditTime: 2021-03-26 14:08:30
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/25_function_return_echo.sh
### 
# max2.sh: 取两个大整数中的最大值。
#  这是前一个例子 "max.sh" 的修改版,
#+ 这个版本可以比较两个大整数。
EQUAL=0             # 如果两个值相等，那就返回这个值。
E_PARAM_ERR=-99999  # 没有足够多的参数，那就返回这个值。
#           ^^^^^^    任意超出范围的参数都可以传递进来。
max2 ()             # "返回" 两个整数中最大的那个。
{
    if [ -z "$2" ]
    then
        echo $E_PARAM_ERR
        return
    fi
    if [ "$1" -eq "$2" ]
    then
        echo $EQUAL
        return
    else
        if [ "$1" -gt "$2" ]
        then
            retval=$1
        else
            retval=$2
        fi
    fi
    echo $retval            # 输出 (到 stdout), 而没有用返回值。
                            # 为什么?
}

#return_val=$(max2 33001 33997)

return_val=`max2 33001 33997`
#            ^^^^             函数名
#                 ^^^^^ ^^^^^ 传递进来的参数
#  这其实是命令替换的一种形式：
#+ 可以把函数看作一个命令，
#+ 然后把函数的stdout赋值给变量“return_val".
# ========================= OUTPUT ========================
if [ "$return_val" -eq "$E_PARAM_ERR" ]
then
    echo "Error in parameters passed to comparison function!"
elif [ "$return_val" -eq "$EQUAL" ]
then
    echo "The two numbers are equal."
else
    echo "The larger of the two numbers is $return_val."
fi
# =========================================================
#  练习:
#  ---------
#  1) 找到一种更优雅的方法，
#+    去测试传递给函数的参数。
#  2) 简化”输出“段的if/then结构。
#  3) 重写这个脚本，使其能够从命令行参数中获得输入。




month_length ()  # 把月份作为参数。
{                # 返回该月包含的天数。
    monthD="31 28 31 30 31 30 31 31 30 31 30 31"  # 作为局部变量声明？
    echo "$monthD" | awk '{ print $'"${1}"' }'    # 小技巧.
#                             ^^^^^^^^^
# 传递给函数的参数  ($1 -- 月份), 然后传给 awk.
# Awk 把参数解释为"print $1 . . . print $12" (这依赖于月份号)
# 这是一个模板，用于将参数传递给内嵌awk的脚本：
#                                 $'"${script_parameter}"'
#    这里是一个简单的awk结构：
#    echo $monthD | awk -v month=$1 '{print $(month)}'
#    使用awk的-v选项，可以把一个变量值赋给
#+   awk程序块的执行体。
#    感谢 Rich.
#  需要做一些错误检查，来保证月份好正确，在范围（1-12）之间， 
#+ 别忘了检查闰年的二月。
}
# ----------------------------------------------
# 用例：
month=4        # 以四月为例。
days_in=$(month_length $month)
echo $days_in  # 30