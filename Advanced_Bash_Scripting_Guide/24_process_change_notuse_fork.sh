#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 10:52:27
 # @LastEditTime: 2021-03-26 11:08:49
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/24_process_change_notuse_fork.sh
### 
#  wr-ps.bash: 使用进程替换的 while-read 循环。
#  示例由 Tomas Pospisek 贡献。
# （ABS指南作者做了大量改动。）


#在子shell中 echo 命令用管道输送给 while-read 循环时会出现问题，下面是避免的方法：

# 管道符号“|” 会在子shell中运行 子进程里面定义的变量 父进程是看不到的
# 而 < <(command)不会
echo
echo "random input" | while read i
do
  global=3D": Not available outside the loop."
  # ... 因为在子 shell 中运行。
  echo $i
  name=i
done
echo "name=${name}"
echo "\$global (从子进程之外) = $global"
# $global (从子进程之外) 


echo; echo "--"; echo

echo "------------------------------------------------------------"

while read i
do
  echo $i
  global=3D": Available outside the loop."
  name=$i
  # ... 因为没有在子 shell 中运行。
done < <( echo "random input" )

echo "name=${name}"
echo "\$global (使用进程替换) = $global"
#  随机输入
#  $global (使用进程替换)= 3D: Available outside the loop.
echo; echo "##########"; echo



echo "------------------------------------------------------------"

# 同样道理 . . .
declare -a inloop
index=0
cat $0 | while read line
do
  inloop[$index]="$line"
  ((index++))
  # 在子 shell 中运行，所以 ...
done
echo "OUTPUT = "
echo ${inloop[*]}           # ... 什么也没有显示。



echo; echo "--"; echo

declare -a outloop
index=0
while read line
do
  outloop[$index]="$line"
  ((index++))
  # 没有在子 shell 中运行，所以 ...
done < <( cat $0 )
echo "OUTPUT = "
echo ${outloop[*]}          # ... 整个脚本的结果显示出来。




echo "---------------------------------------------------------"

declare -a array0
while read
do
    array0[${#array0[@]}]="$REPLY"
done < <( sed -e 's/bash/CRASH-BANG!/' $0 | grep bin | awk '{print $1}' )
#  由进程替换来设置'read'默认变量（$REPLY）。
#+ 然后将变量复制到一个数组。
echo '/////////////////////////////////'
echo "${array0[@]}"
exit $?
