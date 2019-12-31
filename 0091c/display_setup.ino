//开机显示界面
void display_setup()
{
  u8g2.setCursor(0, 15);
  u8g2.print("By:甘草酸不酸");
  u8g2.setCursor(0, 32);
  u8g2.print("version:0.9c");
  u8g2.nextPage();

  //等待数据稳定
  for (int a = 0; a < 63; a++)
  {
    read_vcc();
    lv_bo();
    int mun = map(trsd_num0, 0, 600, 63, 34);
    u8g2.setCursor(0, 48);
    u8g2.drawPixel(a, mun);
    u8g2.sendBuffer();
  }
}
