#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-22 17:39:42
 # @LastEditTime: 2021-03-22 17:39:43
 # @LastEditors: your name
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/10_03_change_format_suff.sh
### 
#  cvt.sh:
#  将目录下所有的 MacPaint 文件转换为 "pbm" 格式。
#  使用由 Brian Henderson (bryanh@giraffe-data.com) 维护的
#+ "netpbm" 包下的 "macptobpm" 二进制工具。
#  Netpbm 是大多数 Linux 发行版的标准组成部分。
OPERATION=macptopbm
SUFFIX=pbm          # 新的文件名后缀。
if [ -n "$1" ]
then
  directory=$1      # 如果已经通过脚本参数传入了目录名的情况……
else
  directory=$PWD    # 否则就使用当前工作目录。
fi
#  假设目标目录下的所有 MacPaint 图像文件都拥有
#+ ".mac" 的文件后缀名。
for file in $directory/*    # 文件名匹配。
do
  filename=${file%.*c}      #  从文件名中删除 ".mac" 后缀
                            #+ ('.*c' 匹配 '.' 与 'c' 之间的
                            #  所有字符，包括其本身)。
  $OPERATION $file > "$filename.$SUFFIX"
                            # 将转换结果重定向到新的文件。
  rm -f $file               # 在转换后删除原文件。
  echo "$filename.$SUFFIX"  # 将记录输出到 stdout 中。
done
exit 0
# 练习：
# -----
# 这个脚本会将当前工作目录下的所有文件进行转换。
# 修改脚本，使得它仅转换 ".mac" 后缀的文件。