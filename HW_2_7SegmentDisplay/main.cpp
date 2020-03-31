#include "mbed.h"

Serial pc( USBTX, USBRX );

// Push wave to picoscope
AnalogOut Aout(DAC0_OUT);

// Read wave from picoscope
AnalogIn Ain(A0);
int sample = 128;
int i;
float ADCdata[128];
int TimeState =0;
float InitialTime = 0.0;
float FinalTime = 0.0;
float FrequencySum = 0.0;
int ZeroNum = 0;
int frequency = 0;

// 7 segment display setup( suppose frequency < 999 )
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[3];

// LED switch setup
DigitalIn  Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);

redLED = 0;
greenLED = 0;

int main(){
  //Calculate sine wave frequency
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
      FrequencySum += FinalTime-InitialTime;
      InitialTime = FinalTime = 0;
      ZeroNum++;
    }
    wait(1./sample);
  }
  frequency = round(FrequencySum/ZeroNum);
  //*
  // Save frequency value as table--------------------------
  int hun = floor(frequency/100);
  int ten = floor((frequency-hun*100)/10);
  int one = floor((frequency-hun*100-ten*10));
  switch (hun){
    case 0:
      table[1]={0x3F};
    case 1:
      table[1]={0x06};
    case 2:
      table[1]={0x5B};
    case 3:
      table[1]={0x4F};
    case 4:
      table[1]={0x66};
    case 5:
      table[1]={0x6D};
    case 6:
      table[1]={0x7D};
    case 7:
      table[1]={0x07};
    case 8:
      table[1]={0x7F};
    case 9:
      table[1]={0x6F};
  }
  switch (ten){
    case 0:
      table[2]={0x3F};
    case 1:
      table[2]={0x06};
    case 2:
      table[2]={0x5B};
    case 3:
      table[2]={0x4F};
    case 4:
      table[2]={0x66};
    case 5:
      table[2]={0x6D};
    case 6:
      table[2]={0x7D};
    case 7:
      table[2]={0x07};
    case 8:
      table[2]={0x7F};
    case 9:
      table[2]={0x6F};
  }
  switch (one){
    case 0:
      table[3]={0xBF};
    case 1:
      table[3]={0x86};
    case 2:
      table[3]={0xDB};
    case 3:
      table[3]={0xCF};
    case 4:
      table[3]={0xE6};
    case 5:
      table[3]={0xED};
    case 6:
      table[3]={0xFD};
    case 7:
      table[3]={0x87};
    case 8:
      table[3]={0xFF};
    case 9:
      table[3]={0xEF};
  }
//*/
  //*
  //LED switch and 7 segment display--------------------------------------
  if( Switch == 1 ){
  greenLED = 0;
  redLED = 1;
  for (int j = 0; j<3; j = j+1){
    display = table[i];
    wait(1);
  }
  }
  else{
  redLED = 0;
  greenLED = 1;
  }
  //*/
  }
}