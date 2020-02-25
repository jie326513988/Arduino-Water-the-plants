//开机显示界面
void display_setup()
{
  u8g2.setCursor(0, 15);
  u8g2.print("By:甘草酸不酸");
  u8g2.setCursor(0, 32);
  u8g2.print("version:0.90c");
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

void display_setup1()
{
  for (uint8_t a = 0; a < 255; a++)
  {
    read_vcc();
    lv_bo();
  }
  float y1;
  float y2;
  float y3;
  float y4;
  float y5;
  for (int x = 0; x < 127; x++)
  {
    u8g2.firstPage();
    u8g2.drawHLine(0, 63, 127);
    do {
      if (x <= 30)
      {
        y1 = (0.03555 * (x * x)) - (0.06666 * x) + 33;
        u8g2.drawDisc(x, y1, 1, U8G2_DRAW_ALL);
      }
      else if (x > 30 && x <= 60)
      {
        y2 = (0.06888 * (x * x)) - (6.23333 * x) + 189;
        u8g2.drawDisc(x, y2, 1, U8G2_DRAW_ALL);
      }
      else if (x > 60 && x <= 90)
      {
        y3 = (0.04285 * (x * x)) - (6.38571 * x) + 291.857;
        u8g2.drawDisc(x, y3, 1, U8G2_DRAW_ALL);
      }
      else if (x > 90 && x <= 106)
      {
        y4 = (0.0625 * (x * x)) - (12.25 * x) + 659.25;
        u8g2.drawDisc(x, y4, 1, U8G2_DRAW_ALL);
      }
      else if (x > 106)
      {
        y5 = (0.0181818 * (x * x)) - (4.236363 * x) + 307.763636;
        u8g2.drawDisc(x, y5, 1, U8G2_DRAW_ALL);
      }
    } while ( u8g2.nextPage() );
  }
  u8g2.setCursor(0, 15);
  u8g2.print("By:甘草酸不酸");
  u8g2.setCursor(0, 32);
  u8g2.print("version:0.9c");
  u8g2.nextPage();
  delay(600);
}
