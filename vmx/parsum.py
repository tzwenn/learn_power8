#!/usr/bin/env python

sum_rangeT = lambda n: (n * (n + 1)) / 2
sum_range = lambda s, e: sum_rangeT(e) - sum_rangeT(s) + s
