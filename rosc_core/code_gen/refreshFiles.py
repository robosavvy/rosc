# -*- coding: utf-8 -*-

'''
    This just updates the package source file list inside ../cmake/rosc_core_src_list.cmake
'''

import os
import ntpath
import sys
import re

if __name__ == '__main__':
    currentFilePath = os.path.realpath(__file__)
    currentPath, filename = ntpath.split(currentFilePath)

    fileList = []
    fileSize = 0
    folderCount = 0
    rootdir = currentPath+"/../src"

    for root, subFolders, files in os.walk(rootdir):
        folderCount += len(subFolders)
        for file in files:
            f = os.path.join(root,file)
            fileSize = fileSize + os.path.getsize(f)
            if re.search('.c', f):
                fileList.append(f.replace(currentPath+"/../",''))

 

with open(currentPath + "/../cmake/rosc_core_src_list.cmake", mode='w') as filelistfile:
    filelistfile.write("SET(ROSC_CORE_SOURCES\n")
    for file in fileList:
        filelistfile.write(file + "\n")
    filelistfile.write(")\n")
    
    
    
     