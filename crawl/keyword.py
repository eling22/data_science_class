import os
import re
import requests
import time
from bs4 import BeautifulSoup
import tool


def main():
    t_start = time.time()

    keyword = "正妹"
    start_date = 101
    end_date = 102

    #output_file = tool.OpenWriteFile("keyword("+keyword+")["+str(start_date)+"-"+str(end_date)+"].txt")
    content_ar = tool.ReadFile("all_articles.txt")

    url_list = tool.GetUrlFromTxt(content_ar,start_date,end_date)
    #for url_index in range(len(url_list)) :
    url_index = 0
    url = url_list[url_index]
    #print(str(url_index+1) + "/" + str(len(url_list)))
    time.sleep(0.5)
    content_url = requests.get(url).text
    content_html = BeautifulSoup(content_url,'html.parser')
    words = content_html.find(id="main-content").text.split("※")[0]
    print(words)
    if words.find(keyword) != -1 :
        print("exixt!!!")

    
    t_end = time.time()
    print ("It cost %f sec" % (t_end - t_start))



if __name__ == '__main__':
    main()