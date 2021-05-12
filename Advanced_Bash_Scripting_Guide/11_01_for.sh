#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-23 15:43:12
 # @LastEditTime: 2021-03-23 17:19:47
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/11_01_for.sh
### 
# 列出太阳系的所有行星。
for planet in MercuryVenusEarthMarsJupiterSaturnUranusNeptunePluto
do
  echo $planet  # 每一行输出一个行星。
done
echo; echo


for planet in "Mercury Venus Earth Mars Jupiter Saturn Uranus Neptune Pluto"
# 所有的行星都输出在一行上。
# 整个 'list' 被包裹在引号中时是作为一个单一的变量。
# 为什么？因为空格也是变量的一部分。
do
  echo $planet
done



for planet in "Mercury 36" "Venus 67" "Earth 93" "Mars 142" "Jupiter 483"
do
set -- $planet  #  解析变量 "planet"
#+ 并将其每个部分赋值给位置参数。
# "--"  比如 $planet 为空或者以破折号开头。
# 因为位置参数会被覆盖掉，因此需要先保存原先的位置参数。
# 你可以使用数组来保存
#         original_params=("$@")
    echo "$1        $2,000,000 miles from the sum"
#-------两个制表符---将后面的一系列 0 连到参数 $2 上。
done



FILES="/usr/sbin/accept
/usr/sbin/pwck
/usr/sbin/chroot
/usr/bin/fakefile
/sbin/badblocks
/sbin/ypbind"
# 你可能会感兴趣的一系列文件。
# 包含一个不存在的文件，/usr/bin/fakefile。
echo
for file in $FILES
do
    echo "--------------------------"
    echo $file
    echo "--------------------------"
done


filename="*sh"
for file in $filename
do
 echo "Contents of $file"
 echo "---"
 echo  "$file"
 echo
done



for file in *
#           ^  Bash 在检测到通配表达式时，
#+             会进行文件名扩展。
do
    ls -l  "$file" 
    # 列出 $PWD（当前工作目录）下的所有文件。
#  回忆一下，通配符 "*" 会匹配所有的文件名，
#+ 但是，在文件名扩展中，他将不会匹配以点开头的文件。
#  如果没有匹配到文件，那么它将会扩展为它自身。
#  为了防止出现这种情况，需要设置 nullglob 选项。
#+    (shopt -s nullglob)。
#  感谢 S.C.
done



NUMBERS="9 7 3 8 37.53"
for number in  `echo $NUMBERS`   # for number in 9 7 3 8 37.53
do
  echo  "$number "
done


LIMIT=10
for((a=1; a <= LIMIT ; a++))# 双圆括号语法，不带 $ 的 LIMIT
do
    echo -n "$a "
done
echo; echo


# 基础版
for a in 1 2 3 4 5 6 7 8 9 10
do
    echo -n "$a "
done

echo; echo
# +==========================================+
# 使用 "seq"
for a in `seq 10`
do
  echo -n "$a "
done
echo; echo
# +==========================================+


# +==========================================+
# 使用大括号扩展语法
# Bash 3+ 版本有效。
for a in {1..10}
do
  echo -n "$a "
done
echo; echo
# +==========================================+


# +==========================================+
# 我们现在使用C语言中的逗号运算符来使得两个变量同时增加

for((a=1, b=1; a <= LIMIT ; a++, b++))
do # 逗号连接操作。
  echo -n "$a-$b "
done
echo; echo

# +==========================================+
for((n=1; n<=10; n++))
# 没有 do！
{
  echo -n "* $n *"
}
# 没有 done！
# +==========================================+

echo; echo

#  但是注意在典型的 for 循环 for n in [list] ... 中，
#+ 需要在结尾加一个分号。
#大括号在同一行才需要加分号
echo "循环大括号"
for n in 1 2 3
{ 
     echo -n "$n ";
}
#               ^
echo; echo


for a in `seq 10`
{
    echo -n "$a "
}
echo; echo

for a in 1 2 3 4 5 6 7 8 9 10
{
    echo -n "$a "
}
