/*
 * Game controller script
 *  for getting controller input to the raspberry pi/pandaboard
 *  Namely...
 *      -Analogue stick controls
 *      -Special move button
 *      -Potential pedals
 *      -Special move switches
 *      
 * Send input either via bluetooth or via RF
 *  get commands and use SerialCommand to do stuff since apparently the
 *  things it reads goes through 
 *  might not be any point to this though since we'll always be using the
 *  controls
 * 
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//serial command stuff
#include <SoftwareSerial.h>
#include <SerialCommand.h>

SerialCommand sCmd;

RF24 radio(7, 8); //cns, ce pins
//const byte addresses[][6] = {"1Node", "2Node"};
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

const int min_payload_size = 4;
const int max_payload_size = 32;
const int payload_size_increments_by = 1;
int next_payload_size = min_payload_size;
 

//analog stick pins
 const int rX = A0;
 const int rY = A1;
 const int lX = A2;
 const int lY = A3;

//mapped analogRead values
 int rYval, rXval, lYval, lXval, spec1, spec2, super, fire1, fire2, pedal;

/*
  special buttons
  maybe map this to a resistor ladder like in the arduino tutorial
  and calculate it from there, rathre than using a bunch of pins
*/
//DO NOT USE THESE
 const int specButton2 = 6;
 const int specButton = 5;
 
 const int superButton = 4;
 const int fire2B = 3;
 const int fire1B = 2;
 const int pedalB = 5;
 


 bool bc = false;
 
 //msg is the string to send out
 //apparently msg length is like 23 + \0
 int messageLength = 23;
 String msg = "";
 char msgBuff[max_payload_size+1];

 //encode the current controller state
String EncodeState()
{
  String m = ""; 
  m+='x';
  m+=rXval;
  m+='|';
  m+=rYval;
  m+='|';
  m+=lXval;
  m+='|';
  m+=lYval;
  m+='|';
  m+=fire1;
  m+='|';
  m+=fire2;
  m+='|';
  m+=super;
 // m+='|';
  //m+=pedal;
  //terminating character
  m+='\n';

  return m;
}


void setup() {
  // put your setup code here, to run once:
  pinMode(specButton, INPUT_PULLUP);
  pinMode(specButton2, INPUT_PULLUP);
  pinMode(superButton, INPUT_PULLUP);

  pinMode(fire1B, INPUT_PULLUP);
  pinMode(fire2B, INPUT_PULLUP);

  //probably want something faster than 9600 for this
  Serial.begin(9600);
  while (!Serial);
  
  radio.begin();
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  //radio.setPALevel(RF24_PA_MIN);
  //radio.stopListening();

  // enable dynamic payloads
  radio.enableDynamicPayloads();
  //radio.setAutoAck(1);                    // Ensure autoACK is enabled
  // optionally, increase the delay between retries & # of retries
  //radio.setRetries(15,15);

  radio.startListening();

  sCmd.clearBuffer();
  //sCmd.addCommand("START", startCMD);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  MapAnalogue();
  spec1 = digitalRead(specButton);
  spec2 = digitalRead(specButton2);
  super = digitalRead(superButton);

  fire1 = digitalRead(fire1B);
  fire2 = digitalRead(fire2B);
  pedal = digitalRead(pedalB);
  

  //create msg string here
  msg = EncodeState();
  Serial.println(msg);
  //Serial.flush();
  //broadcast the message to the RPi
  //no longer need to broadcast
  //Broadcast(msg);

  //pause for a moment
  delay(5);
}

void MapAnalogue()
{
  rXval = map(analogRead(rX), 0, 1023, -50, 50);
  rYval = map(analogRead(rY), 0, 1023, -50, 50);
  lXval = map(analogRead(lX), 0, 1023, -50, 50);
  lYval = map(analogRead(lY), 0, 1023, -50, 50);
}

//broadcast the game input to the RPi
void Broadcast(String msg)
{
  radio.stopListening();

  //Serial.println("Here's the message!");
  //Serial.println(msg);
  msg.toCharArray(msgBuff, messageLength+1);
  //Serial.println("Bump");
  if(!radio.write( msgBuff, strlen(msgBuff) ))
  {
    //Serial.println("Failed");
  }

  // Now, continue listening
  radio.startListening();

  // Wait here until we get a response, or timeout
  unsigned long started_waiting_at = micros();
  bool timeout = false;

  //stall until we're sure the RPi has read the input or we've timed out
  //for now, half a second
  while ( ! radio.available() && !timeout )
    if (micros() - started_waiting_at > 200000 )
    {
      timeout = true;
      //Serial.println("Timed out..");
    }

  if(!timeout)
  {
    unsigned long got_time;
    radio.read(&got_time, sizeof(unsigned long));
  }

  //try again later
  delay(1);
}
