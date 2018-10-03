import requests
import time
import tool

def main(start_date,end_date):
    t_start = time.time()

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
        pic_url_list = tool.GetPicUrl(content_url)
        for pic_url in pic_url_list :
            output_file.write(pic_url + "\n")


    output_file.close()
    t_end = time.time()
    print ("It cost %f sec" % (t_end - t_start))
