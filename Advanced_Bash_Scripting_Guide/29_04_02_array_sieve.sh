#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 15:48:22
 # @LastEditTime: 2021-03-26 15:48:22
 # @LastEditors: your name
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/29_04_02_array_sieve.sh
### 
# 优化过的埃拉托色尼素数筛子
# 脚本由Jared Martin编写, ABS Guide 的作者作了少许修改.
# 在ABS Guide 中经过了许可而使用(感谢!).
# 基于Advanced Bash Scripting Guide中的脚本.
# http://tldp.org/LDP/abs/html/arrays.html#PRIMES0 (ex68.sh).
# http://www.cs.hmc.edu/~oneill/papers/Sieve-JFP.pdf (引用)
# Check results against http://primes.utm.edu/lists/small/1000.txt
# Necessary but not sufficient would be, e.g.,
#     (($(sieve 7919 | wc -w) == 1000)) && echo "7919 is the 1000th prime"
UPPER_LIMIT=${1:?"Need an upper limit of primes to search."}
Primes=( '' $(seq ${UPPER_LIMIT}) )
typeset -i i t
Primes[i=1]='' # 1不是素数 
until (( ( i += 1 ) > (${UPPER_LIMIT}/i) ))  # 只需要ith-way 检查.
  do                                         # 为什么?
    if ((${Primes[t=i*(i-1), i]}))
    # 很少见， 但是很有指导意义, 在下标中使用算术扩展。
    then
      until (( ( t += i ) > ${UPPER_LIMIT} ))
        do Primes[t]=; done
    fi
  done
# echo ${Primes[*]}
echo   # 改回原来的脚本，为了漂亮的打印(80-col. 展示).
printf "%8d" ${Primes[*]}
echo; echo
exit $?