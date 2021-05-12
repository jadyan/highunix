#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-18 16:34:28
 # @LastEditTime: 2021-03-18 16:36:07
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/0406_variable_whois.sh
### 
# ex18.sh
# 在下面三个可选的服务器中进行 whois 域名查询：
# ripe.net, cw.net, radb.net

# 将这个脚本重命名为 'wh' 后放在 /usr/local/bin 目录下
# 这个脚本需要进行符号链接：
# ln -s /usr/local/bin/wh /usr/local/bin/wh-ripe
# ln -s /usr/local/bin/wh /usr/local/bin/wh-apnic
# ln -s /usr/local/bin/wh /usr/local/bin/wh-tucows


E_NOARGS=75


if [ -z "$1" ]
then
  echo "Usage: `basename $0` [domain-name]"
  exit $E_NOARGS
fi

# 检查脚本名，访问对应服务器进行查询。
case `basename $0` in    # 也可以写:    case ${0##*/} in
    "wh"       ) whois $1@whois.tucows.com;;
    "wh-ripe"  ) whois $1@whois.ripe.net;;
    "wh-apnic" ) whois $1@whois.apnic.net;;
    "wh-cw"    ) whois $1@whois.cw.net;;
    *          ) echo "Usage: `basename $0` [domain-name]";;
esac
exit $?