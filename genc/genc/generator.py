# Software License Agreement (BSD License)
#
# Copyright (c) 2013, Synapticon GmbH
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following
# disclaimer in the documentation and/or other materials provided
# with the distribution.
# * Neither the name of Synapticon GmbH nor the names of its
# contributors may be used to endorse or promote products derived
# from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

import genc
import genc.generator
import genmsg.names
import genmsg.msg_loader
import genmsg.msgs
from Test import add_tabs


class msg(object):
    '''
    classdocs
    '''

    __search_path=[]
    __msg_spec=[]
    __msg_buildup=[]
    __array_depth=0
    __max_array_depth=0
    __depth=0
    

    def __init__(self, msg_spec, search_path):
        '''
        Constructor
        '''
        self.__msg_spec=msg_spec
        self.__search_path=search_path
        self.__create_information(msg_spec)
         
    def get_msgspec_form_field(self, field):
        msg_context = genmsg.msg_loader.MsgContext()
        return genmsg.msg_loader.load_msg_by_type(msg_context, field.base_type, self.__search_path)
        
        
    def getBuildupArray(self):
        return self.__msg_buildup
    
    def printBuildupArray(self):
        for (indent,text, additionalreturn) in self.__msg_buildup:
            if additionalreturn == 1:
                print add_tabs(indent) + text + ",\n"
            else:
                print add_tabs(indent) + text + ","
    
    def add_tabs(self, count):
        tabs=""
        for i in range(count):
            tabs = tabs + "\t"
        return tabs
    
    def __add_base_type_static(self, depth, base_type, name):
        if base_type in ['uint8', 'int8', 'uint16', 'int16', 'uint32', 'int32', 'uint64', 'int64', 'float32', 'float64']:
            return add_tabs(depth) + base_type + "_t"
        elif base_type == 'byte':
            return add_tabs(depth) + 'uint8_t'
        elif base_type == 'char':
            return add_tabs(depth) + 'int8_t'
        elif base_type == 'time' or  base_type == 'duration':
            output= add_tabs(depth) + 'struct\n'
            output+= add_tabs(depth) + '{' + '\n'
            output+= add_tabs(depth+1) + 'uint32_t sec;' + '\n'
            output+= add_tabs(depth+1) + 'uint32_t nsec;' + '\n'
            output+= add_tabs(depth) + '}' + name
            return output

    def __add_base_array_static(self, depth, base_type, name, arraylength):
        str=""
        for line in range(6):
            indent=""
            identation=depth
            
            if line in [2,3,4]:
                identation+=1
                
            for i in range(identation):
                indent+="\t"
                
            if line == 0:
                str+=indent+"struct"
            elif line == 1:
                str+=indent+"{"
            elif line == 2:
                str+=self.__add_base_type_static(identation, base_type, 'data') + "[" + arraylength + "]" + ";"
            elif line == 3:
                str+=indent+"uint32_t size;"
            elif line == 4:
                if not arraylength.isdigit():
                    str+=indent+"bool oversize;"
                else:
                    continue
            elif line == 5:
                str+= indent+"}" + name + ";"
                
            str+="\n"
        return str;
    

    def __create_information(self, spec, prev_field_name=''):
        for field in spec.parsed_fields():
            if field.base_type not in ['byte','char','bool','uint8','int8','uint16','int16','uint32','int32','uint64','int64','float32','float64','string','time','duration']:
                submessage_entry="ROS_MSG_BUILDUP_TYPE_SUBMESSAGE"
                #get the inner message type
                innerSpec = self.get_msgspec_form_field(field)
                if (field.is_array):
                    submessage_entry+="ARRAY"
                    if(field.array_len == None): #array with undefined length
                        submessage_entry+="_UL"
                    self.__array_depth+=1
                    if (self.__array_depth+1 > self.__max_array_depth):
                        self.__max_array_depth+=1
                self.__msg_buildup.append((self.__depth,submessage_entry, 0))
                self.__depth+=1
                self.__create_information(innerSpec, field.name)
                self.__depth-=1
                if(field.is_array):
                    self.__array_depth-=1
            else:
                field_depth=self.__depth
                if(field.is_array):
                    field_depth+=1
                    array_entry="ROS_MSG_BUILDUP_TYPE_ARRAY"
                    if(field.array_len == None):
                        array_entry+="_UL"
                    self.__msg_buildup.append((self.__depth, array_entry, 0))
    
                    if(field.base_type == 'string'): #Stringarray ...
                        if (self.__array_depth+2 >self.__max_array_depth):
                            self.__max_array_depth=self.__array_depth+2
                    else:
                        if (self.__array_depth+1 >self.__max_array_depth):
                            self.max_array_depth=self.__array_depth+1
                                           
                if(field.base_type == 'string'):#String type, which is actually a array itself
                    if (self.__array_depth+1 >self.__max_array_depth):
                        self.max_array_depth=self.__array_depth+1

                
                self.__msg_buildup.append((field_depth, "ROS_MSG_BUILDUP_TYPE_" + field.base_type.upper(),1))
        self.__msg_buildup.append((self.__depth,'ROS_MSG_BUILDUP_TYPE_MESSAGE_END',1))
    