import threading
import os
import re
import requests
import time
import queue
from bs4 import BeautifulSoup

class UrlThread(threading.Thread):
    def __init__(self, url_queue,content_queue) :
        threading.Thread.__init__(self)
        self.url_queue = url_queue
        self.content_queue = content_queue
    def run(self):
        while self.url_queue.qsize() > 0 :
            url = self.url_queue.get()
            content = requests.get(url).text
            soup = BeautifulSoup(content,'html.parser')
            ptt_list = soup.find_all(class_="r-ent")
            for part in ptt_list :
                date = part.find(class_="date").string.replace('/','').strip()
                author = part.find(class_="author").string
                self.content_queue.put([date,author])


def main() :
    t_start = time.time()

    url_queue = queue.Queue()
    content_queue = queue.Queue()

    for ptt_index in range(1992,2000):
        url = "https://www.ptt.cc/bbs/Beauty/index"+ str(ptt_index) +".html"
        url_queue.put(url)

    threads = []
    for i in range(5):
        urlthread = UrlThread(url_queue,content_queue)
        urlthread.start()
        threads.append(urlthread)
    
    for thread in threads :
        thread.join()

    results = []

    size = content_queue.qsize()
    for _ in range(size):
        results.append(content_queue.get())
    print(results)


    print("Done.")

    t_end = time.time()
    print ("It cost %f sec" % (t_end - t_start))

if __name__ == '__main__':
    main()