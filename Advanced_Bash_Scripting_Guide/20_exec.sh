#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 08:59:46
 # @LastEditTime: 2021-03-26 09:21:14
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/20_exec.sh
### 
# 使用 'exec' 重定向 标准输入 .



echo "jadyan" >>data-file
echo "anchengrui" >>data-file
echo "annuonuo" >>data-file

exec 6<&0          # 链接文件描述符 #6 到标准输入.
                   # .
exec < data-file   # 标准输入被文件 "data-file" 替换
read a1            # 读取文件 "data-file" 首行.
read a2            # 读取文件 "data-file" 第二行
echo
echo "Following lines read from file."
echo "-------------------------------"
echo $a1
echo $a2
echo; echo; echo

#  现在在之前保存的位置将从文件描述符 #6 将 标准输出 恢复.
#+ 且关闭文件描述符 #6 ( 6<&- ) 让其他程序正常使用.
#
# <&6 6<&-    also works.
exec 0<&6 6<&-  
#<&- 关闭输入


echo -n "Enter data  "
read b1  # 现在按预期的，从正常的标准输入 "read".
echo "Input read from stdin."
echo "----------------------"
echo "b1 = $b1"
echo

rm -rf data-file


#重定向输出
echo"---------------------------------------------------------------------"


LOGFILE=logfile.txt
exec 6>&1           # 链接文件描述符 #6 到标准输出.
                    # 保存标准输出.

exec > $LOGFILE     # 标准输出被文件 "logfile.txt" 替换.


# ----------------------------------------------------------- #
# 所有在这个块里的命令的输出都会发送到文件 $LOGFILE.
echo -n "Logfile: "
date
echo "-------------------------------------"
echo
echo "Output of \"ls -al\" command"
echo
ls -al
echo; echo
echo "Output of \"df\" command"
echo
df
# ----------------------------------------------------------- #

exec 1>&6 6>&-      # 关闭文件描述符 #6 恢复 标准输出.
echo
echo "== stdout now restored to default == "
echo
ls -al
echo
exit 0