#!/usr/bin/python2.7
#-*-coding:utf-8-*-

""" A conveter for IDP

This module converts the IDP file to the front-end xml of AlphaDict.
"""

# IDP:
#   The Internet Dictionary Project's  goal was to create royalty-free 
#   translating dictionaries through the help of the Internet's citizens
#   Here is the websit: 
#     "http://www.june29.com/IDP/IDPfiles.html"
#     "http://www.june29.com/IDP/"
#
# File Format
#   The dictionary files are lists of pairs of English and other-language words, in the form:
#   English-word	Other-language-word
#   ...
#   where the pairs are separated by a tab character, and each pair is terminated by a carriage return character. 
#     dog	perro
#     dog	perra
#
# Changes:
#   Init version: Jun 2, 2014 LiQiong Lee<LiQiong.kartorz.lee@gmail.com>

import sys, os, codecs

from lib_aldictxml import *

usage = """Usage: idp_to_aldictxml [file_in] [file_out]
Convert IDP file to front-end xml of AlphaDict.
file_in:  The IDP dict file.
file_out: Specify the output xml file.

Default is at the input file path appending ".xml" as a suffix to the output file
For example:
    idp_to_aldictxml.py  foo.txt
    idp_to_aldictxml.py  foo.txt foo
"""

if len(sys.argv) < 2:
	print usage
	sys.exit()

# Open input file
try:
	f_input = codecs.open(sys.argv[1], mode='r', encoding='ascii', errors='ignore')
except IOError:
	sys.stderr.write("The file(%s) does not exist \n" %(sys.argv[1]))
	sys.exit()

if len(sys.argv) == 3:
	output_path = sys.argv[2]
else:
	output_path = sys.argv[1]
	i = output_path.rfind('.')
	if i == -1:
		output_path = sys.argv[1] + ".xml"
	else:
		output_path = output_path[:i] + ".xml"

if not xml_alphadict_openfile(output_path):
	f_input.close()
	sys.exit()

try:
	header = {}
        header["dictversion"] = "0"
        header["srclan"] = "english"
        header["detlan"] = "french"
        header["publisher"] = "IDP"
        header["publishdate"] = "2007-02-01"
        header["dictname"] = "french"
        xml_alphadict_writeheader(header)

        for line in f_input:
            if not line.startswith('#'):
                phonetics = []
                explanation = []
                words = []
                phrase = []
                phrase = line.split('\t')
                words.append(phrase[0])
                explanation.append(phrase[1])
                xml_alphadict_writeword(words, phonetics, explanation)
                try :
				    print words[0]
				except:
				    pass

finally:
	f_input.close()
	xml_alphadict_close()
	sys.stdout.write("\n")
