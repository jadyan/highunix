#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-23 08:49:43
 # @LastEditTime: 2021-03-23 09:02:09
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/10_04_awk.sh
### 
# substring-extraction.sh
String=23skidoo1
#      012345678    Bash
#      123456789    awk
# 注意不同字符串索引系统：
# Bash 中第一个字符的位置为0。
# Awk 中第一个字符的位置为1。


echo ${String:2:4} # 从第3位开始（0-1-2），4个字符的长度
                                         # skid

#  将空的 "echo" 通过管道传递给 awk 作为一个模拟输入，
#+ 这样就不需要提供一个文件名来操作 awk 了。
# Awk 中与 ${string:pos:length} 等价的是 substr(string,pos,length)。
 echo |awk '{print substr("${String}",3,4)}'
 echo |awk '{print substr("'"${String}"'",3,4)}'


 # 同样的：
echo | awk '
{ print index("'"${String}"'", "skid")      # 3
}                                           # （skid 从第3位开始）
'   # 这里使用 awk 等价于 "expr index"。

echo "测试"
echo ${String}|awk '{print substr($1, 3, 4)}'

echo ${String}|awk '{print index($1, "skid")}'