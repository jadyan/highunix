#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 14:30:50
 # @LastEditTime: 2021-03-26 16:23:04
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/29_array.sh
### 

clear;

:<<EOF
新版本的Bash支持一维数组。 数组元素可以使用符号variable[xx] 来初始化。
另外，脚本可以使用declare -a variable语句来制定一个数组。 如果想引用一个数组元素（也就是取值），
可以使用大括号，访问形式为 ${element[xx]} 。
EOF

#set -o verbose

area[11]=23
area[13]=37
area[51]=UFOs
#  数组成员不一定非得是相邻或连续的。
#  数组的部分成员可以不被初始化。
#  数组中允许空缺元素。
#  实际上，保存着稀疏数据的数组（“稀疏数组”） 
#+ 在电子表格处理软件中是非常有用的。
echo -n "area[11] = "
echo ${area[11]}    #  需要{大括号}。
echo -n "area[13] = "
echo ${area[13]}
echo "Contents of area[51] are ${area[51]}."
# 没被初始化的数组成员打印为空值（null变量）。
echo -n "area[43] = "
echo ${area[43]}
echo "(area[43] unassigned)"
echo
# 两个数组元素的和被赋值给另一个数组元素。
area[5]=`expr ${area[11]} + ${area[13]}`
echo "area[5] = area[11] + area[13]"
echo -n "area[5] = "
echo ${area[5]}
area[6]=`expr ${area[11]} + ${area[51]}`
echo "area[6] = area[11] + area[51]"
echo -n "area[6] = "
echo ${area[6]}
# 这里会失败，是因为不允许整数与字符串相加。
echo; echo; echo
# -----------------------------------------------------------------
# 另一个数组, "area2".
# 另一种给数组变量赋值的方法...
# array_name=( XXX YYY ZZZ ... )
area2=( zero one two three four )
echo -n "area2[0] = "
echo ${area2[0]}
# 啊哈，从0开始计算数组下标（也就是，数组的第一个元素为[0],而不是[1]).
echo -n "area2[1] = "
echo ${area2[1]}    # [1] 是数组的第二个元素。
# -----------------------------------------------------------------
echo; echo; echo
# -----------------------------------------------
# 第三个数组， "area3".
# 另外一种给数组元素赋值的方法...
# array_name=([xx]=XXX [yy]=YYY ...)
area3=([17]=seventeen [24]=twenty-four)
echo -n "area3[17] = "
echo ${area3[17]}
echo -n "area3[24] = "
echo ${area3[24]}
# -----------------------------------------------




echo "Bash允许把变量当成数据来操作，即使这个变量没有明确地被声明为数组。"
string=abcABC123ABCabc
echo ${string[@]}   # abcABC123ABCabc
echo ${string[*]}   # abcABC123ABCabc
echo ${string[0]}   # abcABC123ABCabc
echo ${string[1]}   # 没有输出！
                    # 为什么?
echo ${#string[@]}  # 1
                    # 数组中只有一个元素。
                    # 就是这个字符串本身。



echo "-------------------------------------------------------------"
array=( zero one two three four five )
# 数组元素 0   1   2    3     4    5
echo ${array[0]}       #  0
echo ${array:0}         #  0
                        #  第一个元素的参数扩展,
                        #+ 从位置0(#0)开始（即第一个字符）

echo ${array:1}         #  ero
                        #  第一个元素的参数扩扎，
                        #+ 从位置1（#1）开始（即第二个字符）。

echo "--------------"


echo ${#array[0]}       #  4
                        # 第一个数组元素的长度。
echo ${#array}          #4
                        # 第一个数组元素的长度。
                        #  (另一种表示形式)

echo ${#array[1]}       #第二个元素的个数


#数组元素个数
echo ${#array[*]}
echo ${#array[@]}

echo "------------------"

array2=( [0]="first element" [1]="second element" [3]="fourth element" )
#            ^     ^       ^     ^      ^       ^     ^      ^       ^
# 引用允许嵌入的空格,在每个单独的数组元素中。
echo ${array2[0]}       # 第一个元素
echo ${array2[1]}       # 第二个元素
echo ${array2[2]}       #
                        # 因为并没有被初始化，所以此值为null。
echo ${array2[3]}       # 第四个元素.
echo ${#array2[0]}      # 13    (第一个元素的长度)
echo ${#array2[*]}      # 3     (数组中元素的个数)