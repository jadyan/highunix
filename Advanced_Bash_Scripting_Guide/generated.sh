	#!/bin/bash
	echo "This is a generated shell script."
	#  注意我们现在在一个子 shell 内,
	#+ 我们不能访问 "外部" 脚本变量.
	echo "Generated file will be named: $OUTFILE"
	#  上面这行并不能按照预期的正常工作
	#+ 因为参数扩展已被禁用.
	#  相反的, 结果是文字输出.
	a=7
	b=3
	let "c = $a * $b"
	echo "c = $c"
	exit 0
