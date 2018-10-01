import requests
import time
import os
import re
from bs4 import BeautifulSoup

def main():

    t_start = time.time()
    file_ar = open(os.getcwd() +'\\crawl\\'+'all_articles.txt','w',encoding = 'utf8')
    file_po = open(os.getcwd() +'\\crawl\\'+'all_popular.txt','w',encoding = 'utf8')

#1992-2340(include)
    for ptt_index in range(1992,2341):
        print (ptt_index)
        url = "https://www.ptt.cc/bbs/Beauty/index"+ str(ptt_index) +".html"
        r = requests.get(url)
        content = r.text
        soup = BeautifulSoup(content,'html.parser')
        ptt_list = soup.find_all(class_="r-ent")

        for part in ptt_list :
            author = part.find(class_="author")
            if author.string == "-" :
                continue

            date = part.find(class_="date").string.replace('/','').strip()
            if ptt_index == 1992 and int(date) > 1000 :
                continue
            if ptt_index == 2340 and int(date) < 1000 :
                continue

            title = part.find(class_="title").find('a')
            if title == None :
                continue
            
            if title.string == None :
                fp = title.find('span').get('data-cfemail')
                r = int(fp[:2],16)
                email = ''.join([chr(int(fp[i:i+2], 16) ^ r) for i in range(2, len(fp), 2)])
                title = title.text.rstrip("[email protected]").rstrip().rstrip("[email protected]") + email
            else :
                title = title.string
                if title[1:3] == "公告" :
                    continue

            url = part.find('a').get('href')
            url = "https://www.ptt.cc/"+url
            #print (date)
            #print (title)
            #print (url)
            file_ar.write(date + ',' + title + ',' + url + '\n')

            if part.find(class_="nrec").string == "爆" :
                file_po.write(date + ',' + title + ',' + url + '\n')
        
        time.sleep(0.5)

    file_ar.close()
    file_po.close()
    t_end = time.time()

    print ("It cost %f sec" % (t_end - t_start))

if __name__ == '__main__':
    main()



