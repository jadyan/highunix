#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 10:44:52
 # @LastEditTime: 2021-03-26 10:50:14
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/24_process_change.sh
### 

:<<'EOF'
用管道 将一个命令的 标准输出 输送到另一个命令的 标准输入 是个强大的技术。但是如果你需要用管道输送多个命令的 标准输出 怎么办？这时候 进程替换 就派上用场了。

进程替换 把一个（或多个）进程 的输出送到另一个进程的 标准输入。

样板
命令列表要用括号括起来

>(command_list)
<(command_list)
EOF


echo >(true)


cat <(ls -l)
# 等价于    ls -l | cat
sort -k 9 <(ls -l /bin) <(ls -l /usr/bin) <(ls -l /usr/local/bin)
#  列出 3 个主要 'bin' 目录的文件，按照文件名排序。
#  注意，有三个（数一下）单独的命令输送给了 'sort'。


cat <( ls . ) <( ls ../ )