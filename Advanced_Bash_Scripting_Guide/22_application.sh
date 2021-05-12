#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 10:19:55
 # @LastEditTime: 2021-03-26 10:19:55
 # @LastEditors: your name
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/22_application.sh
### 
# logevents.sh
# 作者: Stephane Chazelas.
# 用于 ABS 许可指南.
# 事件记录到文件.
# 必须 root 身份执行 (可以写入 /var/log).
ROOT_UID=0     # 只有 $UID 为 0 的用户具有 root 权限.
E_NOTROOT=67   # 非 root 会报错.
if [ "$UID" -ne "$ROOT_UID" ]
then
  echo "Must be root to run this script."
  exit $E_NOTROOT
fi  
FD_DEBUG1=3
FD_DEBUG2=4
FD_DEBUG3=5
# === 取消下面两行注释来激活脚本. ===
# LOG_EVENTS=1
# LOG_VARS=1
log()  # 时间和日期写入日志文件.
{
echo "$(date)  $*" >&7     # *追加* 日期到文件.
#     ^^^^^^^  命令替换
                           # 见下文.
}
case $LOG_LEVEL in
 1) exec 3>&2         4> /dev/null 5> /dev/null;;
 2) exec 3>&2         4>&2         5> /dev/null;;
 3) exec 3>&2         4>&2         5>&2;;
 *) exec 3> /dev/null 4> /dev/null 5> /dev/null;;
esac
FD_LOGVARS=6
if [[ $LOG_VARS ]]
then exec 6>> /var/log/vars.log
else exec 6> /dev/null                     # 清空输出.
fi
FD_LOGEVENTS=7
if [[ $LOG_EVENTS ]]
then
  # exec 7 >(exec gawk '{print strftime(), $0}' >> /var/log/event.log)
  # 上述行在最近高于 bash 2.04 版本会失败，为什么?
  exec 7>> /var/log/event.log              # 追加到 "event.log".
  log                                      # 写入时间和日期.
else exec 7> /dev/null                     # 清空输出.
fi
echo "DEBUG3: beginning" >&${FD_DEBUG3}
ls -l >&5 2>&4                             # 命令1 >&5 2>&4
echo "Done"                                # 命令2 
echo "sending mail" >&${FD_LOGEVENTS}
# 输出信息 "sending mail" 到文件描述符 #7.
exit 0