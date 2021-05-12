#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-24 14:23:56
 # @LastEditTime: 2021-03-24 14:58:31
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/19_04_generate_script.sh
### 
# generate-script.sh
# Based on an idea by Albert Reiner.
OUTFILE=generated.sh         # Name of the file to generate.
# -----------------------------------------------------------



#生成脚本或者程序代码时可以用禁用参数的方式来输出文本.

# '嵌入文档涵盖了生成脚本的主体部分.
(
cat <<'EOF'
	#!/bin/bash
	echo "This is a generated shell script."
	#  注意我们现在在一个子 shell 内,
	#+ 我们不能访问 "外部" 脚本变量.
	echo "Generated file will be named: $OUTFILE"
	#  上面这行并不能按照预期的正常工作
	#+ 因为参数扩展已被禁用.
	#  相反的, 结果是文字输出.
	a=7
	b=3
	let "c = $a * $b"
	echo "c = $c"
	exit 0
EOF
) > $OUTFILE
# -----------------------------------------------------------
#  在上述的嵌入文档内引用'限制符'防止变量扩展

if [ -f "$OUTFILE" ]
then
   chmod 755 $OUTFILE
   # 生成可执行文件.
else
   echo "Problem in creating file: \"$OUTFILE\""
fi
#  这个方法适用于生成 C, Perl, Python, Makefiles 等等






echo "--------------------------------------------------"
echo "可以从嵌入文档的输出设置一个变量的值. 这实际上是种灵活的 命令替换."
variable=$(
cat <<SETVAR
This variable
runs over multiple lines.
SETVAR
)
echo "$variable"


echo "----------------------------------------------------"
echo "同样的脚本里嵌入文档可以作为函数的输入."

GetPersonalData ()
{
  read firstname
  read lastname
  read address
  read city 
  read state 
  read zipcode
} # 可以肯定的是这应该是个交互式的函数, 但 . . .



# 作为函数的输入.
GetPersonalData <<-RECORD001
Bozo
Bozeman
2726 Nondescript Dr.
Bozeman
MT
21226
RECORD001


echo
echo "$firstname $lastname"
echo "$address"
echo "$city, $state $zipcode"
echo

echo "---------------------------------------"
echo "匿名” 嵌入文档"

#相当于c++ #if 0 #endif
: <<COMMENTBLOCK
echo "This line will not echo."
这些注释没有 "#" 前缀.
则是另一种没有 "#" 前缀的注释方法.
&*@!!++=
上面这行不会产生报错信息,
因为 bash 解释器会忽略它.
COMMENTBLOCK


echo "Exit value of above \"COMMENTBLOCK\" is $?."   # 0

echo "Just before commented-out code block."
#  下面这个在双破折号之间的代码不会被执行.
#  ===================================================================

: <<DEBUGXXX
for file in *
do
 echo "$file"
done
DEBUGXXX

echo "Just after commented-out code block."


#######################################################################
#  注意, 然而, 如果将变量中包含一个注释的代码块将会引发问题
# 需要''
: <<'COMMENTBLOCK2'
  echo "This line will not echo."
  &*@!!++=
  ${foo_bar_bazz?}
  $(rm -rf /tmp/foobar/)
  $(touch my_build_directory/cups/Makefile)
COMMENTBLOCK2



#说明文档
if [ "$1" = "-h"  -o "$1" = "--help" ]     # Request help.
then                                       # Use a "cat script" . . .
  cat <<DOCUMENTATIONXX
List the statistics of a specified directory in tabular format.
---------------------------------------------------------------
The command-line parameter gives the directory to be listed.
If no directory specified or directory specified cannot be read,
then list the current working directory.
DOCUMENTATIONXX

fi

echo "----------------------------------------------------------------"

# 然而, 下面这样也能运行.
cat <<EOF
Hello!
Single exclamation point follows!
!
    EOF


exit $DOC_REQUEST
