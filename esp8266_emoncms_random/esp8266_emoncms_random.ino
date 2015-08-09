#include <SoftwareSerial.h>
 
#define DEBUG true
SoftwareSerial esp8266(10,11);

///send value random Emoncms     https://www.youtube.com/channel/UCv1D6zrC0ZL0PSgM6tdEpPg                         

#define APIKEY "6a36f95a048ff8ba64f5c0eb18204381" //local
#define TARGET_IP "192.168.1.66"///              //local


#define TARGET_PORT "80"     
#define ID "MAIRA"  //        SSID
#define PASS "3113884544"  // PASS

                             
                             
void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600); 
   
   

    sendData("AT+RST\r\n",1000,DEBUG);      
    sendData("AT+CWMODE=1\r\n",1000,DEBUG); 
    
    String cmd="AT+CWJAP=\"";  
    cmd+=ID;
    cmd+="\",\"";
    cmd+=PASS;
    cmd+="\"";      
    sendData( cmd+"\r\n",1000,DEBUG);  
    
    sendData("AT+CIPMUX=0\r\n",1000,DEBUG); 
    
}
 
void loop()
{
    
    String webpage = "AT+CIPSTART=\"TCP\",\""; 
    webpage += TARGET_IP;
    webpage += "\",80\r\n";   
      
    sendData(webpage,1000,DEBUG);        
     
    String dato ((random(0, 100)), DEC);   
          
    String webpage1 = "GET /emoncms/input/post.json?json={variable_arduino:"+ dato +"}&apikey="+APIKEY+"  HTTP/1.0\r\n";
    webpage1+="Host: ";
    webpage1+=TARGET_IP;
    webpage1+="\r\n";
    webpage1+="User-Agent: test\r\n"    ;
    webpage1+="\r\n";   
      
    String cipsend = "AT+CIPSEND=";     
    cipsend+= webpage1.length();
    cipsend+="\r\n";     
     
    sendData(cipsend,1000,DEBUG);
    sendData(webpage1,1000,DEBUG);   
                
    sendData("AT+CIPCLOSE=0\r\n",1500,DEBUG);       
     
    delay(7000);  //7 Sec  

  
}
 
 
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    esp8266.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {
        // The esp has data so display its output to the serial window 
        char c = esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}
