PS: 'Release.zip' 是打包好的，无需配置环境可以直接运行.exe
# 目录
- [目录](#目录)
- [背景](#背景)
- [环境](#环境)
- [项目](#项目)
  - [简介](#简介)
  - [文件结构](#文件结构)
  - [展示](#展示)
    - [说明](#说明)
    - [效果](#效果)
- [TODO](#todo)



# 背景
起源是我一次操作系统实验用c++封装了一下，顺便发了一篇CSDN博客，虽然有伙伴觉得还不错，但我对这代码觉得非常糟糕，也因为自己没花太多时间：
[页面置换算法模拟设计--CSDN_Leisure_水中鱼](http://t.csdnimg.cn/n3lp7)

于是便借着操作系统实践课做了这个项目。这不是对博客中代码的重构延续，而是重新从头开发的。

第一个版本于2023/12/12完成，花费20小时(正第一次研究利用Qt创建动画，费了些功夫。小慢)，隔天又花了几个小时重构了部分代码，修复一些bug，添加选项卡等。

以后有可能会往里面陆续添加些有意思的，也可能就此结束。


# 环境
- c++ 17
- qt 6.5.2
- cmake 3.27.5
- mingw 13.1.0


# 项目
## 简介
设计了生产者-消费者模拟演示系统。生产者模拟多个进程生产指令放入指令队列中，消费者模拟处理机从指令队列中取出指令并映射为页进行处理，实现页面置换算法。利用Qt产生动画，模拟演示这一过程更加生动形象，可以用于操作系统的教学中。

## 文件结构
- main.cpp
- CMakeLists.txt
- view 负责窗口显示等
  - mianwindow.*
  - startwidget.*
  - showwidget.*
- model
  - showlabel.* 基于QLabel为了实现动画效果封装的模型
- other
  - ... ... 基于ShowLabel类：生产者、消费者、缓冲队列、页面置换算法；等的封装

## 展示
### 说明
![show image](https://github.com/supine0703/OS_QASys/blob/main/docs/images/show.png "show image")

### 效果
![show gif](https://github.com/supine0703/OS_QASys/blob/main/docs/images/show.gif "show gif")

# TODO
... ...
