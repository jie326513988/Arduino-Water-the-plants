//显示决策
void Display()
{
  if (zjm_state == 1) //主界面
  {
    if (zjm_count == 0) zjm_00();
    else if (zjm_count == 1) zjm_1();
    else if (zjm_count == 2) zjm_2();
    else if (zjm_count == 3) zjm_3();
  }
  else if (szjm_state == 1) //设置界面
  {
    if (szjm_count == 0) szjm_00_1p();
    else if (szjm_count == 1) szjm_1();
    else if (szjm_count == 2) szjm_2();
    else if (szjm_count == 3) szjm_3();
    else if (szjm_count == 4) szjm_4();
    
    else if (szjm_count == 5) szjm_5();
    else if (szjm_count == 6) szjm_6();
    else if (szjm_count == 7) szjm_7();
    else if (szjm_count == 8) szjm_8();
  }
}
