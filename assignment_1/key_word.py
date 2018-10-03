import time
import tool

def main(key_word, start_date, end_date):
    t_start = time.time()

    output_file = tool.OpenWriteFile("keyword("+key_word+")["+str(start_date)+"-"+str(end_date)+"].txt")
    content_ar = tool.ReadFile("all_articles.txt")
    url_list = tool.GetUrlFromTxt(content_ar,start_date,end_date)
    content_list = tool.GetKeyWordFormUrlList_thread(url_list,key_word)
    for content in content_list :
        output_file.write(content[2] + "\n")
    
    output_file.close()
    t_end = time.time()
    print ("It cost %f sec" % (t_end - t_start))
