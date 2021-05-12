#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-24 10:56:56
 # @LastEditTime: 2021-03-24 11:17:40
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/19_insert_file.sh
### 
# 非交互的使用 `vi` 编辑文件.
# 仿照 'sed'.




#wall <<zzz23EndOfMessagezzz23
#E-mail your noontime orders for pizza to the system administrator.
#    (Add an extra dollar for anchovy or mushroom topping.)
## 额外的信息文本.
## 注意: 'wall' 会打印注释行.
#zzz23EndOfMessagezzz23

# 更有效的做法是通过
#         wall < 信息文本
#  然而, 在脚本里嵌入信息模板不乏是一种迅速而又随性的解决方式.
#exit



#ex 说明
#https://blog.csdn.net/yexiangCSDN/article/details/80981393

E_BADARGS=85
if [ -z "$1" ]
then
  echo "Usage: `basename $0` filename"
  exit $E_BADARGS
fi
TARGETFILE=$1
# 插入两行到文件中保存
#--------Begin here document-----------#
vi $TARGETFILE <<x23LimitStringx23
i
This is line 1 of the example file.
This is line 2 of the example file.

ZZ
x23LimitStringx23
#----------End here document-----------#
#  注意 "^" 对 "[" 进行了转义 
#+ 这段起到了和键盘上按下 Control-V <Esc> 相同的效果.
#  Bram Moolenaar 指出这种情况下 'vim' 可能无法正常工作
#+ 因为在与终端交互的过程中可能会出现问题.
exit
