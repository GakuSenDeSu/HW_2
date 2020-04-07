#include "mbed.h"

Serial pc( USBTX, USBRX );
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
int sample = 256;
int i;
float ADCdata[256];
//for frequency calculation
int TimeState =0;
Timer InitialTime;
Timer FinalTime;
float initial = 0.0;
float final = 0.0;
float FrequencySum = 0.0;
int ZeroNum = 0;
int frequency;


// 7 segment display setup( suppose frequency < 999 )
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
char table_2[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0x87,0xFF, 0xEF};
char table_1[3];

// LED switch setup
DigitalIn Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);

// Simulately running: Switch(with 7 segment display)+ print to python
Thread thread1;
Thread thread2;
Thread thread3;
Thread thread4;

void wave_thread(){
  while(true){
    for (i = 0; i < sample; i++){
      ADCdata[i] = Ain;
      wait(1./sample);
    }
    for (i = 0; i < sample; i++){ // print to python file
      //pc.printf("%1.3f\r\n", ADCdata[i]);
      // Calculate frequency
      InitialTime.start();
      FinalTime.start();
      if (ADCdata[i] == 1.000 && TimeState == 0){
        TimeState = 1;
        initial = InitialTime.read();
        InitialTime.reset();
      }
      else if (ADCdata[i] == 1.000 && TimeState == 1){
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
    frequency = round(FrequencySum/ZeroNum);
  }
}

void table_thread(){
  while(true){
    int hun = floor(frequency/100);
    int ten = floor((frequency-hun*100)/10);
    int one = frequency-hun*100-ten*10;
    table_1[0] = table[hun];
    table_1[1] = table[ten];
    table_1[2] = table_2[one];
    wait_us(100000);
  }
}

void segment_thread(){
  while (true){
    if( Switch == 0 ){
      greenLED = 1;
      redLED = 0;
      for (int j = 0; j<3; j = j+1){
        display = table_1[j];
        wait_us(500000);
      }
    }
    else{
      redLED = 1;
      greenLED = 0;
      for (int j = 0; j<3; j = j+1){
        display = 0x00;
        wait_us(500000);
      }
    }
  }
}

void output_thread(){
  while(true){
    Aout = Ain;
    wait(1./sample);
  }
}

int main(){
  redLED = 1;
  greenLED = 1;
  //Calculate sine wave frequency
  thread1.start(wave_thread);
  //frequency = 100;
  //Save frequency value as table
  thread2.start(output_thread);
  // 7 segment display
  thread3.start(table_thread);
  //output sine wave from K66F to picoscope
  thread4.start(segment_thread);
}