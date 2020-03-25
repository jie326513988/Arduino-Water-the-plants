//动态调参一阶滤波算法
void lv_bo()
{
  trsd_num0 = filter_1();
}

#define Threshold_1     5    //阈值1用于一阶带参滤波器，AD大于此值时，计数增加
#define Threshold_2     24   //阈值2用于一阶带参滤波器，计数值大于此值时，增大参数，增强滤波跟随
float K = 0.0;                 //滤波系数
uint8_t num_x = 0;           //滤波计数器
//int NEW_DATA = 0;
//int OLD_DATA = 0;
boolean flag = 0;            //本次数据变化方向

/*****带系数修改的一阶滤波函数
  入口： NEW_DATA    新采样的角度值
         OLD_DATA    上次滤波获得的角度结果
         K           滤波系数(代表在滤波结果中的权重)
         flag        上次数据变化方向
  出口： return OLD_DATA;      本次滤波角度结果
*/

int filter_1()
{
  NEW_DATA = analogRead(trsd_read);
  //判断变化方向是否一致 flag=1为一致，flag=0为不一致
  if (NEW_DATA >= OLD_DATA)
  {
    NEW_DATA = analogRead(trsd_read);
    if (NEW_DATA >= OLD_DATA) flag = 1;
    else flag = 0;
  }
  else if (NEW_DATA < OLD_DATA)
  {
    NEW_DATA = analogRead(trsd_read);
    if (NEW_DATA < OLD_DATA) flag = 1;
    else flag = 0;
  }

  if (flag) //变化方向一致时执行
  {
    num_x++;
    if (abs(NEW_DATA - OLD_DATA) > Threshold_1) //当变化大于Threshold_1的时候，进行计数器num快速增加，以达到快速增大K值，提高跟随性
    {
      num_x += 2;
    }
    if (num_x > Threshold_2) //计数阈值设置，当递增或递减速度达到一定速率时，增大K值
    {
      K = K + 0.1;      //0.2为K的增长值，看实际需要修改
      num_x = 0;
    }
  }
  else
  {
    num_x = 0;
    K = 0.01;   //变化稳定时K值，看实际修改
  }
  OLD_DATA = float((1 - K) * OLD_DATA + K * NEW_DATA);
  return OLD_DATA;
}
