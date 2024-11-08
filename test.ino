void setup() {
  Serial.begin(115200);
  Serial.println("Please enter the data in the format: temperature,condition (e.g., 25,Sunny)");
  //setupTemperatureReceiver();
  setupDisplayModule();
  setupLEDModule();
}

void loop() {
  if (Serial.available() > 0) {  
    String input = Serial.readStringUntil('\n');

    Serial.println("Received: " + input);
    Serial.println("Please use the format: temperature,condition (e.g., 25,Sunny)");

    int temperature = 0;
    String condition = "";

    int commaIndex = input.indexOf(',');
    if (commaIndex > 0) {
      temperature = input.substring(0, commaIndex).toInt();
      condition = input.substring(commaIndex + 1);
      Serial.println("tempreture: " + temperature);
      Serial.println("tcondition: " + condition);
      displayTemperature(temperature);
      changeLEDColor(condition);
    } else {
      Serial.println("Invalid format. Please use: temperature,condition");
    }
  }
}
