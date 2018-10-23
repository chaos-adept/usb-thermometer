#include <DallasTemperature.h>

#include <OneWire.h>
 
#define PIN_DS18B20 2
#define PIN_LED_1 3
#define PIN_LED_2 4
#define PIN_LED_3 5
#define PIN_LED_ALARM 6

int led_pins[] = {PIN_LED_1, PIN_LED_2, PIN_LED_3, PIN_LED_ALARM};
int PIN_LED_COUNT = 4;

float baseTemperature = 15;
float maxTemperature = 24;
 

OneWire oneWire(PIN_DS18B20);

DallasTemperature dallasSensors(&oneWire);

DeviceAddress sensorAddress;
 
void setup(void){

  Serial.begin(9600);
  Serial.println("Temperature sensor with DallasTemperature");

  for (int i = 0; i < PIN_LED_COUNT; i++) {
    pinMode(led_pins[i], OUTPUT);
    digitalWrite(led_pins[i], HIGH);

  }
   
  
  Serial.print("search device...");
  dallasSensors.begin();
  Serial.print("device has been found ");
  Serial.print(dallasSensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  Serial.print("power spy supply method: ");
  if (dallasSensors.isParasitePowerMode()) Serial.println("enbaled");
  else Serial.println("disabled");

  //sensorAddress = { 0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF0 };

  if (!dallasSensors.getAddress(sensorAddress, 0)) Serial.println("device cant be found");

  Serial.print("device address: ");
  printAddress(sensorAddress);
  Serial.println();

  dallasSensors.setResolution(sensorAddress, 12);
 
  Serial.print("Sensor resolution: ");
  Serial.print(dallasSensors.getResolution(sensorAddress), DEC);
  Serial.println();
}
 
void loop(void){

  Serial.print("Measuring temperature...");
  dallasSensors.requestTemperatures(); 
  Serial.println("done");

  printTemperature(sensorAddress);

  delay(1000);
}

void printTemperature(DeviceAddress deviceAddress){
  float tempC = dallasSensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.println(tempC);

  float maxTemperatureIncreasing = maxTemperature - baseTemperature;
  int ledCount = PIN_LED_COUNT-1;
  float ledMaxBrightness = HIGH;
  float tempPercent = (tempC - baseTemperature) / maxTemperatureIncreasing;
  Serial.println(tempPercent);
  for (int indx = 0; indx < ledCount; indx++) {
    int led = led_pins[indx];
    float ledPercentTo = (float)(indx + 1) / (float)ledCount;
    float ledPercentFrom = (float)(indx) / (float)ledCount;
    (tempPercent > ledPercentFrom) ? digitalWrite(led, HIGH) : digitalWrite(led, LOW);
  }

  tempPercent > 1 ? digitalWrite(PIN_LED_ALARM, HIGH) : digitalWrite(PIN_LED_ALARM, LOW);
}
 

void printAddress(DeviceAddress deviceAddress){
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
