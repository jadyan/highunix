#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-19 10:40:52
 # @LastEditTime: 2021-03-19 10:42:48
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/05_05_variable_Transferred_meaning2.sh
### 

#  简单转义与引用
echo \z               #  z
echo \\z              # \z
echo '\z'             # \z
echo '\\z'            # \\z
echo "\z"             # \z
echo "\\z"            # \z


                      #  命令替换
echo `echo \z`        #  z
echo `echo \\z`       #  z
echo `echo \\\z`      # \z
echo `echo \\\\z`     # \z
echo `echo \\\\\\z`   # \z
echo `echo \\\\\\\z`  # \\z
echo `echo "\z"`      # \z
echo `echo "\\z"`     # \z


                      # Here Document
cat <<EOF
\z
EOF                   # \z
cat <<EOF
\\z
EOF                   # \z
