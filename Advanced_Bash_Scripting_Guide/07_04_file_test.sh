#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-19 15:09:10
 # @LastEditTime: 2021-03-19 15:14:04
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/07_04_file_test.sh
### 
#下列每一个test选项在满足条件时，返回0（真）。
#
#-e
#检测文件是否存在
#
#-a
#检测文件是否存在
#
#等价于 -e。不推荐使用，已被弃用[^1]。
#
#-f
#文件是常规文件(regular file)，而非目录或 设备文件
#
#-s
#文件大小不为0
#
#-d
#文件是一个目录
#
#-b
#文件是一个 块设备
#
#-c
#文件是一个 字符设备

device0="/dev/sda2"    # /   (根目录)

if [ -b "$device0" ]
then
  echo "$device0 is a block device."
fi

device1="/dev/ttyS1"   # PCMCIA 调制解调卡
if [ -c "$device1" ]
then
  echo "$device1 is a character device."
fi

function show_input_type()
{
   [ -p /dev/fd/0 ] && echo PIPE || echo STDIN
}

#-P文件是一个 管道设备
show_input_type "Input"                           # STDIN
echo "Input" | show_input_type                    # PIPE


#-h
#文件是一个 符号链接
#
#-L
#文件是一个符号链接
#
#-S
#文件是一个 套接字
#
#-t
#文件（文件描述符）与终端设备关联
#
#该选项通常被用于 测试 脚本中的 stdin [ -t 0 ] 或 stdout [ -t 1 ] 是否为终端设备。
#
#-r
#该文件对执行测试的用户可读
#
#-w
#该文件对执行测试的用户可写
#
#-x
#该文件可被执行测试的用户所执行
#
#-g
#文件或目录设置了 set-group-id sgid 标志
#
#如果一个目录设置了 sgid 标志，那么在该目录中所有的新建文件的权限组都归属于该目录的权限组，而非文件创建者的权限组。该标志对共享文件夹很有用。
#
#-u
#文件设置了 set-user-id suid 标志。
#
#一个属于 root 的可执行文件设置了 suid 标志后，即使是一个普通用户执行也拥有 root 权限[^2]。对需要访问硬件设备的可执行文件（例如 pppd 和 cdrecord）很有用。如果没有 suid 标志，这些可执行文件就不能被非 root 用户所调用了。
#
#-rwsr-xr-t    1 root       178236 Oct  2  2000 /usr/sbin/pppd
#设置了 suid 标志后，在权限中会显示 s。
#
#-k
#设置了粘滞位(sticky bit)。
#
#标志粘滞位是一种特殊的文件权限。如果文件设置了粘滞位，那么该文件将会被存储在高速缓存中以便快速访问[^3]。如果目录设置了该标记，那么它将会对目录的写权限进行限制，目录中只有文件的拥有者可以修改或删除文件。设置标记后你可以在权限中看到 t。
#
#drwxrwxrwt    7 root         1024 May 19 21:26 tmp/
#如果一个用户不是设置了粘滞位目录的拥有者，但对该目录有写权限，那么他仅仅可以删除目录中他所拥有的文件。这可以防止用户不经意间删除或修改其他人的文件，例如 /tmp 文件夹。（当然目录的所有者可以删除或修改该目录下的所有文件）
#
#-O
#执行用户是文件的拥有者
#
#-G
#文件的组与执行用户的组相同
#
#-N
#文件在在上次访问后被修改过了
#
#f1 -nt f2
#文件 f1 比文件 f2 新
#
#f1 -ot f2
#文件 f1 比文件 f2 旧
#
#f1 -ef f2
#文件 f1 和文件 f2 硬链接到同一个文件
#
#!
#取反——对测试结果取反(如果条件缺失则返回真)。
