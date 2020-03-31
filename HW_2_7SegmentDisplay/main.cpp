#include "mbed.h"

Serial pc( USBTX, USBRX );
// Push wave to picoscope
AnalogOut Aout(DAC0_OUT);
// Read wave from picoscope
AnalogIn Ain(A0);
int TimeState = 0;
float InitialTime = 0.0;
float FinalTime = 0.0;
float frequency = 0.0;
// 7 segment display setup
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
// LED switch setup
DigitalIn  Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);

int main(){
  while(1){
      //calculate sine wave frequency
      if (Ain == 0 && TimeState ==0){
        InitialTime = time(NULL);
        TimeState = 1;
      }
      else if (Ain == 0 && TimeState ==1){
          FinalTime =time(NULL);
          TimeState = 0;
      }
      frequency = 2*(FinalTime-InitialTime);

      //change frequency to table => table[3]={,,}
      Aout = sin(2*3.14159/frequency);//Ain should be frequency
      wait(0.001);
      if( Switch == 1 ){ //LED setup
      greenLED = 0;
      redLED = 1;
      for (int i = 0; i<3; i = i+1){
          display = table[i];
          wait(1);
          }
      }
      else{
      redLED = 0;
      greenLED = 1;
      }
  }
}