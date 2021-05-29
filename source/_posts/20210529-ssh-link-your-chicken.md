---
title: 用 ssh 连接你的设备
urlname: ssh-link-your-chicken
date: 2021-05-29 17:10:02
tags:
- 配置
- 工具
categories:
- 技术
top:
---

<span style="color:rgba(0,0,0,0.4);"><del>感谢某位老哥的辛勤付出，现在 oi 圈 ssh 这个名字 dssq 可能性微存。</del></span>

ssh 主要用于远程登录，登录了目标远程主机后就可以用 ssh 来进行命令处理了。

<!-- more -->

## 基本用法

只需要一条命令，就可以以用户名`user`，登录目标主机`host`：

```bash
ssh user@host
```

默认端口是 22，但我买的灵车 vps 给的 ssh 端口是 66103。此时就需要用`-p`参数来修改端口了。

```bash
ssh -p 66103 user@host
```

## 安全隐患

第一次登录目标主机时，会提示「无法确认目标主机真实性」，并附注其公钥指纹（的 md5 值）。用户自行核对后~~（当然实际上我们一般显然不会这么做）~~，就可以输入密码，正确则登录成功。

远程主机的公钥在第一次登录后会被保存在`~/.ssh/known_hosts`中，下次登录时就不会再警告目标真实性问题。

## 公钥登录

密码登录太烦人，可以使用公钥登录来免去输入密码。

原理大概是用户把公钥放在目标主机上，登录时主机发给用户一个随机字符串，用用户的私钥加密后返回再用主机拥有的公钥解密，成功则认为用户可信，允许登录。

按理来说直接用下面的命令可以将公钥传到远程主机上：

```bash
ssh-copy-id user@host
```

但是我的小鸡它不行，于是检查远程主机的`/etc/ssh/sshd_config`，把下面几行前的注释（`#`）删掉：
```plain
　　RSAAuthentication yes
　　PubkeyAuthentication yes
　　AuthorizedKeysFile .ssh/authorized_keys
```
然后重启远程主机的 ssh，ubuntu 用`service ssh restart`，debian 用`/etc/init.d/ssh restart`。
再尝试`ssh-copy-id`，怎么还是不行。

怒了，直接下面一条：

```bash
ssh user@host 'mkdir -p .ssh && cat >> .ssh/authorized_keys' < ~/.ssh/id_rsa.pub
```

直接写入远程主机`~/.ssh/authorized_keys`，终于现在可以使用公钥登录了。