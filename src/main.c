#include <wiringPi.h>
#include <lcd.h>
#include <string.h> /* for strncpy */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <stdio.h>
#include <errno.h>
#include <wiringPiI2C.h>
//USE WIRINGPI PIN NUMBERS
#define LCD_RS  25               //Register select pin
#define LCD_E   24               //Enable Pin
#define LCD_D4  23               //Data pin 4
#define LCD_D5  22               //Data pin 5
#define LCD_D6  21               //Data pin 6
#define LCD_D7  14               //Data pin 7

int main()
{
        int fd;
        int8_t result;
        fd = wiringPiI2CSetup(0x4a);
        result = wiringPiI2CReadReg8(fd, 0);
        char buf[64];
        char p[64];
        snprintf(buf, sizeof(buf), "%d", result);
        getIP(p);
        wiringPiSetup();
        pinMode (5, OUTPUT);
        int lcd = lcdInit (2, 16, 4,  11,10 , 0,1,2,3,0,0,0,0);
        while(1){
                sleep(3);
                lcdHome (lcd);
                lcdClear (lcd);
                result = wiringPiI2CReadReg8(fd, 0);
                snprintf(buf, sizeof(buf), "%d", result);
                if (result < 20)  digitalWrite (5, HIGH);
                lcdPuts(lcd, "TEMP: ");
                lcdPuts(lcd,buf);
                lcdPosition(lcd, 0, 1);
                lcdPuts(lcd, p);
        }
        return 0;
}

void getIP(char* outStr){
        FILE *fp;
        char returnData[64];
        int count = 0;
        fp = popen("/sbin/ifconfig wlan0", "r");
        while (fgets(returnData, 64, fp) != NULL)
        {
                if (count == 2){
                        pclose(fp);
                        char *p;
                        p = strtok(returnData, " ");
                        int maxDi = 0;
                        for(int i=0;i<strlen(p);i++){
                                if(isdigit(p[i])){
                                        maxDi = i+1;
                                };
                        };
                        for(int i=0;i<maxDi;i++){
                                outStr[i] = p[i];
                        };
                        outStr[maxDi] = '\0';
                };
                count++;
        }
        pclose(fp);
}