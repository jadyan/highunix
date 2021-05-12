
###
 # @Author: your name
 # @Date: 2021-03-22 17:41:42
 # @LastEditTime: 2021-03-22 17:41:43
 # @LastEditors: your name
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/10_03_02_change_format_suff.sh
### 
#!/bin/bash
# 将图像批处理转换成不同的格式。
# 假设已经安装了 imagemagick。（在大部分 Linux 发行版中都有）
INFMT=png   # 可以是 tif, jpg, gif 等等。
OUTFMT=pdf  # 可以是 tif, jpg, gif, pdf 等等。
for pic in *"$INFMT"
do
  p2=$(ls "$pic" | sed -e s/\.$INFMT//)
  # echo $p2
  convert "$pic" $p2.$OUTFMT
done
exit $?