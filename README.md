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
- 240*320的TFT屏幕，可到[https://m.tb.cn/h.UcuMUz7?tk=b3n52x5cSHe](https://m.tb.cn/h.UcuMUz7?tk=b3n52x5cSHe)购买（像素很重要，不一样的像素需要自己重写UI）
- 杜邦线（至少8根母对母）
- 电焊笔
# 4.安装步骤
## 4.1.arduino 环境搭建
**1.安装arduino IDE**
* 点击进入[Arduino官网](https://www.arduino.cc/en/software)；下滑找到1.8.x版本的下载链接（不推荐使用2.0版本）
![enter image description here](https://s1.ax1x.com/2022/09/24/xABolD.png)
![enter image description here](https://s1.ax1x.com/2022/09/24/xADp6g.png)

**2.IDE配置**
* 打开Arduino IDE，点击文件 >>首选项，在附加开发板管理器网址填入`https://github.com/espressif/arduino-esp32/releases/download/2.0.2/package_esp32_index.json`
![enter image description here](https://s1.ax1x.com/2022/09/24/xADF7n.png)

* 点击工具>>开发板管理，进入后搜索esp32，搜到开发板定义并安装（此步骤可能需要科学上网，不懂可以看看别人的环境搭建教程）
* 点击项目>>加载库>>管理库，下载头文件所缺少的依赖库
* 点击工具，配置开发板相关参数（端口视自己电脑端口分配而定）![enter image description here](https://s1.ax1x.com/2022/09/24/xAD8tx.png)
## 4.2屏幕驱动库TFT_eSPI的配置
**1.基本配置**
* 打开库的文件夹找到User_Setup_Select.h并打开；
![enter image description here](https://s1.ax1x.com/2022/09/24/xArd2T.png)
下滑找到屏幕驱动IC并取消注释（根据自己屏幕驱动选择，上方购买链接的屏幕驱动为ST7789），然后保存关闭
![enter image description here](https://s1.ax1x.com/2022/09/24/xAr2Px.png)
* 打开库文件夹>>User_Setups>>Setup24_ST7789.h，配置如图
![enter image description here](https://s1.ax1x.com/2022/09/24/xArvQS.png)
![enter image description here](https://s1.ax1x.com/2022/09/24/xAsiiq.png)
保存关闭

**2.字库配置**
* 我已将需要显示的文字配置到Font.h字库，直接引用即可
* 对于有添加文字的需求或者对自定义字库有兴趣的推荐看B站up主[DIY攻城狮](https://space.bilibili.com/393307558)的[使用Arduino TFT_eSPI库自定义中文字库 - 哔哩哔哩 (bilibili.com)](https://www.bilibili.com/read/cv14663437?spm_id_from=333.999.0.0)这一篇专栏教程
## 4.3 程序代码的配置
**1.将main.cpp的代码复制到arduino，配置相关参数**
![enter image description here](https://s1.ax1x.com/2022/09/24/xAyOKS.png)
**2.上述部分参数获取方法**
* 原神米游社的cookie
打开米游社官网并登录![enter image description here](https://s1.ax1x.com/2022/09/24/xA6uP1.jpg)
按F12找到控制台，在控制台下方输入document.cookie，回车
![enter image description here](https://s1.ax1x.com/2022/09/24/xAcT6f.png)
* 高德天气key

百度搜索高德开放平台，登录并认证为个人开发者；

点击右上角控制台>>应用管理>>我的应用>>创建新应用，名称随便，类型选择天气，点击创建；

服务平台选择web服务![enter image description here](https://s1.ax1x.com/2022/09/24/xAc0YR.png)

创建完成后可在控制台查看应用key，将其复制到代码里
![enter image description here](https://s1.ax1x.com/2022/09/24/xAc4fI.png)

## 4.4项目文件夹的配置
* 配置完代码之点击另存为，找到自己喜欢的路径点击保存
* 将字库与图片库放入刚刚生成的文件夹
![enter image description here](https://s1.ax1x.com/2022/09/24/xAgdu8.png)

## 4.5接线图
**1.开发板**
![enter image description here](https://s1.ax1x.com/2022/09/24/xA2ixP.jpg)


**2.TFT屏**
![enter image description here](https://s1.ax1x.com/2022/09/24/xA2EqS.jpg)

## 4.6编译上传
配置完上述步骤之后在arduino点击上传（向右箭头），上传完成即可！



**PS：开发板有2个LED灯已在代码中声明，可根据自己需要使用**
