#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 13:46:37
 # @LastEditTime: 2021-03-26 13:46:38
 # @LastEditors: your name
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/25_function_indirect.sh
### 
# ind-func.sh: 将一个间接引用传递给函数。
echo_var ()
{
    echo "$1"
}
message=Hello
Hello=Goodbye
echo_var "$message"         # Hello
# 现在，让我们传递一个间接引用给函数。
echo_var "${!message}"     # Goodbye
echo "-------------"
# 如果我们改变“hello”的值会发生什么？
Hello="Hello, again!"
echo_var "$message"        # Hello
echo_var "${!message}"     # Hello, again!
exit 0