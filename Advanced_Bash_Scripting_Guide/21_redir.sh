#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 09:40:30
 # @LastEditTime: 2021-03-26 09:57:49
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/21_redir.sh
### 
# redir2.sh


echo "anzhenrui">>myfile.txt
echo "anchenrui">>myfile.txt
echo "anlinlin">>myfile.txt
echo "jadyan">>myfile.txt
echo "Smith">>myfile.txt


if [ -z "$1" ]
then
  Filename=myfile.txt       # 如果不指定文件名的默认值.
else
  Filename=$1
fi  
#+ Filename=${1:-names.data}
#  can replace the above test (parameter substitution).
count=0


while [ "$name" != Smith ]  # 为什么变量 "$name" 加引号?
do
  read name                 # 从 $Filename 读取值, 而不是 标准输入.
  echo $name
  let "count += 1"
done <"$Filename"           # 重定向标准输入到文件 $Filename. 
#    ^^^^^^^^^^^^


echo; echo "$count names read"; echo



echo "----------------------------------------"
while read line; 
do
    echo $line
done <"$Filename" 
echo  "----------------------------------------"



#  注意在一些老的脚本语言中,
#+ 循环的重定向会跑在子 shell 的环境中.
#  因此, $count 返回 0, 在循环外已经初始化过值.
#  Bash 和 ksh *只要可能* 会避免启动子 shell ,
#+ 所以这个脚本作为样例运行成功.
#  (感谢 Heiner Steven 指出这点.)
#  然而 . . .
#  Bash 有时候 *能* 在 "只读的 while" 循环启动子进程 ,
#+ 不同于 "while" 循环的重定向.
abc=hi
echo -e "1\n2\n3" | while read line
     do 
        abc="$line"
        echo $abc
     done
echo $abc


rm -rf  myfile.txt
