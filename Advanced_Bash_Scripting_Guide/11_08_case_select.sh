#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-24 09:49:12
 # @LastEditTime: 2021-03-24 09:51:42
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/11_08_case_select.sh
### 
# 测试字符的种类。


#对变量进行引用不是必须的，因为在这里不会进行字符分割。
#条件测试语句必须以右括号 ) 结束。[^1]
#每一段代码块都必须以双分号 ;; 结束。
#如果测试条件为真，其对应的代码块将被执行，而后整个 case 代码段结束执行。
#case 代码段必须以 esac 结束（倒着拼写case）。



echo; echo "Hit a key, then hit return."
read Keypress
case "$Keypress" in
    [[:lower:]]   ) echo "Lowercase letter";;
    [[:upper:]]   ) echo "Uppercase letter";;
    [0-9]         ) echo "Digit";;
  *             ) echo "Punctuation, whitespace, or other";;
esac      #  字符范围可以用[方括号]表示，也可以用 POSIX 形式的[[双方括号]]表示。
# 在这个例子的第一个版本中，用来测试是小写还是大写字符使用的是 [a-z] 和 [A-Z]。
# 这在一些特定的语言环境和 Linux 发行版中不起效。
# POSIX 形式具有更好的兼容性。
# 感谢 Frank Wang 指出这一点。
# 练习：
# -----
# 这个脚本接受一个单字符然后结束。
# 修改脚本，使得其可以循环接受输入，并且检测键入的每一个字符，直到键入 "X" 为止。
# 提示：将所有东西包在 "while" 中。
exit 0