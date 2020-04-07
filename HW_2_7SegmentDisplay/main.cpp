#include "mbed.h"

Serial pc( USBTX, USBRX );
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
int sample = 1024;
int i;
float ADCdata[1024];
//for frequency calculate
int TimeState =0;
Timer InitialTime;
Timer FinalTime;
float initial = 0.0;
float final = 0.0;
float FrequencySum = 0.0;
int ZeroNum = 0;
int frequency = 0;


// 7 segment display setup( suppose frequency < 999 )
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
char table1[3] = {0x3F,0x3F,0xBF};

// LED switch setup
DigitalIn Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);

// Simulately running: Switch(with 7 segment display)+ print to python
Thread thread1;
Thread thread2;
Thread thread3;

void wave_thread(){
  while(true){
    for (i = 0; i < sample; i++){
      Aout = Ain;
      ADCdata[i] = Ain;
      wait(1./sample);
    }
    for (i = 0; i < sample; i++){ // print to python file
      //pc.printf("%1.3f\r\n", ADCdata[i]);
      InitialTime.start();
      FinalTime.start();
      if (ADCdata[i] == 0.3 && TimeState == 0){
        TimeState = 1;
        initial = InitialTime.read();
        InitialTime.reset();
      }
      else if (ADCdata[i] == 0.3 && TimeState == 1){
        TimeState = 0;
        final = FinalTime.read();
        FinalTime.reset();
      }
      if (initial !=0 && final != 0){
        FrequencySum += (FinalTime-InitialTime);
        initial = 0;
        final = 0;
        ZeroNum++;
      }
      wait_us(100000);
    }
    frequency = 100;
    // round(FrequencySum/ZeroNum);
    pc.printf("%7.3f\r\n", frequency);
  }
}

void table_thread(){
  while(true){
    int hun = floor(frequency/100);
    int ten = floor((frequency-hun*100)/10);
    int one = frequency-hun*100-ten*10;
    table1[0] = table[hun];
    table1[1] = table[ten];
    table[2] = 0x80|table[one];
  }
}

void segment_thread(){
  while (true){
    if( Switch == 0 ){
      greenLED = 1;
      redLED = 0;
      for (int j = 0; j<3; j = j+1){
        display = table1[j];
        wait_us(100000);
      }
    }
    else{
      redLED = 1;
      greenLED = 0;
      for (int j = 0; j<3; j = j+1){
        display = 0x00;
        wait_us(100000);
      }
    }
  }
}

int main(){
  redLED = 1;
  greenLED = 1;
  //Calculate sine wave frequency
  thread1.start(wave_thread);
  //Save frequency value as table
  thread2.start(table_thread);
  // 7 segment display
  thread3.start(segment_thread);
}