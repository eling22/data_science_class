import os
import re
import requests
import time
from bs4 import BeautifulSoup
import tool


def main():
    t_start = time.time()

    start_date = 101
    end_date = 201

    output_file = tool.OpenWriteFile("popular["+str(start_date)+"-"+str(end_date)+"].txt")
    content_po = tool.ReadFile("all_popular.txt")

    url_list = tool.GetUrlFromTxt(content_po,start_date,end_date)
    #print("number of popular articles: "+str(len(url_list)))
    output_file.write("number of popular articles: "+str(len(url_list))+"\n")

    for url_index in range(len(url_list)) :
        url = url_list[url_index]
        print(str(url_index+1) + "/" + str(len(url_list)))

        time.sleep(0.5)
        content_url = requests.get(url).text
        content_html = BeautifulSoup(content_url,'html.parser')
        pic_list = content_html.find_all(target = "_blank")
        for pic in pic_list : 
            pic_link = pic.get('href')
            pic_extension = tool.GetFileExtension(pic_link)
            extension_list = ["jpg", "jpeg", "png", "gif"]
            if pic_extension not in extension_list :
                continue
            #print(pic_link)
            output_file.write(pic_link+"\n")

    output_file.close()
    t_end = time.time()
    print ("It cost %f sec" % (t_end - t_start))


if __name__ == '__main__':
    main()