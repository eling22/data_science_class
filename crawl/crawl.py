import requests
from bs4 import BeautifulSoup

def main():

#file = open('all_article.txt','w')

#url = "https://www.ptt.cc/bbs/Beauty/index.html"
    url = "https://www.ptt.cc/bbs/Beauty/index2650.html"
    r = requests.get(url)
    content = r.text

    soup = BeautifulSoup(content,'html.parser')
    ptt_list = soup.find_all(class_="r-ent")

    for part in ptt_list :
        author = part.find(class_="author")
        print (author.string)
        if author.string != "-" :
            date = part.find(class_="date")
            title = part.find(class_="title").find('a')
            url = part.find('a').get('href')
            author = part.find(class_="author")
            print (date.string)
            print (title.string)
            print ("https://www.ptt.cc/"+url)


#file.close()


if __name__ == '__main__':
    main() 
