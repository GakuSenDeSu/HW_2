#include "mbed.h"

Serial pc( USBTX, USBRX );
// Push wave to picoscope
AnalogOut Aout(DAC0_OUT);
// Read wave from picoscope
AnalogIn Ain(A0);
int sample = 128;
int i;
float ADCdata[128];
int ZeroNum =0;
// 7 segment display setup
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
// LED switch setup
DigitalIn  Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);

int main(){
  while(1){
      //calculate sine wave frequency
      for (i = 0; i < sample; i++){
        Aout = Ain;
        ADCdata[i] = Ain;
        if (Ain==0 && TimeState = 0){
          int TimeState=1;
          float InitialTime=time(NULL);
        }
        else if(Ain ==0 && TimeState =1){
          TimeState=0;
          float FinalTime=time(NULL);
        }
        if (InitialTime !=0 && FinalTime !=0){
          float FrequencySum += FinalTime-InitialTime;
          ZeroNum++;
        }
        wait(1./sample);
      }
      int frequency = rint(FrequencySum/ZeroNum);
      for (i = 0; i < sample; i++){
        pc.printf("%1.3f\r\n", ADCdata[i]);
        
        wait(0.1);
        }
      //change frequency to table => table[3]={,,}

      //LED switch and 7 segment display
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