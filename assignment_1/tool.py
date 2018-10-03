import os
import sys
import requests
from bs4 import BeautifulSoup
import queue
import threading

def GetUrlFromTxt(content_ar, start_date, end_date) :
    url_list = []
    for line in content_ar.split('\n') :
        part = line.split(',')
        if len(part) == 3 :
            date = part[0]
            if int(date) < start_date or int(date) > end_date :
                continue
            #title = part[1]
            url = part[2]
            url_list.append(url)
    return url_list

def GetFileExtension(path) :
    return os.path.splitext(path)[1].strip('.')

def ReadFile(filename) :
    file_ = open(sys.path[0]+'\\'+ filename,'r',encoding = 'utf8')
    content = file_.read()
    file_.close()
    return content

def OpenWriteFile(filename) :
    return open(sys.path[0]+'\\'+ filename,'w',encoding = 'utf8')

def GetPicUrl(content_url) :
    content_html = BeautifulSoup(content_url,'html.parser')
    pic_list = content_html.find_all(target = "_blank")
    pic_url_list =[] 
    for pic in pic_list : 
        pic_link = pic.get('href')
        pic_extension = GetFileExtension(pic_link)
        extension_list = ["jpg", "jpeg", "png", "gif"]
        if pic_extension not in extension_list :
            continue
        pic_url_list.append(pic_link)
    return pic_url_list

class PushThread(threading.Thread):
    def __init__(self, url_queue,content_queue,size,lock) :
        threading.Thread.__init__(self)
        self.url_queue = url_queue
        self.content_queue = content_queue
        self.size = size
        self.lock = lock
    def run(self):
        while self.url_queue.qsize() > 0 :

            url = self.url_queue.get()
            content_url = requests.get(url).text

            self.lock.acquire()
            print(str(self.size - self.url_queue.qsize())+"/"+str(self.size))
            self.lock.release()

            content_html = BeautifulSoup(content_url,'html.parser')
            push_list = content_html.find_all(class_="push")
            for part in push_list :
                if part.find(class_="f3 hl push-userid")==None :
                    continue
                userid = part.find(class_="f3 hl push-userid").string
                tag = part.find(class_="hl push-tag")
                if tag == None :
                    tag = part.find(class_="f1 hl push-tag")
                tag = tag.string.strip()
                if tag == "→" :
                    continue
                else :
                    self.content_queue.put([userid,tag])


def GetPushFormUrlList_thread(url_list) :
    url_queue = queue.Queue()
    content_queue = queue.Queue()
    lock = threading.Lock()
    for url in url_list :
        url_queue.put(url)

    size = url_queue.qsize()
    threads = []
    for _ in range(20) :
        urlthread = PushThread(url_queue,content_queue,size,lock)
        urlthread.start()
        threads.append(urlthread)
    for thread in threads :
        thread.join()

    results = []
    size = content_queue.qsize()
    for _ in range(size):
        results.append(content_queue.get())
    return results

class KeyWordThread(threading.Thread):
    def __init__(self, url_queue,content_queue,size,lock,keyword) :
        threading.Thread.__init__(self)
        self.url_queue = url_queue
        self.content_queue = content_queue
        self.size = size
        self.lock = lock
        self.keyword = keyword
    def run(self):
        while self.url_queue.qsize() > 0 :

            item = self.url_queue.get()
            index = item[0]
            url = item[1]
            content_url = requests.get(url).text

            self.lock.acquire()
            print(str(self.size - self.url_queue.qsize())+"/"+str(self.size))
            self.lock.release()

            content_html = BeautifulSoup(content_url,'html.parser')
            words = content_html.find(id="main-content").text.split("※")[0]
            if words.find(self.keyword) != -1 :
                #print("exixt!!!")
                content_url = requests.get(url).text
                pic_url_list = GetPicUrl(content_url)
                for pic_url_index in range(len(pic_url_list)) :
                    pic_url = pic_url_list[pic_url_index]
                    self.content_queue.put([index,pic_url_index,pic_url])

def GetKeyWordFormUrlList_thread(url_list,keyword) :
    url_queue = queue.Queue()
    content_queue = queue.Queue()
    lock = threading.Lock()

    for index in range(len(url_list)) :
        url = url_list[index]
        url_queue.put([index,url])

    size = url_queue.qsize()
    threads = []
    for _ in range(20) :
        urlthread = KeyWordThread(url_queue,content_queue,size,lock,keyword)
        urlthread.start()
        threads.append(urlthread)
    for thread in threads :
        thread.join()

    results = []
    size = content_queue.qsize()
    for _ in range(size):
        results.append(content_queue.get())
    results.sort()
    return results
    



