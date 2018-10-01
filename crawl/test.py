import threading
import os
import re
import requests
import time

t_start = time.time()

threads = []
for i in range(35):
    print(1992+i)
    url = "https://www.ptt.cc/bbs/Beauty/index"+ str(1992+i) +".html"
    threads.append(threading.Thread(target = requests.get, args = (url,)))
    threads[i].start()

# 主執行緒繼續執行自己的工作
# ...

# 等待所有子執行緒結束
for i in range(35):
    threads[i].join()

print("Done.")

t_end = time.time()
print ("It cost %f sec" % (t_end - t_start))