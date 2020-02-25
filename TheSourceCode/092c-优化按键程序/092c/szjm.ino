//设置界面
void szjm_0_1p() //第一页
{
  u8g2.setCursor(1, 14);
  u8g2.print("浇水上限");
  u8g2.setCursor(95, 14);
  u8g2.print("-");
  u8g2.setCursor(102, 14);
  u8g2.print(trsd_set_sx);

  u8g2.setCursor(1, 30);
  u8g2.print("浇水下限");
  u8g2.setCursor(95, 30);
  u8g2.print("-");
  u8g2.setCursor(102, 30);
  u8g2.print(trsd_set_xx);

  u8g2.setCursor(1, 46);
  u8g2.print("休眠时间");
  u8g2.setCursor(102, 46);
  u8g2.print(xmsj);

  u8g2.setCursor(1, 62);
  u8g2.print("清除异常");
  u8g2.setCursor(102, 62);
  u8g2.print(cgq_error);
  u8g2.setCursor(117, 62);
  u8g2.print(sb_error_ts);
}
void szjm_0_2p() //第二页
{
  u8g2.setCursor(1, 14);
  u8g2.print("传感器开/关");
  u8g2.setCursor(102, 14);
  u8g2.print(cgq_sw_state);

  u8g2.setCursor(1, 30);
  u8g2.print("水泵超时");
  u8g2.setCursor(102, 30);
  u8g2.print(sbcs_set_num);

  u8g2.setCursor(1, 46);
  u8g2.print("水泵接口翻转");
  u8g2.setCursor(102, 46);
  u8g2.print(sbjkfz_set_state);

  u8g2.setCursor(1, 62);
  u8g2.print("电压校准");
  u8g2.setCursor(102, 62);
  u8g2.print(dyjz_set_num);
}

void szjm_00_1p()
{
  u8g2.firstPage();
  do {
    szjm_0_1p();
  } while ( u8g2.nextPage() );
}

void szjm_00_2p()
{
  u8g2.firstPage();
  do {
    szjm_0_2p();
  } while ( u8g2.nextPage() );
}

void szjm_1() //画选框-浇水上限
{
  u8g2.firstPage();
  do {
    if (trsd_set_sx_state == 0) szjm_00_1p();
    else if (trsd_set_sx_state == 1) szjm_0_1p();
    u8g2.drawFrame(0, 0, 127, 17);
  } while ( u8g2.nextPage() );
}

void szjm_2() //画选框-浇水下限
{
  u8g2.firstPage();
  do {
    if (trsd_set_xx_state == 0) szjm_00_1p();
    else if (trsd_set_xx_state == 1) szjm_0_1p();
    u8g2.drawFrame(0, 16, 127, 17);
  } while ( u8g2.nextPage() );
}

void szjm_3()//画选框-休眠时间
{
  u8g2.firstPage();
  do {
    if (xmsj_state == 0) szjm_00_1p();
    else if (xmsj_state == 1) szjm_0_1p();
    u8g2.drawFrame(0, 32, 127, 17);
  } while ( u8g2.nextPage() );
}

void szjm_4()//画选框-清除异常
{
  u8g2.firstPage();
  do {
    if (qcyc_xk_state == 0) szjm_00_1p();
    else if (qcyc_xk_state == 1) szjm_0_1p();
    u8g2.drawFrame(0, 48, 127, 16);
  } while ( u8g2.nextPage() );
}

void szjm_5() //画选框-传感器开/关
{
  u8g2.firstPage();
  do {
    if (cgqkg_xk_state == 0) szjm_00_2p();
    else if (cgqkg_xk_state == 1) szjm_0_2p();
    u8g2.drawFrame(0, 0, 127, 17);
  } while ( u8g2.nextPage() );
}

void szjm_6() //画选框-水泵超时时间
{
  u8g2.firstPage();
  do {
    if (sbcs_xk_state == 0) szjm_00_2p();
    else if (sbcs_xk_state == 1) szjm_0_2p();
    u8g2.drawFrame(0, 16, 127, 17);
  } while ( u8g2.nextPage() );
}

void szjm_7()//画选框-水泵接口翻转
{
  u8g2.firstPage();
  do {
    if (sbjkfz_xk_state == 0) szjm_00_2p();
    else if (sbjkfz_xk_state == 1) szjm_0_2p();
    u8g2.drawFrame(0, 32, 127, 17);
  } while ( u8g2.nextPage() );
}

void szjm_8()//画选框-电压校准
{
  u8g2.firstPage();
  do {
    if (dyjz_xk_state == 0) szjm_00_2p();
    else if (dyjz_xk_state == 1) szjm_0_2p();
    u8g2.drawFrame(0, 48, 127, 16);
  } while ( u8g2.nextPage() );
}
