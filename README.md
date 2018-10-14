Temperature Alarmer
======
[![JavaScript Style Guide](https://img.shields.io/badge/code_style-standard-brightgreen.svg)](https://standardjs.com)

It is a basic temperature Alarmer based on the [DS18B20](https://www.aliexpress.com/premium/DS18B20.html?d=y&blanktest=0&origin=y&SearchText=DS18B20&tc=ppc&initiative_id=SB_20181013225623&isViewCP=y&catId=0) sensor and [johnny-five](http://johnny-five.io).
# Pins 
* 6 - Alarm Pin
* 3-5 - Indication of progress reaching the max from the base temperature. Each pin(1-3) gets brightness/pwd signal in one of three ranges
    * 3 led - 0-33%
    * 4 led - 33%-66%
    * 5 led- 66%-100%.
* 2 - Input data from the sensor

*Important*: You need to have _4.7 kOm_ resister. 
Refer to the [schema](http://johnny-five.io/examples/temperature-ds18b20/#breadboard-for-thermometer-ds18b20)


# How to use

1. Install/Deploy [ConfigurableFirmata](https://github.com/firmata/ConfigurableFirmata) on your arduino device
1. Install latest [nodejs](https://nodejs.org/)
1. Run `npm i`
1. Run `npm start -- --baseTemp <number> --maxIncrease <number>`

    for example:
        
        npm run start -- --baseTemp 24 --maxIncrease 0.3
    
    also optionally, You could define `serialPort`, by default it detects automatically
    
        npm run start -- --baseTemp 24 --maxIncrease 0.3 --serialPort COM5
