#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 15:50:32
 # @LastEditTime: 2021-03-26 15:50:33
 # @LastEditors: your name
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/29_04_03_array_stack.sh
### 
# stack.sh: 模拟压入栈
# 类似于CPU 栈, 压入栈依次保存数据项, 
#+ 但是取数据时, 却反序进行, 后进先出.
BP=100        #  栈数组的基址指针.
            #  从元素100 开始.
SP=$BP        #  栈指针.
            #  将其初始化为栈"基址"(栈底)
Data=        #  当前栈的数据内容.
            #  必须定义为全局变量,
            #+ 因为函数所能够返回的整数存在范围限制.
            # 100    基址                <-- Base Pointer
            #  99    第一个数据元素
            #  98    第二个数据元素
            # ...    更多数据
            #        最后一个数据元素    <-- Stack pointer
declare -a stack
push()        # 压栈
{
    if [ -z "$1" ]        # 没有可压入的数据项?
    then
        return 
    fi
    let "SP -= 1"        # 更新栈指针.
    stack[$SP]=$1
    return 
}
pop()     #从栈中弹出数据项. 
{ 
    Data=                        # 清空保存数据项的中间变量
    if [ "$SP" -eq "$BP" ]        # 栈空?
    then
        return 
    fi                        # 这使得SP不会超过100,
                            #+ 例如, 这可以防止堆栈失控.
    Data=${stack[$SP]}
    let "SP += 1"            # 更新栈指针
    return
}
status_report()            # 打印当前状态
{
    echo "-------------------------------------"
    echo "REPORT"
    echo "Stack Pointer = $SP"
    echo "Just popped \""$Data"\" off the stack."
    echo "-------------------------------------"
    echo
}
# =======================================================
# 现在, 来点乐子.
echo
# 看你是否能从空栈里弹出数据项来.
pop
status_report
echo
push garbage
pop
status_report            # 压入Garbage, 弹出garbage.
value1=23;            push $value1
value2=skidoo;        push $value2
value3=LAST;        push $value3
pop                    # LAST
status_report
pop                    # skidoo
status_report
pop                    # 23
status_report        # 后进，先出!
# 注意: 栈指针在压栈时减,
#+ 在弹出时加.
echo
exit 0
# =======================================================
#
# 练习：
#
# 1) 修改"push()"函数，
#     + 使其调用一次就能够压入多个数据项。
# 2) 修改"pop()"函数,
#    + 使其调用一次就能弹出多个数据项.
# 3) 给那些有临界操作的函数添加出错检查.
#     说明白一些, 就是让这些函数返回错误码, 
#    + 返回的错误码依赖于操作是否成功完成, 
#    + 如果没有成功完成, 那么就需要启动合适的处理动作.
# 4) 以这个脚本为基础,
#    + 编写一个用栈实现的四则运算计算器.