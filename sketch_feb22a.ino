#include "SoftwareSerial.h";
#include "DHT.h";
#include "string.h";
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN            7

#define NUMPIXELS      1

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second
DHT dht;




int bluetoothTx = 2;
int bluetoothRx = 4;
int MOTOR2_PIN1 = 3;
int MOTOR2_PIN2 = 5;
int MOTOR1_PIN1 = 6;
int MOTOR1_PIN2 = 9;

//switch

int sw_door   = 1;
int sw_window = 1;

int sw_light  = 0;



char command[20];
int  waiting = 0;
int waiting_contor;


int red = 0;
int blue = 0;
int green = 0;
int color_count = 0;

int enabled = 0;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup()
{
  //led
  pinMode(13,OUTPUT);
  Serial.begin(9600);


  bluetooth.begin(115200);
  bluetooth.print("$$$");
 
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);
  
  
  delay(100);
  bluetooth.println("U,9600,N");
  bluetooth.begin(9600);
  
  delay(1000);

  bluetooth.print("$$$");
   // bluetooth.println("+");
  //senzor
   dht.setup(12); // data pin 2
   
  //adafruit neopixel
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.

}

void loop()
{
  if(bluetooth.available()) {
   
    
 
   
   char toSend =(char)bluetooth.read();
   
   Serial.println(toSend);
   if(toSend == 'z')
    enabled = 1;
   if(waiting == 0 && enabled == 1 && toSend != 'z'){
     
   char mychar[3];  
     
   mychar[0] = toSend;
   mychar[1] = '.';
   mychar[2] = '\0';
   
   bluetooth.print(mychar);
    
     switch(toSend){ //cases
     
      case '1':
       if(sw_light == 0){
        digitalWrite(13,HIGH);
        sw_light = 1;
       }else{
        digitalWrite(13,LOW);
        sw_light = 0;
       }        
       bluetooth.print("done.");
      break;
  
      case '2':
      
       if(sw_door == 1){
         analogWrite(MOTOR2_PIN1,0);
         analogWrite(MOTOR2_PIN2,255);
         
         delay(2100);
         
         analogWrite(MOTOR2_PIN1,0);
         analogWrite(MOTOR2_PIN2,0);
         sw_door = 0;
         

       }
       else{
         analogWrite(MOTOR2_PIN1,255);
         analogWrite(MOTOR2_PIN2,0);
         
         delay(2000);
         
         analogWrite(MOTOR2_PIN1,0);
         analogWrite(MOTOR2_PIN2,0);         
         
         sw_door = 1;
       }
      delay(1500);
      bluetooth.print("done.");
      break;
     
      case '3':
       if(sw_window == 1){
         analogWrite(MOTOR1_PIN1,0);
         analogWrite(MOTOR1_PIN2,255);
         
         delay(1500);
         
         analogWrite(MOTOR1_PIN1,0);
         analogWrite(MOTOR1_PIN2,0);
         sw_window = 0;
       }
       else{
         analogWrite(MOTOR1_PIN1,255);
         analogWrite(MOTOR1_PIN2,0);
         
         delay(1500);
         
         analogWrite(MOTOR1_PIN1,0);
         analogWrite(MOTOR1_PIN2,0);         
         
         sw_window = 1;
       }
      delay(1500);
      bluetooth.print("done.");
      break;       
      
        
      
     case '4':
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(0, pixels.Color(0,0,0)); // Moderately bright green color.
    
        pixels.show(); // This sends the updated pixel color to the hardware.
    
        delay(delayval);
        delay(1500);
          bluetooth.print("done.");
     break;
     
      
      case '5':{
       

          delay(dht.getMinimumSamplingPeriod());

          int humidity = (int) dht.getHumidity();
          int temperature = (int) dht.getTemperature();
          
          Serial.println(temperature);
 
          char ret[2];
          
          //memset(ret,0,sizeof(ret));
          
          ret[0] = '0'+temperature;
          ret[1] = '.';
          ret[2] = '\0';
       
          String my(ret);
          
          bluetooth.print(my);
           
          
      break;
      }
      default:
      
       waiting = 1;
       Serial.println("waiting");
       bluetooth.print("waiting.");
      break;
    
     }
     
   }else{
  
  if(toSend == '.'){
     waiting = 0; 
     
   // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(0, pixels.Color(red,green,blue)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); 
     
     red = 0;
     blue = 0;
     green = 0;
     color_count = 0;
     bluetooth.print("done.");
    }else{
      
      if(toSend != '-'){

        if(color_count == 0)
         red = red*10+ (toSend - '0');
        if(color_count == 1)
         green = green*10 + (toSend - '0');
        if(color_count == 2)
         blue = blue*10 + (toSend - '0');
      }
      else
       color_count++;
       
    }
     
   }
  }
  
    if(Serial.available()) {
    char toSend = (char)Serial.read();
    bluetooth.print(toSend);
    
  
  }

}

//seonzor
int readDistanceMediata(int count, int pin) {
  int sum = 0;
  for (int i = 0; i<count; i++) {
    float volts = analogRead(pin) * ((float) 5 / 1024);
    float distance = 65 * pow(volts, -1.10); 
    sum = sum + distance;
    delay(5);
  } 
  return (int) (sum/count);
} 
