#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 10:08:46
 # @LastEditTime: 2021-03-26 10:11:07
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/21_redir_for_in_out.sh
### 
rm -rf myfile.txt  $Filename.new

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
Savefile=$Filename.new         # 报错的结果的文件名.
FinalName=Jonah                # 停止 "read" 的终止字符.
line_count=`wc $Filename | awk '{ print $1 }'`  # 目标文件行数.
for name in `seq $line_count`
do
  read name
  echo "$name"
  if [ "$name" = "$FinalName" ]
  then
    break
  fi  
done < "$Filename" > "$Savefile"     # 重定向标准输入到文件 $Filename,
#    ^^^^^^^^^^^^^^^^^^^^^^^^^^^       并且报错结果到备份文件.

exit 0