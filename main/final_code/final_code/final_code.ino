#include <Ticker.h>
#include "config.h"
#include <AdafruitIO_WiFi.h>

// ------------------------- Constants and Initialization -------------------------
Ticker timer;

int D1 = 14;
int D2 = 15;
int D3 = 33;
int D4 = 13;
int latchPin = 12;
int clockPin = 27;
int dataPin = 14;

int count = 0;
int numbers[4];
int cathodePins[] = {14, 15, 33, 13};

byte table[10] = {
  B11111100, B01100000, B11011010, B11110010, B01100110,
  B10110110, B10111110, B11100000, B11111110, B11110110
};

float fahrenheitValue = 0.0; 
String precipitationCondition = ""; 

// Adafruit IO & WiFi
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
AdafruitIO_Feed *fahrenheitdisplay = io.feed("fahrenheitdisplay");
AdafruitIO_Feed *weatherconditions = io.feed("weatherconditions");

// LED Pin Definitions
const int PIN_BLUE_LED = 5;  
const int PIN_WHITE_LED = 18; 
const int PIN_RED_YELLOW = 26;
const int PIN_GREEN_YELLOW = 25;
const int PIN_BLUE_YELLOW = 4;
const int PIN_RED_GRAY = 16;
const int PIN_GREEN_GRAY = 17;
const int PIN_BLUE_GRAY = 21;

// ------------------------- Setup Functions -------------------------
void setupDisplayModule() {
  Serial.begin(115200);
  pinMode(D4, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  digitalWrite(D4, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D1, HIGH);
  Serial.println("Enter a number between 0 and 9999");
}

void setupTemperatureReceiver() {  
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(1000);  // Short delay
  Serial.println("Starting setup...");

  // Test WiFi connection
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" Connected to WiFi!");

  // Connect to Adafruit IO
  Serial.print("Connecting to Adafruit IO...");
  io.connect();

  // Wait for Adafruit IO connection to complete
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    io.run();  // Maintain the connection process by calling io.run()
    delay(500);
  }
  Serial.println(" Connected to Adafruit IO!");

  // Set up the message handler for the feed
  fahrenheitdisplay->onMessage(handleMessage);
  weatherconditions->onMessage(handleMessageprecipitation);
}

void setupRGBLED(int redPin, int greenPin, int bluePin) {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void setupLEDModule() {

  pinMode(PIN_BLUE_LED, OUTPUT);
  pinMode(PIN_WHITE_LED, OUTPUT);

  setupRGBLED(PIN_RED_YELLOW, PIN_GREEN_YELLOW, PIN_BLUE_YELLOW);
  setupRGBLED(PIN_RED_GRAY, PIN_GREEN_GRAY, PIN_BLUE_GRAY);
  turnOffAllLEDs();
}

// ------------------------- Main Setup and Loop -------------------------
void setup() {
  Serial.begin(115200);
  setupTemperatureReceiver(); 
  setupDisplayModule();      
  setupLEDModule();         
}

void loop() {

  io.run();
  
  int temperature = getTemperatureFromAPI();
  String condition = getConditionFromAPI();

  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Condition: ");
  Serial.println(condition);

  displayTemperature(temperature);
  changeLEDColor(condition);  

  delay(5000); 
}

// ------------------------- Display Functions -------------------------
void displayTemperature(int temperature) {
  timer.detach();
  screenOff();
  separate(temperature);
  timer.attach_ms(1, Display);
}

void separate(long num) {
  numbers[0] = num / 1000;
  num = num % 1000;
  numbers[1] = num / 100;
  num = num % 100;
  numbers[2] = num / 10;
  numbers[3] = num % 10;
}

void Display() {
  screenOff();
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, table[numbers[count]]);
  digitalWrite(cathodePins[count], LOW);
  digitalWrite(latchPin, HIGH);
  count++;
  if (count == 4) {
    count = 0;
  }
}

void screenOff() {
  digitalWrite(D4, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D1, HIGH);
}

// ------------------------- LED Control Functions -------------------------
void setColor(int redPin, int greenPin, int bluePin, int R, int G, int B) {
  analogWrite(redPin, R);
  analogWrite(greenPin, G);
  analogWrite(bluePin, B);
}

void turnOffAllLEDs() {
  digitalWrite(PIN_BLUE_LED, LOW);
  digitalWrite(PIN_WHITE_LED, LOW);
  setColor(PIN_RED_YELLOW, PIN_GREEN_YELLOW, PIN_BLUE_YELLOW, 0, 0, 0);
  setColor(PIN_RED_GRAY, PIN_GREEN_GRAY, PIN_BLUE_GRAY, 0, 0, 0);
}

void changeLEDColor(String forecast){
  turnOffAllLEDs();

  //the following strings store the varous IFTTT weather report words I've discovered so far
  String rain = String("Rain");
  String lightrain = String("Light Rain");
  String rainshower = String ("Rain Shower");
  String snow = String("Snow");
  String cloudy = String("Cloudy");
  String mostlycloudy = String("Mostly Cloudy");
  String partlycloudy = String("Partly Cloudy");
  String clearsky = String("Clear");
  String fair = String("Fair");
  String sunny = String("Sunny");
  String rainandsnow = String("Rain and Snow");
  String snowshower = String("Snow Shower");

  //if raining
  if (forecast.equalsIgnoreCase(rain) || forecast.equalsIgnoreCase(lightrain) || forecast.equalsIgnoreCase(rainshower)){
    digitalWrite(PIN_BLUE_LED, HIGH); //LED will be blue: color code #0066ff
  }

  //if snow
  if (forecast.equalsIgnoreCase(snow) || forecast.equalsIgnoreCase(rainandsnow) || forecast.equalsIgnoreCase(snowshower)){
    digitalWrite(PIN_WHITE_LED, HIGH);//LED will be white: color code #ffffff
  }

  //if sunny
  if (forecast.equalsIgnoreCase(clearsky) || forecast.equalsIgnoreCase(fair) || forecast.equalsIgnoreCase(sunny)){
    setColor(PIN_RED_YELLOW, PIN_GREEN_YELLOW, PIN_BLUE_YELLOW, 250, 100, 192); //LED will be yellow: color code #ffff00
  }

  //if cloudy
  if (forecast.equalsIgnoreCase(cloudy) || forecast.equalsIgnoreCase(mostlycloudy) || forecast.equalsIgnoreCase(partlycloudy)){
    setColor(PIN_RED_GRAY, PIN_GREEN_GRAY, PIN_BLUE_GRAY, 166, 166, 166);//LED will be grey: color code #a6a6a6
  }





}

// ------------------------- Adafruit IO Handlers -------------------------

// This function is called every time fahrenheitdisplay gets a new value
void handleMessage(AdafruitIO_Data *data) {
  fahrenheitValue = data->toFloat();
  Serial.println("Data point read from Fahrenheit feed:");
  
  // Print the data as a float if the feed contains numeric data
  Serial.print("Fahrenheit: ");
  Serial.println(fahrenheitValue); // Use toFloat if the feed contains numbers
  // If your feed contains text, use this instead
}
void handleMessageprecipitation(AdafruitIO_Data *data){
  precipitationCondition = data->toString();
  Serial.println("Data point read from precipitation feed");
  Serial.print("Weather Condtions:");
  Serial.println(precipitationCondition);  // pritns out text related data
}

int getTemperatureFromAPI() {
  return static_cast<int>(fahrenheitValue); 
}

String getConditionFromAPI() {
  return precipitationCondition;
}

