#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-19 16:41:53
 # @LastEditTime: 2021-03-19 16:42:21
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/08_01_greatest_common_divisor.sh
### 
# gcd.sh: 最大公约数
#         使用欧几里得算法
#  两个整数的最大公约数（gcd）
#  是两数能同时整除的最大数
#  欧几里得算法使用辗转相除法
#    In each pass,
#       dividend <---  divisor
#       divisor  <---  remainder
#    until remainder = 0.
#    The gcd = dividend, on the final pass.
#
#  关于欧几里得算法更详细的讨论，可以查看:
#  Jim Loy's site, http://www.jimloy.com/number/euclids.htm.
# ------------------------------------------------------
# 参数检查
ARGS=2
E_BADARGS=85

if [ $# -ne "$ARGS" ]
then
  echo "Usage: `basename $0` first-number second-number"
  exit $E_BADARGS
fi
# ------------------------------------------------------
gcd ()
{
  dividend=$1             #  随意赋值，
  divisor=$2              #  两数谁大谁小是无关紧要的，
                          #  为什么?
  remainder=1             #  如果在测试括号里使用了一个未初始化的变量，
                          #  会报错的。
  until [ "$remainder" -eq 0 ]
  do    #  ^^^^^^^^^^  该变量必须在使用前初始化！
    let "remainder = $dividend % $divisor"
    dividend=$divisor     # 对被除数，除数重新赋值
    divisor=$remainder
  done                    # 欧几里得算法
}                         # 最后的 $dividend 就是最大公约数（gcd）

gcd $1 $2

echo; echo "GCD of $1 and $2 = $dividend"; echo
# 练习 :
# ---------
# 1) 检查命令行参数，保证其为整数，
#+   如果有错误，捕捉错误并在脚本退出前打印出适当的错误信息。
# 2) 使用本地变量(local variables)重写gcd()函数。
exit 0