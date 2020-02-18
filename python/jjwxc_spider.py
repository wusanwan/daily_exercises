"""
jjwxc_spider.py

1. 爬取榜单
2. 解析列表页
3. 解析文章页 
4. 将相关信息存入表格中
"""

import requests

# import time
import csv
from pyquery import PyQuery as pq


def GetPage(url):  # 请求网页
    headers = {
        'User-Agent': 'Mozilla/5.0(Macintosh;Intel Mac OS X 10_13_3)AppleWebKit/537.36(KHTML,like Gecko)Chrome/65.0.3325.162 Safari/537.36'
    }
    response = requests.get(url, headers=headers)
    if response.status_code == requests.codes.ok:
        response.encoding = 'gbk'
        return response.text
    else:
        print("错误原因" + str(response.status_code))
        return None


def ParsePage(html):  # 解析页面
    doc = pq(html)
    tr = doc('.cytable tr')
    count = 0
    booklist = []
    # 列表页提取作者名，书名，类型，风格，字数，是否完结，发表年份，简介，标签，文章链接
    for item in tr.items():
        if count != 0:
            author = item('td:nth-child(1) a').text()
            book = item('td:nth-child(2) a').text()
            booktype = item('td:nth-child(3)').text()
            style = item('td:nth-child(4)').text()
            length = item('td:nth-child(6)').text()
            isfinish = item('td:nth-child(5)').text()
            year = item('td:nth-child(8)').text()
            year = year[0:4]

            title = item('td:nth-child(2) a').attr('title')
            index = title.find("标签：")
            brief = title[3: index - 1]
            tags = title[index+3:]
            link = item('td:nth-child(2) a').attr('href')

            # 打开提取内容页信息 主角 收藏数 文案 视角
            content = GetPage("http://www.jjwxc.net/" + link)
            # 内容页有可能被锁了
            if content is not None:
                own = pq(content)
                temp = own('head meta[name="Keywords"]').attr('content')
                index = temp.find("主角：")
                index1 = temp.find("┃")
                roles = temp[index + 3:index1]
                view = own('.rightul li:nth-child(2)').text()
                view = view[6:8]
                intro = own('.smallreadbody #novelintro').text()
                collected = own('.sptd span[itemprop="collectedCount"]').text()
                # time.sleep(1) 其实不设也没关系
                print(str(count), author, book, booktype, style, length, isfinish, year, roles, sep="  ")
                dict = {"author": author, "book": book, "booktype": booktype, "style": style, "length": length, "isfinish": isfinish, "year": year, "brief": brief, "tags": tags, "roles": roles, "collected": collected, "view": view, "intro": intro}
                booklist.append(dict)
        count += 1
    return booklist


def WriteCSV(booklist, index):   # 将数据写入csv
    headers = ["author", "book", "booktype", "style", "length", "isfinish", "year", "brief", "tags", "roles", "collected", "view", "intro"]
    with open("d://lab1data"+str(index)+".csv", "a", encoding="utf-8", newline="") as fp:
        writer = csv.DictWriter(fp, headers)
        writer.writeheader()
        writer.writerows(booklist)


def main(po, co):   # PageOffset,ChannelOffset
    url = 'http://www.jjwxc.net/bookbase.php?fw0=0&fbsj=0&ycx0=0&xx'+str(co)+'='+str(co)+'&mainview0=0&sd0=0&lx0=0&fg0=0&sortType=0&isfinish=0&collectiontypes=ors&searchkeywords=&page='+str(po)
    html = GetPage(url)
    return ParsePage(html)


if __name__ == '__main__':
    for i in range(6):  # 频道偏移量
        total = []
        for j in range(5):  # 页数偏移量
            print("开始第%d个频道的第%d页……" % (i, j+1))
            total.extend(main(j+1, i))
        WriteCSV(total, i)
