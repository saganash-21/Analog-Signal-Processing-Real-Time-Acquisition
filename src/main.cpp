#include <Arduino.h>

#define ldr_pin 35
#define red_pin 5
#define blue_pin 18
#define green_pin 19
#define sound_pin 32



void setup() {
  // put your setup code here, to run once:
  pinMode(ldr_pin, INPUT);
  pinMode(red_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(sound_pin, INPUT);
  Serial.begin(115200);
}

void loop() {
  // int soundData = analogRead(sound_pin);
  // Serial.print(">Sound_Data: ");
  // Serial.println(soundData);



  // static unsigned long lastLDRupdate = 0;
  // if((millis() - lastLDRupdate) >= 20){
  //   int ldrData = analogRead(ldr_pin);
  //   float voltage = ldrData * (3.3 / 4095);

  //   if(voltage < (1.1 - 0.05)){
  //     digitalWrite(red_pin, HIGH);
  //     digitalWrite(blue_pin, LOW);
  //     digitalWrite(green_pin, LOW);
  //   }
  //   else if(voltage < (2.2 - 0.05) && voltage >= (1.1 + 0.05)){
  //     digitalWrite(blue_pin, HIGH);
  //     digitalWrite(red_pin, LOW);
  //     digitalWrite(green_pin, LOW);   
  //   }
  //   else if (voltage < 3.3 && voltage >= 2.2){
  //     digitalWrite(green_pin, HIGH);
  //     digitalWrite(blue_pin, LOW);
  //     digitalWrite(red_pin, LOW);
  //   }

  //   // Serial.print(">LDR_Raw: ");
  //   // Serial.println(ldrData);
  //   Serial.print(">Voltage: ");
  //   Serial.println(voltage);
  // }

 

  unsigned long startMillis = millis();
  int peakToPeak = 0;
  int signalMax = 0;
  int signalMin = 4095;

  // 1. Collect sound data for 50ms
  while (millis() - startMillis < 50) {
    int sample = analogRead(sound_pin);
    if (sample < 4095) { // filter out outlier spikes
      if (sample > signalMax) signalMax = sample;
      else if (sample < signalMin) signalMin = sample;
    }
  }
  
  // 2. Calculate the "Volume" (Amplitude)
  peakToPeak = signalMax - signalMin; 
  float soundVolt = analogRead(sound_pin) * (3.3/ 4095);

  // 3. Plotting
  Serial.print(">Sound_Voltage:");
  Serial.println(soundVolt); 
  Serial.print(">Sound_Intensity:"); 
  Serial.println(peakToPeak); // 

  
  int ldrData = analogRead(ldr_pin);
  float voltage = ldrData * (3.3 / 4095.0);
    if(voltage < (1.1 - 0.05)){
      digitalWrite(red_pin, HIGH);
      digitalWrite(blue_pin, LOW);
      digitalWrite(green_pin, LOW);
    }
    else if(voltage < (2.2 - 0.05) && voltage >= (1.1 + 0.05)){
      digitalWrite(blue_pin, HIGH);
      digitalWrite(red_pin, LOW);
      digitalWrite(green_pin, LOW);   
    }
    else if (voltage < 3.3 && voltage >= 2.2){
      digitalWrite(green_pin, HIGH);
      digitalWrite(blue_pin, LOW);
      digitalWrite(red_pin, LOW);
    }  
 
  Serial.print(">LDR_Voltage:");
  Serial.println(voltage);



}


