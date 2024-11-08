void setup() {
  Serial.begin(115200);
  setupTemperatureReceiver(); 
  setupDisplayModule();      
  setupLEDModule();         
}

void loop() {
  int temperature = getTemperatureFromAPI();
  String condition = getConditionFromAPI();
  displayTemperature(temperature);
  changeLEDColor(condition);  
}
