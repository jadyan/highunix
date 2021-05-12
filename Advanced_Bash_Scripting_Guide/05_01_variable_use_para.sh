#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-18 16:50:13
 # @LastEditTime: 2021-03-18 17:03:19
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/05_01_variable_use_para.sh
### 

List="one two three"

for a in $List     # 空白符将变量分成几个部分。
do
  echo "$a"
done

echo "---"


for a in "$List"   # 在单一变量中保留所有空格。
do #     ^     ^
  echo "$a"
done



function varitest()
{
    echo "--------"
    echo  $#
    until [  -z "$1" ]; 
    do
        echo $1
        shift||break
    done
    echo "--------"
}
variable1="a variable containing five words"
varitest This is $variable1
varitest "This is $variable1"


variable2=""
varitest  $variable2 $variable2 $variable2  ## 不带参数执行COMMAND命令。

varitest "$variable2" "$variable2" "$variable2"   # 带上3个参数执行COMMAND命令。

varitest "$variable2 $variable2 $variable2"    # 带上1个参数执行COMMAND命令（2空格）

