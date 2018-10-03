import requests
import time
import tool
from bs4 import BeautifulSoup

def main():
    t_start = time.time()

    not_exist_list = ["https://www.ptt.cc/bbs/Beauty/M.1490936972.A.60D.html","https://www.ptt.cc/bbs/Beauty/M.1494776135.A.50A.html","https://www.ptt.cc/bbs/Beauty/M.1503194519.A.F4C.html","https://www.ptt.cc/bbs/Beauty/M.1504936945.A.313.html","https://www.ptt.cc/bbs/Beauty/M.1505973115.A.732.html","https://www.ptt.cc/bbs/Beauty/M.1507620395.A.27E.html","https://www.ptt.cc/bbs/Beauty/M.1510829546.A.D83.html","https://www.ptt.cc/bbs/Beauty/M.1512141143.A.D31.html"]

    file_ar = tool.OpenWriteFile("all_articles.txt")
    file_po = tool.OpenWriteFile("all_popular.txt")
    #1992-2340(include)
    for ptt_index in range(1992,2341):
        print ( str(ptt_index-1991) + "/" +str(2341-1992+1) )
        url = "https://www.ptt.cc/bbs/Beauty/index"+ str(ptt_index) +".html"
        content = requests.get(url).text
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

            url = "https://www.ptt.cc" + part.find('a').get('href')

            if url in not_exist_list :
                continue

            file_ar.write(date + ',' + title + ',' + url + '\n')

            if part.find(class_="nrec").string == "爆" :
                file_po.write(date + ',' + title + ',' + url + '\n')
        
        time.sleep(0.5)

    file_ar.close()
    file_po.close()

    t_end = time.time()
    print ("It cost %f sec" % (t_end - t_start))




