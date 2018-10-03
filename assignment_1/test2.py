import os
import sys
from operator import itemgetter

slist = [[3,'aaa'],[2,'bb'],[5,'ccc']]
print(slist)
slist.sort(key = itemgetter(0))
print(slist)

