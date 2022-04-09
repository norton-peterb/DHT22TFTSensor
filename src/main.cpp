#include <Arduino.h>
#include <dht_nonblocking.h>
#include <TFT.h>
#include <SPI.h>

#define DHT_SENSOR_TYPE DHT_TYPE_22
#define CS 53
#define DC 9
#define RST 8

static const int DHT_SENSOR_PIN = 5;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
TFT TFTscreen = TFT(CS,DC,RST);

float temperature;
float humidity;
char valBuffer[8];
char valBuffer2[8];
char buffer[24];

static bool measure_environment(float *temperature, float *humidity, DHT_nonblocking *sensor)
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( sensor->measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}

void setup() {
  TFTscreen.begin();
}

void writeDataTFT() {
  TFTscreen.background(0x20,0x00,0x00);
  TFTscreen.stroke(0xFF,0xFF,0xFF);
  TFTscreen.setTextSize(2);
  TFTscreen.text("DHT READING",10,0);
  dtostrf(temperature,-1,1,valBuffer);
  dtostrf(humidity,-1,1,valBuffer2);
  sprintf(buffer,"%s C",valBuffer);
  TFTscreen.setTextSize(3);
  TFTscreen.text(buffer,30,30);
  sprintf(buffer,"%s %%",valBuffer2);
  TFTscreen.text(buffer,30,70);
}

void loop() {
  while( measure_environment( &temperature, &humidity, &dht_sensor ) == false ) {
  }
  writeDataTFT();
  delay(60000);
}
