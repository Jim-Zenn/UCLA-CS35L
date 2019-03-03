#!/bin/bash

for x in "`ls /usr/bin | awk 'NR%101==505008404%101'`";
do
    ldd `which $x`    
done
