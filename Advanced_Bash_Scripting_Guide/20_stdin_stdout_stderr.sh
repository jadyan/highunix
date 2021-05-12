#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-24 15:12:19
 # @LastEditTime: 2021-03-24 15:22:12
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/20_stdin_stdout_stderr.sh
### 

cat <<'EOF'
   COMMAND_OUTPUT >
      # 重定向标准输出到一个文件.
      # 如果文件不存在则创建，否则覆盖.
      ls -lR > dir-tree.list
      # 创建了一个包含目录树列表的文件.
   : > filename
      # ">" 清空了文件.
      # 如果文件不存在，则创建了个空文件 (效果类似 'touch').
      # ":" 是个虚拟占位符, 不会有输出.
   > filename    
      # ">" 清空了文件.
      # 如果文件不存在，则创建了个空文件 (效果类似 'touch').
      # (结果和上述的 ": >" 一样， 但在某些 shell 环境中不能正常运行.)
   COMMAND_OUTPUT >>
      # 重定向标准输出到一个文件.
      # 如果文件不存在则创建，否则新内容在文件末尾追加.
      # 单行重定向命令 (只作用于本身所在的那行):
      # --------------------------------------------------------------------
   1>filename
      # 以覆盖的方式将 标准错误 重定向到文件 "filename."
   1>>filename
      # 以追加的方式将 标准输出 重定向到文件 "filename."
   2>filename
      # 以覆盖的方式将 标准错误 重定向到文件 "filename."
   2>>filename
      # 以追加的方式将 标准错误 重定向到文件 "filename."
   &>filename
      # 以覆盖的方式将 标准错误 和 标准输出 同时重定向到文件 "filename."
      # 在 bash 4 中才有这个新功能.
   M>N
     # "M" 是个文件描述符, 如果不明确指定，默认为 1.
     # "N" 是个文件名.
     # 文件描述符 "M" 重定向到文件 "N."
   M>&N
     # "M" 是个文件描述符, 如果不设置默认为 1.
     # "N" 是另一个文件描述符.
      #==============================================================================
EOF

echo "----------------------------"

# 重定向 标准输出，一次一行.
LOGFILE=script.log
echo "This statement is sent to the log file, \"$LOGFILE\"." 1>$LOGFILE
echo "This statement is appended to \"$LOGFILE\"." 1>>$LOGFILE
echo "This statement is also appended to \"$LOGFILE\"." 1>>$LOGFILE
echo "This statement is echoed to stdout, and will not appear in \"$LOGFILE\"."
# 这些重定向命令在每行结束后自动"重置".




cat <<'EOF'
 2>&1
      # 重定向 标准错误 到 标准输出.
      # 错误信息发送到标准输出相同的位置.
        >>filename 2>&1
            bad_command >>filename 2>&1
            # 同时将 标准输出 和 标准错误 追加到文件 "filename" 中 ...
        2>&1 | [command(s)]
            bad_command 2>&1 | awk '{print $5}'   # found
            # 通过管道传递 标准错误.
            # bash 4 中可以将 "2>&1 |" 缩写为 "|&".
   i>&j
      # 重定向文件描述符 i 到 j.
      # 文件描述符 i 指向的文件输出将会重定向到文件描述符 j 指向的文件
   >&j
      # 默认的标准输出 (stdout) 重定向到 j.
      # 所有的标准输出将会重定向到 j 指向的文件.
   0< FILENAME
    < FILENAME
      # 从文件接收输入.
      # 类似功能命令是 ">", 经常会组合使用.
      #
      # grep search-word <filename
   [j]<>filename
      #  打开并读写文件 "filename" ,
      #+ 并且分配文件描述符 "j".
      #  如果 "filename" 不存在则创建.
      #  如果文件描述符 "j" 未指定, 默认分配文件描述符 0, 标准输入.
      #
      #  这是一个写指定文件位置的应用程序. 
EOF

echo 1234567890 > outfile    # 写字符串到 "File".
exec 3<> outfile             # 打开并分配文件描述符 3 给 "File" .
read -n 4 <&3             # 读取 4 字符.
echo -n . >&3             # 写一个小数点.
exec 3>&-                 # 关闭文件描述符 3.
cat outfile                  # ==> 1234.67890



cat <<"EOF"
n<&-
    关闭输入文件描述符 n.
0<&-, <&-
    关闭标准输入.
n>&-
    关闭输出文件描述符 n.
1>&-, >&-
    关闭标准输出.
EOF