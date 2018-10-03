/*--------------------------------------------------------------
  Program:      battery_tester

  Description:  Reads analog value on pin A3 that is connected to a voltage divider to
                read the voltage of the battery. A fully charged battery is about 12.5 Vdc.
                Displays the battery charge in percentage on a small 128 x 64 oled display when the battery
                is connected.

  Hardware:     Arduino Nano

  Software:     Developed using Atom with the Platformio package

  Date:         28 September 2018
  Author:       Eddie L Hill
  --------------------------------------------------------------*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


#define NUM_SAMPLES 10                // number of samples taken of the battery voltage measurement
#define VoltageDivider1 11.006        // voltage divide ratio
#define timeDelay 1000                // delay in ms to when the voltage is measured when load is applied, 4 sec
#define switchPin 6                   // mode switch input pin
#define RX 0                          // RX mode
#define TX 1                          // TX mode


int sum = 0;                          // sum of samples taken
int firstMeas = 0;                    // first measurement from the A/D
unsigned char sample_count = 0;       // current sample number
float voltage = 0.0;                  // calculated voltage
int buttonState = 0;                  // variable for reading the state of the start push button switch
float voltPercentage;                 // the voltage in percentage
int switchVal = 0;                    // switch value
bool mmode;                           // mode place holder

/************************************* start of program **********************************************************/

void setup()
{
  // initialize serial port.
  // serial port used for displaying the voltage
  // and other things for debugging purposes
  Serial.begin(9600);
  // initialize display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  display.display();
  pinMode(switchPin, INPUT_PULLUP);           // input for toggle switch input
}

/************************************ start of main loop ********************************************************/

void loop()
{

  // read the mode switch
  switchVal = digitalRead(switchPin);
  if (switchVal)
    mmode = TX;
  else
    mmode = RX;

  voltage = 0;          // reset voltage variable

  // take a number of 10 analog samples and add them up and average
  firstMeas = analogRead(A3);         // throw away first measurement per Atmel data sheet
  while (sample_count < NUM_SAMPLES) {
    sum += analogRead(A3);
    sample_count++;
    delay(10);
  }

  // calculate the voltage
  voltage = ((float)sum / (float)(NUM_SAMPLES) * 5.01) / 1024.0;

  // calculate battery voltage
  voltage = voltage * VoltageDivider1;

  // set the mode (TX or RX) according to the mode switch position
  if (!mmode)
    voltPercentage = voltage / 12.38;  // 12.38 vdc is a fully charged battery of RX
  else
    voltPercentage = voltage / 13.59;  // 13.59 vdc is a fully charged battery of TX

  // calculate the percentage of voltage
  voltPercentage = voltPercentage * 100;

  // text display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // display the voltage measured in percentage
  display.println("Battery voltage:");
  display.println("");
//display.print("level:");
//display.print((int)voltPercentage, DEC);
  //display.print("%");
  display.setTextSize(2);

  display.print((float)voltage,2);
  display.print(" V");
  display.display();
  // wait here
  while (1);
}

/***************************************** end of main loop ***************************************************/

/***************************************** end of program *****************************************************/
