#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-24 13:53:19
 # @LastEditTime: 2021-03-24 14:21:40
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/19_03_insert_file_cat.sh
### 
# 和之前的样例一样, 但...
#  嵌入文档内的 '-' ，也就是 <<-
#+ 抑制了文档行首的 'tab',
#+ 但 *不是* 空格.

#行首的tab不会被输出，但是空格受影响
name="jadyan"
cat <<- ENDOFMESSAGE
	This is line 1 of the message.
	This is line 2 of the message.
	This is line 3 of the message.
	This is line 4 of the message.
    This is the last line of the message.
    ${name} 变量替换

    # 这个注释在输出时显示 (为什么?).
    
ENDOFMESSAGE
# 脚本的输出将左对齐.
# 行首的 tab 将不会输出.
# 上面 5 行的 "信息" 以 tab 开始, 不是空格.
# 空格不会受影响 <<- .
# 注意这个选项对 *内嵌的* tab 没有影响.


echo "------------------------------------------"

NAME="John Doe"
RESPONDENT="the author of this fine script"



#   当'限制符'引用或转义时不会有参数替换.
#   下面的嵌入文档也有同样的效果
#   cat <<"Endofmessage"
#   cat <<\Endofmessage

cat <<'Endofmessage'
    Hello, there, $NAME.
    Greetings to you, $NAME, from $RESPONDENT.
Endofmessage



cat <<-"Endofmessage"
	Hello, there, $NAME.
	Greetings to you, $NAME, from $RESPONDENT.
Endofmessage