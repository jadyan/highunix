#!/bin/bash
###
 # @Author: your name
 # @Date: 2021-03-26 11:16:33
 # @LastEditTime: 2021-03-26 11:18:35
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /jadyan/code/book/Advanced_Bash_Scripting_Guide/myprocesstest.sh
### 


cat myfile.txt|while read line
do
	name="xxxxxxx"
	echo $line
done

echo "$name"


while read line
do
	name2="xxxxxxx"
	echo $line
done < myfile.txt

echo "$name2"




para=`while read line 
do 
	name2="xxxxxxx" 
	echo $line      
done < myfile.txt`

echo "---------------"
echo "$para"

