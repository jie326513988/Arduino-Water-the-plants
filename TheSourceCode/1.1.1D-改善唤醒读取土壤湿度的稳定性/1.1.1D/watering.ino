//控制水泵
uint16_t old_trsd_num0 = 0;       //旧的土壤湿度值
boolean old_trsd_num0_state = 1;  //旧土壤湿度存储标志位

void js_jc() //水泵浇水和检测
{
  /***判断是否要浇水***/
  if ((trsd_num0 >= trsd_set_xx)  && sb_sw_state == 1) //达到设定值下限，开始浇水
  {
    js_state = 1;
  }
  else if ((trsd_num0 <= trsd_set_sx) || trsd_num0 == 0 || cgq_sw_state == 0 || sb_sw_state == 0) //达到设定值上限，停止浇水
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
      if (y >= -5 && sb_sw_state == 1)
      {
        //差值小于5，且水泵有打开，进入休眠
        sb_error_ts += 1;
        wdt_reset();
        wdt_data = hxsj;
        xm_state = 1;
      }
      else if (y >= -5 && sb_sw_state == 0) wdt_data = hxsj;  //差值小于5，且水泵没有打开，立刻进入休眠
    }

    /*水泵浇水的代码*/
    if (trsd_num0 > trsd_set_sx) sb_out = 1;
    else if (trsd_num0 <= trsd_set_sx) sb_out = 0;
  }
}
