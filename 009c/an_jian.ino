//按键决策
uint16_t ld = 0;
void anjian()
{
  if (zjm_state == 1) //主界面的按键
  {
    if (digitalRead(up) == 0 ) //按下 上按键
    {
      while (digitalRead(up) == 0)
      {
        Display();
        ld++;
        if (ld > 8)
        {
          if (oled1d_num < 255) oled1d_num += 5;
          u8g2.setCursor(58, 43);
          u8g2.print(oled1d_num);
          u8g2.sendBuffer();
          u8g2.setContrast(oled1d_num);
        }
      }
      ld = 0;
      zjm_count ++;
    }
    else if (digitalRead(down) == 0 ) //按下 下按键
    {
      while (digitalRead(down) == 0)
      {
        Display();
        ld++;
        if (ld >8)
        {
          if (oled1d_num > 5) oled1d_num -= 5;
          u8g2.setCursor(58, 43);
          u8g2.print(oled1d_num);
          u8g2.sendBuffer();
          u8g2.setContrast(oled1d_num);
        }
      }
      ld = 0;
      zjm_count --;
    }
    else if (digitalRead(confirm) == 0 )//按下 确认键
    {
      while (digitalRead(confirm) == 0) Display();
      switch (zjm_count)
      {
        case 1:  //设置
          zjm_state = !zjm_state;
          szjm_state = ! szjm_state;
          break;
        case 2:  //休眠
          wdt_reset();
          wdt_data = hxsj;
          xm_state = 1;
          break;
        case 3:  //手动开关浇水功能
          sb_sw_state = !sb_sw_state;
          break;
      }
    }
  }

  else if (szjm_state == 1) //设置界面的按键
  {
    uint8_t  i = trsd_set_sx_state + trsd_set_xx_state + xmsj_state + qcyc_xk_state + cgqkg_xk_state + sbcs_xk_state + sbjkfz_xk_state + dyjz_xk_state;

    if (digitalRead(up) == 0 ) //按下 上按键
    {
      if (i < 1) szjm_count += 1; //选框下移
      else if (trsd_set_sx_state == 1) trsd_set_sx += 5;  //土壤湿度上限设置加
      else if (trsd_set_xx_state == 1) trsd_set_xx += 5;  //土壤湿度下限设置加
      else if (xmsj_state == 1) xmsj += 1;                //休眠时间设置数值加
      else if (sbcs_xk_state == 1) sbcs_set_num += 1;     //水泵超时数值加
      else if (dyjz_xk_state == 1) dyjz_set_num += 1;     //电压校准数值加
      delay(30);
    }
    else if (digitalRead(down) == 0 ) //按下 下按键
    {
      if (i < 1) szjm_count -= 1; //选框上移
      else if (trsd_set_sx_state == 1) trsd_set_sx -= 5; //土壤湿度上限设置减
      else if (trsd_set_xx_state == 1) trsd_set_xx -= 5; //土壤湿度上限设置减
      else if (xmsj_state == 1) xmsj -= 1;               //休眠时间设置数值减
      else if (sbcs_xk_state == 1) sbcs_set_num -= 1;     //水泵超时数值加
      else if (dyjz_xk_state == 1) dyjz_set_num -= 1;    //电压校准数值减
      delay(30);
    }
    else if (digitalRead(confirm) == 0 ) //按下 确认键
    {
      while (digitalRead(confirm) == 0) Display();
      switch (szjm_count)
      {
        case 0:   //空白返回
          szjm_count = 1;
          zjm_state = !zjm_state;
          szjm_state = ! szjm_state;
          break;
        case 1:  //设置上限
          trsd_set_sx_state = !trsd_set_sx_state;
          EEPROM.put(trsd_set_sx_eeprom, trsd_set_sx);
          break;
        case 2:  //设置下限
          trsd_set_xx_state = !trsd_set_xx_state;
          EEPROM.put(trsd_set_xx_eeprom, trsd_set_xx);
          break;
        case 3:  //休眠时间设置
          xmsj_state = !xmsj_state;
          EEPROM.put(xmsj_set_eeprom, xmsj);
          break;
        case 4:  //清楚异常
          cgq_error = 0;
          sb_error = 0;
          cgq_sw_state = 1;
          break;
        case 5:  //传感器开/关
          cgq_sw_state = !cgq_sw_state;
          break;
        case 6:  //水泵超时
          sbcs_xk_state = !sbcs_xk_state;
          EEPROM.update(sbcs_set_eeprom, sbcs_set_num);
          break;
        case 7:  //水泵接口翻转
          sbjkfz_set_state = !sbjkfz_set_state;
          EEPROM.write(sbjkfz_set_eeprom, sbjkfz_set_state);
          break;
        case 8:  //电压校准
          dyjz_xk_state = !dyjz_xk_state;
          EEPROM.update(dyjz_set_eeprom, dyjz_set_num);
          break;
      }
    }
  }
}
void xianzhi()//数值限制
{
  if (zjm_count > 3) zjm_count = 0;
  if (zjm_count < 0) zjm_count = 3;

  if (szjm_count > 8) szjm_count = 0;
  if (szjm_count < 0) szjm_count = 8;

  if (trsd_set_sx < 350) trsd_set_sx = 350;
  if (trsd_set_sx > 610) trsd_set_sx = 610;
  if (trsd_set_sx > trsd_set_xx) trsd_set_sx = trsd_set_xx;

  if (trsd_set_xx > 610) trsd_set_xx = 610;
  if (trsd_set_xx < 350) trsd_set_xx = 350;
  if (trsd_set_xx < trsd_set_sx) trsd_set_xx = trsd_set_sx;
  if (xmsj > 999) xmsj = 0;

  if (sb_error == 1 || cgq_error == 1)
  {
    sb_sw_state = 0;
    cgq_sw_state = 0;
  }
  if (sb_error == 1 || cgq_error == 1 || sb_sw_state == 0 || sb_out == 0)
  {
    MsTimer2::stop();  //关闭水泵检测计时
    sbyz_js = 0;        //水泵运转计时置零
    sb_jc_count = 0;    //水泵检测计数置零
    sb_js_state = 1;    //水泵检测计时状态复位
  }
}
