/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _RTC_H    /* Guard against multiple inclusion */
#define _RTC_H
#define DS3231_ADDRESS 0x68

typedef struct {
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;
} rtc_t;



void initRTC(void);
void readRTC(rtc_t *cl);
void setalarm1(rtc_t *cl);
void setRTCalarm_per_minute();
void clearAlarm();
uint16_t getTemp();
void setRTC(rtc_t *cl);


#endif /* _RTC_H */

/* *****************************************************************************
 End of File
 */
