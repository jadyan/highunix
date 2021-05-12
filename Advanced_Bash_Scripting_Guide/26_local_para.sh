#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 14:16:19
 # @LastEditTime: 2021-03-26 14:19:36
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/26_local_para.sh
### 
# ex62.sh: 函数内部的局部变量与全局变量。



:<<"EOF"
局部变量
如果变量用local来声明，那么它就只能够在该变量被声明的代码块中可见。 
这个代码块就是局部范围。 在一个函数中，一个局部变量只有在函数代码中才有意义.[1]

在函数被调用之前，所有在函数中声明的变量，在函数外部都是不可见的，当然也包括那些被明确声明为local的变量。


EOF

func () {
    local loc_var=23       # 声明为局部变量。
    echo                   # 使用'local'内建命令
    echo "\"loc_var\" in function = $loc_var"
    global_var=999         # 没有声明为局部变量。 # 默认为全局变量。
    echo "\"global_var\" in function = $global_var"
}
func
# 现在，来看看局部变量“loc_var”在函数外部是否可见。
echo
echo "\"loc_var\" outside function = $loc_var"
                                    # $loc_var outside function =
                                    # 不行, $loc_var 不是全局可见的.
echo "\"global_var\" outside function = $global_var"
                                    # $在函数外部global_var = 999
                                    # $global_var 是全局可见的.
echo
#  与C语言相比，在函数内声明的Bash变量
#+ 除非它被明确声明为local时，它才是局部的。



echo "-------------------------------------------------"
func ()
{
    global_var2=37    #  变量只在函数体内可见
                     #+ 在函数被调用之前。
}                    #  函数结束
echo "global_var2 = $global_var2"  # global_var =
                                 #  函数 "func" 还没被调用，
                                 #+ 所以$global_var 在这里还不是可见的.
func
echo "global_var2 = $global_var2"  # global_var = 37
                                 # 已经在函数调用的时候设置。"