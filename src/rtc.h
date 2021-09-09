#ifndef DS1302RTC_h
#define DS1302RTC_h
#include <stdint.h>
#include<ATTinyCore.h>

#define DS1302_SCLK_PIN   PA5
#define DS1302_IO_PIN     PA6
#define DS1302_CE_PIN     PA2


typedef struct
{
    //time
  uint8_t sec;
  uint8_t min;
  uint8_t hour;
  uint8_t weekDay;
  //Date
  uint8_t day;
  uint8_t month;
  uint8_t year;  
}rtc_t;




//principals functions 

uint8_t ds1302_read(uint8_t adress);
uint8_t ds1302_write(uint8_t adress, uint8_t data) ;
void ds1302_init() ;
void ds1302_setdatetime(rtc_t rtc);
void ds1302_getdatetime(rtc_t *rtc);
uint8_t  ds1302_validation (rtc_t t);

 uint8_t bcdToDec (uint8_t bcd);
 uint8_t decTobcd (uint8_t dec);

 #endif