const int PIN_RED = 26;
const int PIN_GREEN = 25;
const int PIN_BLUE = 34;

void setupLEDModule() {

  //CONFIGURE PINS TO OUTPUT
  pinMode (PIN_RED, OUTPUT);
  pinMode (PIN_GREEN, OUTPUT);
  pinMode (PIN_BLUE, OUTPUT);

}

void setColor(int R, int G, int B) {
  //Function to make changing the color easier
  analogWrite (PIN_RED, R);
  analogWrite (PIN_GREEN, G);
  analogWrite (PIN_BLUE, B);
}

void changeLEDColor(String forecast){
  //main function for changing colors based on inputs from weather
  setColor(0, 0, 0); //start with LED off


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
    setColor(0, 102, 255); //LED will be blue: color code #0066ff
  }

  //if snow
  if (forecast.equalsIgnoreCase(snow) || forecast.equalsIgnoreCase(rainandsnow) || forecast.equalsIgnoreCase(snowshower)){
    setColor(255, 255, 255); //LED will be white: color code #ffffff
  }

  //if sunny
  if (forecast.equalsIgnoreCase(clearsky) || forecast.equalsIgnoreCase(fair) || forecast.equalsIgnoreCase(sunny)){
    setColor(255, 255, 0); //LED will be yellow: color code #ffff00
  }

  //if cloudy
  if (forecast.equalsIgnoreCase(cloudy) || forecast.equalsIgnoreCase(mostlycloudy) || forecast.equalsIgnoreCase(partlycloudy)){
    setColor(166, 166, 166); //LED will be grey: color code #a6a6a6
  }





}
