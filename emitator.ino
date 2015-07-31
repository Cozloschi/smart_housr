  #include <SPI.h>
  #include <Ethernet.h>
  #include <ctype.h>
  #include "SoftwareSerial.h";
  
  #define ACTIONS_MAX  3
  
  int actions_contor = 0;
  
  int bluetoothTx = 2;
  int bluetoothRx = 3;
  
  int initialize = 0;
  int length_str = 0;
  int actions_nr = 0;//actions string
  char SavedChar;
 
    
  SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
  
  byte mac[] = { 
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  IPAddress ip(192,168,1,177);
  
  EthernetServer server(80);
  
  char url[] = "192.168.1.176";
  
  EthernetClient client;
  
  void setup() {
    
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
     while (!Serial) {
      ; 
    }
  
    bluetooth.begin(115200);
    bluetooth.print("$$$");
    delay(100);
    bluetooth.println("U,9600,N");
    bluetooth.begin(9600);
    
    delay(1000);
    bluetooth.print("$$$");
    delay(1000);
   
    //connect to other bluetooth device
    bluetooth.println("C,0006666815E4");
    
    // start the Ethernet connection and the server:
    Ethernet.begin(mac, ip);
    server.begin();
    Serial.print("server is at ");
    Serial.println(Ethernet.localIP());
    
  }
  
  char str[90];
  char get[100]; //get param
  char* stack[10]; // actions list 
  int stack_count = 0;
  int first_req = 0;
  boolean param = false;
  
  void clear_char(){
   
   memset(str,0,sizeof(str));
    
  }
  
  void loop() {
     int i=0;
     char mychar[1];
     boolean make_req = false;
       
     //send data
      if(Serial.available()) {
        char toSend = (char)Serial.read();
        if(toSend == 'z'){
          bluetooth.print(toSend);
        
        }
        Serial.println(toSend);
    }
    
  
    // listen for incoming clients
    EthernetClient client = server.available();
    if (client) {
      first_req = 1;
      Serial.println("new client");
     
      boolean currentLineIsBlank = true;
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          
          //check if it's GET param
          if(c == ' ') param = false;
          if(c == '?') param = true;
          
          char mychar[2];
          
          //prepare char for append, convert to string
          mychar[1] = '\0';
          mychar[0] = c;
             
          if(param == true && c != '?')
           strcat(str,mychar);
           
           
          // check if http ended
          if (c == '\n' && currentLineIsBlank) {
          
          //add it to stack
          stack[stack_count++] = str;
         
          bluetooth.print(stack[stack_count-1]);//send data, wait for the answer
          
          //clear the char
          clear_char();
  
          //send http response back
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Access-Control-Allow-Origin: *");
          client.println();
          client.println("done.");        
          client.stop();
          
          }
        }
      }
    }
            
          //read response
          while(bluetooth.available()) {
           char toSend = (char)bluetooth.read();
        
           //convert toSend char to string
           char mychar2[2];
    
        
           mychar2[0] = toSend;
           mychar2[1] = '\0'; //for append
          
           if(toSend != '.' && ((toSend >= 'A' && toSend <= 'Z')||(toSend >= 'a' && toSend <= 'z') || (toSend >= '1' && toSend <= '9'))){ //end
            strcat(get,mychar2);
            length_str++;
            SavedChar=toSend;
           }    
           else
            make_req = true;
            
            
            memset(mychar2,0,sizeof(mychar2));
          
         
           if(make_req && first_req == 1){
           
             
             //print array
           if(length_str == 1 ){
             
            
            if(actions_contor < ACTIONS_MAX){
             //Serial.println("good");  
            Serial.println(actions_contor);
            actions_contor++;
            //Serial.println(SavedChar);
             length_str = 0;
             
             if(actions_contor == 1){
               actions_nr = actions_nr * 10 + (SavedChar - '0');
               actions_contor = 1; 
             }
             
             if(actions_contor == 2){
             actions_nr = actions_nr * 10 + (SavedChar - '0');
             //Serial.println(actions_nr);
             //clear actions sttringg
              actions_contor = 2;
              
              char test[5];
              
              sprintf(test, "%s%d", "a", actions_nr);
              
              char str_cat[10];
              
              strcat(str_cat,"GET /search?q=");
              strcat(str_cat,test);
              strcat(str_cat," HTTP/1.1");
              
               if (client.connect(url, 8080)) {
                Serial.println("connected");
                // Make a HTTP request:
                client.println(str_cat);
                client.println("Host: www.google.com");
                client.println("Connection: close");
                client.println();
              } 
              
             }
             if(actions_contor == 3){
               char test[5];
              
              
              sprintf(test, "%s%d", "a", actions_nr);
              
              char str_cat[10];
              
              strcat(str_cat,"GET /search?q=");
              strcat(str_cat,test);
              strcat(str_cat," HTTP/1.1");
              
               if (client.connect(url, 8080)) {
                Serial.println("connected");
                // Make a HTTP request:
                client.println(str_cat);
                client.println("Host: www.google.com");
                client.println("Connection: close");
                client.println();
              } 
              
              
              
               sprintf(test, "%s%d", "a", actions_nr);
              
               actions_nr = 0; 
               actions_contor = 0;
               length_str = 0;               
              
               
              }
            }
           }else{
            
           length_str = 0;
             
            //if we got the response, clear stack
            for(i=1;i<=9;i++)
             stack[i-1] = stack[i];
             
            stack_count--;
            
         
            //clear memory
            memset(get,0,sizeof(get));
            memset(mychar2,0,sizeof(mychar2));
              
            break;
        
           }
  
         }
        }

       if(bluetooth.available()) {
         char toSend = (char)bluetooth.read();
       }
    

   }

