## 一款迷你型的植物浇水系统，包括代码、电路板原理图PCB图和适用3D打印的外壳
![](https://github.com/jie326513988/Arduino-Water-the-plants/blob/master/picture/v9.2-001.png)
![](https://github.com/jie326513988/Arduino-Water-the-plants/blob/master/picture/v9.1%2B%20(6).jpg)
#### 注意！所有版本PCB未加到电池低压保护，电池过放会导致电池寿命下降，请使用带保护板的电池，不要让电池电压低于3.0V。
#### 2021-1-2 LDO版本已验证成功，适合3D打印的外壳正在绘制，迟点上传

## 特点
* 根据自己设定的土壤湿度阈值进行浇水，而不是简单定时浇水，避免不必要的浇水。
* 无浇水任务时自动进入休眠以节省电量，休眠电流0.6ma，满足数周充一次电（小型花盆），配合太阳能充电器可实现长期无人监管。
* MX612E电机驱动芯片，支持pwm调节水泵速度，带过热断电保护。
* 主控PCB+电容式土壤湿度检测PCB的组合结构，主控集成土湿驱动电路，传感器可飞线，也可将从侧面焊接到主控板上，实现更高的集成度。
* 观看视频 https://www.bilibili.com/video/BV1UV411b7vG

## 使用说明（必看）
* 按键1：确认/唤醒，按键2：上/加/左，按键3：下/减/右，按下下键不放再按下确认键即可软重启系统
* 主界面可切换 AUTO/OFF 模式，AUTO模式手动浇水不可用、可自动休眠和唤醒，OFF模式可手动浇水、不会自动休眠和唤醒
* 主控板没有电池低压保护电路，电池过放会导致电池寿命下降，请使用带保护板的电池，不要让电池电压低于3.0V
* 初次使用建议校准参考电压，这样电量显示比较准。测量屏幕的供电电压就是参考电压
* 使用低放电倍率的电池可能会导致水泵启动时触发电池保护板的过流保护而断电，打开水泵平滑启动功能可改善这一现象，原因是有些水泵的启动电流太高导致触发保护，平滑启动可降低水泵的启动电流
* 使用6V蠕动泵，管径越大出水量越多，建议5mm的外径，3mm内径的硅胶管
* 出水口不能离土壤湿度传感器太远，太远湿度传递慢会导致花盆满水溢出
* 浇水速度不能太快，调节PWM MAX即可调节浇水速度，太快会导致只有传感器附近的土是湿的
* 想追求完美的浇水效果的话，需要将土壤湿度传感器水平埋在花盆底部，做好传感器的防水措施就行
* 使用4个M2x12螺丝固定面壳
## 菜单设置说明
* 土壤湿度菜单：设置浇水的上限和下限，启动浇水的延迟时间，停止浇水的延迟时间
* 水泵菜单：设置水泵超时时间（湿度增长率小于4%且持续设定的时间则自动停机休眠），设置水泵是否需要平滑启动，平滑启动的初始PWM值和PWM增量值
* 休眠&亮度菜单：设置休眠时间、亮屏时间、OLED亮度
* 电压&频率菜单：设置电池低压报警值、参考电压值、PWM频率、PWM最大值
* 土湿校准菜单：设置校准土壤湿度的范围，建议暴露在空气中校准为10%，浸泡在水中为95%
* 恢复出厂设置：重置所有参数

## 硬件配置
* 使用500mah，3.7V锂电池供电，休眠耗电0.6ma
* 外接OLED II2模块
* 带充电、升压、LDO电路
* 电机驱动电路
* 带电容式土壤湿度传感器驱动电路   

## 烧录说明
1.将我提供的库文件放好位置<br>
2.方法a：使用空芯片，再用AVR ICSP在线烧录器或离线烧录器烧录引导或带引导的固件，之后使用CH340串口模块即可更新程序<br>
  方法b：买一块UNO，上传好程序，将芯片拆下来再焊到浇水板子上<br>
  方法c：使用Arduino uno作为icsp烧录器对空芯片进行烧录引导，再通过CH340上传程序<br>
3.使用ch340模块更新程序，需要手动复位，打开Arduino IDE，文件-首选项-显示详情输出-上传勾选，点击主界面上传按钮，看到下方出现Overriding Baud Rate: 115200，迅速按下复位按钮即可更新程序<br><br>
![](https://github.com/jie326513988/Arduino-Water-the-plants/blob/master/picture/PCB(1).png)
![](https://github.com/jie326513988/Arduino-Water-the-plants/blob/master/picture/PCB(2).png)
![](https://github.com/jie326513988/Arduino-Water-the-plants/blob/master/picture/v9.1%2B%20(1).jpg)
![](https://github.com/jie326513988/Arduino-Water-the-plants/blob/master/picture/v9.1%2B%20(2).jpg)
![](https://github.com/jie326513988/Arduino-Water-the-plants/blob/master/picture/v9.1%2B%20(3).jpg)
![](https://github.com/jie326513988/Arduino-Water-the-plants/blob/master/picture/v9.1%2B%20(4).jpg)
![](https://github.com/jie326513988/Arduino-Water-the-plants/blob/master/picture/v9.1%2B%20(5).jpg)
![](https://github.com/jie326513988/Arduino-Water-the-plants/blob/master/picture/UI_SET.jpg)
