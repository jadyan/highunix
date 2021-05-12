#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 09:58:49
 # @LastEditTime: 2021-03-26 10:04:37
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/21_redir_while.sh
### 
# 这是之前的另一种形式的脚本.
#  Heiner Steven 提议在重定向循环时候运行在子 shell 可以作为一个变通方案
#+ 因此直到循环终止时循环内部的变量不需要保证他们的值

echo "anzhenrui">>myfile.txt
echo "anchenrui">>myfile.txt
echo "anlinlin">>myfile.txt
echo "jadyan">>myfile.txt
echo "Smith">>myfile.txt



if [ -z "$1" ]
then
  Filename=myfile.txt     # 如果不指定文件名的默认值.
else
  Filename=$1
fi  

:<<'EOF'
exec 3<&0                 # 保存标准输入到文件描述符 3.
exec 0<"$Filename"        # 重定向标准输入.
EOF
exec <"$Filename"   #标准输入为文件

count=0
echo



while [ "$name" != Smith ]
do
  read name               # 从重定向的标准输入($Filename)读取值.
  echo $name
  let "count += 1"
done                      #  从 $Filename 循环读
                          #+ 因为第 20 行.

: <<-"EOF"
exec 0<&3                 # 恢复早前的标准输入.
exec 3<&-                 # 关闭临时的文件描述符 3.
EOF


echo; echo "$count names read"; echo
exit 0