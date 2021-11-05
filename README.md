# SONOFF-SENSORS

This is a generic software for sonoff basic r2 and r3, mini, s20 and also for self build ESP8266-01 switches. Depending on the defines at the start you can make a choice.
The program has a lot of smart features. All settings can be done via the webinterface. The state of the sensors and/or the switch is communicated via mqtt. Iit is a very nice alternative for tasmota, because this can be operated by anyone.

See it in action on youtube: https://youtu.be/zvR9k_P_OLU

## features
- very easy to understand and operate, help available on most pages.
- Easy wifi config portal
- 4 timers with extensive options
- ota software update
- can switch on temperature, moist, light, motion and generic digital sensor
- works with 7 different sensors
- can be switched via mqtt and http
- Fast asyc webserver
- Smart timekeeping
- A lot of system info on the webpage
- state of the switch and sensor values are sent by mqtt

## compile this sketch
Use arduino ide with the esp822 version 2.7.1 installed under boardmanager. The ota updates won't work with other versons.

Or use one of the binary files to flash directly on your ESP device. If you flash a WEMOS you can test it in a very easy way.

## the hardware
It is nothing more than a Sonoff r2 / r3 / s20 / mini with a header soldered on it to be able to flash it. How to do this is easy to google.
If you want to use a sensor, you also need the header. Here is the wiring diagram.

![connections](https://user-images.githubusercontent.com/12282915/139238631-da64f4d7-3c4c-4940-8107-e4137dd748a7.jpg)

The version TRIAC is an esp8266 that controls a triac to switch a mains lamp.  Here is the schematic.![ipswitch_timer](https://user-images.githubusercontent.com/12282915/140542416-c3e9c617-79ae-4633-90b7-2a6650e32e30.png)




