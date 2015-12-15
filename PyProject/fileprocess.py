# -*- coding: utf-8 -*-
"""
Created on Fri Dec  4 15:14:32 2015

@author: liucong
"""

#写文件
with open('test.txt','wt') as out_file:
    out_file.write('该文件被写入文件中\n看到我了吧！')
    
#读文件
with open('test.txt','rt') as in_file:
    text=in_file.read()
    
print(text)