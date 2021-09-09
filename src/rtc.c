#include "rtc.h"
#include <util/delay.h>
#include "Arduino.h"
#include <ATTinyCore.h>
#define DS1302_READBIT           0
#define DS1302_SECONDS           0x80
#define DS1302_MINUTES           0x82
#define DS1302_HOURS             0x84
#define DS1302_WEEKDAY           0x86
#define DS1302_MONTH             0x88
#define DS1302_DAY               0x8A
#define DS1302_YEAR              0x8C
#define DS1302_ENABLE            0x8E
//#define DS1302_TRICKLE           0x90
//#define DS1302_CLOCK_BURST       0xBE
//#define DS1302_CLOCK_BURST_WRITE 0xBE
//#define DS1302_CLOCK_BURST_READ  0xBF
//#define DS1302_RAM_START         0xC0
//#define DS1302_RAM_END           0xFC
//#define DS1302_RAM_BURST         0xFE
//#define DS1302_RAM_BURST_WRITE   0xFE
//#define DS1302_RAM_BURST_READ    0xFF

//how to start the communication
void start (void) {
	digitalWrite(DS1302_CE_PIN , LOW); //set CE pin off 
	pinMode(DS1302_CE_PIN, OUTPUT);  
	
	digitalWrite(DS1302_SCLK_PIN, LOW);
	pinMode(DS1302_SCLK_PIN, OUTPUT);
	
	pinMode(DS1302_IO_PIN, OUTPUT);
	
	digitalWrite(DS1302_CE_PIN, HIGH);
	delayMicroseconds(3);
}
//to stop the communication 
void stop (void) {
	digitalWrite(DS1302_CE_PIN, LOW);
	delayMicroseconds(3); 
}
//
byte toggleread(void) {
	byte i, data;
	
	data = 0;
	for( i = 0; i <= 7; i++) {
		digitalWrite(DS1302_SCLK_PIN, HIGH);
		delayMicroseconds(1);
	
		digitalWrite(DS1302_SCLK_PIN, LOW);
		delayMicroseconds(1);
		//write a bit of a numeri cvariable 
		bitWrite( data, i, digitalRead(DS1302_IO_PIN)); 
	}
	return data;
}

void togglewrite (byte data, byte var) {
	int i;
	
	for( i = 0; i <= 7; i++) { 
		digitalWrite(DS1302_IO_PIN, bitRead(data, i));  
		delayMicroseconds(1);
		//clk high = 
		digitalWrite(DS1302_SCLK_PIN, HIGH);     
		delayMicroseconds(1);
		
		if( var && i == 7) {
			pinMode(DS1302_IO_PIN, INPUT);
		} else {
			digitalWrite(DS1302_SCLK_PIN, LOW);
			delayMicroseconds(1); 
		}
	}
}
//read the time from rtc
uint8_t read (int address) {
	uint8_t    data;
	bitSet(address, DS1302_READBIT);  
	
	start();
	togglewrite(address, true);  
	data = toggleread();
	stop();
	
	return data;
}
//set time and date from structure
void write ( uint8_t address, uint8_t data) {
	bitClear(address, DS1302_READBIT);   
	
	start();
	togglewrite(address, false); 
	togglewrite(data, false); 
	stop();  
}

void ds1302_init(){
	write(DS1302_ENABLE,  0);
	write(DS1302_SECONDS, 0); 
	write(DS1302_MINUTES, 0);
	write(DS1302_HOURS, 0);
	write(DS1302_WEEKDAY, 0);
	write(DS1302_DAY, 0);
	write(DS1302_MONTH, 0);
	write(DS1302_YEAR, 0);
}


//test of validity of the time&date
uint8_t  ds1302_validation (rtc_t t){
if(t.sec >= 0 && t.sec <= 59)
    if(t.min >= 0 && t.min <= 59)
      if(t.hour >= 0 && t.hour <= 23)
	  if(t.weekDay >= 1 && t.weekDay <= 7)
        if(t.day >= 1 && t.day <= 31)
          if(t.month >= 1 && t.month <= 12)
              if(t.year >= 0 && t.year <= 99)
               return 0;                   // Success

  return 1 ;
 // Error
}

//functions for conversion 
 uint8_t bcdToDec (uint8_t bcd){
return ((bcd/16*10)+(bcd % 10));
}
 uint8_t decTobcd (uint8_t dec){
	return ((dec /10 * 16)+  (dec %10));
}
//set the time and date
void ds1302_setdatetime(rtc_t rtc){

	write(DS1302_HOURS,decTobcd(rtc.hour));
	write(DS1302_MINUTES,decTobcd(rtc.min));
    write(DS1302_SECONDS,decTobcd(rtc.sec));
	write(DS1302_WEEKDAY, decTobcd(rtc.weekDay));
    write(DS1302_DAY,    decTobcd(rtc.day))	;
    write(DS1302_HOURS,   decTobcd(rtc.month));
	write(0x82,decTobcd(rtc.year));	
}


//read the current time and time from rtc
void ds1302_getdatetime(rtc_t *rtc){
    rtc->sec = bcdToDec(read(DS1302_SECONDS));
	rtc->min = bcdToDec(read(DS1302_MINUTES));
	rtc->hour = bcdToDec(read(DS1302_HOURS));
	rtc->weekDay =bcdToDec(read(DS1302_WEEKDAY));
	rtc->month =bcdToDec(read(DS1302_MONTH));
    rtc->day =bcdToDec (read(DS1302_DAY));
    rtc->year =bcdToDec(read(DS1302_YEAR));
	pinMode(DS1302_CE_PIN, OUTPUT);
	digitalWrite(DS1302_CE_PIN, LOW);

};
