
#include "rtc.h"
#include<Arduino.h>
#include<ATTinyCore.h>
#define DS1302_SCLK_PIN   PA5
#define DS1302_IO_PIN     PA6
#define DS1302_CE_PIN     PA2
 rtc_t rtc ; 
void setup()
{

 ds1302_init() ;
rtc.sec=30;
rtc.min=41;
rtc.hour=6;
rtc.weekDay=4; // Thuesday 
rtc.day=2;
rtc.month=9; //September
rtc.year=21;
ds1302_setdatetime(rtc);
int x ;
x=ds1302_validation();
if (x=0)
{
    Serial.println("the time is valid ")
}
else 
{
    Serial.println("the time is incorrect");
}

}
void loop ()
{
ds1302_getdatetime(&rtc);

}