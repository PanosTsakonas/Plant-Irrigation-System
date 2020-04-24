# Plant-Watering

This project was build for fun and it uses the NodeMcu 32s for the host microcontroller, alongside a waveshare soil moisture sensor 
a DHT22 temperature/humidity sensor and a valve connected to the water supply. This projects aims to automate the watering of plants using the above mentioned sensors
add WiFi to get the actual date/time and send the data from the sensors to a Thingspeak webpage for monitoring the state of the garden.
The data sent are the temperature, air humidity, soil humidity expressed as a percentage, soil humidity raw sensor value and the number
of times the plants were watered.

The NodeMcu 32s is an ESP32 board with 38 pins and can output both 3.3 and 5 VDC. Below you can find a link to the pinout diagram of the board.

https://grobotronics.com/images/companies/1/NodeMCU-32S-pinout.jpg?1567071865358

The board is powered from a normal plug of 220 VAC and through a transformer and a rectifier the voltage drops to 5VDC and it is hooked to the VIN 5V pin of the ESP32
and a ground. This makes it ideal to use with any plug in your garden!

Once you set your WiFi SSID and Password you have to create a new channel on ThingSpeak. Once you do that copy the Write API key to the
code and you are set! You can download a ThnigSpeak widget on your phone (I know it works for Andriod devices haven't try the Apple phones yet)
and you can see the sensor readings as soon as they are uploaded to the ThingSpeak channel!

Once you set everything and all the sensors are connected then you can turn the system on. The principal of the system is that it connects to the Internet
get the time in GMT (you have to modify the time offset for your country) and by default the system will start getting sensor data once the time is 17:00.
If the soil moisture is less than 80% the valve connected to the water supply will open and it will start watering the plants. Every minute
the soil moisture sensor will take a reading. The process is terminated when either the soil moisture reading is higher than 80% or after 5 minutes of watering.

This is a fail safe mechanism for when the soil moisture sensor needs to be replaced.

Then the soil moisture sensor data alongside the temperature, air humidity and number of waterings data will be uploaded to your ThingSpeak channel and can be viewed from
your mobile device if you have the widget.

Then the ESP32 will go into deep sleep waking up every 30 minutes and changing a counter value. As it is set right now from September till May (From Autumn though Spring) the system will check the soil moisture every 2 days
and from the end of Spring (May) till the beginning of Autumn (September) it will check the state of the soil moisture every day.
One issue i had during the development was that the ESP32 that was used for this project had an isue with long deep sleep. Ideally i wanted to make the
ESP32 sleep for 24 or 48 hours based on the season, yet that was proven to be impossible. The longest I could make the sensor sleep was for 30 minutes so I developed the rest of the 
code with that in mind. If you have a different ESP32 breakout board make sure to test if it can sleep for longer periods of time and then you can modify the code.

For the 1st of May and the 1st of September the system will take sensor measurements regardless of when the last watering took place. Again if your breakout board can handle 24 or 48 hour deep sleeps
then this wont be an issue.

# Hookup Guide

I have uploaded a JPEG file with a Fritzing schematic of my Hookup. 

# Soil Moisture Sensor

Connect the VCC to ESP32 pin 18.
Connect the GND to a ground.
Connect the Aout/SIG to ESP32 pin 36.
The reason why we connect the VCC of this sensor to pin 18 is that we control when the sensor is getting data. These types of soil moisture sensor tend to stop working quite fast (they cost around 5-6 euros so you can understand why) and controlling the power on time can increase the life expectancy of the module. A word of caution is that when you use these types of sensors the output value is just an analog reading. You basically get a number that corresponds to the resistance of the two metal rods of the sensor. The more water between the two the higher the reading. So in order to get percentage value you have to calibrate them beforehand. Calibration is really easy and in the code I have added a line for "auto" calibration. That means that from my initial calibration i got a reading value when the sensor was completely soaked in water of around 
1812. I used this value to start the system but when the output of the sensor is hgher than that value, i compensate for that by taking the mean of the two. That's my "auto-calibration". That value is saved as an RTC_DATA_ATTR variable meaning that it won't be lost during deep sleep but when the system is restarted either by taking the plug out or from a power outage then that value is lost. Perhaps you can save that value to the ESP32 EEPROM and never worry about that but honestly it wasn't such a big deal; but there is also this option if anyone wants to explore it.

# DHT22 Temperature and Humidity Sensor

To fascilitate the reader imagine that you are holding the DHT22 with the grid layout facing you. Then the most left pin is the VCC and the most right pin is the Ground.

Connect VCC pin (most left) to ESP32 3.3 VDC.
Connect the digital singal pin of the DHT22 to ESP32 pin 23 and you have to add a 10 KOhm resistor that connects the 3.3 VDC with the digital signal pin as well. (See my schematic for more info).
The next pin is not connected.
Connect the ground pin (most right) to ESP32 ground.

# 5 VDC 1 channel relay

Connect the signal pin of the realy to ESP32 pin 13.
Connect the Ground replay pin to ESP32 ground.
Connect the Power realy pin to ESP32 5VDC.

The way i have setup the code for the relay switch you have to use the common and normal open as exits to open and close the
valve.

For the hookup section that is everything. Now let's have a look at the code.

# Code

This part is the TLDR version of my code. Firstly as mentioned in the soil moisture sensor during deep sleep all values that are not "saved" are lost. There are two ways to save values either by saving them to an EEPROM or using the RTC_DATA_ATTR. What we achieve with this is that during sleep all values that have the RTC_DATA_ATTR before them will continue to have the value they had before the system goes to sleep. This is quite handy and important as we can save the "auto-calibration" value and the counters for knowing how long before the next watering session begins. I wont get into more details for the RTC_DATA_ATTR but i strongly suggest you read the following guide which is quite useful (https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/). So the system starts and it waits to connect to your WiFi. Once this process is done then it gets the time and date. It checks if the hour is 17 (it uses the 24 hour format!) and if k=48. k is the counter that keeps growing every 30 minutes. If that is the case then it checks the temperature, air humidity and soil humididty. If the soil humidity is less than 80% it opens the valve through the relay and starts watering. Every 1 minute the system checks the soil humidity value. This happens maximum 5 times (5 minutes) at which point the valve closes and the system reports the soil humidity tempreature and air humidity values and total amount of waterings. If the soil humidity is greater than 100% then the "auto-calibration" step occurs and the value is adjusted. The counter is set to k=0 and the esp32 sleeps. The first time you start the system it doesn't go through the first if statement and that is done via the RTC_DATA_ATTR int l=0; The reason why this exists is that the first time the system is booted you don't care about the k counter and also allows for the system to get measurements on the 1st of May and September, where the transition to a different counter step is implemented. If your ESP32 can sleep for 24 or 48 hours then this whole section is irrelevant to you can you have to modify it accordingly. When the time is not 17 then the k counter is increase by a predefined step, and the ESP32 sleeps for another 30 minutes. A word of advice for the esp_deep_sleep() function. When this function is called the ESP32 goes to sleep. The argument inside has to be expressed in μs (10^-6 secs). So if you are trying to see if your breakout board can handle the 24 or 48 hour deep sleep then use the following argument with my code: esp_deep_sleep(microsec*timeStamp1*k) where k in that case will be either 24 or 48 and change timeStamp1=3600-60*timeStamp1+20. Modify the remaining code as you see fit. 

# Conclusion

To conclude firstly i hope that you found my explantions helpful. Secondly the whole project, because it consists of cheap sensors costs roughly 25 euros and i got a junction box for another 15 euros. The cost of the valve depends on what you are getting, but I already had an old one and that was one of the reasons that i begun this project. So a rough estimate is that you can make the whole project with 70 euros, which is quite nice and cheap.
