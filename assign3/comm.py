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

import sys, argparse

def writeline(args, line, writeType):
    if (writeType == 1) and args.output_unique_file1:
        sys.stdout.write(line)
    elif (writeType == 2) and args.output_unique_file2:
        sys.stdout.write('\t{0}'.format(line))
    elif (writeType == 3) and args.output_duplicate:
        sys.stdout.write('\t\t{0}'.format(line))

def compare_files(args):
    # When -u option is not provided, assume that the two files are sorted
    while (args.file1 != []) and (args.file2 != []):
        if args.file1[0] < args.file2[0]:
            writeline(args, args.file1.pop(0), 1)
        elif args.file1[0] == args.file2[0]:
            writeline(args, args.file1.pop(0), 3)
            args.file2.pop(0)
        else:
            writeline(args, args.file2.pop(0), 2)
    for line in args.file1:
        writeline(args, line, 1)
    for line in args.file2:
        writeline(args, line, 2)

def compare_files_unsorted(args):
    args.file1 = list(args.file1)
    args.file2 = list(args.file2) 
    printQueueMap1 = {line : [] for line in args.file1}
    printQueueMap2 = {line : [] for line in args.file2}
    sortedFile1 = sorted(args.file1)
    sortedFile2 = sorted(args.file2)
        
    while (sortedFile1 != []) and (sortedFile2 != []):
        if sortedFile1[0] < sortedFile2[0]:
            printQueueMap1[sortedFile1.pop(0)].append(1)
        elif sortedFile1[0] == sortedFile2[0]:
            printQueueMap1[sortedFile1.pop(0)].append(3)
            printQueueMap2[sortedFile2.pop(0)].append(None)
        else:
            printQueueMap2[sortedFile2.pop(0)].append(2)
    for line in sortedFile1:
        printQueueMap1[line].append(1)
    for line in sortedFile2:
        printQueueMap2[line].append(2)

    # Print out lines according to the order of the original files
    for line in args.file1:
        printQueue = printQueueMap1[line]
        printType = printQueue.pop(0)
        if (printType == 1) and args.output_unique_file1:
            writeline(args, line, 1)
        if (printType == 3) and args.output_duplicate:
            writeline(args, line, 3)

    for line in args.file2:
        printQueue = printQueueMap2[line]
        printType = printQueue.pop(0)
        if (printType == 2) and args.output_unique_file2:
            writeline(args, line, 2)

def main():
    # Initialize the argument parser; add appropriate attributes to it
    parser = argparse.ArgumentParser(prog='comm',
        description='Select or reject lines common to two files')
    # Add required arguments to the parser
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
    parser.add_argument("file1", type=argparse.FileType('r'))
    parser.add_argument("file2", type=argparse.FileType('r'))
    args = parser.parse_args()

    args.file1 = list(args.file1)
    args.file2 = list(args.file2)

    # Start comparing files and write to stdout according to set options
    if args.unsorted:
        compare_files_unsorted(args)
    else:
        compare_files(args)

if __name__ == "__main__":
    main()