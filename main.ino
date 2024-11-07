#include "TemperatureReceiver.h"
#include "DisplayModule.h"
#include "LEDModule.h"

void setup() {
  Serial.begin(115200);
  setupTemperatureReceiver(); 
  setupDisplayModule();      
  setupLEDModule();         
}

void loop() {
  int temperature = getTemperatureFromAPI();
  displayTemperature(temperature);
  changeLEDColor(temperature);  
}
