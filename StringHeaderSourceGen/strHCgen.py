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
    f = open('string_def.yaml')
    dataMap = yaml.load(f)

    for item in dataMap:
        currentVariable=dataMap.get(item,0)
        currentComment=currentVariable.get("comment",0)
        currentStringlist=currentVariable.get("stringlist",0)
        if(currentVariable==0 or currentComment==0 or currentStringlist ==0):
            print "Error missing specification for variable: ", item
            exit(1)
        print '#define' ' __',item

    f.close()
        