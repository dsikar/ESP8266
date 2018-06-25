
#include <bitswap.h>
#include <chipsets.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <cpp_compat.h>
#include <dmx.h>
#include <FastLED.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <fastpin.h>
#include <fastspi.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <pixelset.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>

/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 by Daniel Eichhorn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */



#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>



ESP8266WebServer server(80);


struct password_typ{
  const char * ssid;
  const char * password;
  } ;

password_typ The_Passwords[ ] = {
  {"RD WiFi",       "thornfire"},       // work
  {"EE-xffxe5",     "tick-broke-far"},  // house main
  {"EE-xffxe5_EXT", "tick-broke-far"}}; // house extension

const int NUMBER_OF_PASSWORDS = (sizeof(The_Passwords) / sizeof(The_Passwords));

int ln = 0;
String TimeDate = "";

int LastTimeTimeReadFromInternet = 0;

#define DEMO_DURATION 5000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;


String WiFiAddress;

//
// LED strip info 
// START

#define LED_PIN           1 // Pin 0 does not seem to work
#define MIN_NUM_LEDS     30
#define FULL_NUM_LEDS    264
// controlled via web interface.
int Brightness = 20;
#define LED_TYPE    TM1809
#define COLOR_ORDER GRB


CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

CRGBArray<FULL_NUM_LEDS> leds;

//
// LED strip info 
// END


#define MODE_DEMO_OFF   0
#define MODE_DEMO_1     1
#define MODE_DEMO_2     2




int CurrentMode = MODE_DEMO_OFF;


// true -  connects to local router, phones talk through that.
// false - starts as a local server.  phones connect directly to it.
#define CONNECT_TO_LOCAL_WIFI false



//
// AttemptToConnect()
// Allows the higher level to work through list of known routers to connect
//
int AttemptToConnect( int index )
{
  WiFi.begin(The_Passwords[index].ssid, The_Passwords[index].password);

  Serial.println("");
  Serial.print("Trying router : ");
  Serial.print(The_Passwords[index].ssid);
  Serial.println("");  


  int connection_wait = 0;
  
  // Wait for connection
  while ((WiFi.status() != WL_CONNECTED) && (connection_wait < 100)) {
    delay(500);
    Serial.print(".");
    connection_wait++;
  }

  if (WiFi.status() == WL_CONNECTED)
  {  
    Serial.println("");
    Serial.print("Connected to : ");
    Serial.println(The_Passwords[index].ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    WiFiAddress = WiFi.localIP().toString();
  
    delay(1000);
  }

  return WiFi.status();
}


const char* ssid = "Spain pw1-8";  
const char* password = "123456789";  
WiFiServer WiFiserver(5000);  

// http://embeddedlaboratory.blogspot.co.uk/2016/10/control-devices-over-wifi-using-phone.html#!/2016/10/control-devices-over-wifi-using-phone.html
void SetupAsServer()
{
  // Create Access Point  
  WiFi.softAP("Configuting Access Point..");  
  WiFi.softAP(ssid, password);  
  IPAddress myIP = WiFi.softAPIP();  
  Serial.println("Access Point IP is: ");  
  Serial.println(myIP);  
  // Start the server  
  WiFiserver.begin();  
  Serial.println("Server started");  


  WiFiAddress = myIP.toString();
}


void setup_server()
{
  if (MDNS.begin("esp8266")) 
  {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);        // main page
  
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  // Example, if we want more we can add them
  server.on("/Green", []()
  {
    server.send(200, "text/plain", "Okay -- Mode 1!");;
    CurrentMode = MODE_DEMO_1;
  });
  
  server.onNotFound(handleNotFound);

  server.begin();
  
  Serial.println("HTTP server started");
}


void ConnectToWifi()
{
  int password_index = 0;

  // move through routers until we can connect.
  while ( AttemptToConnect(password_index) != WL_CONNECTED)
  {
     password_index++;
     if ( password_index >= NUMBER_OF_PASSWORDS )
       password_index = 0;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();


  // using macros only used if debug build
  Serial.begin(115200);

  int password_index = 0;

  if (CONNECT_TO_LOCAL_WIFI == true)
    ConnectToWifi();  // connect to local WiFi Router
  else
    SetupAsServer();  // configure as server, no need for router.



  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, FULL_NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( Brightness );
  LEDs_AllOFF();


  setup_server();
}


//
// handleRoot()
// Handle display of root page
//
void handleRoot() {

  if (server.hasArg("Mode")) 
  { 
    Serial.println("Submit into Root");
    handleSubmit(); 
  } 


  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;



  const int MAX_PAGE_SIZE = 1000;
  char page_to_send[ MAX_PAGE_SIZE ];


  char option_line_0 [] = "<INPUT type=\"radio\" name=\"Mode\" value=\"0\">Demo OFF";
  char option_line_1 [] = "<INPUT type=\"radio\" name=\"Mode\" value=\"1\">Demo 1";
  char option_line_2 [] = "<INPUT type=\"radio\" name=\"Mode\" value=\"2\">Demo 2";
  char option_line_3 [] = "<INPUT type=\"radio\" name=\"Mode\" value=\"3\">Brighter";
  char option_line_4 [] = "<INPUT type=\"radio\" name=\"Mode\" value=\"4\">Dimmer";

  
  String current_mode_string = "DEMO";

  getDisplayMode( &current_mode_string );
  
  // construct the web page into the character buffer.
  
  snprintf ( page_to_send, MAX_PAGE_SIZE,
    "<html>\
      <head>\
        <meta http-equiv='refresh' content='10'/>\
        <title>LED Lighting Controller - Home Page</title>\
        <style>\
          body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
        </style>\
      </head>\
      <body>\
        <h1>LED Lighting Controller - Home Page</h1>\
        <p>Uptime: %02d:%02d:%02d</p>\
        Current Mode : %s <br>\
        Brightness : %d<br>\
        <FORM action=\"/\" method=\"post\">\
      <P>Update LED<br>\
      %s <br>%s<br>%s<br>%s<br>%s<br>\
      <INPUT type=\"submit\" value=\"Update\">\
     </P>\
    </FORM>\ 
    </body>\
    </html>",
    hr, min % 60, sec % 60, 
    current_mode_string.c_str(),
    Brightness,
    option_line_0,
    option_line_1,
    option_line_2,
    option_line_3,
    option_line_4
  );

  // enhanced page
  server.send ( 200, "text/html", page_to_send );

}

void handleSubmit() 
{ 
  String Modevalue = "0"; 

  if (!server.hasArg("Mode")) return returnFail("BAD ARGS"); 
  
  Modevalue = server.arg("Mode"); 
  
  if (Modevalue == "0") 
  { 
    CurrentMode = MODE_DEMO_OFF;
    LEDs_AllOFF();
    //server.send(200, "text/html", TEST_PAGE_HTML); 
  } 
  else if (Modevalue == "1") 
  { 
    CurrentMode = MODE_DEMO_1;
    //server.send(200, "text/html", TEST_PAGE_HTML); 
  } 
  else if (Modevalue == "2") 
  { 
    CurrentMode = MODE_DEMO_2;
    //server.send(200, "text/html", TEST_PAGE_HTML); 
  } 
  else if (Modevalue == "3") // brighter
  { 
    if ( Brightness < 200)
      Brightness += 25;
    
    FastLED.setBrightness( Brightness );
  } 
  else if (Modevalue == "4") // dimmer
  { 
    if ( Brightness > 50)
      Brightness -= 25;
  } 
  else
  { 
    // FAILURE
    returnFail( "Bad request, need 0 to 6");
  } 
} 


void handleNotFound(){

  String message = "404\n\nFile Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  //message += "Options include\nRed\nGreen\nDemo\nChase\n\nThese are not pages these are instructions.\n\n";
  server.send(404, "text/plain", message);
  //digitalWrite(led, 0);
}

void returnFail(String msg) 
{ 
  server.sendHeader("Connection", "close"); 
  server.sendHeader("Access-Control-Allow-Origin", "*"); 
  server.send(500, "text/plain", msg + "\r\n"); 
} 

// just there to remember coding options
void LEDTest_1()
{
  static uint8_t hue;  
  
  for(int i = 0; i < FULL_NUM_LEDS/2; i++) {   
    // fade everything out
    leds.fadeToBlackBy(40);

    // let's set an led value
    leds[i] = CHSV(hue++,255,255);

    // now, let's first 20 leds to the top 20 leds, 
    leds(FULL_NUM_LEDS/2,FULL_NUM_LEDS-1) = leds(FULL_NUM_LEDS/2 - 1 ,0);
    FastLED.delay(33);
  }
}


void LED_Mode_1()
{
  static int counter = 0;
  
   // Set all LEDs to white
   for(int LEDIndex = 0; LEDIndex < FULL_NUM_LEDS; LEDIndex = LEDIndex + 1) {

      switch( counter ) 
      {
        case 0:
          if (( LEDIndex % 3 ) == 1)
            leds[LEDIndex] = CRGB::Blue;
          else if (( LEDIndex % 3 ) == 2)
            leds[LEDIndex] = CRGB::Red;
          else
           leds[LEDIndex] = CRGB::White;
        break;
        
        case 1:
          if (( LEDIndex % 3 ) == 1)
            leds[LEDIndex] = CRGB::White;
          else if (( LEDIndex % 3 ) == 2)
            leds[LEDIndex] = CRGB::Blue;
          else
           leds[LEDIndex] = CRGB::Red;
        break;
        
        default:
          if (( LEDIndex % 3 ) == 1)
            leds[LEDIndex] = CRGB::Red;
          else if (( LEDIndex % 3 ) == 2)
            leds[LEDIndex] = CRGB::White;
          else
           leds[LEDIndex] = CRGB::Blue;
        break;
      }
      
   }
   FastLED.show();

   // Wait a little bit
   delay(150);

   //LEDs_AllOFF();
   //delay(150);

   counter++;
   counter = counter % 3;
}
void LED_Mode_2()
{
   // Set all LEDs to white
   for(int LEDIndex = 0; LEDIndex < FULL_NUM_LEDS; LEDIndex = LEDIndex + 1) {

      if (( LEDIndex % 3 ) == 1)
        leds[LEDIndex] = CRGB::Blue;
      else if (( LEDIndex % 3 ) == 2)
        leds[LEDIndex] = CRGB::Red;
      else
        leds[LEDIndex] = CRGB::White;
   }
   FastLED.show();

   // Wait a little bit
   delay(150);

}


void LEDs_AllOFF() {
   // Set all LEDs to Black
   for(int LEDIndex = 0; LEDIndex < FULL_NUM_LEDS; LEDIndex = LEDIndex + 1) {
      leds[LEDIndex] = CRGB::Black;
   }
   FastLED.show();
}


void getDisplayMode( String * Mode_Str )
{
  String current_mode_string = "UNKNOWN";
  
  if ( CurrentMode == MODE_DEMO_OFF )  *Mode_Str = "OFF";
  if ( CurrentMode == MODE_DEMO_1 )    *Mode_Str = "MODE 1";
  if ( CurrentMode == MODE_DEMO_2 )    *Mode_Str = "MODE 2";
}
  

void loop() {

  switch( CurrentMode )
  {
    case MODE_DEMO_OFF:
    {
      LEDs_AllOFF();
    }
    break;

    case MODE_DEMO_1 : 
      LED_Mode_1();
    break;

    case MODE_DEMO_2 : 
      LED_Mode_2();
    break;


    default : 
      break;
  } // end switch

  
  delay(10);

  server.handleClient();

}
