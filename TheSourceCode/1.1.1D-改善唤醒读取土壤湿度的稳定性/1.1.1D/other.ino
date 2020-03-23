//传感器、电压、休眠唤醒初始化
void sb_sw() //水泵开关
{
  if (sb_sw_state == 1)
  {
    if (sbjkfz_set_state == 0)
    {
      digitalWrite(moto_input_1, sb_out);
      digitalWrite(moto_input_2, 0);
    }
    else if (sbjkfz_set_state == 1)
    {
      digitalWrite(moto_input_1, 0);
      digitalWrite(moto_input_2, sb_out);
    }
  }
  else if (sb_sw_state == 0)
  {
    digitalWrite(moto_input_1, 0);
    digitalWrite(moto_input_2, 0);
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

void read_vcc_time() //1s读取一次电压和充电状态
{
  if (millis() - vcc_time < 1500) return;
  vcc_time = millis();
  read_vcc();
  cd_read_num = analogRead(cd_read);
}

void read_vcc() //读取电压
{
  float vcc = 0.0;
  for (uint8_t i = 0; i < 20; i++)
  {
    vcc += analogRead(vcc_read);
  }
  bat_vcc = (vcc / 20.0) * (REFVCC_1024) + dyjz_set_num;
}

void xmhx_init()  //休眠唤醒后对水泵传感器屏幕初始化
{
  if (xm_state == 1)
  {
    js_state = 0;
    sb_out = 0;
    cgq_sw_state = 1;          //传感器状态标志位置1
    sb_sw_state = 1;          //水泵状态标志位置1
    cgq_sw();
    for (uint8_t i = 0; i < 255; i++) {
      lv_bo();
      delay(1);
    }
    u8g2.setPowerSave(0);     //开启屏幕
    xm_state = 0;             //休眠状态标志位置0
  }
}

void xianzhi()//数值限制
{
  if (zjm_count > 3) zjm_count = 0;
  else if (zjm_count < 0) zjm_count = 3;

  if (szjm_count > 9) szjm_count = 0;
  else if (szjm_count < 0) szjm_count = 9;

  if (trsd_set_sx < 200) trsd_set_sx = 200;
  else if (trsd_set_sx > 610) trsd_set_sx = 700;
  else if (trsd_set_sx > trsd_set_xx) trsd_set_sx = trsd_set_xx;

  if (trsd_set_xx > 700) trsd_set_xx = 700;
  else if (trsd_set_xx < 200) trsd_set_xx = 200;
  else if (trsd_set_xx < trsd_set_sx) trsd_set_xx = trsd_set_sx;

  if (xmsj > 999) xmsj = 0;
  else if (xmsj < 0) xmsj = 999;

  if (cgq_error == 1)
  {
    sb_sw_state = 0;
    cgq_sw_state = 0;
  }

  if (dyjz_set_num > 99) dyjz_set_num = 0;
  else if (dyjz_set_num < -99) dyjz_set_num = 0;

  if (cgq_error == 1 || sb_sw_state == 0 || sb_out == 0)
  {
    MsTimer2::stop();  //关闭水泵检测计时
    sbyz_js = 0;        //水泵运转计时置零
    sb_jc_count = 0;    //水泵检测计数置零
    sb_js_state = 1;    //水泵检测计时状态复位
  }
}
