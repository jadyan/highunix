#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 13:50:06
 # @LastEditTime: 2021-03-26 13:50:06
 # @LastEditors: your name
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/25_function_return.sh
### 
# return-test.sh
# 函数所能返回的最大正整数为255.

return_test ()         # 传给函数什么值，就返回什么值。
{
    return $1
}
return_test 27          # o.k.
echo $?                 # 返回27.
return_test 255         # Still o.k.
echo $?                 # 返回 255.
return_test 257         # 错误!
echo $?                 # 返回 1 (对应各种错误的返回码).
# =========================================================
return_test -151896    # 能返回一个大负数么?
echo $?                # 能否返回 -151896?
                        # 不行! 返回的是 168.
#  Bash 2.05b 之前的版本
#+ 允许返回大负数。
#  这可能是个有用的特性。
#  Bash之后的新版本修正了这个漏洞。
#  这可能会影响以前所编写的脚本。
#  一定要小心!
# =========================================================
exit 0