#!/usr/bin/python2.7
#-*-coding:utf-8-*-

""" A conveter for bilingual dictionaries on wetsit dicts.info

This module converts dicts.info's  bilingual dictionary file to the front-end xml of AlphaDict.
"""

# Dicts.info:
#   A free dictionaries Project.
#   Here is the websit: 
#     "http://www.dicts.info/"
#     "http://www.dicts.info/uddl.php"
#
# File Format
#   src-language-word ; src-language-word2  \t	Other-language-word
#   ...
#   where the pairs are separated by a tab character, and each pair is terminated by a carriage return character. 
#   April  ; Apr	四月 [sì yuè]	time
#
# Changes:
#   Init version: Jun 3, 2014 LiQiong Lee<LiQiong.kartorz.lee@gmail.com>

import sys, os, codecs

from lib_aldictxml import *

usage = """Usage: dictsinfo_to_aldictxml [file_in] [file_out]
Convert dicts.info's bilingual dictionary file to front-end xml of AlphaDict.
file_in:  The bilingual dict file.
file_out: Specify the output xml file.

Default is at the input file path appending ".xml" as a suffix to the output file
For example:
    dictsinfo_to_aldictxml.py  foo.txt
    dictsinfo_to_aldictxml.py  foo.txt foo
"""

if len(sys.argv) < 2:
	print usage
	sys.exit()

# Open input file
try:
	f_input = codecs.open(sys.argv[1], mode='r', encoding='utf-8', errors='ignore')
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
        header["srclan"] = "any"
        header["detlan"] = "any"
        header["publisher"] = "www.dicts.info"
        header["publishdate"] = "2014-01-01"
        header["dictname"] = "bilingual dictionary"
        xml_alphadict_writeheader(header)

        for line in f_input:
            if line.startswith('#'):
                continue
            if line.find('\t') == -1:
                continue
                
            phonetics = []
            explanation = []
            words = []
            phrase = []
            phrase = line.split('\t')
            words = phrase[0].split(';')
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

