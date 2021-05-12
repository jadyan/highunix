#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-19 16:45:36
 # @LastEditTime: 2021-03-19 17:03:04
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/08_02_operator.sh
### 
# 使变量自增1，10种不同的方法实现
n=1; echo -n "$n "
let "n = $n + 1"   # 可以使用 let "n = n + 1"
echo -n "$n "


#  ":" 是必要的，不加的话，bash会将
#+ "$((n = $n + 1))"看做一条命令。
: $((n = $n + 1)) # ：这里可理解成是空命令
echo -n "$n "

(( n = n + 1 ))
#  更简洁的写法。
#  感谢 David Lombard指出。
echo -n "$n "


n=$(($n + 1))
echo -n "$n "


: $[ n = $n + 1 ]
#  ":" 是必要的，不加的话，bash会将
#+ "$[ n = $n + 1 ]"看做一条命令。
#  即使"n"是字符串，也是可行的。
echo -n "$n "

n=$[ $n + 1 ]
#  即使"n"是字符串，也是可行的。
#* 不要用这种写法，它已被废弃且不具有兼容性。
#  感谢 Stephane Chazelas.
echo -n "$n "


# 使用C风格的自增运算符也是可以的
let "n++"          # let "++n" 可行
echo -n "$n "


(( n++ ))          # (( ++n ))  可行
echo -n "$n "


: $(( n++ ))       # : $(( ++n )) 可行
echo -n "$n "

: $[ n++ ]         # : $[ ++n ] 可行
echo -n "$n "
echo


a=2147483646
echo "a = $a"        # a = 2147483646

let "a+=1"           # 自增 "a"
echo "a = $a"        # a = 2147483647
let "a+=1"           # 再次自增"a"，超出取值范围。
#我的电脑上没有出错
echo "a = $a"        # a = -2147483648
                     #      错误：超出范围，
                     #+     最左边的符号位被重置，
                     #+     结果变负


#注意，Bash并不支持浮点运算，Bash会将带小数点的数看做字符串。
a=1.5
#let "b = $a + 1.3"  # 报错