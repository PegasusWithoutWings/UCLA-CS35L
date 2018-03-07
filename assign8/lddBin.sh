#!/bin/bash

for x in "$(ls /usr/bin | awk 'NR%101==805040704%101' $1)"; do
    y=`which $x`
    ldd $y
done

exit 0
