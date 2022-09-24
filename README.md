# 原神树脂与天气实况桌面信息屏幕(esp32c3)
基于合宙esp32c3与TFT屏幕的原神与天气桌面信息牌
# 1.项目说明
1.此项目为二次开源，部分代码来源于原作者，在此感谢原作者对二次开源的支持；

2.原作者giehub仓库地址:https://github.com/SuInk/resin-esp32c3
博客地址:https://suink.cn/post/resin-esp32c3/#more

# 2.效果演示
![enter image description here](https://s1.ax1x.com/2022/09/24/xA09C8.jpg)

# 3.材料准备
-   合宙esp32c3开发板
- 240*320的TFT屏幕（像素很重要，不一样的像素需要自己重写UI）
- 杜邦线（至少8跟母对母）
- 电焊笔
# 4.安装步骤
## 4.1.arduino 环境搭建
1.安装arduino IDE
* 点击进入[Arduino官网](https://www.arduino.cc/en/software)；下滑找到1.8.x版本的下载链接（不推荐使用2.0版本）
![enter image description here](https://s1.ax1x.com/2022/09/24/xABolD.png)
![enter image description here](https://s1.ax1x.com/2022/09/24/xADp6g.png)

2.IDE配置
* 打开Arduino IDE，点击文件 >>首选项，在附加开发板管理器网址填入`https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json`
![enter image description here](https://s1.ax1x.com/2022/09/24/xADF7n.png)

* 点击工具>>开发板管理，进入后搜索esp32，搜到开发板定义并安装（此步骤可能需要科学上网，不懂可以看看别人的环境搭建教程）
* 点击项目>>加载库>>管理库，下载头文件所缺少的依赖库
* 点击工具，配置开发板相关参数（端口不一样）![enter image description here](https://s1.ax1x.com/2022/09/24/xAD8tx.png)
## 4.2
