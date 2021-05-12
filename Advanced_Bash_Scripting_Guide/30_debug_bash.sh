#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 16:19:43
 # @LastEditTime: 2021-03-26 16:24:04
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/30_debug_bash.sh
### 
 # Setting a variable to null will not trigger the error/abort.
 # unset_var=

:<<'EOF'
sh -n scriptname不会运行脚本, 只会检查脚本的语法错误. 这等价于把set -n或set -o noexec插入脚本中. 注意, 某些类型的语法错误不会被这种方式检查出来.

sh -v scriptname将会在运行脚本之前, 打印出每一个命令. 这等价于把set -v或set -o verbose插入到脚本中.

选项-n和-v可以同时使用. sh -nv scriptname将会给出详细的语法检查.

sh -x scriptname会打印出每个命令执行的结果, 但只使用缩写形式. 这等价于在脚本中插入set
-x或set -o xtrace.

把set -u或set -o nounset插入到脚本中, 并运行它, 就会在每个试图使用未声明变量的地方给出一个unbound variable错误信息.



set命令用来打开脚本中的选项. 你可以在脚本中任何你想让选项生效的地方插入set -o option-name, 或者使用更简单的形式,
set -option-abbrev. 这两种形式是等价的.

set -o verbose
# # 打印出所有执行前的命令.

set -v
# 与上边的例子具有相同的效果.


还有另一种可以在脚本中启用选项的方法, 那就是在脚本头部, #!的后边直接指定选项.

#!/bin/bash -x
#
# 下边是脚本的主要内容.



EOF



set -u   # Or   set -o nounset
 echo $unset_var   # Unset (and undeclared) variable.
 echo "Should not echo!"
 #sh t2.sh
 #t2.sh: line 6: unset_var: unbound variable