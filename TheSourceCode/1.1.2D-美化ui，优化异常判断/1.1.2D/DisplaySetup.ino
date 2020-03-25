//开机显示界面
void display_setup()
{
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 15);
    u8g2.print("By:甘草酸不酸");
    u8g2.setCursor(0, 32);
    u8g2.print("Version:1.1.2D");
  } while ( u8g2.nextPage());
  for (uint8_t a = 0; a < 255; a++) {
    //等待数据稳定
    read_vcc();
    lv_bo();
    delay(1);
  }
}

void display_ld()
{
  u8g2.firstPage();
  do {
    lv_bo();
    zjm_0();
    uint8_t ld_num;
    ld_num = map(oled1d_num, 5, 255, 5, 48); //将数值映射为进度条X值
    u8g2.drawRFrame(46, 36, 50, 7, 2);    //圆角空心方形
    u8g2.drawBox(47, 37, ld_num, 5);   //实心方形，进度条
    u8g2.setFont(u8g2_font_baby_tn);
    u8g2.setDrawColor(2);               //这个函数控制反色显示
    u8g2.setCursor(70, 42);
    u8g2.print(oled1d_num);
    u8g2.setDrawColor(1);
    u8g2.setContrast(oled1d_num);
    u8g2.setFont(chinese8);
  } while ( u8g2.nextPage() );
}
