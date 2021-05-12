#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 10:12:13
 # @LastEditTime: 2021-03-26 10:17:29
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/21_redir_if_else.sh
### 

rm -rf myfile.txt
echo "anzhenrui">>myfile.txt
echo "anchenrui">>myfile.txt
echo "anlinlin">>myfile.txt
echo "jadyan">>myfile.txt
echo "Smith">>myfile.txt


if [ -z "$1" ]
then
  Filename=myfile.txt   # 如果不指定文件的默认值.
else
  Filename=$1
fi  
TRUE=1
if [ "$TRUE" ]          # if true    和   if :   都可以工作.
then
 read name
 echo $name
fi <"$Filename"
#  ^^^^^^^^^^^^
# 只读取文件的首行.
# "if/then" test 除非嵌入在循环内部否则没办法迭代.



function doesOutput()
 # 当然这也是个外部命令.
 # 这里用函数进行演示会更好一点.
{
  ls -al *.sh | awk '{print $5,$9}'
}


nr=0          #  我们希望在 'while' 循环里可以操作这些
totalSize=0   #+ 并且在 'while' 循环结束时看到改变.
while read fileSize fileName ; do
  echo "$fileName is $fileSize bytes"
  let nr++
  totalSize=$((totalSize+fileSize))   # Or: "let totalSize+=fileSize"
done<<EOF
$(doesOutput)
EOF


echo "------------------------------------------------------"

nr=0          #  我们希望在 'while' 循环里可以操作这些
totalSize=0   #+ 并且在 'while' 循环结束时看到改变.
while read line ; do
  echo "**********************$line*************************"
done<<EOF
$(doesOutput)
EOF


exit 0