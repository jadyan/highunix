#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-24 11:06:48
 # @LastEditTime: 2021-03-24 11:10:27
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/19_insert_file_ex.sh
### 
#  替换所有的以 ".txt" 后缀结尾的文件的 "Smith" 为 "Jones"

echo "Smith\nJones"> extest.txt
ORIGINAL=Smith
REPLACEMENT=Jones
for word in $(fgrep -l $ORIGINAL *.txt)
do
    # -------------------------------------
    ex $word <<EOF
        :%s/$ORIGINAL/$REPLACEMENT/g
        :wq
EOF
    # :%s is the "ex" substitution command.
    # :wq is write-and-quit.
    # -------------------------------------

#rm -rf extest.txt
done