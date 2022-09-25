/*
   by SuInk uid 143177803 https://suink.cn && Mu-Jie https://github.com/xfmujie
*/

#define BL 11
#define D12 12 //主板led灯
#define D13 13

#include <WiFi.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <string.h>
#include <MD5Builder.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#include "Font.h"
#include "BG_Top.h"
#include "BG_central.h"
#include "BG_bottom.h"
#include "WIFI_G.h"
#include "WIFI_R.h"

/*参数配置*/
const char *ssid = "***";                           //WIFI账户
const char *password = "***";                       //WIFI密码
String uid = "***";                                 //原神uid
String server = "cn_gf01";                          //如果是b服，填cn_qd01
String cookie = "cookie_token=***; account_id=***"; //抓包获取的cookie
String city = "***";                                //所在城市或地区
String key = "***";                                 //高德天气申请的key

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 8 * 3600, 60000);
String day[] = {"周日", "周一", "周二", "周三", "周四", "周五", "周六"};
String url_01 = "https://api-takumi-record.mihoyo.com/game_record/app/genshin/api/dailyNote?server=" + server + "&role_id=" + uid;
String url_02 = "https://restapi.amap.com/v3/weather/weatherInfo?city=" + city + "&key=" + key;
int i = 25;
void setup()
{
  pinMode(D12, OUTPUT);
  pinMode(D13, OUTPUT);
  digitalWrite(D12, LOW);
  digitalWrite(D13, LOW);
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.loadFont(Font);

  /*背景图片*/
  tft.pushImage(0, 22, 240, 121, (uint16_t *)gImage_BG_central);
  tft.pushImage(0, 143, 240, 177, (uint16_t *)gImage_BG_bottom);

  tft.print("wifi connected...");
  WiFi.begin(ssid, password); // ssid , password
  timeClient.begin();
  delay(3000);
}

void loop()
{

  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(ssid, password);
    delay(3000);
  }

  unsigned long timestamp = timeClient.getEpochTime() - 8 * 3600; // 时间戳
  String timestampstr = String(timestamp);
  String str1 = "salt=xV8v4Qu54lUKrEYFZkJhB8cuOh9Asafs&t=" + timestampstr + "&r=143177&b=&q=role_id=" + uid + "&server=" + server;
  HTTPClient http;

  /*请求原神接口*/
  http.begin(url_01);
  MD5Builder md5;
  md5.begin();
  md5.add(str1);
  md5.calculate();
  String sign = md5.toString();
  String ds = timestampstr + ",143177," + sign;
  http.addHeader("DS", ds);
  http.addHeader("x-rpc-app_version", "2.19.1");
  http.addHeader("User-Agent", "Mozilla/5.0 (iPhone; CPU iPhone OS 13_2_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) miHoYoBBS/2.11.1");
  http.addHeader("x-rpc-client_type", "5");
  http.addHeader("Referer", "https://webstatic.mihoyo.com/");
  http.addHeader("Cookie", cookie);
  int website_return_code = http.GET(); //请求米哈游服务器
  String mhy_return_text = http.getString();
  http.end();
  Serial.println(mhy_return_text);

  /*请求高德天气接口*/
  http.begin(url_02);
  int httpCode_02 = http.GET();
  String gd_return_text = http.getString();
  http.end();
  Serial.println(gd_return_text);

  int Strnum = mhy_return_text.length() + gd_return_text.length(); //计算返回字符的总长度
  DynamicJsonDocument doc(Strnum);

  deserializeJson(doc, mhy_return_text); //将response转化为json格式
  JsonObject obj = doc.as<JsonObject>();
  String current_resin = obj["data"]["current_resin"];
  int resin_recovery_time = obj["data"]["resin_recovery_time"];
  String extra_reward = obj["data"]["is_extra_task_reward_received"];
  String zby_rec_day = obj["data"]["transformer"]["recovery_time"]["Day"];
  String zby_rec_hour = obj["data"]["transformer"]["recovery_time"]["Hour"];
  String zby_rec_min = obj["data"]["transformer"]["recovery_time"]["Minute"];
  String current_home_coin = obj["data"]["current_home_coin"];
  String max_home_coin = obj["data"]["max_home_coin"];

  int Hour = timeClient.getHours();
  int Min = timeClient.getMinutes();
  int Rectime = resin_recovery_time / 60 + Hour * 60 + Min; //定义恢复时间（分钟数）
  int Rechour = Rectime / 60;
  int Recmin = Rectime % 60;

  String Which_day = (Rechour < 24) ? "今天" : "明天";
  Rechour = (Rechour > 24) ? Rechour % 24 : Rechour;
  String str_hour = String(Rechour);
  String str_min = String(Recmin);
  str_hour = (Rechour < 10) ? "0" + str_hour : str_hour;
  str_min = (Recmin < 10) ? "0" + str_min : str_min;
  str_hour = (current_resin != "null") ? str_hour : "??";
  str_min = (current_resin != "null") ? str_min : "??";

  /*解析高德天气返回的json*/
  deserializeJson(doc, gd_return_text);
  JsonObject lives = doc["lives"][0];
  String weather = lives["weather"].as<String>();
  String temperature = lives["temperature"].as<String>();
  String humidity = lives["humidity"].as<String>();
  String winddirection = lives["winddirection"].as<String>();
  String windpower = lives["windpower"].as<String>();
  windpower = (windpower == "≤3") ? "<=3" : windpower;

  /*使用Sprite类绘制显示内容，可解决刷新闪屏问题*/
  spr.createSprite(240, 177);
  spr.pushImage(0, 0, 240, 177, (uint16_t *)gImage_BG_bottom);
  spr.loadFont(Font);
  spr.setTextColor(tft.color565(83, 108, 188), TFT_WHITE);
  spr.drawString(weather, 22, 7);
  spr.drawString(temperature + "度", 102, 7);
  spr.drawString("湿度" + humidity + "  " + winddirection + "风" + windpower + "级", 22, 30);

  spr.setTextColor(tft.color565(157, 35, 172), TFT_WHITE);
  spr.drawString(current_resin + "/160 树脂", 22, 53);
  spr.drawString(Which_day + " " + str_hour + ":" + str_min + " 回满", 22, 75);
  if (zby_rec_hour == "0" && zby_rec_day == "0")
    spr.drawString("质变仪 " + zby_rec_min + " 分钟", 22, 98);
  else if (zby_rec_min == "0" && zby_rec_day == "0")
    spr.drawString("质变仪 " + zby_rec_hour + " 小时", 22, 98);
  else
    spr.drawString("质变仪 " + zby_rec_day + " 天", 22, 98);

  extra_reward == "false" ? spr.drawString("今日委托奖励未领！", 22, 122) : spr.drawString("洞天宝钱" + current_home_coin + "/" + max_home_coin, 22, 122);

  spr.setTextColor(TFT_WHITE, TFT_PINK);
  spr.drawString("@昔枫沐杰", 68, 157);
  spr.pushSprite(0, 143);
  spr.unloadFont();
  spr.deleteSprite();

  for (i; i < 30; i++)
  {
    timeClient.update();
    Serial.print("Epoch Time:");
    Serial.println(timestamp);
    struct tm *ptm = gmtime((time_t *)&timestamp);
    String date;

    date = (Hour < 8) ? String(ptm->tm_mday + 1) : String(ptm->tm_mday);
    String month = String(ptm->tm_mon + 1);

    spr.createSprite(240, 22);
    spr.pushImage(0, 0, 240, 22, (uint16_t *)gImage_BG_Top); //状态栏
    spr.loadFont(Font);
    spr.setTextColor(tft.color565(25, 113, 149), TFT_WHITE);
    spr.drawString(month + "." + date, 3, 2);//获取日期
    spr.drawString(timeClient.getFormattedTime(), 66, 2);//获取时间
    spr.drawString(day[timeClient.getDay()], 174, 2);//获取星期几
    WiFi.status() == WL_CONNECTED ? spr.pushImage(215, 0, 23, 23, (uint16_t *)gImage_WIFI_G) : spr.pushImage(215, 0, 23, 23, (uint16_t *)gImage_WIFI_R);
    spr.pushSprite(0, 0);
    spr.unloadFont();
    spr.deleteSprite();
    delay(1000);
  }
  i = 0;
}
