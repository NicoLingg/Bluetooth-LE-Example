/*********************************************************************
 This is an example for our nRF51822 based Bluefruit LE modules

 Pick one up today in the adafruit shop! 

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution

 Modified by Nico Lingg (2017) for the Bluetooth LE UART Example
*********************************************************************/

#include <Arduino.h>
#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "Adafruit_SHT31.h"

#include "BluefruitConfig.h"
#include "Wire.h"

/*------------------------------------------------------------------------*/
#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/


/* SHT31 default address */
#define SH31_ADDR 0x44


/* Create the bluefruit object, either software serial...uncomment these lines  */
/* !! AdafruitBluefruitLE nRF51 Library required !! */
SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);


/* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
// Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


/* Create Adafruit_SHT31 object - !! Adafruit_SHT31 Library required !! */
Adafruit_SHT31 sht31 = Adafruit_SHT31();


/* Globale enum (= states) for state machines */
enum state_LED {LED_OFF, LED_ON_WAIT, LED_ON, LED_OFF_WAIT} s_LED = LED_OFF;
enum state_Sensor {SENSOR_WAIT, SENSOR_READ_DATA, SENSOR_SEND_DATA} s_Sensor = SENSOR_WAIT;

/* Flags for LED 'State Machine' */
int FlagLED=0;

/* Counter for Sensor 'state Machine' */
int Counter=0; // state machine every 10 ms -> 3 secs -> Counter = 300;

/* Global Variables */
float t;
float h;
int temperature;
int humidity;


/* A Small Helper */
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

/**************************************************************************/
/*!
    @brief  This function sets up the hardware, the BLE module and 
            the SHT31 (this function is called automatically on startup)
*/
/**************************************************************************/
void setup(void)
{
  /* disable global interrupts */ 
    
  /* apparently needed for Flora & Micro (Adafruit) */
  while (!Serial);  
  delay(500);

  /* Initialize serial port for debugging */ 
  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit Command Mode Example"));
  Serial.println(F("---------------------------------------"));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  /* Initialize timer for interrupt - 100Hz */
  //timer1_init();
  
  /* Initialize digital pin LED_BUILTIN as an output and set it to low */
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Test - delete
 pinMode(2, OUTPUT);
 pinMode(3, OUTPUT);
 pinMode(4, OUTPUT);
  
  /* Initialize SHT31 temperature and humidity Sensor */
  Serial.println("Initialising SHT31");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }


  /* Initialize the BLE module */
  Serial.print(F("Initialising the Bluefruit LE module: "));
  
  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){ 
      delay(200);
      //try again
      if(!ble.factoryReset()){
        error(F("Couldn't factory reset"));
      }
    }
  }

  /* Disable command echo from Bluefruit - we don't need that */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  /* User interaction is required - connect to QT application */
  Serial.println(F("Please use Qt application to connect with the BLE module"));
  Serial.println();

  ble.verbose(false);
  
  /* Wait until Qt application is connected - flash LED to indicate state */
  while (! ble.isConnected()) {
    
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(200);
  }
  digitalWrite(LED_BUILTIN, LOW);

  /* LED Activity command is only supported from 0.6.6 */
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("******************************"));
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
    Serial.println(F("******************************"));
  }

}

/**************************************************************************/
/*!
    @brief  Main loop 
*/
/**************************************************************************/
void loop(void)
{
  SM_LED();
  SM_SENSOR();
  //delay(250);
}

/**************************************************************************/
/*!
    @brief  'State Machine' for build-in LED - this function turns on and 
             off the built-in LED
*/
/**************************************************************************/
void SM_LED()
{    
   /* Check for incoming characters */
   digitalWrite(2, digitalRead(2) ^ 1); // toggle LED pin
    ble.println("AT+BLEUARTRX");
    ble.readline();
    if (strcmp(ble.buffer, "OK") == 0){
      // no data
    }
    
    /* Some data was found, its in the buffer */
    Serial.print(F("[Recv] ")); Serial.println(ble.buffer);

    /* Check what was sent and set LED (1==LED_ON, 0==LED_OFF) */
    if(strcmp(ble.buffer, "LED_ON") == 0){
      FlagLED = 1;
    }
    if(strcmp(ble.buffer, "LED_OFF") == 0){
      
      FlagLED = 0;
    }
    /* check response status */
    ble.waitForOK();
    
    switch(s_LED)
    {
      case LED_OFF:
        digitalWrite(LED_BUILTIN, LOW);
        s_LED = LED_ON_WAIT;
        break;

      case LED_ON_WAIT:
        if(FlagLED==1) s_LED=LED_ON;
        break;
             
      case LED_ON:
        digitalWrite(LED_BUILTIN, HIGH);
        s_LED = LED_OFF_WAIT;
        break;

      case LED_OFF_WAIT:
        if(FlagLED==0) s_LED=LED_OFF;
        break; 
    }
  
}

/**************************************************************************/
/*!
    @brief  'State Machine' for the SHT31 temperature and humidity sensor
             This function reads the sensor values and sends it via
             Bluetooth LE to the Central
*/
/**************************************************************************/
void SM_SENSOR()
{

  switch(s_Sensor)
  {
    /* After three loops change state -> sensor read data (one loop is approximatly 1000ms, because of the sht31-library -> three loops ~ 3000ms -> temperature\humidity measurement every 3 seconds */
    case SENSOR_WAIT:
      if(Counter>=3) 
      {
        s_Sensor = SENSOR_READ_DATA;
        Counter=0; 
      }
      break;

    case SENSOR_READ_DATA:
        t = sht31.readTemperature();
        h = sht31.readHumidity();
        temperature = (int)(t*100);
        humidity = (int)(h*100);
        s_Sensor = SENSOR_SEND_DATA; 
      break;

    case SENSOR_SEND_DATA:
      sensor_send_data();
      s_Sensor = SENSOR_WAIT;
      break;   
  }
  /* increase counter */
  Counter++;
}

/**************************************************************************/
/*!
    @brief  This funtion sends the sensor data via Bluetooth to the Qt-app
*/
/**************************************************************************/
void sensor_send_data(){
  
 if (ble.isConnected())
 {
    /* Send sensor data to PC/QT application */
    ble.print("AT+BLEUARTTX=");
    ble.print("Temp");
    ble.print(temperature);
    ble.print("Hum");
    ble.print(humidity);
    ble.println("XYZ");
    
    /* check response status */
    if (! ble.waitForOK() ) {
        Serial.println(F("Failed to send?"));
      }
 }
 
} 



