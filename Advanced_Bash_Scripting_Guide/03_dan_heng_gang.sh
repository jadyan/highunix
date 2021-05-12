#!/bin/bash
#重定向输入输出[短横线]。
###
 # @Author: your name
 # @Date: 2021-03-12 15:02:53
 # @LastEditTime: 2021-03-17 10:22:59
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/03_dan_heng_gang.sh
### 
#cat -
#cat - 输出由键盘读入的标准输入(stdin) 到 标准输出(stdout)。但是在真实应用的 I/O 重定向中是否有使用 ‘-‘？


# 可以理解成 标准输出
#-“ 能够通过管道将标准输出重定向到其他命令中。这就可以做到像在某个文件前添加几行这样的事情。
#
#使用 diff 比较两个文件的部分内容：
#
#rep Linux file1 | diff file2 -


#  将当前目录下24小时之内修改过的所有文件备份成一个
#  "tarball" (经 tar 打包`与 gzip 压缩) 文件
BACKUPFILE=backup-$(date +%m-%d-%Y)
#                 在备份文件中嵌入时间
#                 感谢 Joshua Tschida 提供的建议
#如果$1为空(没有传入参数 那么旧用$BACKUPFILE)
archive=${1:-$BACKUPFILE}
#  如果没有在命令行中特别制定备份格式，
#  那么将会默认设置为 "backup-MM-DD-YYYY.tar.gz"。
tar cvf - `find . -mtime -1 -type f -print` > $archive.tar
gzip $archive.tar
echo "Directory $PWD backed up in archive file \"$archive.tar.gz\"."
#  Stephane Chazeles 指出如果目录中有非常多的文件，
#  或文件名中包含空白符时，上面的代码会运行失败。
# 他建议使用以下的任意一种方法：
# -------------------------------------------------------------------
#   find . -mtime -1 -type f -print0 | xargs -0 tar rvf "$archive.tar"
#   使用了 GNU 版本的 "find" 命令。
#   find . -mtime -1 -type f -exec tar rvf "$archive.tar" '{}' \;
#   兼容其他的 UNIX 发行版，但是速度会比较慢
# -------------------------------------------------------------------
exit 0




#~+
#当前工作目录。它等同于内部变量 $PWD。
#
#~-
#先前的工作目录。它等同于内部变量 $OLDPWD。
#
#=~
#正则表达式匹配。将在 Bash version 3 章节中介绍。
#
#^
#行起始符。在正则表达式中，”^” 代表一行文本的开始。
#
#^, ^^
#参数替换中的大写转换符（在Bash第4版新增）