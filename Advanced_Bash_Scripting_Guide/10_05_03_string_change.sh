#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-23 10:33:35
 # @LastEditTime: 2021-03-23 10:52:32
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/10_05_03_string_change.sh
### 
var1=abcd-1234-defg
echo "var1 = $var1"


t=${var1#*-*}
echo ${var1#*-*}
echo "var1 (with everything, up to and including first - stripped out) = $t"
#  t=${var1#*-} 效果相同，
#+ 因为 # 只匹配最短的字符串，
#+ 并且 * 可以任意匹配，其中也包括空字符串。
# （感谢 Stephane Chazelas 指出这一点。）



t=${var##*-*}
echo "If var1 contains a \"-\", returns empty string...   var1 = $t"

t=${var1%*-*}
echo "var1 (with everything from the last - on stripped out) = $t"
echo


# -------------------------------------------
path_name=/home/bozo/ideas/thoughts/for.today
# -------------------------------------------

echo "path_name = $path_name"
t=${path_name##/*/}
echo "path_name, stripped of prefixes = $t"

 #t=${path_name%/}
 t=${path_name##*/} 
echo "path_name, stripped of prefixes = $t"

t=`basename $path_name`
echo "path_name, stripped of prefixes = $t"



t=${path_name%/*.*}
# 同 t=`dirname $path_name` 效果相同。
echo "path_name, stripped of suffixes = $t"
# 在一些情况下会失效，比如 "../", "/foo////", # "foo/", "/"。
#  在删除后缀时，尤其是当文件名没有后缀，目录名却有后缀时，
#+ 事情会变的非常复杂。

# 在这里与 t=`basename $path_name` 效果相同。
#  t=${path_name%/}; t=${t##*/}  是更加通用的方法，
#+ 但有时仍旧也会出现问题。
#  如果 $path_name 以换行结束，那么 `basename $path_name` 将会失效，
#+ 但是上面这种表达式却可以。




echo
t=${path_name:11}
echo "$path_name, with first 11 chars stripped off = $t"
t=${path_name:11:5}
echo "$path_name, with first 11 chars stripped off, length 5 = $t"
echo


t=${path_name/bozo/clown}
echo "$path_name with \"bozo\" replaced by \"clown\" = $t"
t=${path_name/today/}
echo "$path_name with \"today\" deleted = $t"
#全局替换
t=${path_name//o/O}
echo "$path_name with all o's capitalized = $t"
t=${path_name//o/}
echo "$path_name with all o's deleted = $t"




echo "----------------------------------------------------------
\${var/#Pattern/Replacement}
替换 var 前缀部分匹配到的 Pattern 为 Replacement。

\${var/%Pattern/Replacement}
替换 var 后缀部分匹配到的 Pattern 为 Replacement。
----------------------------------------------------------
"

v0=abc1234zip1234abc    # 初始值。
echo "v0 = $v0"         # abc1234zip1234abc
echo

# 在字符串首部进行匹配
v1=${v0/#abc/ABCDEF}    # abc1234zip123abc
                        # |-|
echo "v1 = $v1"         # ABCDEF1234zip1234abc
                        # |----|

# 在字符串尾部进行匹配
v2=${v0/%abc/ABCDEF}    # abc1234zip123abc
                        #              |-|
echo "v2 = $v2"         # abc1234zip1234ABCDEF
                        #               |----|
echo


#  --------------------------------------------
#  必须在字符串的最开始或者最末尾的地方进行匹配，
#+ 否则将不会发生替换。
#  --------------------------------------------
v3=${v0/#123/000}       # 虽然匹配到了，但是不在最开始的地方。
echo "v3 = $v3"         # abc1234zip1234abc
                        # 没有替换。
v4=${v0/%123/000}       # 虽然匹配到了，但是不在最末尾的地方。
echo "v4 = $v4"         # abc1234zip1234abc
                        # 没有替换。





echo "---------------------------------------------
\${!varprefix*}, \${!varprefix@}
匹配先前声明过所有以 varprefix 作为变量名前缀的变量。"

# 这是带 * 或 @ 的间接引用的一种变换形式。
# 在 Bash 2.04 版本中加入了这个特性。
xyz23=whatever
xyz24=

a=${!xyz*}         #  扩展为声明变量中以 "xyz"
# ^ ^   ^           + 开头变量名。
echo "a = $a"      #  a = xyz23 xyz24

a=${!xyz@}         #  同上。
echo "a = $a"      #  a = xyz23 xyz24

echo "------------------------------------------------"
abc23=something_else
b=${!abc*}
echo "b = $b"      #  b = abc23
c=${!b}            #  这是我们熟悉的间接引用的形式。
echo $c            #  something_else