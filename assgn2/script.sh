#! /bin/sh
#
# script.sh
# Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
#
# Distributed under terms of the MIT license.
#


tr -d '\n' < local.html | grep -o "<td>[^<]*\(<u>[^<]*</u>[^<]*\)*[^<]\+</td> *</tr>" | sed 's/<td>//g;s/<\/td>//g;s/<u>//g;s/<\/u>//g;s/<\/tr>//g'
