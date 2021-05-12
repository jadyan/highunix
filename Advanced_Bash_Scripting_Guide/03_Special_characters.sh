#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-02-23 17:04:57
 # @LastEditTime: 2021-03-02 17:27:46
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/03_Special_characters.sh
### 

#逗号使用函数
commafun()
{
	#a被赋值为9，t2被赋值为15 / 3
	let "t2 = ((a = 9, 15 / 3))"
	echo "$t2"			

	for file in /{.,usr/}bin/*python*
	#             ^    在 /bin 与 /usr/bin 目录中
	#+                 找到所有的以"calc"结尾的可执行文件
	do
        if [ -x "$file" ]
        then
          echo "____$file"
        fi
	done
}


# : 符号
NOP_fun()
{
#空命令[冒号]。它在shell中等价于”NOP”（即no op，空操作）与shell内建命令true有同样的效果。
#它本身也是Bash的内建命令之一，返回值是true（0）。
	while :
	do
		if  [ ${a} == "aa" ]
			then :   # 什么都不做，跳出判断执行下一条语句
		else
			echo "NOP : 使用"
			username="1111"
			#如果username没有设置或者为空，替换为whoami；否则替换为username的值。
			: ${username=`whoami`}
			echo ${username}
			break
		fi
	done
# 等价于
#    while true
#    do
#      ...
#    done

#
	#https://blog.csdn.net/iEearth/article/details/52589020
	echo ": 替换测试说明"
	foo=hellobash
	echo " \$\{foo:-newword\}如果foo为空， 替换为foo"
	echo ${foo:-newword}

	echo " \$\{foo:-newword\} 如果foo不为空， 替换为foo"
	echo ${foo:+newword}
	echo ${foo}

	echo " \$\{foo:=newword\} 如果foo为空， 替换为foo 并赋值给foo"
	#foo2="aaa"
	echo ${foo2:=newword}
	echo ${foo2}

	#info="foo3 empty"
	#echo "${foo3:?info}"
	echo "....$foo3"


	info="jadyan"
	echo "${info:2}"
	echo "${info:2:3}"


	var=helloBASH123
	echo ${var//[[:digit:]]/0}
	echo ${var//[[:lower:]]/x}
	echo ${var//[[:upper:]]/X}

	var=100
	(($var>100?var++:var--))
	echo "$var"

	#${1?"Usage: "aaa ARGUMENT"}

	echo "The name of this function is "$FUNCNAME" "
}


#将冒号作为函数名也是可以的
:()
{
  echo "The name of this function is "$FUNCNAME" "
  # 为什么要使用冒号作函数名？
  # 这是一种混淆代码的方法......

  #这种写法并不具有可移植性，也不推荐使用。事实上，在Bash的最近的版本更新中已经禁用了这种用法。但我们还可以使用下划线 _来替代。
}

#冒号也可以作为非空函数的占位符。
not_empty_fun()
{
	:
}




#感叹号 ！
exclamatory_mark_fun()
{
	echo "$FUNCNAME start"
	#! 当后面跟随的字母不是“空格、换行、回车、=和(”时，做命令替换
	#!n 会引用history中的第n个命令，比如输入！100，就是执行history列表中的第100条命令
	#例如history   !num

	#!-n 获取history中倒数第N个命令并执行，比如输入!-1,就会执行上一条命令
	#!!是!-1的一个alias ，因为执行上一条命令的情况比较多一些，
	#	一般如果只是执行上一条命令只按下键盘中的↑即可，有时候如果命令前加点东西执行起来会快一点（当然↑然后ctrl+a也可以跳到命令最面前插入内容）
	
	#!string引用最近的以 string 开始的命令。这条命令在你运行一个命令之后忘记了这个命令的参数是什么，直接!命令既可
		#例如 #!ps

	#!?string[?] 指向包含这个字符串的命令 (从history中找出命令)
	echo "$FUNCNAME end"
}


#问号？
question_mark_fun()
{
	echo "$FUNCNAME start"

	#测试操作符[问号]。在一些特定的语句中，? 表示一个条件测试。
	#在一个双圆括号结构中，? 可以表示一个类似C语言风格的三元（trinary）运算符的一个组成部分
	var=100
	(( $var>100? var++:var-- ))
	echo "$var"
	
	#通配符。它在进行文件匹配（globbing）时以单字符通配符扩展文件名。
	#在扩展正则表达式中匹配一个单字符。
	echo "$FUNCNAME end"
}



Dollar_mark_fun()
{
	echo "$FUNCNAME start"
	#使用${#}获取变量字符串长度。
	s=hello
	echo "s.length = ${#s}"
	echo "empty= ${#}"


	#使用$#获取脚本或函数参数个数。
	echo "参数个数 $#"

	#"$@"的值为”1”、”2”、”3”
	echo "\$@参数 $@"

	#"$*"的值为”1 2 3”
	echo "\$*参数 $*"

	echo using '$@'
	for x in "$@"
	do
		echo + $x
	done

	echo using '$*'
	for x in "$*"
	do
		echo - $x
	done

	echo "$FUNCNAME end"
}

#()括号函数
function brackets_mark_fun()
{
	echo "$FUNCNAME start"
	#1. 命令组。括号中的命令将会新开一个子shell顺序执行，所以括号中的变量不能够被脚本余下的部分使用。
	#括号中多个命令之间用分号隔开，最后一个命令可以没有分号，各命令和括号之间不必有空格。
	(a2="jadyan"; echo $a;)
	echo $a2

	#2命令替换。等同于`cmd`，shell扫描一遍命令行，发现了$(cmd)结构，便将$(cmd)中的cmd执行一次，
	#得到其标准输出，再将此输出放到原来命令。有些shell不支持，如tcsh。
	p=$(env|grep -i path)
	echo $p

	#3用于初始化数组。如：array=(a b c d)
	arr=("andandan" "annuonuo" "anzhenrui" "anchengrui")
	echo ${!arr[@]}
	for i in ${!arr[@]}
	do
		echo ${arr[i]}
	done

	#(())
	#①整数扩展。这种扩展计算是整数型的计算，不支持浮点型。((exp))结构扩展并计算一个算术表达式的值，
	#	如果表达式的结果为0，那么返回的退出状态码为1，或者 是"假"，而一个非零值的表达式所返回的退出状态码将为0，
	#或者是"true"。若是逻辑判断，表达式exp为真则为1,假则为0。
	#((1.5*4))
	if ((1*4)) 
	then
		echo "(()) ok"
	fi
	

	#只要括号中的运算符、表达式符合C语言运算规则，都可用在$((exp))中，甚至是三目运算符。
	#	作不同进位(如二进制、八进制、十六进制)运算时，输出结果全都自动转化成了十进制。
	#	如：echo $((16#5f)) 结果为95 (16进位转十进制)
	echo $((7*5))
	
	#单纯用也可重定义变量值 比如 a=5 ((a++)) 可将 $a 重定义为6
	aa=5
	((aa++))
	echo ${aa}

	#常用于算术运算比较 双括号中的变量可以不使用$符号前缀 括号内支持多个表达式用逗号分开
	#	只要括号中的表达式符合C语言运算规则,比如可以直接使用for((i=0;i<5;i++)), 
	#	如果不使用双括号, 则为for i in `seq 0 4`或者for i in {0..4}。再如可以直接使用if (($i<5)), 
	#	如果不使用双括号, 则为if [ $i -lt 5 ]。
	for((i=0;i<4;i++))
	do
		echo ${arr[i]}
	done


	
	echo "$FUNCNAME end"
	
	para="andandan is anzhenrui brother"
}

#\{\}花括号函数
Brace_mark_fun()
{
	#相当于列表
	echo "$FUNCNAME start"
	echo \"{These,words,are,quoted}\"   # " 将作为单词的前缀和后缀
	echo "$FUNCNAME end"


	#文件拼接在一起
	# 将 file1, file2 与 file3 拼接在一起后写入 combined_file 中。
	echo "123">>file1
	echo "456">>file2
	echo "789">>file3
	cat {file1,file2,file3} > combined_file
	rm -rf file1 file2 file3

	#file1 : A file1 : B file1 : C file2 : A file2 : B file2 : C
	echo {file1,file2}\ :{\ A," B",' C'}

	##  a b c d e f g h i j k l m n o p q r s t u v w x y z
	echo {a..z}
	
	# 0 1 2 3
	echo {0..3}

	#初始化数组
	base64_charset=( {A..Z} {a..z} {0..9} + / = )
	for i in ${base64_charset[@]}
	do
		echo  $i
	done

	aaa=123
	{
		aaa="321"
	}
	echo ${aaa}
	echo ${para}


	
	#代码块及I/O重定向  line1 line2 各读取一行
	{
	read line1
	read line2
	} < combined_file

	echo "First line in $File is:"
	echo "$line1"
	echo
	echo "Second line in $File is:"
	echo "$line2"

#与由圆括号包裹起来的命令组不同，由花括号包裹起来的代码块不产生子进程
# 将代码块的输出保存至文件中
	{ # 代码块起始
		echo
		echo "Archive Description:"
		((a=1,a++))
		env|grep -i path
		echo "end"              # 代码块结束。
	} > "temp.test"       # 输出重定向至文件。


	# {}
	#文本占位符。在 xargs -i 后作为输出的占位符使用

	# {} \;
	# 路径名。通常在 find 命令中使用，但这不是shell的内建命令
	#在执行find -exec时最后需要加上;，但是分号需要被转义以保证其不会被shell解释。

}

commafun
NOP_fun
:
not_empty_fun
exclamatory_mark_fun
question_mark_fun
Dollar_mark_fun 1 2 3
brackets_mark_fun
Brace_mark_fun
