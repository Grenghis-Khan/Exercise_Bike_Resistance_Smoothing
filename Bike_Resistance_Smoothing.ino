#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int fsrAnalogPin = A0; // FSR is connected to analog 2
int fsrReading;      // the analog reading from the FSR resistor divider
int fsrScaled;
int fsrScaledOld;
int fsrScaledBnc;
int counter = 0;
 
Adafruit_SSD1306 display = Adafruit_SSD1306();
 
#if defined(ESP8266)
  #define BUTTON_A 0
  #define BUTTON_B 16
  #define BUTTON_C 2
  #define LED      0
#elif defined(ESP32)
  #define BUTTON_A 15
  #define BUTTON_B 32
  #define BUTTON_C 14
  #define LED      13
#elif defined(ARDUINO_STM32F2_FEATHER)
  #define BUTTON_A PA15
  #define BUTTON_B PC7
  #define BUTTON_C PC5
  #define LED PB5
#elif defined(TEENSYDUINO)
  #define BUTTON_A 4
  #define BUTTON_B 3
  #define BUTTON_C 8
  #define LED 13
#elif defined(ARDUINO_FEATHER52)
  #define BUTTON_A 31
  #define BUTTON_B 30
  #define BUTTON_C 27
  #define LED 17
#else // 32u4, M0, and 328p
  #define BUTTON_A 9
  #define BUTTON_B 6
  #define BUTTON_C 5
  #define LED      13
#endif
 
#if (SSD1306_LCDHEIGHT != 32)
 #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// Define the number of samples to keep track of for smoothing. The higher the number, the
// more the readings will be smoothed, but the slower the output will respond to
// the input. Using a constant rather than a normal variable lets us use this
// value to determine the size of the readings array.
const int numReadings = 100;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0; 
 
void setup() {  
  Serial.begin(9600);

  // initialize all the smoothing readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
 
  Serial.println("OLED FeatherWing test");
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  Serial.println("OLED begun");


  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);
 
  // Clear the buffer.
  display.clearDisplay();
  display.display();
  
  Serial.println("IO test");
 
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

 
  // text display tests
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  fsrScaledOld=fsrScaled;
  /*display.print("Connecting to SSID\n'adafruit':");
  display.print("connected!");
  display.println("IP: 10.0.1.23");
  display.println("Sending val #0");
  display.setCursor(0,0);
  display.display(); // actually display all of the above
  */
}
 
 
void loop() {
  /*if (! digitalRead(BUTTON_A)) display.print("A");
  if (! digitalRead(BUTTON_B)) display.print("B");
  if (! digitalRead(BUTTON_C)) display.print("C");*/

  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(fsrAnalogPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  Serial.println(average);
  delay(1);        // delay in between reads for stability
  
  //delay(1000);
  //display.clearDisplay();
  //fsrReading = analogRead(fsrAnalogPin);
  fsrReading = average;
  Serial.print("Resistance Level = ");
  fsrScaled = (fsrReading*0.07);
  Serial.println(fsrScaled);
  //display.println("Resist Lvl");
  display.setCursor(50, 0);
    display.setTextColor(BLACK);
    display.print(fsrScaledOld);
    display.print(fsrScaled);
    display.setCursor(50, 0);
    display.setTextColor(WHITE);
    display.print(fsrScaled);
    //fsrScaledOld=fsrScaled;

    
  //if(fsrScaled==fsrScaledOld)
 /* if(counter < 5)
  {
    display.setCursor(50, 16);
    display.setTextColor(BLACK);
    display.print(fsrScaled);
    display.print(fsrScaledOld);
    display.setCursor(50, 16);
    display.setTextColor(WHITE);
    display.println(fsrScaledOld);
    
    
   }
   /*else if(counter < 5)
   {
      if(fsrScaled > fsrScaledBnc + 3 && fsrScaled < fsrScaledBnc - 3)
      {
        display.setCursor(50, 16);
        display.setTextColor(BLACK);
        display.print(fsrScaled);
        //display.print(fsrScaledOld);
        display.setCursor(50, 16);
        display.setTextColor(WHITE);
        display.println(fsrScaledBnc);
        //counter = 0;
      }
   }
   else{
    display.setCursor(50, 16);
    display.setTextColor(BLACK);
    //display.print(fsrScaled);
    display.print(fsrScaledOld);
    display.setCursor(50, 16);
    display.setTextColor(WHITE);
    display.print(fsrScaled);
    fsrScaledOld=fsrScaled;
  }*/

  display.setCursor(0,0);
  delay(100);

  yield();
  display.display();
  // Clear the buffer.
  //display.clearDisplay();
  //display.display();
fsrScaledOld=fsrScaled;
counter = counter + 1;
if (counter > 5)
  {
    counter = 0;
    //fsrScaledBnc = fsrScaled;
  }
  
}

