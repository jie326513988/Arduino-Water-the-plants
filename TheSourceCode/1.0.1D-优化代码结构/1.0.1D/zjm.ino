//主界面
/*光标闪烁原理

   闪烁
  u8g2.firstPage();
  do {
    zjm_2();
    u8g2.drawFrame(93, 46, 33, 18);
  } while ( u8g2.nextPage() );

   不闪烁
    u8g2.firstPage();
  do {
    zjm_1();
    u8g2.drawFrame(93, 46, 33, 18);
  } while ( u8g2.nextPage() );

*/
uint8_t cd_i = 0;
uint8_t sb_x1 = 0;
uint8_t sb_x2 = 0;
uint8_t sb_x3 = 0;

void zjm_0()    //主界面的显示，选框不闪烁
{
  uint16_t trsd_num1  = 0; //映射后土壤湿度传感器读取值
  if (cgq_error == 0)
  {
    u8g2.setCursor(0, 15);
    u8g2.print("土壤湿度");

    if (trsd_num0 >= 200 && trsd_num0 <= 610)
    {
      trsd_num1 = map(trsd_num0, 200, 610, 63, 1); //将数值映射为进度条X值
    }
    u8g2.drawRFrame(0, 21, 65, 9, 2);    //圆角空心方形
    u8g2.drawBox(1, 22, trsd_num1, 7);   //实心方形，进度条
    u8g2.setFont(u8g2_font_baby_tn);
    u8g2.setDrawColor(2);               //这个函数控制反色显示
    u8g2.setCursor(28, 28);
    u8g2.print(trsd_num0);
    u8g2.setCursor(23, 28);
    u8g2.print("-");
    u8g2.setDrawColor(1); //用完记得恢复正常显示

    u8g2.setFont(u8g2_font_iranian_sans_10_t_all); // 水泵运转计时显示
    if (sb_jc_count <= 9)
    {
      u8g2.setCursor(108, 29);
      u8g2.print(sbyz_js);
    }
    else if (sb_jc_count > 9 && sb_jc_count <= 99)
    {
      u8g2.setCursor(105, 29);
      u8g2.print(sbyz_js);
    }
    else if (sb_jc_count > 99 && sb_jc_count <= 199)
    {
      u8g2.setCursor(102, 29);
      u8g2.print(sbyz_js);
    }

    u8g2.setFont(chinese8);
    u8g2.setCursor(96, 15);
    u8g2.print("水泵");
    u8g2.setCursor(96, 31);
    if (sb_out == 1) //水泵运转动画
    {
      if (sb_x1 < 10)
      {
        u8g2.drawLine(108 + sb_x1, 18 , 116 + sb_x1, 18);
        sb_x1 += 1;
      }
      else if (sb_x1 >= 10 && sb_x2 < 22)
      {
        sb_x2 += 1;
        u8g2.drawLine(118 - sb_x2, 18 , 126 - sb_x2, 18);
      }
      else if (sb_x2 >= 22 && sb_x3 < 12)
      {
        sb_x3 += 1;
        u8g2.drawLine(96 + sb_x3, 18 , 104 + sb_x3, 18);
      }
      else if (sb_x3 >= 12)
      {
        u8g2.drawLine(108, 18 , 116 , 18);
        sb_x1 = 0;
        sb_x2 = 0;
        sb_x3 = 0;
      }
    }
  }
  else if (cgq_error == 1)
  {
    u8g2.drawBox(0, 0, 127, 34);
    u8g2.setCursor(8, 23);
    u8g2.setDrawColor(0);
    u8g2.print("湿度传感器异常");
    u8g2.setDrawColor(1);
  }

  u8g2.setFont(u8g2_font_oskool_tr);
  u8g2.setCursor(0, 43);
  u8g2.print("VCC");
  u8g2.setCursor(20, 43);
  u8g2.print(bat_vcc, 2);
  if (sb_sw_state == 1)
  {
    u8g2.setCursor(98, 43);
    u8g2.print("AOTO");
  }
  else if (sb_sw_state == 0)
  {
    u8g2.setCursor(102, 43);
    u8g2.print("OFF");
  }

  if (cd_read_num == 0) // 充电动画
  {
    cd_i += 5;
    u8g2.drawHLine(0, 44, cd_i);
    if (cd_i > 40) cd_i = 0;
  }

  u8g2.setFont(chinese8);
  u8g2.setCursor(1, 61);
  u8g2.print("设定");
  u8g2.setCursor(48, 61);
  u8g2.print("休眠");
  u8g2.setCursor(96, 61);
  u8g2.print("浇水");

  /*调试信息显示*/
  /*u8g2.setCursor(50, 43);
    u8g2.print(sb_jc_count);
    u8g2.setCursor(70, 43);
    u8g2.print(old_trsd_num0);*/

  /*u8g2.setCursor(50, 43);
    u8g2.print(sb_jc_count);
    u8g2.setCursor(70, 43);
    u8g2.print(js_state);*/
  //u8g2.setCursor(70, 43);
  //u8g2.print(cgq_error_num);
}

void zjm_00()  //主界面的显示，选框闪烁
{
  u8g2.firstPage();
  do {
    zjm_0();
  } while ( u8g2.nextPage() );
}

void zjm_1() //画选框-设置
{
  u8g2.firstPage();
  do {
    zjm_00();
    u8g2.drawFrame(0, 46, 33, 18);       //空心方形
  } while ( u8g2.nextPage() );
}

void zjm_2() //画选框-休眠
{
  u8g2.firstPage();
  do {
    zjm_00();
    u8g2.drawFrame(47, 46, 33, 18);       //空心方形
  } while ( u8g2.nextPage() );
}

void zjm_3() //画选框-浇水
{
  u8g2.firstPage();
  do {
    zjm_00();
    u8g2.drawFrame(95, 46, 33, 18);       //空心方形
  } while ( u8g2.nextPage() );
}
