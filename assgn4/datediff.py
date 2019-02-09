#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2019 Jim Zenn <zenn@ucla.edu>
#
# Distributed under terms of the MIT license.

"""
This simple function checks the second difference between two specified time.
"""
from datetime import datetime
import time

fmt = '%Y-%m-%d %H:%M:%S'
a = '1918-11-11 03:00:00'
b = '2019-02-09 12:25:31'

d1 = datetime.strptime(a, fmt)
d2 = datetime.strptime(b, fmt)

d1_ts = time.mktime(d1.timetuple())
d2_ts = time.mktime(d2.timetuple())

print(int(d2_ts - d1_ts))

