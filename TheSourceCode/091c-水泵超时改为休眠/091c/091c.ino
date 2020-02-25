/* 版本更新说明
    0091c
    1.将水泵超时的警告取消，改成水泵超时后进入休眠
    2.优化休眠唤醒后误判断传感器错误
    3.设置界面湿度数值前面增加负号
    009c
    1.设置界面第二页增加4项的设置
    传感器开关
    水泵超时
    水泵接口翻转
    电压校准
    2.修正水泵运作时间显示
    3.主界面下长按按键可调节亮度
    4.优化一些代码逻辑
    008c
    1.主界面砍掉水泵PWM输出(硬件问题pwm输出会导致卡死)
    2.主界面增加电池充电时的动画
    3.主界面增加水泵运转时间提示
    4.主界面增加水泵运转时的动画
*/
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <EEPROM.h>
#include <Arduino.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#include <MsTimer2.h>
#endif
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define sb_i1 6          //电机驱动输入引脚1
#define sb_i2 5          //电机驱动输入引脚2
#define cgq_vcc 12       //传感器电源正极
#define led 13           //led指示灯（暂时未启用）
#define up 2             //上按键
#define down 3           //下按键
#define confirm 4        //确认按键
#define vcc_read A3      //读取电压的引脚
#define trsd_read A0     //读取土壤湿度的引脚
#define cd_read A2       //读取充电状态的引脚

uint16_t trsd_num0  = 0;           //土壤湿度传感器读取值
uint16_t trsd_num1  = 0;           //土壤湿度传感器读取值

int8_t zjm_count = 0;           //主界面选择计数
boolean zjm_state = 1;          //主界面选择状态
int8_t szjm_count = 1;          //设置界面计数
boolean szjm_state = 0;         //设置选择状态
uint8_t szjm_ys = 1;            //设置界面的页数

boolean sb_sw_state = 0;             //水泵开关状态
volatile uint8_t sb_jc_count = 0;    //水泵出水检测计数
boolean sb_js_state = 0;             //水泵计时状态
volatile boolean sb_out = 0;         //水泵输出

uint16_t trsd_set_sx = 450;     //土壤湿度设定值上限
boolean trsd_set_sx_state = 0;  //设定值上限选框状态
uint16_t trsd_set_xx = 620;     //土壤湿度设定值下限
boolean trsd_set_xx_state = 0;  //设定值下限选框状态
uint16_t xmsj = 1;              //休眠时间
boolean xmsj_state = 0;         //休眠时间选框状态
boolean cgq_sw_state = 1;       //传感器开关状态
boolean qcyc_xk_state = 0;       //清除异常选框状态

boolean cgqkg_xk_state = 0;       //传感器开/关选框状态
boolean sbcs_xk_state = 0;        //水泵超时选框状态
uint8_t sbcs_set_num = 60;        //水泵超时设置数值
uint16_t sbyz_js = 0;              //水泵运转时间记录
boolean sbjkfz_xk_state = 0;      //水泵接口翻转选框状态
boolean sbjkfz_set_state = 0;     //水泵接口翻转设置状态
boolean dyjz_xk_state = 0;        //电压校准选框状态
int8_t dyjz_set_num = 3;         //电压校准数值
uint8_t oled1d_num = 30;             //屏幕亮度数值

volatile int wdt_data = 0;         //看门狗计算
volatile uint8_t hxsj = 4;         //唤醒时间
volatile boolean xm_state = 0;     //休眠状态
volatile boolean js_state = 0;     //浇水状态
volatile boolean cgq_error = 0;    //传感器错误计数
volatile boolean sb_error_ts = 0;     //水泵错误提示计数

volatile float VCC = 0;               //电压
uint32_t vcc_time;                   //电源采样周期
volatile boolean cd_read_state = 0;  //充电状态
uint8_t cd_read_num = 200;           //充电数值

const uint8_t trsd_set_sx_eeprom = 0;      //土壤湿度设定值上限存储地址，4个字节
const uint8_t trsd_set_xx_eeprom = 4;      //土壤湿度设定值下限存储地址，4个字节
const uint8_t xmsj_set_eeprom = 8;         //休眠设定值下限存储地址，4个字节
const uint8_t dyjz_set_eeprom = 12;        //电压校准设定值存储地址，1个字节
const uint8_t sbjkfz_set_eeprom = 13;      //水泵接口翻转设置状态存储地址，1个字节
const uint8_t sbcs_set_eeprom = 14;      //水泵接口翻转设置状态存储地址，1个字节
const uint8_t oledld_num_eeprom = 15;      //水泵接口翻转设置状态存储地址，1个字节
void setup()
{
  pinMode(cgq_vcc, OUTPUT);
  pinMode(sb_i1, OUTPUT);
  pinMode(sb_i2, OUTPUT);
  pinMode(led, OUTPUT);

  pinMode(cd_read, INPUT);
  pinMode(vcc_read, INPUT);
  pinMode(trsd_read, INPUT);
  pinMode(up, INPUT_PULLUP);
  pinMode(confirm, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);

  digitalWrite(cgq_vcc, 1);
  analogWrite(sb_i1, 0);
  analogWrite(sb_i2, 0);
  for (uint8_t a = 0; a < 5; a++)
  {
    digitalWrite(led, 1);
    delay(100);
    digitalWrite(led, 0);
    delay(100);
  }


  //EEPROM.put第一次写入去掉注释，第二次以后注释上
  /*EEPROM.put(trsd_set_sx_eeprom, 450);
    EEPROM.put(trsd_set_xx_eeprom, 550);
    EEPROM.put(xmsj_set_eeprom, 24);
    EEPROM.update(dyjz_set_eeprom, 3);
    EEPROM.update(sbjkfz_set_eeprom, 0);
    EEPROM.update(sbcs_set_eeprom, 60);
    EEPROM.update(oledld_num_eeprom, 50);*/

  //EEPROM.get第一次写入注释上，第二次以后写入去掉注释
  EEPROM.get(trsd_set_sx_eeprom, trsd_set_sx);
  EEPROM.get(trsd_set_xx_eeprom, trsd_set_xx);
  EEPROM.get(xmsj_set_eeprom, xmsj);
  EEPROM.get(dyjz_set_eeprom, dyjz_set_num);
  EEPROM.get(sbjkfz_set_eeprom, sbjkfz_set_state);
  EEPROM.get(sbcs_set_eeprom, sbcs_set_num);
  EEPROM.get(oledld_num_eeprom, oled1d_num);

  u8g2.begin();
  u8g2.setContrast(oled1d_num);   //设置对比度0-255
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
    EEPROM.update(oledld_num_eeprom, oled1d_num);
    u8g2.setPowerSave(1);  //关闭屏幕
    xm_state = 1;          //休眠状态标志位置1
    power_adc_disable();
    sleep_avr();           //进入休眠
    power_adc_enable();
  }
}

void setup_watchdog(int ii) //看门狗计数初始化
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
  sbyz_js += 1;
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
/*boolean acd_adc_en(boolean a)
  {
  if (a == 1)
  {
    //ON ACD
    ACSR &= ~_BV(ACIE);   //禁止模拟比较器中断后再对ACD位操作，否则ACD位变化会发生中断，写入0关闭
    ACSR &= ~_BV(ACD);    //开启模拟比较器
    ACSR |= _BV(ACIE);    //开启模拟比较器中断
    //ON ADC
    ADCSRA |= _BV(ADEN);   //ADC使能开启
    ADCSRA |= _BV(ADIF);   //ADC中断使能开启
  }
  if (a == 0)
  {
    //OFF ACD
    ACSR &= ~_BV(ACIE);    //禁止模拟比较器中断后再对ACD位操作，否则ACD位变化会发生中断，写入0关闭
    ACSR |= _BV(ACD);      //关闭模拟比较器 ,写入1关闭
    //OFF ADC
    ADCSRA |= _BV(ADIF);   //清除标志，写1清除
    ADCSRA &= ~_BV(ADIE);  //ADC中断使能关闭
    ADCSRA &= ~_BV(ADEN);  //ADC使能关闭，写入0关闭
  }
  }*/
