#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 09:31:43
 # @LastEditTime: 2021-03-26 09:38:06
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/20_exec_not_need_son.sh
### 
# avoid-subshell.sh
# Matthew Walker 的建议.

:<<-'EOF'
I/O 重定向是种明智的规避 inaccessible variables within a subshell 问题的方法.

样例 20-4. 规避子 shell
#没怎么看懂
EOF

Lines=0
echo

echo "anzhenrui">>myfile.txt
echo "anchenrui">>myfile.txt
echo "anlinlin">>myfile.txt
echo "jadyan">>myfile.txt


cat myfile.txt | while read line;
                 do {
                    echo $line
                    (( Lines++ ));  #  递增变量的值趋近外层循环
                                   #  使用子 shell 会有问题.
                 }
                 done

echo "Number of lines read = $Lines"     # 0
                                         # 报错!
echo "------------------------"


echo "------------------------"
exec 3<> myfile.txt
while read line <&3
do {
  echo "$line"
  (( Lines++ ));                   #  递增变量的值趋近外层循环.
                                   #  没有子 shell，就不会有问题.
}
done
exec 3>&-
echo "Number of lines read = $Lines"     # 8
echo

rm -rf  myfile.txt
exit 0

