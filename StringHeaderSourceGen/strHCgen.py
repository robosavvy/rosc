# -*- coding: utf-8 -*-

'''
   Copyright by Synapticon GmbH (www.synapticon.com)  ©2012
   
   This file is part of ROScNode Library
   
   ROScNode Library is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   ROScNode Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with ROScNode Library. If not, see <http://www.gnu.org/licenses/>.

   File created by Christian Holl on 11.09.2012
'''

import yaml

if __name__ == '__main__':

    
    print("Generating String header and source file")
    # with open("string_def", mode='br') as file:
        #paimport yaml
        
    #Load and parse string file
    with open("string_def.yaml", mode='r') as stringDefFile:
        dataMap = yaml.load(stringDefFile)

    #Open header and source for generating them
    with open("msg_strings.h", mode='w') as headerfile:
        with open("msg_strings.c", mode='w') as sourcefile:
            #Write file headers
            headerfile.write('/*\n')
            headerfile.write(' *  WARNING WARNING WARNING WARNING\n')
            headerfile.write(' *    FILE GENERATED AUTOMATICALLY\n')
            headerfile.write(' *  WARNING WARNING WARNING WARNING\n')
            headerfile.write(' *\n')
            headerfile.write(' *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2012\n')
            headerfile.write(' *\n')
            headerfile.write(' *  msg_strings.h\n')
            headerfile.write(' *\n')
            headerfile.write(' *  This file is part of ROScNode Library.\n')
            headerfile.write(' *\n')
            headerfile.write(' *  ROScNode Library is free software: you can redistribute it and/or modify\n')
            headerfile.write(' *  it under the terms of the GNU General Public License as published by\n')
            headerfile.write(' *  the Free Software Foundation, either version 3 of the License, or\n')
            headerfile.write(' *  (at your option) any later version.\n')
            headerfile.write(' *\n')
            headerfile.write(' *  ROScNode Library is distributed in the hope that it will be useful,\n')
            headerfile.write(' *  but WITHOUT ANY WARRANTY; without even the implied warranty of\n')
            headerfile.write(' *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n')
            headerfile.write(' *  GNU General Public License for more details.\n')
            headerfile.write(' *\n')
            headerfile.write(' *  You should have received a copy of the GNU General Public License\n')
            headerfile.write(' *  along with ROScNode Library. If not, see <http://www.gnu.org/licenses/>.\n')
            headerfile.write(' *\n')
            headerfile.write(' *    File Creator by Christian Holl\n')
            headerfile.write(' *\n')
            headerfile.write(' */\n')
            headerfile.write('#ifndef MSG_STRINGS_H_\n#define MSG_STRINGS_H_')
            headerfile.write(' \n')
            headerfile.write(' \n')
            headerfile.write(' \n')
            
            
            sourcefile.write('/*\n')
            sourcefile.write(' *  WARNING WARNING WARNING WARNING\n')
            sourcefile.write(' *    FILE GENERATED AUTOMATICALLY\n')
            sourcefile.write(' *  WARNING WARNING WARNING WARNING\n')
            sourcefile.write(' */\n')
            sourcefile.write('#include "msg_strings.h"')
            sourcefile.write(' \n')
            sourcefile.write(' \n')

            

            for item in dataMap:
                currentVariable=dataMap.get(item,0)
                currentComment=currentVariable.get("comment",0)
                currentStringDict=currentVariable.get("stringlist",0)
                currentDefineShort=currentVariable.get("defineShort",0)
                if(currentVariable==0 or 
                   currentComment==0 or 
                   currentStringDict==0 or
                   currentDefineShort==0):
                    print "Error missing specification for variable: ", item
                    exit(1)
                    
                    
                    
                #String list extraction and sorting
                currentStringList = currentStringDict.keys()
                currentStringList.sort()
                
                
                #Inserting comment for define
                headerfile.write('/**\n')   
                headerfile.write(' * Macro for inserting ')
                headerfile.write(currentComment);
                headerfile.write(' string numbers into any enum');
                headerfile.write('\n */\n')                     
                    
                
                #Creating the definition for being able to insert it into any enum    
                headerfile.write('#define __')   
                headerfile.write(item.upper())
                headerfile.write('(ENUM) \\\n')
                
                #Writing the start of the array to the sourcefile
                sourcefile.write('const char *')           
                sourcefile.write(item)
                sourcefile.write('[]\n')
                sourcefile.write('{\n\t')

                for str in currentStringList:
                    headerfile.write("\t ENUM ## _")
                    headerfile.write(currentDefineShort.upper())
                    headerfile.write('_')
                    
                    sourcefile.write('"')
                    sourcefile.write(str)
                    sourcefile.write('"')
                    
                    
                    if(currentStringDict.get(str,0)):
                        headerfile.write(currentStringDict.get(str,0))
                    else:
                        headerfile.write(str.upper())
                    
                    if(currentStringList[-1] != str):
                        headerfile.write(', \\\n')
                        sourcefile.write(',\n\t');
                        
                headerfile.write('\n\n')
                sourcefile.write('\n}\n\n')

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
    

        