//显示决策
void Display()
{
  if (display_count == zjm) //主界面
  {
    if (zjm_count == 0) zjm_00();     //显示主界面没有选框
    else if (zjm_count == 1) zjm_1(); //画选框到设置
    else if (zjm_count == 2) zjm_2(); //画选框到休眠
    else if (zjm_count == 3) zjm_3(); //画选框到浇水
  }
  else if (display_count == szjm) //设置界面
  {
    if (szjm_count == 0) szjm_00_1p();       //显示设置界面第1页没有选框
    else if (szjm_count == 1) szjm_1();      //画选框到浇水上限
    else if (szjm_count == 2) szjm_2();      //画选框到浇水下限
    else if (szjm_count == 3) szjm_3();      //画选框到休眠时间
    else if (szjm_count == 4) szjm_4();      //画选框到清除异常

    else if (szjm_count == 5) szjm_5();      //画选框到传感器开/关
    else if (szjm_count == 6) szjm_6();      //画选框到水泵超时时间
    else if (szjm_count == 7) szjm_7();      //画选框到水泵接口翻转
    else if (szjm_count == 8) szjm_8();      //画选框到电压校准
    else if (szjm_count == 9) szjm_00_2p();  //显示设置界面第2页没有选框
  }
}
