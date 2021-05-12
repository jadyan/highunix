#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-24 09:53:34
 # @LastEditTime: 2021-03-24 09:53:51
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/11_09_case_use.sh
### 
# 简易的通讯录数据库
clear # 清屏。
echo "          Contact List"
echo "          ------- ----"
echo "Choose one of the following persons:" 
echo
echo "[E]vans, Roland"
echo "[J]ones, Mildred"
echo "[S]mith, Julie"
echo "[Z]ane, Morris"
echo
read person
case "$person" in
# 注意变量是被引用的。
    "E" | "e" )
    # 同时接受大小写的输入。
        echo
        echo "Roland Evans"
        echo "4321 Flash Dr."
        echo "Hardscrabble, CO 80753"
        echo "(303) 734-9874"
        echo "(303) 734-9892 fax"
        echo "revans@zzy.net"
        echo "Business partner & old friend"
    ;;
        # 注意用双分号结束这一个选项。
    "J" | "j" )
        echo
        echo "Mildred Jones"
        echo "249 E. 7th St., Apt. 19"
        echo "New York, NY 10009"
        echo "(212) 533-2814"
        echo "(212) 533-9972 fax"
        echo "milliej@loisaida.com"
        echo "Ex-girlfriend"
        echo "Birthday: Feb. 11"
    ;;
    # Smith 和 Zane 的信息稍后添加。
    *         )
    # 缺省设置。
    # 空输入（直接键入回车）也是执行这一部分。
    echo
    echo "Not yet in database."
    ;;
esac
echo
# 练习：
# -----
# 修改脚本，使得其可以循环接受多次输入而不是只显示一个地址后终止脚本。
exit 0