#include "mbed.h"    //mbed-os-5.15.4
#include "TextLCD.h" //change default to lcm1602

#define ECHO D3
#define TRIG D2
#define GND  D10
#define VCC  D8
#define BUTTON D9

DigitalIn  echo(ECHO);
DigitalOut trig(TRIG, 0);
DigitalOut gnd(GND, 0);
DigitalOut vcc(VCC, 1);

I2C i2c(I2C_SDA, I2C_SCL);
Serial pc(USBTX, USBRX);
TextLCD_I2C lcd(&i2c, 0x4E, TextLCD::LCD20x2, TextLCD::HD44780);
Timer t;

InterruptIn interrup(BUTTON);
static bool run = true;
void changeMode(){
    run=!run;
}
int getCm(){
    t.reset();
    while (echo);
    trig = 1;
    wait_us(10);
    trig = 0;
    while(!echo);
    t.start();
    while(echo);
    t.stop();
    return t.read_us()/58;
}
int main(){
    interrup.rise(changeMode);
    lcd.cls();
    lcd.setBacklight(TextLCD::LightOn);
    lcd.setCursor(TextLCD::CurOff_BlkOff);
    lcd.printf("Distance=");
    while (true) {
        lcd.setAddress(0, 1);
        if(run){
            lcd.printf("Run "); 
            lcd.setAddress(9, 0);
            lcd.printf("%dcm    ", getCm()); 
            wait_us(100000);
            }
            else lcd.printf("Stop");
    }
}
