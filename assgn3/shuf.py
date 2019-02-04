#! /usr/local/cs/bin/python3
#
# Copyright 2019 Jim Zenn <zenn@ucla.edu>
#
# Distributed under terms of the MIT license.

import random
import sys
from optparse import OptionParser

usage_msg = """
Write a random permutation of the input lines to standard output.

With no FILE, or when FILE is -, read standard input.

Mandatory arguments to long options are mandatory for short options too.
  -i, --input-range=LO-HI   treat each number LO through HI as an input line
  -n, --head-count=COUNT    output at most COUNT lines
  -r, --repeat              output lines can be repeated
      --help     display this help and exit
      --version  output version information and exit """

version_msg = """
shuf (Simplified version based on GNU coreutils 8.30)
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

    def __next__(self):
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

    # parse commands' options
    parser = OptionParser(version=version_msg, usage=usage_msg)
    parser.add_option("-i", "--input-range", type="string",
                      action="store", dest="input_range", default=None,
                      help="treat each number LO through HI as an input line")
    parser.add_option("-n", "--head-count", type="int",
                      action="store", dest="head_count", default=-1,
                      help="output at most COUNT lines")
    parser.add_option("-r", "--repeat",
                      action="store_true", dest="repeat", default=False,
                      help="output lines can be repeated")
    (options, args) = parser.parse_args(sys.argv[1:])

    # make sure the syntax of the options are correct
    if options.input_range is not None:
        # with a input range, we do not need an input file
        if len(args) != 0:
            parser.error("extra operand '%s'" % args[0])
            return
        # get the start and end of the range
        try:
            start_str, end_str = (options.input_range).split("-")
        except ValueError:
            parser.error("invalid input range: %s" % options.input_range)
            return
        start, end = int(start_str), int(end_str)
        # the end value should be after start value
        if end < start:
            parser.error("invalid input range: %s" % options.input_range)
            return
        # create the range list
        lines = [str(x) for x in range(start, end + 1)]
    # if neither input range nor an input file is specified, use stdin
    elif len(args) == 0 or args[0] == '-':
        lines = sys.stdin.readlines()
    # if one input file is specified
    else:
        # check there is only one input file
        if len(args) != 1:
            parser.error("extra operand '%s' " % args[1])
            return
        filename = args[0]
        file_s = open(filename)
        lines = file_s.read().splitlines()
        file_s.close()

    # write to stdout line by line
    shuf = Shuf(lines, options.repeat, options.head_count)
    for line in shuf:
        sys.stdout.write(line)


if __name__ == "__main__":
    main()
