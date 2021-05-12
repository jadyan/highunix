#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 15:05:00
 # @LastEditTime: 2021-03-26 15:15:11
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/29_array_string.sh
### 
# array-strops.sh: 用于数组的字符串操作。
# 本脚本由Michael Zick 所编写.
# 通过了授权在本书中使用。
# 修复: 05 May 08, 04 Aug 08.
#  一般来说，任何类似于 ${name ... }(这种形式)的字符串操作
#+ 都能够应用于数组中的所有字符串元素，
#+ 比如说${name[@] ... } 或者 ${name[*] ...} 这两种形式。

clear;
arrayZ=( one two three four five five )
echo


# 提取尾部的子串。
echo ${arrayZ[@]:0}     # one two three four five five
#                ^       所有元素 
echo ${arrayZ[@]:1}     # two three four five five
#                ^        element[0]后边的所有元素.
echo ${arrayZ[@]:1:2}     # two three
#                  ^    只提取element[0]后边的两个元素.
echo "---------"


# 子串删除 
# 从字符串的开头删除最短的匹配。
echo ${arrayZ[@]#f*r}   # one two three five five
#               ^       # 匹配将应用于数组的所有元素。 
                        # 匹配到了"four",并且将它删除。


# 从字符串的开头删除最长的匹配
echo ${arrayZ[@]##t*e}  # one two four five five
#               ^^      # 匹配将应用于数组的所有元素
                        # 匹配到了 "three" ,并且将它删除。

# 从字符串的结尾删除最短的匹配
echo ${arrayZ[@]%h*e}   # one two t four five five
#               ^       # 匹配将应用于数组的所有元素
                        # 匹配到了 "hree" ,并且将它删除

# 从字符串的结尾删除最长的匹配
echo ${arrayZ[@]%%h*e}  # one two four five five
#               ^^      # 匹配将应用于数组的所有元素
                        # 匹配到了 "three" ,并且将它删除。


# 子串替换
# 第一个匹配到的子串将会被替换。
echo ${arrayZ[@]/fiv/XYZ}   # one two three four XYZe XYZe
#               ^           # 匹配将应用于数组的所有元素


# 所有匹配到的子串将会被替换。
echo ${arrayZ[@]//iv/YY}    # one two three four fYYe fYYe
                            # 匹配将应用于数组的所有元素

# 删除所有的匹配子串
# 如果没有指定替换字符串的话，那就意味着'删除'...
echo ${arrayZ[@]//fi/}      # one two three four ve ve
#               ^^          # 匹配将应用于数组的所有元素


# 替换字符串前端子串
echo ${arrayZ[@]/#fi/XY}    # one two three four XYve XYve
#                ^          # 匹配将应用于数组的所有元素

# 替换字符串后端子串
echo ${arrayZ[@]/%ve/ZZ}    # one two three four fiZZ fiZZ
#                ^            # 匹配将应用于数组的所有元素

echo ${arrayZ[@]/%o/XX}        # one twXX three four five five
#                ^            # 为什么?



echo "-----------------------------"
replacement() {
    echo -n "!!!"
}
echo ${arrayZ[@]/%e/$(replacement)}
#                ^  ^^^^^^^^^^^^^^
# on!!! two thre!!! four fiv!!! fiv!!!
# replacement()的标准输出就是那个替代字符串.
# Q.E.D: 替换动作实际上是一个‘赋值’。
echo "------------------------------------"



#  使用"for-each"之前:
echo ${arrayZ[@]//*/$(replacement optional_arguments)}
#                ^^ ^^^^^^^^^^^^^
# !!! !!! !!! !!! !!! !!!
#  现在，如果Bash只将匹配到的字符串
#+ 传递给被调用的函数...
echo
exit 0
#  在将处理后的结果发送到大工具之前，比如-- Perl, Python, 或者其它工具
#  回忆一下:
#    $( ... ) 是命令替换。
#    一个函数作为子进程运行。
#    一个函数将结果输出到stdout。
#    赋值，结合"echo"和命令替换，
#+   可以读取函数的stdout.
#    使用name[@]表示法指定了一个 "for-each"
#+   操作。
#  Bash比你想象的更加强力.