#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 10:05:11
 # @LastEditTime: 2021-03-26 10:05:24
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/21_redir_until.sh
### 
# 同先前的脚本一样, 不过用的是 "until" 循环.

echo "anzhenrui">>myfile.txt
echo "anchenrui">>myfile.txt
echo "anlinlin">>myfile.txt
echo "jadyan">>myfile.txt
echo "Smith">>myfile.txt


if [ -z "$1" ]
then
  Filename=myfile.txt         # 如果不指定文件的默认值.
else
  Filename=$1
fi  
# while [ "$name" != Smith ]
until [ "$name" = Smith ]     # 变  !=  为 =.
do
  read name                   # 从 $Filename 读取值, 而不是标准输入.
  echo $name
done <"$Filename"             # 重定向标准输入到文件 "$Filename". 
#    ^^^^^^^^^^^^
# 和之前的 "while" 循环样例相同的结果.
exit 0