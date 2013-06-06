# -*- coding: utf-8 -*-

'''
/*
 *    Copyright (c) 2013, Synapticon GmbH
 *    All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are met: 
 *
 *    1. Redistributions of source code must retain the above copyright notice, this
 *       list of conditions and the following disclaimer. 
 *    2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution. 
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *    The views and conclusions contained in the software and documentation are those
 *    of the authors and should not be interpreted as representing official policies, 
 *    either expressed or implied, of the FreeBSD Project.
 *
 *  strHCgen.py created by Christian Holl
 */
'''
import re
import yaml
import os
import ntpath

if __name__ == '__main__':
    currentFilePath = os.path.realpath(__file__)
    currentPath, filename = ntpath.split(currentFilePath)

    
    
    print("Generating String header and source file")
    # with open("string_def", mode='br') as file:
        #paimport yaml
        
    #Load and parse string file
    with open(currentPath + "/string_def.yaml", mode='r') as stringDefFile:
        dataMap = yaml.load(stringDefFile)

    #Open header and source for generating them
    with open(currentPath + "/../include/rosc/string_res/msg_strings.h", mode='w') as headerfile:
        with open(currentPath + "/../src/rosc/string_res/msg_strings.c", mode='w') as sourcefile:
           
            #licence and generation warning:
            genwarn_licence="""
/*
 *    WARNING WARNING WARNING WARNING
 *     FILE GENERATED AUTOMATICALLY!
 *    WARNING WARNING WARNING WARNING
 *     TO CHANGE ANYTHING GO INTO
 *             strHCgen.py
 *
 *    Copyright (c) 2013, Synapticon GmbH
 *    All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright notice, this
 *       list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *    The views and conclusions contained in the software and documentation are those
 *    of the authors and should not be interpreted as representing official policies,
 *    either expressed or implied, of the FreeBSD Project.
 */           
"""
            
            
            #Write file headers
            headerfile.write(genwarn_licence)
            sourcefile.write(genwarn_licence)
            
            headerfile.write('#ifndef MSG_STRINGS_H_\n#define MSG_STRINGS_H_')
            headerfile.write(' \n\n\n')
            
            sourcefile.write('#include <rosc/string_res/msg_strings.h>')
            sourcefile.write(' \n\n')


        


            for item in dataMap:
                currentVariable=dataMap.get(item,0)
                currentComment=currentVariable.get("comment",0)
                currentStringDict=currentVariable.get("stringlist",0)
                currentDefineShort=currentVariable.get("defineShort",0)
                if(currentVariable==0 or 
                   currentComment==0 or 
                   currentStringDict==0 or
                   currentDefineShort==0):
                    print("Error missing specification for variable: ", item)
                    exit(1)
                    
                #String list extraction and sorting
                currentStringList = currentStringDict.keys()
                currentStringList.sort()
                    
                #Inserting comment for length definition
                headerfile.write('/**\n')   
                headerfile.write(' * Length of the string array ')
                headerfile.write(item);
                headerfile.write('\n */\n')     
                
                #Inserting length definition
                headerfile.write('#define ')   
                headerfile.write(item.upper())
                headerfile.write('_LEN \t')
                headerfile.write(str(currentStringList.__len__()))
                headerfile.write('\n\n');
                    
              
                
                
                #Inserting comment for define
                headerfile.write('/**\n')   
                headerfile.write(' * Macro for inserting ')
                headerfile.write(currentComment);
                headerfile.write(' string numbers into any enum');
                headerfile.write('\n */\n')                     
                    
                
                #Creating the definition for being able to insert it into any enum    
                headerfile.write('#define ')   
                headerfile.write(item.upper())
                headerfile.write('(ENUM) \\\n')
                
                #Writing the start of the array to the sourcefile
                sourcefile.write('const char *')           
                sourcefile.write(item)
                sourcefile.write('[] =\n')
                sourcefile.write('{\n\t')

                for strng in currentStringList:
                    headerfile.write("\t ENUM ## _")
                    headerfile.write(currentDefineShort.upper())
                    headerfile.write('_')
                    
                    sourcefile.write('"')
                    sourcefile.write(strng)
                    sourcefile.write('"')
                    
                    
                    if(currentStringDict.get(strng,0)):
                        headerfile.write(currentStringDict.get(strng,0))
                    else:
                        headerfile.write(re.sub('[.\-/#\'*+ ~`´\\?=}{&%$§"!^°|:;,]','_',strng.upper()))
                    
                    if(currentStringList[-1] != strng):
                        headerfile.write(', \\\n')
                        sourcefile.write(',\n\t');
                        
                headerfile.write('\n\n')
                sourcefile.write('\n};\n\n')

                #Inserting Comment for variable declaration
                headerfile.write('/**\n')   
                headerfile.write(' * This array contains the strings for ')
                headerfile.write(currentComment);
                headerfile.write('\n */\n')  
                
                #Inserting the type declaration
                headerfile.write('extern const char *')
                headerfile.write(item)
                headerfile.write('[];\n\n\n')
                
            headerfile.write('#endif /* MSG_STRINGS_H_ */')
    

        