#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 10:06:36
 # @LastEditTime: 2021-03-26 10:06:49
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/21_redir_for.sh
### 


echo "anzhenrui">>myfile.txt
echo "anchenrui">>myfile.txt
echo "anlinlin">>myfile.txt
echo "jadyan">>myfile.txt
echo "Smith">>myfile.txt


if [ -z "$1" ]
then
  Filename=myfile.txt          # 如果不指定文件的默认值.
else
  Filename=$1
fi  
line_count=`wc $Filename | awk '{ print $1 }'`
#           目标文件的行数.
#
#  非常作和不完善, 然而这只是证明 "for" 循环中的重定向标准输入是可行的
#+ 如果你足够聪明的话.
#
# 简介的做法是     line_count=$(wc -l < "$Filename")
for name in `seq $line_count`  # 回忆下 "seq" 可以输入数组序列.
# while [ "$name" != Smith ]   --   比 "while" 循环更复杂的循环   --
do
  read name                    # 从 $Filename 读取值, 而不是标准输入.
  echo $name
  if [ "$name" = Smith ]       # 这需要所有这些额外的设置.
  then
    break
  fi  
done <"$Filename"              # 重定向标准输入到文件 "$Filename". 
#    ^^^^^^^^^^^^
exit 0