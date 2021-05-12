#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-24 09:45:19
 # @LastEditTime: 2021-03-24 09:46:20
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/11_07_circle_continue.sh
### 
# "continue N" 命令可以影响其上 N 层循环。
for outer in I II III IV V           # 外层循环
do
  echo; echo -n "Group $outer: "
  # --------------------------------------------------------------------
  for inner in 1 2 3 4 5 6 7 8 9 10  # 内层循环
  do
    if [[ "$inner" -eq 7 && "$outer" = "III" ]]
    then
      continue  2 # 影响两层循环，包括“外层循环”。
                  # 将其替换为普通的 "continue"，那么只会影响内层循环。
    fi
    echo -n "$inner "  # 7 8 9 10 将不会出现在 "Group III."中。
  done
  # --------------------------------------------------------------------
done
echo; echo
# 思考：
# 想一个 "continue N" 在脚本中的实际应用情况。
exit 0