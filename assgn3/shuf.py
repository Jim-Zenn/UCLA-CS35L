#! /usr/bin/python
#
# Copyright 2019 Jim Zenn <zenn@ucla.edu>
#
# Distributed under terms of the MIT license.

import random, sys
from optparse import OptionParser

usage_msg = """
       shuf [OPTION]... [FILE]
  or:  shuf -i LO-HI [OPTION]...
Write a random permutation of the input lines to standard output.

With no FILE, or when FILE is -, read standard input.

Mandatory arguments to long options are mandatory for short options too.
  -i, --input-range=LO-HI   treat each number LO through HI as an input line
  -n, --head-count=COUNT    output at most COUNT lines
  -r, --repeat              output lines can be repeated
      --help     display this help and exit
      --version  output version information and exit """

version_msg = """
shuf (Python 2, Simplified version based on GNU coreutils 8.30)
Copyright (C) 2018 Jim Zenn
Distributed under terms of the MIT Licence.
There is NO WARRANTY, to the extent permitted by law.
Written by Jim Zenn.
"""

class Shuf:
    def __init__(self, lines, repeat, count):
        random.shuffle(lines)
	self.lines = lines
        self.repeat = repeat
        self.count = count
        self.index = 0

    def __iter__(self):
        return self

    def next(self):
        if self.count == 0 or self.index == len(self.lines):
            raise StopIteration
        if self.repeat:
            result = random.choice(self.lines)
	else:
            result = self.lines[self.index]
            self.index += 1
	if self.count > 0:
            self.count -= 1 
        return result


def main():
    
    # parse the options
    parser = OptionParser(version=version_msg, usage=usage_msg)

    # --input-range (-i)
    parser.add_option("-i", "--input-range", type="string",
                      action="store", dest="input_range", default=None, help="treat each number LO through HI as an input line")
    # --head-count (-n)
    parser.add_option("-n", "--head-count", type="int",
                      action="store", dest="head_count", default=-1,
                      help="output at most COUNT lines")
    # --repeat (-r)
    parser.add_option("-r", "--repeat",
                      action="store_true", dest="repeat", default=False,
                      help="output lines can be repeated")

    (options, args) = parser.parse_args(sys.argv[1:])

    # check the syntax of the options
    if options.input_range is not None:
	if len(args) != 0:  # with the input_range, we do not need input file
            parser.error("extra operand '%s'" % args[0])
            return
        try:
	    start_str, end_str = (options.input_range).split("-")
        except ValueError:
            parser.error("invalid input range: " + options.input_range)
            return
	start, end = int(start_str), int(end_str) 
        if end < start:
            parser.error("invalid input range: " + options.input_range)
            return
        lines = [str(x) for x in range(start, end + 1)]
    elif len(args) == 0 or args[0] == '-':
        # read from standard input
	lines = sys.stdin.readlines()
    else:
	if len(args) != 1:
            parser.error("extra operand '%s' " % args[1])
            return
        filename = args[0]
        file_s = open(filename)
        lines = file_s.read().splitlines()
        file_s.close()


    # write
    shuf = Shuf(lines, options.repeat, options.head_count)
    for line in shuf:
        print line


if __name__ == "__main__":
    main()

