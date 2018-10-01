import os

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
    file_ = open(os.getcwd() +'\\crawl\\'+ filename,'r',encoding = 'utf8')
    content = file_.read()
    file_.close()
    return content

def OpenWriteFile(filename) :
    return open(os.getcwd() +'\\crawl\\'+ filename,'w',encoding = 'utf8')