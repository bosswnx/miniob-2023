# MiniOB 概述

MiniOB 是 [OceanBase](https://github.com/oceanbase/oceanbase) 团队基于华中科技大学数据库课程原型，联合多所高校重新开发的、专为零基础的同学设计的数据库入门学习项目。我们的目标是为在校学生、数据库从业者、爱好者或对基础技术感兴趣的人提供一个友好的数据库学习项目。

MiniOB 整体代码简洁，容易上手，设计了一系列由浅入深的题目，帮助同学们从零基础入门，迅速了解数据库并深入学习数据库内核。MiniOB 简化了许多模块，例如不考虑并发操作、安全特性和复杂的事务管理等功能，以便更好地学习数据库实现原理。我们期望通过 MiniOB 的训练，同学们能够熟练掌握数据库内核模块的功能和协同关系，并具备一定的工程编码能力，例如内存管理、网络通信和磁盘 I/O 处理等, 这将有助于同学在未来的面试和工作中脱颖而出。

# [文档](https://oceanbase.github.io/miniob/)
代码配套文档和相关代码注释已经生成文档，并通过 GitHub Pages 发布。您可以直接访问：[MiniOB GitHub Pages](https://oceanbase.github.io/miniob/).

## 快速上手

为了帮助开发者更好地上手并学习 MiniOB，建议阅读以下内容：

1. [MiniOB 框架介绍](https://oceanbase.github.io/miniob/miniob-introduction.html)
2. [如何编译 MiniOB 源码](https://oceanbase.github.io/miniob/how_to_build.html)
3. [如何运行 MiniOB](https://oceanbase.github.io/miniob/how_to_run.html)
4. [使用 GitPod 开发 MiniOB](https://oceanbase.github.io/miniob/dev-env/dev_by_gitpod.html)
5. [doxygen 代码文档](https://oceanbase.github.io/miniob/design/doxy/html/index.html)

为了帮助大家更好地学习数据库基础知识，OceanBase社区提供了一系列教程。更多文档请参考 [MiniOB GitHub Pages](https://oceanbase.github.io/miniob/)。建议学习：

1. [《从0到1数据库内核实战教程》  视频教程](https://open.oceanbase.com/activities/4921877?id=4921946)
2. [《从0到1数据库内核实战教程》  基础讲义](https://github.com/oceanbase/kernel-quickstart)
3. [《数据库管理系统实现》  华中科技大学实现教材](https://oceanbase.github.io/miniob/lectures/index.html)

## 系统架构

MiniOB 整体架构如下图所示:
![架构](docs/src/images/miniob-introduction-sql-flow.png)

其中:

- 网络模块：负责与客户端交互，收发客户端请求与应答；

- SQL解析：将用户输入的SQL语句解析成语法树；

- 执行计划缓存：执行计划缓存模块会将该 SQL第一次生成的执行计划缓存在内存中，后续的执行可以反复执行这个计划，避免了重复查询优化的过程（未实现）。

- 语义解析模块：将生成的语法树，转换成数据库内部数据结构（部分实现）；

- 查询缓存：将执行的查询结果缓存在内存中，下次查询时，可以直接返回（未实现）；

- 查询优化：根据一定规则和统计数据，调整/重写语法树。(部分实现)；

- 计划执行：根据语法树描述，执行并生成结果；

- 会话管理：管理用户连接、调整某个连接的参数；

- 元数据管理：记录当前的数据库、表、字段和索引元数据信息；

- 客户端：作为测试工具，接收用户请求，向服务端发起请求。


# [OceanBase 大赛](https://open.oceanbase.com/competition)

2022 OceanBase 数据库大赛是由中国计算机学会（CCF）数据库专业委员会指导，OceanBase 与蚂蚁技术研究院学术合作团队联合举办的数据库内核实战赛事。本次大赛主要面向全国爱好数据库的高校学生，以“竞技、交流、成长”为宗旨，搭建基于赛事的技术交流平台，促进高校创新人才培养机制，不仅帮助学生从0开始系统化学习数据库理论知识，提升学生数据库实践能力，更能帮助学生走向企业积累经验，促进国内数据库人才的发展，碰撞出创新的火花。

OceanBase 初赛基于一套适合初学者实践的数据库实训平台 MiniOB，代码量少，易于上手学习，包含了数据库的各个关键模块，是一个系统性的数据库学习平台。基于该平台设置了一系列由浅入深的题目，以帮助同学们更好"零"基础入门。

2023 OceanBase 数据库大赛正在火热进行, 更多详情, 请参考 [OceanBase 大赛](https://open.oceanbase.com/competition/index)。

### 1. 大赛手把手入门教程

[大赛入门教程](https://oceanbase.github.io/miniob/game/gitee-instructions.html)

### 2. 大赛赛题

[赛题介绍](https://oceanbase.github.io/miniob/game/miniob_topics.html) 

### 3. 提交测试

题目完成并通过自测后，大家可以在 [MiniOB 训练营](https://open.oceanbase.com/train?questionId=500003) 上提交代码进行测试。

在提交前, 请参考并学习 [训练营使用说明](https://ask.oceanbase.com/t/topic/35600372)。

### 4. 大赛FAQ

[大赛 FAQ ](https://ask.oceanbase.com/t/topic/35601465)

# 在线开发平台

本仓库基于 Gitpod 建立了快速在线开发平台。点击下面的按钮即可一键体验（建议使用 Chrome 浏览器）。

[![Open in Gitpod](https://gitpod.io/button/open-in-gitpod.svg)](https://gitpod.io/#https://github.com/oceanbase/miniob)

首次进入 Gitpod 时需要安装一些依赖。安装完成后，终端界面会显示 "Dependency installed successfully"。运行 `bash build.sh --make -j4` 命令即可编译 MiniOB。

# Contributing

OceanBase 社区热情欢迎每一位对数据库技术热爱的开发者，期待与您携手开启思维碰撞之旅。无论是文档格式调整或文字修正、问题修复还是增加新功能，都是参与和贡献 OceanBase 社区的方式之一。现在就开始您的首次贡献吧！更多详情，请参考 [社区贡献](CONTRIBUTING.md).

# License

MiniOB 采用 [木兰宽松许可证，第2版](https://license.coscl.org.cn/MulanPSL2), 可以自由拷贝和使用源码, 当做修改或分发时, 请遵守 [木兰宽松许可证，第2版](https://license.coscl.org.cn/MulanPSL2). 

# 社区组织

- [OceanBase 社区交流群 33254054](https://h5.dingtalk.com/circle/healthCheckin.html?corpId=dingd88359ef5e4c49ef87cda005313eea7a&1fe0ca69-72d=16c86a07-83c&cbdbhh=qwertyuiop&origin=1)
- [OceanBase 大赛官方交流群 35326455](https://qr.dingtalk.com/action/joingroup?code=v1,k1,g61jI0RwHQA8UMocuTbys2cyM7vck2c6jNE87vdxz9o=&_dt_no_comment=1&origin=11)
- [OceanBase 官方论坛](https://ask.oceanbase.com/)

# 开发流程

这里针对OB大赛做一个简单的流程建议。

## 分支

在仓库创建出来的时候，就会有一个默认分支，一般是 `main`。大赛后台测试，每次会拉取默认分支进行编译测试。

因此，建议将 `main` 作为“稳定”分支，就是所有的修改，都测试完成后，再 merge 到 `main`。

后续，所有的默认分支都称为 `main`。

如果是团队开发，建议创建一个 `dev` 分支，作为团队共同开发的分支，大家修改的新功能，先 merge 到 `dev`，在 `dev` 测试通过后，再 merge 到 `main`。

对于团队中的成员，可以开启一个自己的分支，或者以功能名称命名的分支，比如 `feature/update`。

## 流程

### 对于团队

1. 创建主分支 `main`

1. 创建开发分支 `dev`

团队准备提交代码到主分支时，执行：

```bash
# 切换到开发分支
git checkout develop
# 拉取最新代码
git pull
# 执行一系列测试，没有问题的时候
# 切换到主分支，并更新到最新
git checkout main
git pull
# 合并开发分支
git merge develop
# 推送到远程仓库，这时官方测试人员，就可以拉到最新的代码
git push
```
### 对于个人

1. 拉取团队仓库代码(`git clone`)
1. 切换到 `dev` 分支(`git checkout develop`)
1. 创建新分支，用于开发新的功能，比如 `git checkout -b feature/update`
1. 功能开发完成，测试通过后，提交代码到本地仓库 git add . && git commit -m 'your commit message'
1. 同步代码到开发分支develop

```sh
# 切换到develop分支
git checkout develop 

# 同步远程分支代码
git pull

# 合并 feature/update 到develop
git merge feature/update

# 合并完成后，推送到远程仓库
git push

# 然后删除自己的feature分支
git branch -d feature/update
```

## 常用命令

查看当前分支

```sh
git branch  # 查看本地分支

git branch -a # 查看所有分支，包括远程分支
```

如何创建分支

```sh
git checkout -b 'your branch name'

git branch -d 'your branch name'  # 删除一个分支
```

如何切换分支

```sh
git checkout 'branch name'
```

如何提交代码

```sh
# 添加想要提交的文件或文件夹
git add 'the files or directories you want to commit' 
# 这一步也可以用 git add . 添加当前目录

# 提交到本地仓库
# -m 中是提交代码的消息，建议写有意义的信息，方便后面查找
git commit -m 'commit message'
```
如何推送代码到远程仓库

```sh
git push
# 可以将多次提交，一次性push到远程仓库
如何合并代码
# 假设当前处于分支 develop 下
git merge feature/update
# 会将 feature/update 分支的修改，merge 到 develop 分支
```

如何临时修改另一个分支的代码

```sh
# 有时候，正在开发一个新功能时，突然来了一个紧急BUG，这时候需要切换到另一个分支去开发
# 这时可以先把当前的代码提交上去，然后切换分支。或者也可以这样：
git stash # 将当前的修改保存起来

git checkout main # 切换到主分支，或者修复BUG的分支

git checkout -b fix/xxx  # 创建一个新分支，用于修复问题

# 修改完成后，merge到main分支
# 然后，继续我们的功能开发
git checkout feature/update # 假设我们最开始就是在这个分支上
git stash pop

# stash 还有很多好玩的功能，大家可以探索一下
```

## 开发流程

1. 对于每一道题，新建一个分支 `feature/<name>` 来进行开发

1. 写完代码，能够过编译和本地测试后，合并到 dev 分支：

    1. 切换到 `dev` 分支：`git checkout dev`

    1. 同步 `dev` 分支最新代码：`git pull`

    1. 合并分支，并处理冲突：`git merge dev -m "commit message"`

1. 进行提测，若通过则使用 github 发送 pr 请求来合并到 `main` 并删除分支：`git branch -d feature/<name>`

1. 若不通过则切换回特性分支并继续开发，重复以上过程：`git checkout feature/<name>`

合并时候的 commit message 需要遵守 commit 规范：https://zhuanlan.zhihu.com/p/182553920