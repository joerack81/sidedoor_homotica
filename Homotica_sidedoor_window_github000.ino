#include <ezButton.h>
#include <Homotica.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SimpleTimer.h>
#include <neotimer.h>


ezButton button(D3);  // create ezButton object that attach to pin 7;
const int wdoor = D5;  // The number of the window door mosfet pin
const int ledon = D7;  // The number of the led

//MODIFY THESE PARAMETERS
const char* ssid = "JOENET";                             //your wifi ssid
const char* password = "";                     //your wifi password


  int cycle=1;
IPAddress ip(10,0,0,180);                             //your arduino IP
IPAddress gateway(10,0,0,1);                         //your network gateway
IPAddress subnet(255, 255, 255, 0);                        //your network subnet mask

unsigned int mUdpPort = 5858;
static String code = "qawsedrf";
Homotica homotica;


Neotimer mytimer = Neotimer(2400); // 1 second timer

void setup() {
  Serial.begin(74800);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    
   pinMode(ledon, OUTPUT);
   digitalWrite(ledon, LOW);

   
 homotica.addUsedPinOutput(D5);
 homotica.setActiveLow();  //<-- only if you need it
 homotica.set(mUdpPort, code);
 
  }
  
  button.setDebounceTime(50); // set debounce time to 50 milliseconds
  button.setCountMode(COUNT_RISING );

}
  
void loop() 
{
  

   homotica.refresh();
    button.loop(); // MUST call the loop() function first
    unsigned long count = button.getCount();
    int btnState = button.getState();

    Serial.println("counts");
    Serial.println(count);

    Serial.println("btnState");
    Serial.println(btnState);
    

  if(button.isPressed())
  {
    Serial.println("The button is pressed");
    mytimer.start();
    digitalWrite(ledon,HIGH);    
  }

  if(mytimer.done())   
 {
    Serial.println("DONE");
    digitalWrite(ledon,LOW);
    mytimer.reset();
    button.resetCount(); 

 }
 
  if(mytimer.waiting())
  {
    
      if (btnState == 1 &&  count > 1 )
          {
                Serial.println("PRESSED");
                digitalWrite(wdoor, LOW);      //opendoor
                delay(2000);     //give time to turn the handle and open the door 
                digitalWrite(wdoor, HIGH);    //close it
          }

  }
    

}    
      
   
    
    


  
    
