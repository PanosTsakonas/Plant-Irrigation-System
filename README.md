# Plant-Watering

This project was build for fun and it uses the NodeMcu 32s for the host microcontroller, alongside a waveshare soil moisture sensor and 
a DHT22 temperature, humidity sensor and a valve connected to the water supply. This projects aims to automate the watering of plants using the above mentioned sensors
add WiFi to get the actual time and send the data from the sensors to a Thingspeak webpage for monitoring the state of the garden.
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

