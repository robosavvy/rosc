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

class msg_static(object):
    '''
    classdocs
    '''
    
    PADDING_LAST_START=0
    PADDING_LAST_BRACKET_OPEN=1
    PADDING_LAST_VAR=2
    PADDING_LAST_BRACKET_CLOSE=3
    
    __search_path=[] #The search paths for packages
    __msg_spec=[] #The specification of the message
    __msg_buildup_array=[] #The array string for the "buildup information"
    __msg_static_struct="" #The struct for defining a static message
    __msg_static_padding_init="" #The initializing values for a padding localization struct #deprecated
    __msg_static_padding_init_last=PADDING_LAST_START #The previous kind of padding init, brackets and so on #deprecated
    __msg_static_padding_init_brackets_open=0 #a counter for the open brackets #unused #deprecated
    __msg_static_size_fields=[] #size of fields needed for the memory filling (structures in arrays only)
    __msg_static_substructure_components=[] #current component calls (c)
    __msg_static_substructure_sizes=[] #substructure component list for creating size array
    __msg_static_member_offsets=[] #message member memory offsets
    __array_depth=0 #current array depth of the message
    __max_array_depth=0 #maximum array depth of the message
    __message_struct_indent_depth=0 #current message depth for structs
    __message_buildup_indent_depth=0
    
    def __init__(self, msg_spec, search_path):
        '''
        Constructor
        '''
        self.__msg_spec=msg_spec
        self.__search_path=search_path
        self.__struct_define_recursive_create(msg_spec)
    
    
    def get_static_member_offsets(self):
        return self.__msg_static_member_offsets
    
    def get_static_in_substructure_sizes(self):
        return self.__msg_static_substructure_sizes
    
    def get_static_buildup_array(self):
        return self.__msg_buildup_array
    
    def get_static_struct(self):
        return self.__msg_static_struct
    
    def get_static_struct_size_fields(self):
        return self.__msg_static_size_fields
    
    def get_padding_init(self):
        return self.__msg_static_padding_init
    
    def print_buildup_array(self):
        for (indent,text, additionalreturn) in self.__msg_buildup_array:
            if additionalreturn == 1:
                print self.__add_tabs(indent) + text + ",\n"
            else:
                print self.__add_tabs(indent) + text + ","
                
        '''
            Prints the definition for a userdefined static struct
        '''
    def print_struct_definition(self):  
        messageType= "rosc_static_msg__" + self.__msg_spec.package + "__" + self.__msg_spec.short_name + "__ ## USER_TYPE "
        lookupName= "rosc_static_msg_lookup__" + self.__msg_spec.package + "__" + self.__msg_spec.short_name + "__ ## USER_TYPE "

        output= "#define ROSC_USERDEF_STATIC_MSG_" + self.__msg_spec.package + "__" + self.__msg_spec.short_name + "(\nUSER_TYPE,\n"
        first=True
        for staticf in self.__msg_static_size_fields:

            if not (staticf.isdigit()):
                if first:
                    first=False
                else:
                    output+=",\n"
                output+=staticf
        output+=")\n"
        output+="typedef \n"
        output+=self.__msg_static_struct
        output+= "\nuint32_t rosc_static_msg_length_definition__" + self.__msg_spec.package +"__"+ self.__msg_spec.short_name + "__ ## USER_TYPE[]={\n"
        first=True 
        for staticf in self.__msg_static_size_fields:
            if first:
                first=False
            else:
                output+=",\n"
            output+= staticf
        output+="};\n"
        output+="union\n"
        output+="{\n"
        output+="\tconst "+ messageType + "msg;\n"
        output+="\tconst char padding_out[ sizeof( " + messageType + ")];\n"
        output+="}"+ lookupName + "="
        output+=self.__msg_static_padding_init  + ";"
        output=output.replace("\n", "\\\n") + "\n"
        print output
        pass
    
        '''
            This gets the message spec from a non base type submessage
            :param field: :class:`MsgContext`
        '''
    def __get_submessage_data_from_field(self, field):
        msg_context = genmsg.msg_loader.MsgContext()
        return genmsg.msg_loader.load_msg_by_type(msg_context, field.base_type, self.__search_path)

    def __add_tabs(self, count):
        tabs=""
        for i in range(count):
            tabs = tabs + "\t"
        return tabs


    '''
        Creates the function for creating a array for the position of struct members
    '''
    def __offset_add(self,field):
        if (field.base_type in ['byte','char','bool','uint8','int8','uint16','int16','uint32','int32','uint64','int64','float32','float64', 'string', 'time', 'duration']):         
            out=""
            for comp in self.__msg_static_substructure_components:
                (comp_str, comp_array, undef_array)=comp
                out+="." + comp_str
                if(comp_array):
                    self.__msg_static_member_offsets.append((out, "." + "size"))
                    if(undef_array):
                        self.__msg_static_member_offsets.append((out, "." + "oversize"))                         
                    out+=".data[0]"
            
            array_sub=''
            if(field.is_array):
                self.__msg_static_member_offsets.append((out + "." +field.name, ".size" ))
                if(field.array_len==None):
                    self.__msg_static_member_offsets.append((out + "." +field.name, ".oversize" ))
                array_sub=".data[0]"
            
            if field.base_type == 'time' or field.base_type == 'duration' :
                self.__msg_static_member_offsets.append((out + "." +field.name+array_sub, ".sec" ))
                self.__msg_static_member_offsets.append((out + "." +field.name+array_sub, ".nsec" ))
            elif (field.base_type == 'string'):
                self.__msg_static_member_offsets.append((out + "." +field.name+array_sub, ".size" ))
                self.__msg_static_member_offsets.append((out + "." +field.name+array_sub, "." + "oversize"))    
                self.__msg_static_member_offsets.append((out + "." +field.name+array_sub, "." + "str_data[0]"))
            else:
                if(field.is_array):
                    self.__msg_static_member_offsets.append((out + "." +field.name, ".data[0]"))
                else:
                    self.__msg_static_member_offsets.append((out, "." + field.name))

    def __substructure_list_add(self, field):
        self.__msg_static_substructure_components.append((field.name,field.is_array, field.array_len==None))

    def __substructure_list_remove(self):
        self.__msg_static_substructure_components.pop()
    
    def __substructure_len_append(self, field):
        if not (field.base_type in ['byte','char','bool','uint8','int8','uint16','int16','uint32','int32','uint64','int64','float32','float64', 'time', 'duration']):         
            out=""
            for comp in self.__msg_static_substructure_components:
                (comp_str, comp_array, undef_array)=comp
                out+="." + comp_str
                if(comp_array):
                    out+=".data[0]"
            out+="." + field.name + ".data[0]"
            self.__msg_static_substructure_sizes.append(out)


    def __msg_buildup_array_entry(self,field):

        entry=""
        if field.base_type not in ['byte','char','bool','uint8','int8','uint16','int16','uint32','int32','uint64','int64','float32','float64','string','time','duration']:
            entry="ROS_MSG_BUILDUP_TYPE_SUBMESSAGE"
            if(field.is_array):
                entry+="ARRAY"
                if(field.array_len == None): #array with undefined length
                    entry+="_UL"
            self.__msg_buildup_array.append((self.__message_buildup_indent_depth, entry, 0))
        else:
            indent=self.__message_buildup_indent_depth
            if(field.is_array):
                entry="ROS_MSG_BUILDUP_TYPE_ARRAY"
                if(field.array_len == None): #array with undefined length
                    entry+="_UL"
                self.__msg_buildup_array.append((self.__message_buildup_indent_depth, entry, 0))
                self.__msg_buildup_array.append((self.__message_buildup_indent_depth+1, "ROS_MSG_BUILDUP_TYPE_" + field.base_type.upper(), 1))
            else:
                self.__msg_buildup_array.append((self.__message_buildup_indent_depth, "ROS_MSG_BUILDUP_TYPE_" + field.base_type.upper(), 1))

    def __struct_define_add_header(self, field, prev_names):        
        if(field == None):
            comment = "Main Message Start"
        else:    
            comment = field.name
        self.__msg_static_struct+=self.__add_tabs(self.__message_struct_indent_depth) + "struct" + "\t/*" + comment +"*/\n"
        self.__msg_static_struct+=self.__add_tabs(self.__message_struct_indent_depth) + "{\n"
        self.__message_struct_indent_depth+=1
        
        if(field != None):
            if(field.is_array):
                self.__msg_static_struct+=self.__add_tabs(self.__message_struct_indent_depth) + "uint32_t size;\n"
                self.__msg_static_size_fields.append(self.__struct_define_Length_or_Def_Param(field, prev_names))

                if(field.array_len == None):
                    self.__msg_static_struct+=self.__add_tabs(self.__message_struct_indent_depth) + "bool oversize;\n"
                    
                if(field.base_type not in ['byte','char','bool','uint8','int8','uint16','int16','uint32','int32','uint64','int64','float32','float64']):
                    self.__msg_static_struct+=self.__add_tabs(self.__message_struct_indent_depth) + "struct"+"\t/*" + comment +" array data*/\n"
                    self.__msg_static_struct+=self.__add_tabs(self.__message_struct_indent_depth) + "{\n"
                    self.__message_struct_indent_depth+=1

             
    def __struct_define_Length_or_Def_Param(self, prev_field, prev_names):
        lendef=""
        if(self.__message_struct_indent_depth !=0):
            if (prev_field.is_array):
                if(prev_field.array_len == None):
                    lendef+='MAX_SIZE_ARRAY' + prev_names + "_" + prev_field.name
                else:
                    lendef=str(prev_field.array_len)
        return lendef
    
    def __struct_define_add_footer(self, field, prev_names):
        self.__message_struct_indent_depth-=1
        if(field==None):
            self.__msg_static_struct+=self.__add_tabs(self.__message_struct_indent_depth) + "}"
        else:
            if (field.is_array and field.base_type not in ['byte','char','bool','uint8','int8','uint16','int16','uint32','int32','uint64','int64','float32','float64']):
                self.__msg_static_struct+=self.__add_tabs(self.__message_struct_indent_depth)+"}data["
                self.__msg_static_struct+=self.__struct_define_Length_or_Def_Param(field, prev_names)
                self.__msg_static_struct+="];\n"
                self.__message_struct_indent_depth-=1
                
            self.__msg_static_struct+=self.__add_tabs(self.__message_struct_indent_depth) + "}" + field.name
            self.__msg_static_struct+=";\n"
        
    def __struct_define_add_variable(self, field, prev_names):
        self.__padding_add_var(field)
        if(field.is_array or field.base_type in ['string', 'time', 'duration']):
            self.__struct_define_add_header(field, prev_names)
            self.__substructure_len_append(field)
            field_out="data"
        else:
            field_out=field.name
                    
        if field.base_type in ['uint8', 'int8', 'uint16', 'int16', 'uint32', 'int32', 'uint64', 'int64', 'float32', 'float64']:
            if(field.base_type in ['float32','float64']):
                self.__msg_static_struct += self.__add_tabs(self.__message_struct_indent_depth) + 'union\n'
                self.__msg_static_struct += self.__add_tabs(self.__message_struct_indent_depth) + '{\n'
                self.__message_struct_indent_depth+=1
                if(field.base_type == 'float32'):
                    self.__msg_static_struct += self.__add_tabs(self.__message_struct_indent_depth) + 'uint32_t ___padding_init_'
                else:
                    self.__msg_static_struct += self.__add_tabs(self.__message_struct_indent_depth) + 'uint64_t ___padding_init_'
                self.__msg_static_struct += field.name + ";\n"

            self.__msg_static_struct += (self.__add_tabs(self.__message_struct_indent_depth) + field.base_type + "_t " + field_out)
           
            if(field.base_type in ['float32','float64']):
                self.__message_struct_indent_depth-=1
                self.__msg_static_struct += ";\n" + self.__add_tabs(self.__message_struct_indent_depth) + '}'
                
        elif field.base_type == 'byte':
           self.__msg_static_struct += (self.__add_tabs(self.__message_struct_indent_depth) + 'uint8_t ' + field_out)
        elif field.base_type == 'bool':
           self.__msg_static_struct += (self.__add_tabs(self.__message_struct_indent_depth) + 'bool ' + field_out)
        elif field.base_type == 'char':
           self. __msg_static_struct += (self.__add_tabs(self.__message_struct_indent_depth) + 'int8_t ' + field_out)
        elif field.base_type == 'time' or  field.base_type == 'duration':
           output= self.__add_tabs(self.__message_struct_indent_depth) + 'uint32_t sec;' + '\n'
           output+= self.__add_tabs(self.__message_struct_indent_depth) + 'uint32_t nsec;' + '\n'
           self.__msg_static_struct+=output
        elif field.base_type == 'string':
           output= self.__add_tabs(self.__message_struct_indent_depth) + 'uint32_t size;' + '\n'
           output+= self.__add_tabs(self.__message_struct_indent_depth) + 'bool oversize;' + '\n'
           output+= self.__add_tabs(self.__message_struct_indent_depth) + 'char str_data['  "MAX_SIZE_STRING" + prev_names + "_" + field.name +'];' + '\n' 
           self.__msg_static_size_fields.append("MAX_SIZE_STRING" + prev_names + "_" + field.name)
           self.__msg_static_struct+=output



        if(field.is_array and field.base_type not in ['string', 'duration', 'time'] ):
            self.__msg_static_struct+= "["
            self.__msg_static_struct+=self.__struct_define_Length_or_Def_Param(field, prev_names)
            self.__msg_static_struct+= "]"
            self.__msg_static_struct+= ";\n"
        
        if(field.is_array or field.base_type in ['string','time','duration']):
            self.__struct_define_add_footer(field, prev_names)
        else:
            self.__msg_static_struct+= ";\n"  

    '''
        Creates the structure for storing information
    '''
    def __struct_define_recursive_create(self, spec, prev_field=None, prev_names=''):
        self.__message_buildup_indent_depth+=1
        self.__struct_define_add_header(prev_field, prev_names)
        self.__padding_open_bracket()
        prev_names_new=""
        if(prev_field != None):
            prev_names_new=prev_names + "_" + prev_field.name
        for field in spec.parsed_fields():
            self.__msg_buildup_array_entry(field)
            if field.base_type not in ['byte','char','bool','uint8','int8','uint16','int16','uint32','int32','uint64','int64','float32','float64','string','time','duration']:
                               
                #####ARRAY DEPTH SIZE DETERMINATION#####
                if (field.is_array):
                    self.__array_depth+=1
                    if (self.__array_depth+1 > self.__max_array_depth):
                        self.__max_array_depth+=1
                ########################################
                

                sub_msg_spec=self.__get_submessage_data_from_field(field)
                self.__substructure_len_append(field)
                self.__substructure_list_add(field)
                self.__struct_define_recursive_create(sub_msg_spec, field, prev_names_new) #Recursive call go one message type deeper
                
                self.__substructure_list_remove()
                
                
                #####ARRAY DEPTH SIZE DETERMINATION#####
                if(field.is_array):
                    self.__array_depth-=1
                ########################################   
            else:                
                self.__offset_add(field)
                self.__struct_define_add_variable(field, prev_names_new)

                if(field.is_array):
                    #####ARRAY DEPTH SIZE DETERMINATION###################
                    if(field.base_type == 'string'): #Stringarray ...
                        if (self.__array_depth+2 >self.__max_array_depth):
                            self.__max_array_depth=self.__array_depth+2
                    else:
                        if (self.__array_depth+1 >self.__max_array_depth):
                            self.max_array_depth=self.__array_depth+1
                    #######################################################
                    
                #####ARRAY DEPTH SIZE DETERMINATION###################               
                if(field.base_type == 'string'):#String type, which is actually a array itself
                    if (self.__array_depth+1 >self.__max_array_depth):
                        self.max_array_depth=self.__array_depth+1
                #######################################################
        self.__msg_buildup_array.append((self.__message_buildup_indent_depth,'ROS_MSG_BUILDUP_TYPE_MESSAGE_END',1))
        self.__message_buildup_indent_depth-=1
        self.__padding_close_bracket()
        self.__struct_define_add_footer(prev_field, prev_names)
       

    def __padding_open_bracket(self):
        if(self.__msg_static_padding_init_last == self.PADDING_LAST_START):
            pass
        elif(self.__msg_static_padding_init_last == self.PADDING_LAST_VAR):
            pass
        elif(self.__msg_static_padding_init_last == self.PADDING_LAST_BRACKET_OPEN):
            pass
        elif(self.__msg_static_padding_init_last == self.PADDING_LAST_BRACKET_CLOSE):
             self.__msg_static_padding_init+=","
        self.__msg_static_padding_init_last = self.PADDING_LAST_BRACKET_OPEN
        
        self.__msg_static_padding_init+="{"
        self.__msg_static_padding_init_brackets_open+=1
        
    def __padding_close_bracket(self):
        if(self.__msg_static_padding_init_last == self.PADDING_LAST_START):
            pass
        elif(self.__msg_static_padding_init_last == self.PADDING_LAST_VAR):
            pass
        elif(self.__msg_static_padding_init_last == self.PADDING_LAST_BRACKET_OPEN):
            pass
        elif(self.__msg_static_padding_init_last == self.PADDING_LAST_BRACKET_CLOSE):
            pass
        self.__msg_static_padding_init_last = self.PADDING_LAST_BRACKET_CLOSE
        
        self.__msg_static_padding_init+="}"
        self.__msg_static_padding_init_brackets_open-=1
        
    def __padding_add_var(self, field):
        fieldtype=field.base_type
        
        if(self.__msg_static_padding_init_last == self.PADDING_LAST_START):
            pass
        elif(self.__msg_static_padding_init_last == self.PADDING_LAST_VAR):
            self.__msg_static_padding_init+=","
        elif(self.__msg_static_padding_init_last == self.PADDING_LAST_BRACKET_OPEN):
            pass
        elif(self.__msg_static_padding_init_last == self.PADDING_LAST_BRACKET_CLOSE):
            self.__msg_static_padding_init+=","
        self.__msg_static_padding_init_last = self.PADDING_LAST_VAR
        
        if(field.is_array):
            if(field.array_len != None): #Defined length?
                self.__msg_static_padding_init+="{ 0x04040404, {"
            else:                         #Undefined length -> add bool oversize init
                self.__msg_static_padding_init+="{ 0x04040404, 0x01, {"
            
        if fieldtype in ['uint8', 'int8', 'char', 'bool']:
            #Padding init struct end
            self.__msg_static_padding_init+='0x01'
            pass
        elif fieldtype in ['uint16', 'int16']:
            #Padding init struct end
            self.__msg_static_padding_init+='0x0202'
            pass
        elif fieldtype in ['uint32', 'int32']:
            self.__msg_static_padding_init+='0x04040404'
            pass
        elif fieldtype in ['uint64', 'int64']:
            self.__msg_static_padding_init+='0x0808080808080808'
            pass     
        elif fieldtype == 'float32':
            self.__msg_static_padding_init+='{0x04040404}'
        elif fieldtype == 'float64':
            self.__msg_static_padding_init+='{0x0808080808080808}'
        elif fieldtype in ['time', 'duration']:
            self.__msg_static_padding_init+='{ 0x04040404,0x04040404}'
            pass
        elif fieldtype == 'string':
            self.__msg_static_padding_init+='{ 0x04040404, 0x01, {0x01} }'
            pass

        if(field.is_array):
            self.__msg_static_padding_init+="} }"
