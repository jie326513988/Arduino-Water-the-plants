/*更新说明 C适合V5版本PCB D适合V7版本PCB
   v1.1.2
   a.美化设置界面ui
   土壤传感器  开
   水泵接口    R+L-
   b.美化按调节亮度的ui(主界面长按加减键)
   c.土壤湿度传感器异常算法
   v1.1.1
   改善唤醒时读取土壤湿度的稳定性
   v1.1.0
   将动态内存从90%减少至55%
   更换选框样式
   v1.0.1
   注意！从旧版本升级的需要重新刷入EERPOM,即烧录两次程序才可使用。
   优化代码结构,去掉多余的代码，提升运行效率，添加更多注释
   修复设置界面的参数会保存两次EEPROM的BUG
   主界面“浇水”上面的英文提示由ON换成AUTO
   设置界面的电压校准改为每次加减0.01V
   修改设置界面的数值排序
*/
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <EEPROM.h>
#include <MsTimer2.h>
#include <Arduino.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
U8G2_SSD1306_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//引脚定义
#define moto_input_1 9          //电机驱动输入引脚1
#define moto_input_2 10          //电机驱动输入引脚2
#define cgq_vcc 4       //传感器电源正极
#define led 13           //led指示灯（暂未启用）
#define confirm 3        //确认按键
#define up 2             //上按键
#define down 7           //下按键
#define vcc_read A7      //读取电压的引脚
#define trsd_read A3     //读取土壤湿度的引脚
#define cd_read A6       //读取充电状态的引脚
//常量
#define REFVCC  5.0                   //参考电压
#define REFVCC_1024  REFVCC/1024.0    //参考电压每级分量
#define trsd_set_sx_eeprom  0         //土壤湿度设定值上限存储地址，4个字节
#define trsd_set_xx_eeprom  4         //土壤湿度设定值下限存储地址，4个字节
#define xmsj_set_eeprom     8         //休眠设定值下限存储地址，4个字节
#define dyjz_set_eeprom     12        //电压校准设定值存储地址，1个字节
#define sbjkfz_set_eeprom   16        //水泵接口翻转设置状态存储地址，1个字节
#define sbcs_set_eeprom     17        //水泵超时翻转设置状态存储地址，1个字节
#define oledld_num_eeprom   18        //OLED亮度翻转设置状态存储地址，1个字节

//变量
uint8_t display_count = 1; //选择显示那一个界面
#define zjm 1              //主界面
#define szjm 2             //设置界面

int8_t zjm_count = 0;         //主界面选择项，决定主界面的闪烁选框停在那一个选项上
int8_t szjm_count = 1;        //设置界面菜单项，决定设置界面的闪烁选框停在那一个选项上
uint8_t szjm_xk_count = 0;    //设置界面选框闪烁选择，决定设置界面的不闪烁选框停在那一个选项上
/* 无              0
   浇水上限        1
   浇水下限        2
   休眠时间        3
   清除异常        4
   传感器开关      5
   水泵超时        6
   水泵接口翻转    7
   电压校准        8
*/

uint16_t trsd_num0  = 0;        //土壤湿度传感器读取值
uint16_t trsd_set_sx = 300;     //土壤湿度设定值上限
uint16_t trsd_set_xx = 450;     //土壤湿度设定值下限
int16_t xmsj = 24;              //休眠时间,小时
boolean cgq_sw_state = 1;       //传感器开关状态
uint8_t sbcs_set_num = 60;      //水泵超时设置数值，秒
boolean sbjkfz_xk_state = 0;    //水泵接口翻转选框状态
boolean sbjkfz_set_state = 0;   //水泵接口翻转设置状态
float dyjz_set_num = 0.00;        //电压校准数值
uint8_t oled1d_num = 30;        //屏幕亮度数值

volatile int wdt_data = 0;      //看门狗计算
volatile uint8_t hxsj = 4;      //唤醒时间4*8，秒
boolean xm_state = 0;           //休眠状态
boolean js_state = 0;           //浇水状态
boolean cgq_error = 0;          //传感器错误计数
uint8_t sb_error_ts = 0;        //水泵错误提示计数

volatile float bat_vcc = 0.0;        //电压
uint32_t vcc_time = 0;               //电压采样周期对比时间，不要改动
uint8_t cd_read_num = 200;           //充电状态接口读取的数值，不要改动

boolean sb_sw_state = 0;             //水泵开关状态
volatile uint8_t sb_jc_count = 0;    //水泵出水检测计数
volatile uint8_t sbyz_js = 0;       //水泵运转时间记录
boolean sb_js_state = 0;             //水泵计时状态
boolean sb_out = 0;                  //水泵输出

uint32_t key_time = 0;    //按键消抖的对比时间，不要改动
int NEW_DATA = 0;
int OLD_DATA = 0;
void setup()
{
  pinMode(cgq_vcc, OUTPUT);
  pinMode(moto_input_1, OUTPUT);
  pinMode(moto_input_2, OUTPUT);
  pinMode(led, OUTPUT);

  pinMode(cd_read, INPUT);
  pinMode(vcc_read, INPUT);
  pinMode(trsd_read, INPUT);
  pinMode(up, INPUT_PULLUP);
  pinMode(confirm, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);

  digitalWrite(cgq_vcc, 1);
  digitalWrite(moto_input_1, 0);
  digitalWrite(moto_input_2, 0);
  /*for (uint8_t a = 0; a < 3; a++)
    {
    digitalWrite(led, 1);
    delay(300);
    digitalWrite(led, 0);
    delay(300);
    }*/
  //EEPROM.put第一次写入去掉注释，第二次以后注释上
  /*EEPROM.put(trsd_set_sx_eeprom, trsd_set_sx);      //浇水上限
    EEPROM.put(trsd_set_xx_eeprom, trsd_set_xx);      //浇水下限
    EEPROM.put(xmsj_set_eeprom, xmsj);                //休眠时间
    EEPROM.put(dyjz_set_eeprom, dyjz_set_num);        //电压校准
    EEPROM.put(sbjkfz_set_eeprom, sbjkfz_set_state);  //水泵接口翻转
    EEPROM.put(sbcs_set_eeprom, sbcs_set_num);        //水泵超时
    EEPROM.put(oledld_num_eeprom, oled1d_num);        //OLED亮度*/

  //EEPROM.get第一次写入注释上，第二次以后写入去掉注释
  EEPROM.get(trsd_set_sx_eeprom, trsd_set_sx);
  EEPROM.get(trsd_set_xx_eeprom, trsd_set_xx);
  EEPROM.get(xmsj_set_eeprom, xmsj);
  EEPROM.get(dyjz_set_eeprom, dyjz_set_num);
  EEPROM.get(sbjkfz_set_eeprom, sbjkfz_set_state);
  EEPROM.get(sbcs_set_eeprom, sbcs_set_num);
  EEPROM.get(oledld_num_eeprom, oled1d_num);

  u8g2.begin();
  u8g2.setContrast(oled1d_num);   //设置亮度0-255
  u8g2.enableUTF8Print();
  u8g2.setFont(chinese8);
  u8g2.setFontMode(1);
  //u8g2.setDrawColor(2);

  display_setup();   //开机画面
  attachInterrupt(0, cz_wdt_data, FALLING); //设置外部中断引脚2
  attachInterrupt(1, cz_wdt_data, FALLING); //设置外部中断引脚3
  setup_watchdog(9);    //设置看门狗中断溢出时间8秒
  vcc_time = millis();  //保证一开机就会显示电池电压
  MsTimer2::set(1 * 1000, sb_jc);  //设置内部定时器1秒中断一次
}

void loop()
{
  if (wdt_data < hxsj)
  {
    xmhx_init();         //休眠唤醒初始化，每次唤醒后只运行一次
    Display();           //显示界面
    read_vcc_time();     //读取电压
    anjian();            //按键
    xianzhi();           //数值限制
    cgq_sw();            //传感器
    sb_sw();             //水泵
    js_jc();             //浇水和检测
  }
  else if (wdt_data >= hxsj)
  {
    sb_sw_state = 0;        //水泵状态标志位置0
    cgq_sw_state = 0;       //传感器状态标志位置0
    sb_sw();                //运行一次关闭水泵
    cgq_sw();               //运行一次关闭传感器
    EEPROM.update(oledld_num_eeprom, oled1d_num); //休眠前将亮度存入eeprom
    u8g2.setPowerSave(1);  //关闭屏幕
    xm_state = 1;          //休眠状态标志位置1
    power_adc_disable();   //关闭ADC
    sleep_avr();           //进入休眠，不会执行下一句语句，只有看门狗中断在运行
    power_adc_enable();   //休眠结束了开启ADC
  }
}

void setup_watchdog(int ii) //看门狗计数初始化，大概8秒
{
  byte bb;
  if (ii > 9 ) ii = 9;
  bb = ii & 7;
  if (ii > 7) bb |= (1 << 5);
  bb |= (1 << WDCE);

  MCUSR &= ~(1 << WDRF);
  // 开始时间序列
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  // 设置新的看门狗超时值
  WDTCSR = bb;
  WDTCSR |= _BV(WDIE);
}
ISR(WDT_vect)  //WDT中断
{
  ++wdt_data;
  if (wdt_data >= 450 * xmsj) wdt_data = 0; //睡够了，我要醒了
}
void cz_wdt_data()  //重置wdt_data
{
  wdt_data = 0;
  wdt_reset();
}
void sb_jc()   //检测水泵是否出水程序中断部分
{
  sb_jc_count += 1;
  sbyz_js += 1;  //水泵运转时间记录
  if (sb_jc_count > sbcs_set_num)
  {
    sb_js_state = 1;    //水泵检测计时状态复位
    sb_jc_count = 10;
  }
}
void sleep_avr()
{
  /*ACD、ADC全关0.12ma,不关0.26ma
     6种节电设置
     SLEEP_MODE_IDLE (0x00<<1)         //空闲模式
     SLEEP_MODE_ADC (0x01<<1)          //ADC噪声抑制模式
     SLEEP_MODE_PWR_DOWN (0x02<<1)     //掉电模式
     SLEEP_MODE_PWR_SAVE (0x03<<1)     //省电模式
     SLEEP_MODE_STANDBY (0x06<<1)      //Standby模式
     SLEEP_MODE_EXT_STANDBY (0x07<<1)  //扩展Standby模式
  */
  cli();                                //关闭全局中断，防止中断引起意外
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  //这里设置睡眠模式
  sleep_enable();                       //开启休眠功能
  sei();                                //开启全局中断
  sleep_cpu();                          //进入休眠
  sleep_disable();                      //关闭休眠功能
  sei();                                //开启全局中断
}
