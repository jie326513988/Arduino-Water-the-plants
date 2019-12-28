//控制水泵传感器电压相关
uint16_t old_trsd_num0 = 0;       //旧的土壤湿度值
uint32_t trsd_num0_time = 0;      //土壤湿度值旧数据采样周期存储
boolean old_trsd_num0_state = 1;  //旧土壤湿度存储标志位

void js_jc() //水泵浇水和检测
{
  /***判断是否要浇水***/
  if ((trsd_num0 >= trsd_set_xx) && sb_error == 0 && sb_sw_state == 1) //达到设定值下限，开始浇水
  {
    js_state = 1;
  }
  else if ((trsd_num0 <= trsd_set_sx) || sb_error == 1 || trsd_num0 == 0 || cgq_sw_state == 0 || sb_sw_state == 0) //达到设定值上限，停止浇水
  {
    js_state = 0;
    sb_out = 0;
  }

  //具体的浇水方法
  if (js_state == 1)
  {
    wdt_reset();       //重置看门狗，防止在浇水时休眠

    /***检测水泵是否出水的程序***/
    if (sb_js_state == 1) //开始浇水前打开内部计时器
    {
      MsTimer2::start();  //开始水泵检测计时
      sb_js_state = 0;       //置零退出每次只运行一次
      old_trsd_num0_state = 1;  //打开旧土壤湿度存储
    }
    if (sb_jc_count == 10 && old_trsd_num0_state == 1) //等待10秒后再存储土壤湿度
    {
      old_trsd_num0 = trsd_num0; //储存当前土壤湿度供60秒后作对比
      old_trsd_num0_state = 0;   //置零退出每次只运行一次
    }
    else if (sb_jc_count == sbcs_set_num) //60秒后对比50秒前的土壤湿度是否有变化
    {
      int y = trsd_num0 - old_trsd_num0;                 //计算与50秒前的差值
      if (y >= -5 && sb_sw_state == 1) sb_error = 1;     //差值小于5，且水泵有打开，报警
      else if (y >= -5 && sb_sw_state == 0) wdt_data = hxsj;  //差值小于5，且水泵没有打开，立刻进入休眠
    }

    /*水泵浇水的代码*/
    if (trsd_num0 > trsd_set_sx) sb_out = 1;
    else if (trsd_num0 <= trsd_set_sx) sb_out = 0;
  }
}

void sb_sw() //水泵开关
{
  if (sb_sw_state == 1)
  {
    if (sbjkfz_set_state == 0)
    {
      digitalWrite(sb_i1, sb_out);
      digitalWrite(sb_i2, 0);
    }
    else if (sbjkfz_set_state == 1)
    {
      digitalWrite(sb_i1, 0);
      digitalWrite(sb_i2, sb_out);
    }
  }
  else if (sb_sw_state == 0)
  {
    digitalWrite(sb_i1, 0);
    digitalWrite(sb_i2, 0);
  }
}

void cgq_sw() //传感器开关
{
  if (cgq_sw_state == 1)
  {
    digitalWrite(cgq_vcc, 1);
    lv_bo();
  }
  else if (cgq_sw_state == 0)
  {
    digitalWrite(cgq_vcc, 0);
    trsd_num0 = 0;
    sb_out = 0;
  }
}

void read_vcc_time() //3s读取一次电压
{
  if (millis() - vcc_time < 3000) return;
  vcc_time = millis();
  read_vcc();
  cd_read_num = analogRead(cd_read);
}

void read_vcc() //读取电压
{
  float vcc = 0;
  for (uint8_t i = 0; i < 10; i++)
  {
    vcc += analogRead(vcc_read) + dyjz_set_num;
  }
  VCC = (vcc / 10) * (5.00 / 1023);
}

void xmhx_init()  //休眠唤醒后对水泵传感器屏幕初始化
{
  if (xm_state == 1)
  {
    cgq_sw_state = 1;          //传感器状态标志位置1
    sb_sw_state = 1;          //水泵状态标志位置1
    cgq_sw();
    for (int i = 0; i < 63; i++) lv_bo();
    u8g2.setPowerSave(0);     //开启屏幕
    xm_state = 0;             //休眠状态标志位置0
  }
}
