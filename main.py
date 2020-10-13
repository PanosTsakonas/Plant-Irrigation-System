from machine import Pin,ADC
from time import sleep
import network
import machine
from ntptime import settime
import utime
import dht 
import urequests
import gc
gc.collect()
sec=1000
sta_if = network.WLAN(network.STA_IF)
sta_if.active(True)
sta_if.connect('Your SSID', 'Your Password')
print("Connecting to WiFi..........")
while not sta_if.isconnected():
            pass
print("Connected to WiFi")
sleep(1)
settime()
utime.localtime()
api_key='IFTTT API KEY'
wa=Pin(13,Pin.IN)
soil_inp=Pin(18, Pin.OUT)
soil=ADC(Pin(36))
sensor=dht.DHT22(machine.Pin(23))
soil.atten(ADC.ATTN_11DB) #set 11dB input attenuation (voltage range roughly 0.0v - 3.6v)
soil_inp.value(0)
wa.value(1)
led1=Pin(2,Pin.OUT)
led1.value(1)
sleep(3)
soil_hum=0
soil_hum1=0
temp=0
air_hum=0

while True:
 
 ts=utime.localtime()
 print(ts)
 hour=ts[3]+3 #This is the time offset in Greece.
 minutes=ts[4]
 month=ts[1]
 print(hour,minutes,month)
 if month<=9 and month>=5:
     n=1
 else:
     n=2
 if hour==17:
     soil_inp.value(1)    
     soil_hum1= soil.read()
     soil_inp.value(0)
     while soil_hum1/2000 <=80:
       wa.value(0)
       soil_inp.value(1)    
       soil_hum1= soil.read()
       soil_inp.value(0)
       sleep(30)
     wa.value(1)
     retry = 0
     while retry < 3:
        try:
            sensor.measure()
            break
        except:
            retry = retry + 1
            print(".", end = "")

     print("")       
     temp=sensor.temperature()
     air_hum=sensor.humidity()
     soil_inp.value(1)    
     soil_hum1= soil.read()
     soil_inp.value(0)
     soil_hum1=soil_hum1/(2000)
     led1.value(0)
     readings={"value1": temp,"value2": air_hum,"value3": soil_hum1}
     request_headers={'Content-Type': 'application/json'}
     request=urequests.post(
         'https://maker.ifttt.com/trigger/DHT22/with/key/'+api_key,
         json=readings,
         headers=request_headers)
     request.close()
     sleep(1)
     ts=utime.localtime()
     minutes1=ts[4]
     machine.deepsleep(sec*(3600*24-(60-minutes1)*60)*n)
 else:
     if hour<=16:
         h_dif=16-hour
         m_dif=60-minutes
     else:
         h_dif=24-(hour-17)
         m_dif=60-minutes
     print("Going to sleep for",h_dif,"hours and",m_dif,"minutes")
     machine.deepsleep(sec*(60*m_dif+3600*h_dif))
