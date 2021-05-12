#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 13:52:47
 # @LastEditTime: 2021-03-26 13:52:48
 # @LastEditors: your name
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/25_function_return_2.sh
### 
#Return_Val=   # 用于保存函数特大返回值的全局变量。
alt_return_test ()
{
    fvar=$1
    Return_Val=$fvar
    return   # 返回 0 (成功).
}
alt_return_test 1
echo $?             #0
echo "return value = $Return_Val"            #1
alt_return_test 256
echo "return value = $Return_Val"           # 256
alt_return_test 257
echo "return value = $Return_Val"           # 257
alt_return_test 25701
echo "return value = $Return_Val"           #25701