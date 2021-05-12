#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 15:26:50
 # @LastEditTime: 2021-03-26 15:29:32
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/29_array_file_init.sh
### 


clear;
filename=sample_file

cat >sample_file<<EOF
1  a  b  c
2  d  e  fg
EOF

#         把文件内容展示到输出    #+ 加载到数组array1.
#
#  array1=( `cat "$filename" | tr '\n' ' '`)
#                           把文件中的换行替换为空格 
# 其实这样做是没必要的，因为Bash在做单词分割的时候， 
#+将会把换行转换为空格。
array1=( `cat "$filename"`)  


#                              1 a b c 2 d e fg
#
#  文件中每个被空白符分割的“单词”
#+ 都被保存到数组的一个元素中。
echo "${array1[@]}"

element_count=${#array1[*]}
echo $element_count          # 8