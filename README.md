# H20 mister timer
Simple repeat timer to control a solenoid valve. I used this to control a water misting system I set up on the patio to make it bearable in the Texas heat.  The problem is you can't leave the misters on all the time because you get soaked but you really don't want to get out of the hammock to keep turning it on and off. 

Code is for the Shelly 1 Relay Module (https://shelly.cloud/products/shelly-1-smart-home-automation-relay/) but should work with any ESP8266 dev board with minor modifications. 

The interface is designed to work with the Blynk (https://blynk.io/) app so you can control the device easily from your smart phone.  

You can flash the code with the Arduino IDE (https://www.arduino.cc/en/main/software) with the appropriate ESP8266 board manager package installed.   You will need a USB to serial UART programmer, an Arduino, or some other MCU to program the Shelly 1 Relay Module.  

Credit to GTT (https://community.blynk.cc/u/gtt/summary) for most of the code for the nested timers. 

I used these products:
https://www.amazon.com/gp/product/B08611N214/ref=ppx_yo_dt_b_asin_title_o03_s00?ie=UTF8&psc=1
https://www.amazon.com/gp/product/B016MP1HX0/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1
https://www.amazon.com/gp/product/B07G33LNDY/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1




