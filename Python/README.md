Finally I have started using Micropython in order to programm my ESP32. I have used the Thonny IDE which is quite nice but there are other IDEs out there so use which one you think is the best for you.
In order for the code to work properly you have to use the same pins i have used previously. I have also added the option to receive an email when the irrigation process has ended using the IFTTT.
This is quite nice and easy way to get things to your email, among other things. So go to their website IFTTT create an account and you are almost ready. Once you have logged in go to Create.
There are 2 things on the webpage you can click.Go to the If this button and click on it. On the search bar type webhooks click on it and click on it again. In the event type name I have used the DHT22 but it can be
anything you want. If you want to stick with mine that is fine. Click on create trigger. Afterwards click on the Then That button. In the search type email and click on the email.
Add the email address where you want the data to be sent to. Once that step is done click on the continue button and you will be redirected to a webpage that will have an API key on top of it.
Copy Paste that api key to the api_key on the main.py. Insert your router's SSID and PASS and the code is ready. Remember that if you are using micropython on your esp32 for the first time you have to 
erase the flash first. This is a process that you have to do at least once and i recommend following the guide here: https://randomnerdtutorials.com/flash-upload-micropython-firmware-esp32-esp8266/
The main advantage susing micropython is that finally i can set my esp32 to deep sleep for 24 hours which has reduced the complexity of the code by a lot!!
