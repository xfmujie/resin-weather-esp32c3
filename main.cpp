/*
   by SuInk uid 143177803 https://suink.cn
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

#include "Font.h"
#include "BG_Top.h"
#include "BG_central.h"
#include "BG_bottom.h"
#include "WIFI_G.h"
#include "WIFI_R.h"

#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

const char *ssid = "IQOO Neo5";                                                                // WIFI账户
const char *password = "12345790ly";                                                           // WIFI密码
String uid = "101152522";                                                                      //原神uid
String server = "cn_gf01";                                                                     //如果是b服，填cn_qd01
String cookie = "cookie_token=aqnl9IBMyeJvI6j70GbpdPzqngKo1pd7tUiIwDOh; account_id=160338366"; //抓包获取的cookie

String url_01 = "https://api-takumi-record.mihoyo.com/game_record/app/genshin/api/dailyNote?server=" + server + "&role_id=" + uid;
String url_02 = "https://restapi.amap.com/v3/weather/weatherInfo?city=鱼峰区&key=4fc98b84aa6d816a7d42cc1fb260eadf"; //高德天气api

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 8 * 3600, 60000);
String day[] = {"周日", "周一", "周二", "周三", "周四", "周五", "周六"};
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
  tft.pushImage(0, 22, 240, 121, (uint16_t *)gImage_BG_central);
  tft.print("WIFI Connected...");
  WiFi.begin(ssid, password); // ssid , password
  /*
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
    tft.print(".");

  }*/
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

  int Hour = timeClient.getHours();
  int Min = timeClient.getMinutes();

  String mhy_return_text;
  String gd_return_text;
  String yq_return_text;
  unsigned long timestamp = timeClient.getEpochTime() - 8 * 3600; // 时间戳
  String timestampstr = String(timestamp);
  String str1 = "salt=xV8v4Qu54lUKrEYFZkJhB8cuOh9Asafs&t=" + timestampstr + "&r=143177&b=&q=role_id=" + uid + "&server=" + server;
  HTTPClient http;
  Serial.println(url_01);

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
  mhy_return_text = http.getString();
  http.end();
  // Serial.println(website_return_code);
  // Serial.println(mhy_return_text);

  http.begin(url_02);
  int httpCode_02 = http.GET();
  gd_return_text = http.getString();
  http.end();
  Serial.println(gd_return_text);

  int Strnum = mhy_return_text.length() + gd_return_text.length() + yq_return_text.length();
  DynamicJsonDocument doc(Strnum);

  deserializeJson(doc, mhy_return_text); //将response转化为json格式
  JsonObject obj = doc.as<JsonObject>();
  String current_resin = obj["data"]["current_resin"];
  int resin_recovery_time = obj["data"]["resin_recovery_time"];
  String extra_reward = obj["data"]["is_extra_task_reward_received"];
  String zby_rec_day = obj["data"]["transformer"]["recovery_time"]["Day"];
  String zby_rec_hour = obj["data"]["transformer"]["recovery_time"]["Hour"];
  String zby_rec_min = obj["data"]["transformer"]["recovery_time"]["Minute"];

  int shuzhi = current_resin.toInt(); //将字符串变量转换为整形
  String Which_day;
  String str_hour;
  String str_min;

  int Rectime = resin_recovery_time / 60 + Hour * 60 + Min; //定义恢复时间
  int Rechour = Rectime / 60;
  int Recmin = Rectime % 60;

  Which_day = (Rechour < 24) ? "今天" : "明天";
  Rechour = (Rechour > 24) ? Rechour % 24 : Rechour;
  str_hour += Rechour;
  str_min += Recmin;
  str_hour = (Rechour < 10) ? "0" + str_hour : str_hour;
  str_min = (Recmin < 10) ? "0" + str_min : str_min;
  str_hour = (current_resin != "null") ? str_hour : "??";
  str_min = (current_resin != "null") ? str_min : "??";

  deserializeJson(doc, gd_return_text);
  JsonObject lives = doc["lives"][0];
  String weather = lives["weather"].as<String>();
  String temperature = lives["temperature"].as<String>();
  String humidity = lives["humidity"].as<String>();
  String winddirection = lives["winddirection"].as<String>();
  String windpower = lives["windpower"].as<String>();
  windpower = (windpower == "≤3") ? "<=3" : windpower;


  spr.createSprite(240, 177);
  spr.pushImage(0, 0, 240, 177, (uint16_t *)gImage_BG_bottom);
  spr.loadFont(Font);
  spr.setTextColor(tft.color565(83, 108, 188), TFT_WHITE);
  spr.drawString(weather, 22, 7);
  spr.drawString(temperature + "度", 102, 7);
  spr.drawString("湿度" + humidity + "  " + winddirection + "风" + windpower + "级", 22, 30);

  spr.setTextColor(tft.color565(157, 35, 172), TFT_WHITE);
  spr.drawString(current_resin + "/160 树脂", 22, 77);
  spr.drawString(Which_day + " " + str_hour + ":" + str_min + " 回满", 22, 100);
 if (extra_reward == "false")
    spr.drawString("今日委托奖励未领！",22,123);
  else
    {
      if(zby_rec_hour == "0" && zby_rec_day == "0")
        spr.drawString("质变仪 " + zby_rec_min + " 分钟", 22, 123);
      else if(zby_rec_min == "0" && zby_rec_day == "0")
        spr.drawString("质变仪 " + zby_rec_hour + " 小时", 22, 123);
      else
        spr.drawString("质变仪 " + zby_rec_day + " 天", 22, 123);
    }
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
    spr.drawString(month + "." + date, 3, 2);
    spr.drawString(timeClient.getFormattedTime(), 66, 2);
    spr.drawString(day[timeClient.getDay()], 174, 2);
    if (WiFi.status() == WL_CONNECTED)
      spr.pushImage(215, 0, 23, 23, (uint16_t *)gImage_WIFI_G);
    else
      spr.pushImage(215, 0, 23, 23, (uint16_t *)gImage_WIFI_R);
    spr.pushSprite(0, 0);
    spr.unloadFont();
    spr.deleteSprite();
    delay(1000);
  }
  i = 0;
}
