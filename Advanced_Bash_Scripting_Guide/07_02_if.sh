#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-19 14:08:43
 # @LastEditTime: 2021-03-19 14:27:00
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/07_02_if.sh
### 

#Linux cmp命令用于比较两个文件是否有差异。

#当相互比较的两个文件完全一样时，则该指令不会显示任何信息。
#若发现有所差异，预设会标示出第一个不同之处的字符和列数编号。若不指定任何文件名称或是所给予的文件名为"-"，则cmp指令会从标准输入设备读取数据。

if cmp a b &>/dev/null  # 消去输出结果
then 
    echo "Files a and b are identical."
else
    echo "Files a and b differ."
fi

#grep -q用于if逻辑判断
#突然发现grep -q 用于if 逻辑判断很好用。
#-q 参数，本意是 Quiet; do not write anything to standard output.  Exit immediately with zero status if any match is found, 
#even if an error was detected.   中文意思为，安静模式，不打印任何标准输出。 如果有匹配的内容则立即返回状态值0。
echo "if grep -q 测试"

echo "hello jadyan" >tempfile

if grep -q "hello" tempfile
then
    echo "ok"
else
    echo "no"
fi

rm -rf tempfile


word=Linux
letter_sequence=inu
if echo "$word"| grep -q "$letter_sequence"
# 使用 -q 选项消去 grep 的输出结果
then
  echo "$letter_sequence found in \"$word\""
else
  echo "$letter_sequence not found in $word"
fi



#if test condition-true 完全等价于 if [ condition-true ]。当语句开始执行时，左括号 [ 是作为调用 test 命令的标记[^1]，而右括号则不严格要求，但在新版本的 Bash 里，右括号必须补上。

# notetest 命令是 Bash 的 内建命令，可以用来检测文件类型和比较字符串。在 Bash 脚本中，test 不调用 sh-utils 包下的文件 /usr/bin/test。同样，[ 也不会调用链接到 /usr/bin/test 的 /usr/bin/[ 文件。


type test
type [
type '[['
type ']]'
type ']'