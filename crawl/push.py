import os
import re
import requests
import time
from bs4 import BeautifulSoup

class UserList :
    def __init__(self) :
        self.dic_good = {}
        self.dic_bad = {}

    def add(self, userid , grade):
        if grade == '推' :
            if userid in self.dic_good :
                self.dic_good[userid] += 1
            else :
                self.dic_good[userid] = 1
        else :
            if userid in self.dic_bad :
                self.dic_bad[userid] += 1
            else :
                self.dic_bad[userid] = 1
    
    def getGoodTagId(self) :
        sort_list = sorted(self.dic_good.items(), key=lambda x: x[0])
        print("================")
        print(sort_list)
        sort_list = sorted(sort_list, key=lambda x: x[1], reverse = True)
        print("================")
        print(sort_list)
        for index in range(10) :
            key = sort_list[index]
            print("like #"+ str(index+1) +": {} {}".format(key[0], key[1]))

    def getBadTagId(self) :
        sort_list = sorted(self.dic_bad.items(), key=lambda x: x[0])
        sort_list = sorted(sort_list, key=lambda x: x[1], reverse = True)
        for index in range(10) :
            key = sort_list[index]
            print("boo #"+ str(index+1) +": {} {}".format(key[0], key[1]))


def main():

    start_date = 101
    end_date = 102

    file_ar = open(os.getcwd() +'\\crawl\\'+'all_articles.txt','r',encoding = 'utf8')
    content_ar = file_ar.read()
    file_ar.close()

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

    all_like = 0
    all_boo = 0
    user_list = UserList()
    for url_index in range(len(url_list)) :
        url = url_list[url_index]
        print(str(url_index+1) + "/" + str(len(url_list)))
        content_url = requests.get(url).text
        content_html = BeautifulSoup(content_url,'html.parser')
        push_list = content_html.find_all(class_="push")
        for part in push_list :
            tag = part.find(class_="hl push-tag")
            if tag == None :
                tag = part.find(class_="f1 hl push-tag")
            tag = tag.string.strip()
            if tag == "→" :
                continue
            elif tag == "推" :
                all_like += 1
            else :
                all_boo += 1
            userid = part.find(class_="f3 hl push-userid").string
            #print (tag)
            #print (userid)
            user_list.add(userid,tag)
        time.sleep(0.1)

    print("all like: " + str(all_like))
    print("all boo: " + str(all_boo))
    user_list.getGoodTagId()
    user_list.getBadTagId()

if __name__ == '__main__':
    main()