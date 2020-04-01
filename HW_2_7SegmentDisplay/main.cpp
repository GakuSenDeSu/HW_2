#include "mbed.h"

Serial pc( USBTX, USBRX );

// Push wave to picoscope
AnalogOut Aout(DAC0_OUT);

// Read wave from picoscope
AnalogIn Ain(A0);
int sample = 256;
float ADCdata[256];
int i;
int TimeState =0;
float InitialTime = 0.0;
float FinalTime = 0.0;
float FrequencySum = 0.0;
int ZeroNum = 1;
int frequency = 0;


// 7 segment display setup( suppose frequency < 999 )
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[3] = {0x3F, 0x3F, 0xBF};

// LED switch setup
DigitalIn  Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);

int main(){
  //Calculate sine wave frequency
  redLED = 1;
  greenLED = 1;
  while(1){
    for (i = 0; i < sample; i++){
      Aout = Ain;
      ADCdata[i] = Ain;
      if (Ain == 0 && TimeState == 0){
        TimeState = 1;
        InitialTime = time(NULL);
      }
      else if(Ain == 0 && TimeState == 1){
        TimeState = 0;
        FinalTime = time(NULL);
      }
      if (InitialTime !=0 && FinalTime != 0){
        FrequencySum += (FinalTime-InitialTime);
        InitialTime = 0;
        FinalTime = 0;
        ZeroNum++;
      }
      wait(1./sample);
    }
    for (i = 0; i < sample; i++){
      pc.printf("%1.3f\r\n", ADCdata[i]);
      wait(0.1);
    }
    frequency = round(FrequencySum/ZeroNum);
  
    //Save frequency value as table--------------------------
    int hun = floor(frequency/100);
    int ten = floor((frequency-hun*100)/10);
    int one = floor((frequency-hun*100-ten*10));
    switch (hun){
      case 0:
        table[1]={0x3F};
        break;
      case 1:
        table[1]={0x06};
        break;
      case 2:
        table[1]={0x5B};
        break;
      case 3:
        table[1]={0x4F};
        break;
      case 4:
        table[1]={0x66};
        break;
      case 5:
        table[1]={0x6D};
        break;
      case 6:
        table[1]={0x7D};
        break;
      case 7:
        table[1]={0x07};
        break;
      case 8:
        table[1]={0x7F};
        break;
      case 9:
        table[1]={0x6F};
        break;
    }
    switch (ten){
      case 0:
        table[2]={0x3F};
        break;
      case 1:
        table[2]={0x06};
        break;
      case 2:
        table[2]={0x5B};
        break;
      case 3:
        table[2]={0x4F};
        break;
      case 4:
        table[2]={0x66};
        break;
      case 5:
        table[2]={0x6D};
        break;
      case 6:
        table[2]={0x7D};
        break;
      case 7:
        table[2]={0x07};
        break;
      case 8:
        table[2]={0x7F};
        break;
      case 9:
        table[2]={0x6F};
        break;
    }
    switch (one){
      case 0:
        table[3]={0xBF};
        break;
      case 1:
        table[3]={0x86};
        break;
      case 2:
        table[3]={0xDB};
        break;
      case 3:
        table[3]={0xCF};
        break;
      case 4:
        table[3]={0xE6};
        break;
      case 5:
        table[3]={0xED};
        break;
      case 6:
        table[3]={0xFD};
        break;
      case 7:
        table[3]={0x87};
        break;
      case 8:
        table[3]={0xFF};
        break;
      case 9:
        table[3]={0xEF};
        break;
    }

  //LED switch and 7 segment display--------------------------------------
    if( Switch == 1 ){
      greenLED = 1;
      redLED = 0;
      for (int j = 0; j<3; j = j+1){
        display = table[j];
        wait(1);
      }
    }
    else{
      redLED = 1;
      greenLED = 0;
    }
  }
}