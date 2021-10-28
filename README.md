# SONOFF-SENSORS

This is a generic software for sonoff basic r2 and r3, also for self build ESP8266 switches. Depending on the defines at the start you can make a choice.
The program has a lot of smart features. All settings can be done via the webinterface. The state of the sensors and/or the switch is communicated via mqtt.

See it in action on youtube: https://youtu.be/zvR9k_P_OLU

## features
- Easy wifi config portal
- 4 timers with extensive options
- ota software update
- can switch on temperature, moist, light, motion and generic digital sensor
- works with 7 different sensors
- can be switched via mqtt and http
- Fast asyc webserver
- Smart timekeeping
- A lot of system info on the webpage

## compile this sketch
Use arduino ide with the esp822 version 2.7.1 installed under boardmanager. The ota updates won't work with other versons.

Or use one of the binary file to flash directly on your ESP device.

## the hardware
It is nothing more than a Sonoff r2 or r3 with a header soldered on it to be able to flash it. How to do this is easy to google.
If you want to use a sensor, here is the wiring diagram.

![connections](https://user-images.githubusercontent.com/12282915/139238631-da64f4d7-3c4c-4940-8107-e4137dd748a7.jpg)



