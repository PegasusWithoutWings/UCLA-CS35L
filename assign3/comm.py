#!/usr/bin/python

"""
Output lines selected randomly from a file

Copyright 2005, 2007 Paul Eggert.
Copyright 2010 Darrell Benjamin Carbajal.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Please see <http://www.gnu.org/licenses/> for a copy of the license.

$Id: randline.py,v 1.4 2010/04/05 20:04:43 eggert Exp $
"""

import random, sys, locale, argparse

class randline:
    def __init__(self, filename):
        f = open(filename, 'r')
        self.lines = f.readlines()
        f.close()

    def chooseline(self):
        return random.choice(self.lines)

def non_negative_integer(string):
    value = int(string)
    if value < 0:
        msg = "Invalid NUMLINES: {0}.".format(string)
        raise argparse.ArgumentTypeError(msg)
    return value

def main():
    parser = argparse.ArgumentParser(prog='comm',
        description='Select or reject lines common to two files')

    parser.add_argument("-1", action='store_false', dest='output_unique_file1',
        default=True, 
        help="Suppress the output column of lines unique to file1.")
    parser.add_argument("-2", action='store_false', dest='output_unique_file2',
        default=True, 
        help="Suppress the output column of lines unique to file2.")
    parser.add_argument("-3", action='store_false', dest='output_duplicate',
        default=True, 
        help="Suppress the output column of lines duplicated in file1 and file2.")
    parser.add_argument("-u", "--unsorted",
        action='store_true', default=False,
        help="Sort the two files if they are unsorted.")
    parser.add_argument("file_name")
    args = parser.parse_args()

    try:
        generator = randline(args.file_name)
        for index in range(args.numlines):
            sys.stdout.write(generator.chooseline())
    except IOError as (errno, strerror):
        parser.error("I/O error({0}): {1}".
                     format(errno, strerror))

if __name__ == "__main__":
    main()