# -*- coding: utf-8 -*-
#! /Users/Jim/.conda/envs/python2/bin/python2
#
# Copyright © 2019 Jim Zenn <zenn@ucla.edu>
#
# Distributed under terms of the MIT license.

import random, sys
from optparse import OptionParser

class shuf:
    def __init__(self, filename):
        f = open(filename, 'r')
        self.lines = f.readlines()
        f.close()

    def chooseline(self):
        return random.choice(self.lines)

usage_msg = """
Usage: shuf [OPTION]... [FILE]
  or:  shuf -i LO-HI [OPTION]...
Write a random permutation of the input lines to standard output.

With no FILE, or when FILE is -, read standard input.

Mandatory arguments to long options are mandatory for short options too.
  -i, --input-range=LO-HI   treat each number LO through HI as an input line
  -n, --head-count=COUNT    output at most COUNT lines
  -r, --repeat              output lines can be repeated
      --help     display this help and exit
      --version  output version information and exit
"""

version_msg = """
shuf (Python 2, Simplified version based on GNU coreutils 8.30)
Copyright (C) 2018 Jim Zenn
Distributed under terms of the MIT Licence.
There is NO WARRANTY, to the extent permitted by law.
Written by Jim Zenn.
"""

def main():
    parser = OptionParser(version=version_msg, usage=usage_msg)
    # --input-range (-i)
    parser.add_option("-i", "--input-range",
                      action="store", dest="arg_input_range", default=None,
                      help="treat each number LO through HI as an input line")
    # --head-count (-n)
    parser.add_option("-n", "--head-count",
                      action="store", dest="arg_head_count", default=None,
                      help="output at most COUNT lines")
    # --repeat (-r)
    parser.add_option("-i", "--repeat",
                      action="store", dest="arg_repeat", default=False,
                      help="output lines can be repeated")

    options, args = parser.parse_args(sys.argv[1:])

    print(options, args)

