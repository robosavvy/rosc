'''
   Copyright by Synapticon GmbH (www.synapticon.com)  2012
   
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

def goingDeep(ddd):
    print "{"
    for item in ddd:

        print item
        if(type(ddd.get(item))==type({})):
            goingDeep(ddd.get(item))
        else:
            print(ddd.get(item))
    print "}"

    


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
            headerfile.write(' *  Copyright by Synapticon GmbH (www.synapticon.com)  \xc2 2012\n')
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

            sourcefile.write('/*\n')
            sourcefile.write(' *  WARNING WARNING WARNING WARNING\n')
            sourcefile.write(' *    FILE GENERATED AUTOMATICALLY\n')
            sourcefile.write(' *  WARNING WARNING WARNING WARNING\n')
            sourcefile.write(' */\n')

            for item in dataMap:
                currentVariable=dataMap.get(item,0)
                currentComment=currentVariable.get("comment",0)
                currentStringlist=currentVariable.get("stringlist",0)
                if(currentVariable==0 or currentComment==0 or currentStringlist ==0):
                    print "Error missing specification for variable: ", item
                    exit(1)


        