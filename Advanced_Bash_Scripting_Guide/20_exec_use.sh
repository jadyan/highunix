#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 09:24:27
 # @LastEditTime: 2021-03-26 09:29:56
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/20_exec_use.sh
### 
# upperconv.sh
# 转化指定的输入文件成大写.


#./20_exec_use.sh read.txt write.txt

E_FILE_ACCESS=70
E_WRONG_ARGS=71
if [ ! -r "$1" ]     # 指定的输入文件是否可读?
then
  echo "Can't read from input file!"
  echo "Usage: $0 input-file output-file"
  exit $E_FILE_ACCESS
fi                   #  同样的错误退出
                     #+ 等同如果输入文件 ($1) 未指定 (为什么?).


if [ -z "$2" ]
then
  echo "Need to specify output file."
  echo "Usage: $0 input-file output-file"
  exit $E_WRONG_ARGS
fi

exec 4<&0
exec < $1            # 将从输入文件读取.

exec 7>&1
exec > $2            # 将写入输出文件.
                     # 假定输出文件可写 (增加检测?)


# -----------------------------------------------
    cat - | tr a-z A-Z   # 转化大写.
#   ^^^^^                # 读取标准输入.
#           ^^^^^^^^^^   # 写到标准输出.
# 然而标准输入和标准输出都会被重定向.
# 注意 'cat' 可能会被遗漏.
# -----------------------------------------------


exec 1>&7 7>&-       # 恢复标准输出.
exec 0<&4 4<&-       # 恢复标准输入.
# 恢复后, 下面这行会预期从标准输出打印.
echo "File \"$1\" written to \"$2\" as uppercase conversion."
exit 0