//开机显示界面
void display_setup()
{
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 15);
    u8g2.print("By:甘草酸不酸");
    u8g2.setCursor(0, 32);
    u8g2.print("Version:1.1.1D");
  } while ( u8g2.nextPage());
  for (uint8_t a = 0; a < 255; a++) {
    //等待数据稳定
    read_vcc();
    lv_bo();
    delay(1);
  }
}
