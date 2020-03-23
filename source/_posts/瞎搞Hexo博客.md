---
title: 瞎搞Hexo博客
date: 2020-03-23 02:16:49
tags: 配置
categories: 技术
top:
---

没错我又开始瞎搞我的 blog 了。

<!-- more -->

## 源码问题

看了 [yansheng836 的 README.md ](https://yansheng836.github.io/README.md)（smwy）和[某个 issue ](https://github.com/hexojs/hexo/issues/3676)，觉得自己也应该把源码一并传到 github 上。

我在 git 仓库里开了个名为 `source` 的 branch，本地也开了个仓库，现在每次更改之后就将源码推送到 `source` 。

然后每次搞完 blog 在 commit 的时候就要多写几行。

```sql
hexo cl && hexo g && hexo d
git add *
git commit -m "~"
git push origin master:source
```

不妨顺便把 `.gitignore` 也贴在这里：

```plain
.DS_Store
Thumbs.db
db.json
*.log
node_modules/
public/
.deploy*/
.gitignore
```

## 渲染问题

总有时候不想让 hexo 渲染某些 blog 里的 `html` 或者 `md` 文件。

于是可以改根目录下的 `_config.yml` 。

[查了一下](https://nettee.github.io/posts/2018/Skip-rendering-files-in-Hexo/)，发现只用改 `skip_render` 即可。

好像路径匹配用的是 glob 表达式~~，感觉和正则表达式有些相像，但规则简单得多~~。

这里就不放了，可以去[阮一峰的博客](http://www.ruanyifeng.com/blog/2018/09/bash-wildcards.html)学习。

## 隐藏博文

大概知道怎么搞了，然而咕了。

~~怎么我老是咕啊~~