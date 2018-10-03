import time
import tool

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
    
    def getGoodTagId(self,output_file) :
        sort_list = sorted(self.dic_good.items(), key=lambda x: x[0])
        sort_list = sorted(sort_list, key=lambda x: x[1], reverse = True)
        for index in range(10) :
            key = sort_list[index]
            output_file.write("like #"+ str(index+1) +": {} {}".format(key[0], key[1])+ "\n")

    def getBadTagId(self,output_file) :
        sort_list = sorted(self.dic_bad.items(), key=lambda x: x[0])
        sort_list = sorted(sort_list, key=lambda x: x[1], reverse = True)
        for index in range(10) :
            key = sort_list[index]
            output_file.write("boo #"+ str(index+1) +": {} {}".format(key[0], key[1])+ "\n")


def main(start_date,end_date):
    t_start = time.time()

    all_like = 0
    all_boo = 0
    user_list = UserList() 

    output_file = tool.OpenWriteFile("push["+str(start_date)+"-"+str(end_date)+"].txt")
    content_ar = tool.ReadFile("all_articles.txt")
    url_list = tool.GetUrlFromTxt(content_ar,start_date,end_date)
    content_list = tool.GetPushFormUrlList_thread(url_list)
    for content in content_list :
        userid = content[0]
        tag = content[1]
        if tag == "推" :
            all_like += 1
        else :
            all_boo += 1
        user_list.add(userid,tag)
    
    output_file.write("all like: " + str(all_like) + "\n")
    output_file.write("all boo: " + str(all_boo) + "\n")
    user_list.getGoodTagId(output_file)
    user_list.getBadTagId(output_file)
    
    output_file.close()
    t_end = time.time()
    print ("It cost %f sec" % (t_end - t_start))