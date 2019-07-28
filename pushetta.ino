#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
const char* ssid= "mPhone";
const char* password = "12345678";

WiFiServer server(80);
HTTPClient http;
void pushetta_push_message(char channel[], String text);

void setup() {
  Serial.begin(115200);
  
  Serial.println("starting wifi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print('.');
  }
  Serial.println("connected to wifi");

}

void loop() {

  HTTPClient http2;
String svrijeme, sdatum;
int httpCode;

http2.begin("http://salapura.com/ip/time_date.aspp");
            
        httpCode = http2.GET();
        if(httpCode > 0) {
            Serial.print("code: ");
            Serial.println(httpCode);
            
            if(httpCode == HTTP_CODE_OK) {

                String payload = http2.getString();
                String first_match = "\"datum\":\"";
                String second_match = "\",";


  int pos_start = 0;
  int pos_end = 0;
  
  String resultHTTP = payload;
      
  pos_start = resultHTTP.indexOf(first_match) + first_match.length() ;      
  pos_end = resultHTTP.indexOf(second_match, pos_start+1);
  
  resultHTTP = resultHTTP.substring(pos_start, pos_end);
  sdatum = resultHTTP;
  Serial.println(resultHTTP);
  resultHTTP = payload;

first_match = "\"vrijeme\":\"";
second_match = "\"}";
  pos_start = resultHTTP.indexOf(first_match) + first_match.length() ;      
  pos_end = resultHTTP.indexOf(second_match, pos_start+1);     

  resultHTTP = resultHTTP.substring(pos_start, pos_end);
  svrijeme = resultHTTP ;
  Serial.println(resultHTTP);
                Serial.println("ALL OK");
            }
        } 
        ////////////////////////////////////////////////////////////
        else {
          Serial.println("error");
        }
        http2.end();
        
delay(5);

if(httpCode == HTTP_CODE_OK)pushetta_push_message("Gimnazija", sdatum + " / " + svrijeme);

delay(60000);

}

void pushetta_push_message(char channel[], String text) {
  char* APIKEY = "53f6730c4d7989d4e8f48ac530c10ecad6b85df4";
  char serverName[] = "api.pushetta.com";

  WiFiClient client;
    client.stop();
    
    if(client.connect(serverName, 80)) {
      client.print("POST /api/pushes/");
      client.print(channel);
      client.println("/ HTTP/1.1");
      client.print("Host: ");
      client.println(serverName);
      client.print("Authorization: Token ");
      client.println(APIKEY);
      client.println("Content-Type: application/json");
      client.print("Content-Length: ");
      client.println(text.length()+46);
      client.println();
      client.print("{ \"body\" : \"");
      client.print(text);
      client.println("\", \"message_type\" : \"text/plain\" }");
      client.println();
      
  Serial.println("POSLAO PORUKU"); //
      
    }
}

