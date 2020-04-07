#include "mbed.h"

Serial pc( USBTX, USBRX );
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
int sample = 256;
int i;
float ADCdata[256];
//for frequency calculate
int TimeState =0;
float InitialTime = 0.0;
float FinalTime = 0.0;
float FrequencySum = 0.0;
int ZeroNum = 1;
int frequency = 0;


// 7 segment display setup( suppose frequency < 999 )
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
char table1[3]

// LED switch setup
InterruptIn Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);

// Simulately running: Switch(with 7 segment display)+ print to python
Thread thread1;
Thread thread2;

void wave_thread(){
  while(true){
    for (i = 0; i < sample; i++){
      Aout = Ain;
      ADCdata[i] = Ain;
      wait(1./sample);
    }
    for (i = 0; i < sample; i++){ // print to python file
      pc.printf("%1.3f\r\n", ADCdata[i]);
      if (ADCdata[i] == 0 && TimeState == 0){
        TimeState = 1;
        InitialTime = time(NULL);
      }
      else if(ADCdata[i] == 0 && TimeState == 1){
        TimeState = 0;
        FinalTime = time(NULL);
      }
      if (InitialTime !=0 && FinalTime != 0){
        FrequencySum += (FinalTime-InitialTime);
        InitialTime = 0;
        FinalTime = 0;
        ZeroNum++;
      }
      wait(0.01);
    }
    frequency = round(FrequencySum/ZeroNum);
  }
}

void table_thread(){
  while(true){
    int hun = floor(frequency/100);
    int ten = floor((frequency-hun*100)/10);
    int one = frequency-hun*100-ten*10;
    table1[3]={table[hun-1], table[ten-1], table[one-1]};
  }
}

int main(){
  //Switch LED
  redLED = 1;
  greenLED = 1;
  while(1){
    //Calculate sine wave frequency
    thread1.start(wave_thread);
    //Save frequency value as table
    thread2.start(table_thread);
  // 7 segment display
    if( Switch == 0 ){
      greenLED = 1;
      redLED = 0;
      for (int j = 0; j<3; j = j+1){
        display = table1[j];
        wait(1);
      }
    }
    else{
      redLED = 1;
      greenLED = 0;
      for (int j = 0; j<3; j = j+1){
        display = 0x00;
        wait(1);
      }
    }
  }
}