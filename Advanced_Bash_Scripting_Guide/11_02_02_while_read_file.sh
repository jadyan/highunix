#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-23 17:37:49
 # @LastEditTime: 2021-03-23 17:44:45
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/11_02_02_while_read_file.sh
### 
filename=temp.test
cat $filename |   # 从文件获得输入。
while read line    # 只要还有可以读入的行，循环就继续。
do
    echo $line
done

echo "---------------------------------------------------------------"


