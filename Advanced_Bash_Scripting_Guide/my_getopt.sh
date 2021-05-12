#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-22 17:47:36
 # @LastEditTime: 2021-03-22 17:51:37
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/my_getopt.sh
### 
# getopt-simple.sh
# 作者: Chris Morgan
# 允许在高级脚本编程指南中使用。
getopt_simple()
{
    echo "getopt_simple()"
    echo "Parameters are '$*'"
    until [ -z "$1" ]
    do
      echo "Processing parameter of: '$1'"
      if [ ${1:0:1} = '/' ]
      then
          tmp=${1:1}               # 删除开头的 '/'
          parameter=${tmp%%=*}     # 取出名称。
          value=${tmp##*=}         # 取出值。
          echo "Parameter: '$parameter', value: '$value'"
          eval $parameter=$value
      fi
      shift
    done
}
# 将所有参数传递给 getopt_simple()。
getopt_simple $*
echo "test is '$test'"
echo "test2 is '$test2'"
exit 0  # 可以查看该脚本的修改版 UseGetOpt.sh。


#---
#sh getopt_example.sh /test=value1 /test2=value2
#Parameters are '/test=value1 /test2=value2'
#Processing parameter of: '/test=value1'
#Parameter: 'test', value: 'value1'
#Processing parameter of: '/test2=value2'
#Parameter: 'test2', value: 'value2'
#test is 'value1'
#test2 is 'value2'