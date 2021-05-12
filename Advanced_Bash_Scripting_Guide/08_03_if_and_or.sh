#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-19 17:04:12
 # @LastEditTime: 2021-03-19 17:08:17
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/08_03_if_and_or.sh
### 
a=24
b=47


if [ "$a" -eq 24 ] && [ "$b" -eq 47 ]
then
  echo "Test #1 succeeds."
else
  echo "Test #1 fails."
fi

#  错误:   if [ "$a" -eq 24 && "$b" -eq 47 ]
#          这样写的话，bash会先执行'[ "$a" -eq 24'
#          然后就找不到右括号']'了...
#
#  注意:  if [[ $a -eq 24 && $b -eq 24 ]]  这样写是可以的
#  双方括号测试结构比单方括号更加灵活。
#  (双方括号中的"&&"与单方括号中的"&&"意义不同)

if [ "$a" -eq 98 ] || [ "$b" -eq 47 ]
then
  echo "Test #2 succeeds."
else
  echo "Test #2 fails."
fi


#  使用 -a 和 -o 选项也具有同样的效果。
#  感谢 Patrick Callahan 指出。


if [ "$a" -eq 24 -a "$b" -eq 47 ]
then
  echo "Test #3 succeeds."
else
  echo "Test #3 fails."
fi


if [ "$a" -eq 98 -o "$b" -eq 47 ]
then
  echo "Test #4 succeeds."
else
  echo "Test #4 fails."
fi

a=rhino
b=crocodile
if [ "$a" = rhino ] && [ "$b" = crocodile ]
then
  echo "Test #5 succeeds."
else
  echo "Test #5 fails."
fi

let "t1 = ((5 + 3, 7 - 1, 15 - 4))"
echo "t1 = $t1"           ^^^^^^  # t1 = 11
# 这里的t1 被赋值了11，为什么？


let "t2 = ((a = 9, 15 / 3))"      # 对"a"赋值并对"t2"求值。
echo "t2 = $t2    a = $a"         # t2 = 5    a = 9

