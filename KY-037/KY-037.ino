// KY-037 Sound detection module, modified from
// https://arduining.com/2015/08/20/nodemcu-breathing-led-with-arduino-ide/

int buttonpin = D1; // define D0 Sensor Interface
int val = 0;// define numeric variables val
 
void setup ()
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode (buttonpin, INPUT) ;// output interface D1 is defined sensor
  Serial.begin(9600);
}
 
void loop ()
{
  val = digitalRead(buttonpin);// digital interface will be assigned a value of pin 3 to read val
  if (val == HIGH) // When the sound detection module detects a signal, LED flashes
  {
    Serial.println("HIGH");
    digitalWrite (LED_BUILTIN, HIGH);
  }
  else
  {
    Serial.println("LOW");
    digitalWrite (LED_BUILTIN, LOW);
  }
  delay(100);
}
