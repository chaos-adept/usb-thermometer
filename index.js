const five = require('johnny-five')
const notifier = require('node-notifier')

const argv = require('optimist')
  .usage('Usage: $0 --baseTemp [num] --maxIncrease [num] --serialPort <PORT>')
  .demand(['baseTemp', 'maxIncrease'])
  .default({baseTemp: 22, maxIncrease: 7})
  .argv
const baseTemperature = argv.baseTemp

const serialPort = argv.serialPort
const maxTemperatureIncreasing = argv.maxIncrease
const maxTemperature = baseTemperature + maxTemperatureIncreasing
console.log(`Base temperature: base ${baseTemperature.toFixed(2)} °C, max ${maxTemperature.toFixed(2)} °C`)

const board = new five.Board({port: serialPort, repl: false})

const ledPins = [3, 4, 5]
const alarmLedPin = 6
const ledCount = ledPins.length
const ledMaxBrightness = 255
let isAlarmOn = false

board.on('ready', function () {
  // This requires OneWire support using the ConfigurableFirmata
  const thermometer = new five.Thermometer({
    controller: 'DS18B20',
    pin: 2
  })

  console.log('Led pins', ledPins)
  console.log('Alarm led pin', alarmLedPin)
  const leds = new five.Leds([...ledPins]) // clone because five mutate/erase array
  const alarmLed = new five.Led(alarmLedPin)

  thermometer.on('change', function () {
    const tempPercent = (this.celsius - baseTemperature) / maxTemperatureIncreasing
    console.log(`${this.celsius.toFixed(2)} / ${maxTemperature.toFixed(2)} °C  | ${(tempPercent * 100).toFixed(2)} %`)

    for (let indx = 0; indx < ledCount; indx++) {
      const led = leds[indx]
      const ledPercentTo = (indx + 1) / ledCount
      const ledPercentFrom = (indx) / ledCount
      const brightness = Math.min(ledMaxBrightness * (tempPercent - ledPercentFrom) / ledPercentTo, ledMaxBrightness);

      (tempPercent > ledPercentFrom) ? (led.on() && led.brightness(brightness)) : led.off()
    }

    if (tempPercent >= 1) {
      !isAlarmOn && notifier.notify({
        title: 'The Temperature Alarm',
        message: `Temperature is ${this.celsius.toFixed(2)} / Max is ${maxTemperature.toFixed(2)} °C`
      })
      alarmLed.on()

      isAlarmOn = true
    } else {
      alarmLed.off()
      isAlarmOn = false
    }
  })
})
