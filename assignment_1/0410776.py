import sys
import crawl
import popular
import push
import key_word
def main() :
    a = 0

if __name__ == '__main__':
    command = sys.argv[1]
    if command == "crawl" :
        crawl.main()
    elif command == "push" :
        start_date = int(sys.argv[2])
        end_date = int(sys.argv[3])
        push.main(start_date,end_date)
    elif command == "popular" :
        start_date = int(sys.argv[2])
        end_date = int(sys.argv[3])
        popular.main(start_date,end_date)
    elif command == "keyword" :
        keyword = str(sys.argv[2])
        start_date = int(sys.argv[3])
        end_date = int(sys.argv[4])
        key_word.main(keyword,start_date,end_date)
    else :
        print("there is not the command: "+ command)
