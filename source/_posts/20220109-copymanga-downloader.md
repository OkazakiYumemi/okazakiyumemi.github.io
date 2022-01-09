---
title: 1.5h 胡的拷贝漫画下载器
urlname: 20220109
date: 2022-01-09 13:46:12
tags: 脚本
categories: 随笔
top:
---

这个网站算是比较良心的，app 内支持下载到本地。结果我打开下载出来的东西，发现它似乎是把一话中的每张图都拼在一起，然后把它们拼成一个文件啥的。加上它限制免费用户一天只能下载 50 话，就不走它的 app 下载了。

然后发现它的网页端也比较良心，于是就从网页端抠图了。为了尽量不给服务器带来压力，下载图片之间都至少间隔 0.5s。~~你看，我是不是也良心。~~

<!-- more -->

因为是我自用的，很粗糙，使用方法是将需要下载的漫画的首页另存为`c.html`，然后运行下面的脚本。

读者想自己跑的话，需要另装的包有`selenium`、`lxml`。还要去下一个 [Chrome Driver](https://chromedriver.chromium.org/downloads)，自己改一改`chrome_path`。哦对还要用`aria2c`（感觉不清真啊）下载图片。

（啊不过这个博客没有读者吧）

```py
from selenium import webdriver
from lxml import etree
import time
import os
import sys

# for https://www.copymanga.com/ (20220108)

def GetImgList(url):
    chrome_path = r'D:\MyScripts\chromedriver.exe'
    chrome_options = webdriver.ChromeOptions()
    # chrome_options.add_argument('blink-settings=imagesEnabled=false')
    # chrome_options.add_experimental_option('prefs', {'profile.managed_default_content_settings.images': 2})
    chrome_options.add_argument('--headless')
    chrome_options.add_experimental_option('excludeSwitches', ['enable-automation'])
    driver = webdriver.Chrome(executable_path = chrome_path, options = chrome_options)

    for retry_time in range(5):
        try:
            driver.get(url)
            break
        except:
            print("Cannot reach the server. Retry [%d]" % retry_time)
            time.sleep(0.8)
    time.sleep(2)
    img_cnt = int(driver.find_element_by_xpath("/html/body/div[1]/span[2]").text)
    print("pages: %d" % img_cnt)

    scroll_height = 0
    while True:
        try:
            driver.find_element_by_xpath("/html/body/div[2]/div/ul/li[%d]" % img_cnt)
            break
        except:
            scroll_height += 400
            driver.execute_script("window.scrollTo(0, %d);" % scroll_height)
            time.sleep(0.05)
    # driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")

    img_list = driver.find_elements_by_xpath("/html/body/div[2]/div/ul/li/img")
    # print(len(img_list))
    img_urls = []

    for img in img_list:
        img_url = img.get_attribute('data-src')
        img_urls.append(img_url)
        # print(img_url)

    # driver.close()
    driver.quit()
    return img_urls

def DownloadChapter(chapter_url, chapter_name):
    print("Downloading Chapter: %s" % chapter_name)
    img_urls = GetImgList(chapter_url)

    print("Get %d images" % len(img_urls))
    if not os.path.exists(chapter_name):
        os.makedirs(chapter_name)
    with(open("./%s/img_list.log" % chapter_name, "w", encoding="utf8")) as f:
        for img_url in img_urls: f.write(img_url + "\n")
    
    img_id = 0
    for img_url in img_urls:
        img_name = str(img_id).zfill(3) + '.jpg'
        os.system("aria2c -x 16 -s 16 -k 1M --console-log-level=warn -d \"%s\" -o \"%s\" %s" % (chapter_name, img_name, img_url))
        img_id += 1
        time.sleep(0.5)
    print('Success!')
    time.sleep(1)

chapter_html = etree.parse('c.html', etree.HTMLParser())
chapters = chapter_html.xpath('/html/body/main/div[2]/div[3]/div/div[2]/div/div[1]/ul[1]/a')
for ch in chapters:
    url = ch.xpath('./@href')[0]
    name = ch.xpath('./@title')[0]
    DownloadChapter(url, name)
```

吐槽：断点续传没有实现，并且没有把已经下载的图片跳过。但我是懒狗，所以不管了。
实在不行可以单话重下嘛。而且我给每一话弄了一个`img_list.log`，出意外的话可以用重下。

看啊，多方便～（完全不是）