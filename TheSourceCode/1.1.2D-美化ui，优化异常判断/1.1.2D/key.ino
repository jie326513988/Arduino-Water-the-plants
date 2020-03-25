//按键决策
uint32_t ld_count = 0;
void anjian()
{
  if (display_count == zjm) //主界面的按键
  {
    if (digitalRead(up) == 0 ) //按下 上按键
    {
      while (digitalRead(up) == 0) //长按减少亮度
      {
        if (ld_count < 110000) ld_count++;
        if (ld_count > 100000)
        {
          if (oled1d_num > 5) oled1d_num -= 5;
          display_ld();
        }
      }
      ld_count = 0;
      zjm_count --;
    }
    else if (digitalRead(down) == 0 ) //按下 下按键
    {
      while (digitalRead(down) == 0) //长按增加亮度
      {
        if (ld_count < 110000) ld_count++;
        if (ld_count > 100000)
        {
          if (oled1d_num < 255) oled1d_num += 5;
          display_ld();
        }
      }
      ld_count = 0;
      zjm_count ++;
    }
    else if (digitalRead(confirm) == 0 )//按下 确认键
    {
      while (digitalRead(confirm) == 0) Display();
      switch (zjm_count)
      {
        case 1:  //设置
          display_count = szjm;
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

  else if (display_count == szjm) //设置界面的按键
  {
    if (millis() - key_time < 90) return;  //防止按键运行过快
    key_time = millis();
    if (digitalRead(up) == 0 ) //按下 上按键
    {
      if (szjm_xk_count == 0)      szjm_count -= 1;   //选框下移
      else if (szjm_xk_count == 1) trsd_set_sx += 5;  //土壤湿度上限设置加
      else if (szjm_xk_count == 2) trsd_set_xx += 5;  //土壤湿度下限设置加
      else if (szjm_xk_count == 3) xmsj += 1;         //休眠时间设置数值加
      else if (szjm_xk_count == 6) sbcs_set_num += 1; //水泵超时数值加
      else if (szjm_xk_count == 8) dyjz_set_num += 0.01; //电压校准数值加
    }
    else if (digitalRead(down) == 0 ) //按下 下按键
    {
      if (szjm_xk_count == 0)      szjm_count += 1;   //选框上移
      else if (szjm_xk_count == 1) trsd_set_sx -= 5;  //土壤湿度上限设置减
      else if (szjm_xk_count == 2) trsd_set_xx -= 5;  //土壤湿度上限设置减
      else if (szjm_xk_count == 3) xmsj -= 1;         //休眠时间设置数值减
      else if (szjm_xk_count == 6) sbcs_set_num -= 1; //水泵超时数值加
      else if (szjm_xk_count == 8) dyjz_set_num -= 0.01; //电压校准数值减
    }
    else if (digitalRead(confirm) == 0 ) //按下 确认键
    {
      while (digitalRead(confirm) == 0) Display();
      switch (szjm_count)
      {
        case 0:   //空白返回
          szjm_count = 1;
          display_count = zjm;
          break;
        case 1:  //设置上限
          if (szjm_xk_count == 0) szjm_xk_count = 1;
          else if (szjm_xk_count == 1) {
            szjm_xk_count = 0;
            EEPROM.put(trsd_set_sx_eeprom, trsd_set_sx);
          }
          break;
        case 2:  //设置下限
          if (szjm_xk_count == 0) szjm_xk_count = 2;
          else if (szjm_xk_count == 2) {
            szjm_xk_count = 0;
            EEPROM.put(trsd_set_xx_eeprom, trsd_set_xx);
          }
          break;
        case 3:  //休眠时间设置
          if (szjm_xk_count == 0) szjm_xk_count = 3;
          else if (szjm_xk_count == 3) {
            szjm_xk_count = 0;
            EEPROM.put(xmsj_set_eeprom, xmsj);
          }
          break;
        case 4:  //清除异常
          cgq_error = 0;
          sb_error_ts = 0;
          cgq_sw_state = 1;
          digitalWrite(cgq_vcc, 1);
          for (uint8_t i = 0; i < 255; i++) {
            lv_bo();
            delay(1);
          }
          break;
        case 5:  //传感器开/关
          cgq_sw_state = !cgq_sw_state;
          if (cgq_sw_state == 1) {
            digitalWrite(cgq_vcc, 1);
            for (uint8_t i = 0; i < 255; i++) {
              lv_bo();
              delay(1);
            }
          }
          break;
        case 6:  //水泵超时
          if (szjm_xk_count == 0) szjm_xk_count = 6;
          else if (szjm_xk_count == 6) {
            szjm_xk_count = 0;
            EEPROM.update(sbcs_set_eeprom, sbcs_set_num);
          }
          break;
        case 7:  //水泵接口翻转
          if (sb_sw_state == 0) {
            sbjkfz_set_state = !sbjkfz_set_state;
            EEPROM.write(sbjkfz_set_eeprom, sbjkfz_set_state);
          }
          break;
        case 8:  //电压校准
          if (szjm_xk_count == 0) szjm_xk_count = 8;
          else if (szjm_xk_count == 8) {
            szjm_xk_count = 0;
            EEPROM.put(dyjz_set_eeprom, dyjz_set_num);
          }
          break;
        case 9:   //空白返回
          szjm_count = 1;
          display_count = zjm;
          break;
      }
    }
  }
}
