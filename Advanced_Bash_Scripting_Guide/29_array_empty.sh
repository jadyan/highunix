#!/bin/bash
# empty-array.sh
#  感谢Stephane Chazelas制作这个例子的原始版本。 
#+ 同时感谢Michael Zick 和 Omair Eshkenazi 对这个例子所作的扩展。
#  以及感谢Nathan Coulter 作的声明和感谢。
# 空数组与包含有空元素的数组，这两个概念不同。
array0=( first second third )
array1=( '' )        # "array1" 包含一个空元素.
array2=( )            # 没有元素. . . "array2"为空 
array3=()            # 这个数组呢?
echo
ListArray()
{
    echo
    echo "Elements in array0:  ${array0[@]}"
    echo "Elements in array1:  ${array1[@]}"
    echo "Elements in array2:  ${array2[@]}"
    echo "Elements in array3:  ${array3[@]}"
    echo
    echo "Length of first element in array0 = ${#array0}"
    echo "Length of first element in array1 = ${#array1}"
    echo "Length of first element in array2 = ${#array2}"
    echo "Length of first element in array3 = ${#array3}"
    echo
    echo "Number of elements in array0 = ${#array0[*]}"  # 3
    echo "Number of elements in array1 = ${#array1[*]}"  # 1  (Surprise!)
    echo "Number of elements in array2 = ${#array2[*]}"  # 0
    echo "Number of elements in array3 = ${#array3[*]}"  # 0
}
# ===================================================================
ListArray
# 尝试扩展这些数组。
# 添加一个元素到这个数组。
array0=( "${array0[@]}" "new1" )
array1=( "${array1[@]}" "new1" )
array2=( "${array2[@]}" "new1" )
array3=( "${array3[@]}" "new1" )
ListArray
# 或者
array0[${#array0[*]}]="new2"
array1[${#array1[*]}]="new2"
array2[${#array2[*]}]="new2"
array3[${#array3[*]}]="new2"
ListArray
# 如果你按照上边的方法对数组进行扩展的话，数组比较像‘栈’
# 上边的操作就是‘压栈’
# ‘栈’的高度为：
height=${#array2[@]}
echo
echo "Stack height for array2 = $height"
# '出栈’就是：
unset array2[${#array2[@]}-1]   # 数组从0开始索引 
height=${#array2[@]}            #+ 这就意味着数组的第一个下标是0
echo
echo "POP"
echo "New stack height for array2 = $height"
ListArray
# 只列出数组array0的第二个和第三个元素。
from=1              # 从0开始索引。
to=2
array3=( ${array0[@]:1:2} )
echo
echo "Elements in array3:  ${array3[@]}"
# 处理方式就像是字符串（字符数组）。
# 试试其他的“字符串”形式。
# 替换:
array4=( ${array0[@]/second/2nd} )
echo
echo "Elements in array4:  ${array4[@]}"
# 替换掉所有匹配通配符的字符串
array5=( ${array0[@]//new?/old} )
echo
echo "Elements in array5:  ${array5[@]}"
# 当你觉得对此有把握的时候...
array6=( ${array0[@]#*new} )
echo # This one might surprise you.
echo "Elements in array6:  ${array6[@]}"
array7=( ${array0[@]#new1} )
echo # 数组array6之后就没有惊奇了。
echo "Elements in array7:  ${array7[@]}"
# 看起来非常像...
array8=( ${array0[@]/new1/} )
echo
echo "Elements in array8:  ${array8[@]}"
# 所以，让我们怎么形容呢？
#  对数组var[@]中的每个元素The string operations are performed on
#+ 进行连续的字符串操作。each of the elements in var[@] in succession.
#  因此：Bash支持支持字符串向量操作，
#  如果结果是长度为0的字符串
#+ 元素会在结果赋值中消失不见。
#  然而，如果扩展在引用中，那个空元素会仍然存在。
#  Michael Zick:   问题--这些字符串是强引用还是弱引用？ 
#  Nathan Coulter:  没有像弱引用的东西
#!    真正发生的事情是
#!+   匹配的格式发生在
#!+   [word]的所有其它扩展之后
#!+   比如像${parameter#word}.
zap='new*'
array9=( ${array0[@]/$zap/} )
echo
echo "Number of elements in array9:  ${#array9[@]}"
array9=( "${array0[@]/$zap/}" )
echo "Elements in array9:  ${array9[@]}"
# 此时，空元素仍然存在
echo "Number of elements in array9:  ${#array9[@]}"
# 当你还在认为你身在Kansas州时...
array10=( ${array0[@]#$zap} )
echo
echo "Elements in array10:  ${array10[@]}"
# 但是，如果被引用的话，*号将不会被解释。
array10=( ${array0[@]#"$zap"} )
echo
echo "Elements in array10:  ${array10[@]}"
# 可能，我们仍然在Kansas...
# (上面的代码块Nathan Coulter所修改.)
#  比较 array7 和array10.
#  比较array8 和array9.
#  重申: 所有所谓弱引用的东西
#  Nathan Coulter 这样解释:
#  word在${parameter#word}中的匹配格式在
#+ 参数扩展之后和引用移除之前已经完成了。
#  在通常情况下，格式匹配在引用移除之后完成。
exit