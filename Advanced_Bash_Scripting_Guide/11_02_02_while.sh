#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-23 17:31:46
 # @LastEditTime: 2021-03-23 17:35:08
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/11_02_02_while.sh
### 

LIMIT=10    # 循环10次。
a=1
while [ "$a" -le  $LIMIT ]
do
  echo -n "$a "
  let "a+=1"
done    # 没什么好奇怪的吧。
echo; echo


# +==============================================+
# 现在我们用C语言风格再写一次。
((a =1))# a=1
# 双圆括号结构允许像C语言一样在赋值语句中使用空格。


while (( a <= LIMIT ))   #  双圆括号结构，
do  #+ 并且没有使用 "$"。
    echo -n "$a "
    ((a +=1))# let "a+=1"
    # 是的，就是这样。
    # 双圆括号结构允许像C语言一样自增一个变量。
done

echo "----------------------------------------------------------"

condition ()
{
    ((t++))
    if [ $t -lt 5 ];then
        return 0 # true 真
    else
        return 1   # false 假
    fi
}
while condition
do
  echo "Still going: t = $t"
done