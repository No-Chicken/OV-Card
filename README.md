# OV-Card

万能卡片，使用STM32L0和RC522设计的IC卡复制器，可以充当卡包。

硬件开源链接：
https://oshwhub.com/no_chicken/mo-neng-ka-pian-OV-Card

视频演示链接：
https://space.bilibili.com/34154740



## 系统框图

系统的硬件总体框图如下图所示

<div align=center>
<img src=".\images\系统框图.png" alt="系统框图" style="zoom:100%;" />
</div>

主控: STM32L051K8U6，供电: TPS63020，显示：1.54寸200*200墨水屏。如果不想买墨水屏，或者想要用超级久，可以使用超低功耗版本，只用LED进行显示。关于卡片信息存储，全部存储在MCU内部的EEPROM里。



## 使用说明

**！！！！！注意：请在上电前就切换好模式，请勿在通电时滑动开关切换模式！！！！！！**

 有3个用户可操作的硬件部分：

1.通过按键按下上电，松开即断电。

2.通过拨轮开关，上电后上下滑动，可以切换选择的位置（即切换卡片）。

3.通过滑动开关切换模式。（通电时，请勿切换模式）

切换模式说明：

①滑到左边是复制模式，上电后可以在所选的存储位置，复制外部卡片，例如已选中门禁卡，只需将外部卡片靠近背面，即可将卡信息写入所选存储位置。

②滑到右边是卡模拟模式，上电后，即将信息写入了IC卡中。只要保持滑动开关在右边，断电后仍然可以刷卡，即被外部读卡器读。

<div align=center>
<img src=".\images\操作图.jpg" alt="操作图" style="zoom:100%;" />
</div>

## 原理说明

功能框图，IC卡信息分区图如下所示。NFC模块通过RC522读取外部IC卡的卡号，即0扇区0区块的数据，然后存储至STM32L051内部的EEPROM中。滑动开关控制内部UID卡的线圈的开合，即加入了使能功能，通过NFC模块，将存储在EEPROM的卡号信息写入UID卡中，即实现了卡片复制功能。

<div align=center>
<img src=".\images\功能框图.png" alt="功能框图" style="zoom:100%;" />
</div>

<div align=center>
<img src=".\images\IC卡扇区.png" alt="IC卡扇区" style="zoom:100%;" />
</div>
