#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-24 10:21:37
 # @LastEditTime: 2021-03-24 10:24:57
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/13_match.sh
### 

#使用 反引号 的算术扩展（通常与 expr 一起使用）
z=`expr $z + 3`  
echo $z


#使用 双圆括号 或 let 的算术扩展。
# $((EXPRESSION)) 是算术扩展。  # 不要与命令替换混淆。

z=$(($z+3))
echo $z
z=$((z+3))                     # 同样正确。
                               # 在双圆括号内，参数引用形式可用可不用。
echo $z

n=0
echo "n = $n"                # n = 0
(( n += 1 ))                 # 自增。
echo "n = $n" 
#(( $n += 1 )) 是错误用法！
echo "n = $n" 


z=0
let z=z+3
echo $z
let "z += 3"  # 引号允许在赋值表达式中使用空格。
              # 'let' 事实上执行的算术运算而非算术扩展
echo $z