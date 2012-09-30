#!/usr/bin/python3
'''
Created on 04.09.2012

@author: choll
'''
import re
import sys
import xml.etree.ElementTree as etree
import xml.dom.minidom
from PyQt4 import Qt
from PyQt4 import QtGui

def TreePrint(root,level=0):
    for i in range(level):
        sys.stdout.write("\t")
    sys.stdout.write(root.tag)
    text="{}".format((root.text)).rstrip()
    text=re.sub("None", "", text);
    print("({})".format(text))
    for tag in root:
        TreePrint(tag, level+1)
        

def ExtractTCP(filename,outfile):
    with open(filename, mode='br') as file:
        file_content=str(file.read()).replace('\\n','\n').replace('\\r','\r').replace('\\\'','\'').replace('\\t','\t')

    matches=re.findall('(?s)((User-Agent|Server).*?(</(methodCall|methodResponse)>))', file_content, re.MULTILINE)    
    
    with open(outfile, mode='w') as outfile: 
        outfile.write('<?xml version="1.0" ?>')
        for match in matches:
            position_xml=match[0].find('<?xml')
            if position_xml == -1:
                outfile.write('\n\n<!--#####################\n')
                outfile.write("   ---No XML Part !?!?---   ")
                outfile.write(match[0])
                outfile.write('\n#####################-->\n')
            else:
                outfile.write('\n\n<!--#####################\n')
                outfile.write(match[0][:position_xml])
                outfile.write('\n#####################-->\n')
                try:                    
                    parsed_XML=xml.dom.minidom.parseString(match[0][position_xml:])
                    pretty_xml_as_string = parsed_XML.toprettyxml()
                    pretty_xml_as_string=re.sub('(<\?xml.*?\?>)','<!-- XML Declaration (removed)-->',pretty_xml_as_string)
                    outfile.write(pretty_xml_as_string)
                except:
                    outfile.write("<!--\nREPORTED AS Not WELL FORMED!!!\n")
                    outfile.write(match[0][position_xml:])
                    outfile.write("-->")



a = Qt.QApplication(sys.argv)
fileDialog_in = QtGui.QFileDialog()
fileDialog_out = QtGui.QFileDialog()

filename_open = fileDialog_in.getOpenFileName(None, 'Open Wireshark Log', '~','Network Log (*.cap, *.dmp, *.pcap, *)')
print(filename_open)
if filename_open == "":
    exit(0)
    
#fileDialog_out.setFilter(dir='*.xml')
filename_save = fileDialog_out.getSaveFileName(None, 'Save Output as XML', '~','XML (*.xml)')
print(filename_save)
if filename_save== "":
    exit(0)


ExtractTCP(filename_open,filename_save)