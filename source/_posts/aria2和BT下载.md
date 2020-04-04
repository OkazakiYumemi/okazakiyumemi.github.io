---
title: aria2和BT下载
date: 2020-04-04 14:41:05
tags: 配置
categories: 技术
top:
---

下 gal 被屑度网盘限速了，在再次寻找对抗限速的方法时顺带搞好了 aria2 环境和 BT 下载环境。可惜没有公网 ip ，否则「下载」二字即可去掉。

~~然而最后也没有成功对抗限速。~~

## aria2

[aria2 GitHub仓库](https://github.com/aria2/aria2)
[AriaNg GitHub仓库](https://github.com/mayswind/AriaNg)
[Aria2 for Chrome](https://chrome.google.com/webstore/detail/aria2-for-chrome/mpkodccbngfoacfalldjimigbofkhgjn)

### 参考资料

[Aria2 & YAAW 使用说明](http://aria2c.com/usage.html)
[老王的自留地 | 关于aria2最完整的一篇](https://ivo-wang.github.io/2019/04/18/%E5%85%B3%E4%BA%8Earia2%E6%9C%80%E5%AE%8C%E6%95%B4%E7%9A%84%E4%B8%80%E7%AF%87/)
[悠然 | Aria2基础上手指南](https://zhuanlan.zhihu.com/p/30666881)
[andforce | aria2使用教程](https://www.jianshu.com/p/affdb6455218)

### 下载

先到 releases 去下载需要的版本。

本人 windows 64 位机下解压所得如下：

```plain
aria2
│  aria2c.exe
│  AUTHORS
│  ChangeLog
│  COPYING
│  LICENSE.OpenSSL
│  NEWS
│  README.html
│  README.mingw
```

以下所指操作位置，若未特别说明，均在 `aria2` 文件夹下。

创建 3 个空文件：`aria2.conf`, `aria2.log`, `aria2.session`，分别作为配置文件、日志文件、会话文件。

### aria2.conf

```plain
## '#'开头为注释内容, 选项都有相应的注释说明, 根据需要修改 ##
## 被注释的选项填写的是默认值, 建议在需要修改时再取消注释  ##

## 文件保存相关 ##

# 文件的保存路径(可使用绝对路径或相对路径), 默认: 当前启动位置
dir=./downloads/
# 日志的保存路径
log=./aria2.log
# 启用磁盘缓存, 0为禁用缓存, 需1.16以上版本, 默认:16M
#disk-cache=32M
# 文件预分配方式, 能有效降低磁盘碎片, 默认:prealloc
# 预分配所需时间: none < falloc ? trunc < prealloc
# falloc和trunc则需要文件系统和内核支持
# NTFS建议使用falloc, EXT3/4建议trunc, MAC 下需要注释此项
file-allocation=falloc
# 断点续传
continue=true

## 下载连接相关 ##

# 最大同时下载任务数, 运行时可修改, 默认:5
max-concurrent-downloads=4
# 同一服务器连接数, 添加时可指定, 默认:1
max-connection-per-server=8
# 最小文件分片大小, 添加时可指定, 取值范围1M -1024M, 默认:20M
# 假定size=10M, 文件为20MiB 则使用两个来源下载; 文件为15MiB 则使用一个来源下载
min-split-size=8M
# 单个任务最大线程数, 添加时可指定, 默认:5
split=32
# 整体下载速度限制, 运行时可修改, 默认:0
max-overall-download-limit=0
# 单个任务下载速度限制, 默认:0
max-download-limit=0
# 整体上传速度限制, 运行时可修改, 默认:0
max-overall-upload-limit=0
# 单个任务上传速度限制, 默认:0
max-upload-limit=0
# 禁用IPv6, 默认:false
disable-ipv6=true
# 连接超时时间, 默认:60
timeout=60
# 最大重试次数, 设置为0表示不限制重试次数, 默认:5
max-tries=0
# 设置重试等待的秒数, 默认:0
retry-wait=0

## 进度保存相关 ##

# 从会话文件中读取下载任务
input-file=./aria2.session
# 在Aria2退出时保存`错误/未完成`的下载任务到会话文件
save-session=./aria2.session
# 定时保存会话, 0为退出时才保存, 需1.16.1以上版本, 默认:0
#save-session-interval=60

## RPC相关设置 ##

# 启用RPC, 默认:false
enable-rpc=true
# 允许所有来源, 默认:false
rpc-allow-origin-all=true
# 允许非外部访问, 默认:false
rpc-listen-all=true
# 事件轮询方式, 取值:[epoll, kqueue, port, poll, select], 不同系统默认值不同
#event-poll=select
# RPC监听端口, 端口被占用时可以修改, 默认:6800
rpc-listen-port=6800
# 设置的RPC授权令牌, v1.18.4新增功能, 取代 --rpc-user 和 --rpc-passwd 选项
#rpc-secret=<TOKEN>
# 设置的RPC访问用户名, 此选项新版已废弃, 建议改用 --rpc-secret 选项
#rpc-user=<USER>
# 设置的RPC访问密码, 此选项新版已废弃, 建议改用 --rpc-secret 选项
#rpc-passwd=<PASSWD>
# 是否启用 RPC 服务的 SSL/TLS 加密,
# 启用加密后 RPC 服务需要使用 https 或者 wss 协议连接
#rpc-secure=true
# 在 RPC 服务中启用 SSL/TLS 加密时的证书文件,
# 使用 PEM 格式时，您必须通过 --rpc-private-key 指定私钥
#rpc-certificate=/path/to/certificate.pem
# 在 RPC 服务中启用 SSL/TLS 加密时的私钥文件
#rpc-private-key=/path/to/certificate.key

## BT/PT下载相关 ##

# 当下载的是一个种子(以.torrent结尾)时, 自动开始BT任务, 默认:true
#follow-torrent=true
# BT监听端口, 当端口被屏蔽时使用, 默认:6881-6999
listen-port=51413
# 单个种子最大连接数, 默认:55
#bt-max-peers=55
# 打开DHT功能, PT需要禁用, 默认:true
enable-dht=false
# 打开IPv6 DHT功能, PT需要禁用
#enable-dht6=false
# DHT网络监听端口, 默认:6881-6999
#dht-listen-port=6881-6999
# 本地节点查找, PT需要禁用, 默认:false
#bt-enable-lpd=false
# 种子交换, PT需要禁用, 默认:true
enable-peer-exchange=false
# 每个种子限速, 对少种的PT很有用, 默认:50K
#bt-request-peer-speed-limit=50K
# 客户端伪装, PT需要
peer-id-prefix=-TR2770-
user-agent=Transmission/2.77
peer-agent=Transmission/2.77
# 当种子的分享率达到这个数时, 自动停止做种, 0为一直做种, 默认:1.0
seed-ratio=0
# 强制保存会话, 即使任务已经完成, 默认:false
# 较新的版本开启后会在任务完成后依然保留.aria2文件
#force-save=false
# BT校验相关, 默认:true
#bt-hash-check-seed=true
# 继续之前的BT任务时, 无需再次校验, 默认:false
bt-seed-unverified=true
# 保存磁力链接元数据为种子文件(.torrent文件), 默认:false
bt-save-metadata=true
```

大概的设置都在这里了。

几个重要的：

- `dir`似乎必须要有，`log`建议改成你的`aria2.log`的位置，`input-file`和`save-session`建议改成`aria2.session`的位置。
- `max-concurrent-downloads`, `max-connection-per-server`, `min-split-size`, `split`根据需求自行修改。

你是否在上述过程中试图打开 `aria2.exe` ？正常情况下，应该会出现一个立即消失的命令行窗口，并且该进程立即消失。

### 开启 aria2.exe

再创建一个文件 `HideRun.vbs` ，写入以下代码：

```vb
CreateObject("WScript.Shell").Run "aria2c.exe --conf-path=aria2.conf",0
```

运行 `HideRun.vbs` ，现在应该可以找到进程 `aria2.exe` 了。

可是没有 GUI 啊，怎么下载啊？

### 控制台

可以使用 [yaaw(zh-hans)](http://aria2c.com/) 来看到自己的 aria2 状态。为了证明您的 aria2 已经正常运行，可以试着下载一个小文件。

然而界面不好看并且每次使用要打开网页太慢了（恼），所以本人换了一个 WebUI ，改成了 AriaNg 。

怕麻烦，直接在 releases 里下载了一个 all-in-one 包，解压之后打开 `index.html` 即可。

对我而言这个界面舒服多了。

### 插件

自行安装即可（感觉效果不佳啊……）。

## qBittorrent

>Q：为什么选择 qBittorrent ？
>A：大佬推荐的。

