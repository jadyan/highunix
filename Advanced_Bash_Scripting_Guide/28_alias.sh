#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 14:27:29
 # @LastEditTime: 2021-03-26 14:27:30
 # @LastEditors: your name
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/28_alias.sh
### 
# alias.sh
shopt -s expand_aliases
# 必须设置此选项, 否则脚本不能别名扩展.
# 首先来点好玩的东西.
alias Jesse_James='echo "\"Alias Jesse James\" was a 1959 comedy starring Bob Hope."'
Jesse_James
echo; echo; echo;
alias ll="ls -l"
# 可以任意使用单引号 (') 或双引号 (") 把别名括起来.
echo "Trying aliased \"ll\":"
ll /usr/X11R6/bin/mk*   #* 别名可以运行.
echo
directory=/usr/X11R6/bin/
prefix=mk*  # See if wild card causes problems.
echo "Variables \"directory\" + \"prefix\" = $directory$prefix"
echo
alias lll="ls -l $directory$prefix"
echo "Trying aliased \"lll\":"
lll         # 所有 /usr/X11R6/bin 文件清单以 mk 开始.
# 别名可以处理连续的变量 -- 包含 wild card -- o.k.
TRUE=1
echo
if [ TRUE ]
then
  alias rr="ls -l"
  echo "Trying aliased \"rr\" within if/then statement:"
  rr /usr/X11R6/bin/mk*   #* 结果报错!
  # 别名在复合的表达式中并没有生效.
  echo "However, previously expanded alias still recognized:"
  ll /usr/X11R6/bin/mk*
fi  
echo
count=0
while [ $count -lt 3 ]
do
  alias rrr="ls -l"
  echo "Trying aliased \"rrr\" within \"while\" loop:"
  rrr /usr/X11R6/bin/mk*   #* 这里的别名也没生效.
                           #  alias.sh: 行 57: rrr: 命令未找到
  let count+=1
done 
echo; echo
alias xyz='cat $0'   # 列出了自身.
                     # 注意强引.
xyz
#  这看起来能工作,
#+ 尽管 bash 文档不介意这么做.
#
#  然而, Steve Jacobson 指出,
#+ "$0" 参数的扩展在上面的别名申明后立刻生效.
exit 0