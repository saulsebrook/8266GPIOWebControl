#include <ESP8266WiFi.h>            
#include <ESP8266WebServer.h>
#define led_pin 12

// Create instance of WebServer on port 80
ESP8266WebServer server(80); 

void setup() {

  // Define GPIO pins to be controlled and initially set to 0
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
  
  // Start serial for debugging
  Serial.begin(115200);

  // Conect to WiFi
  WiFi.begin("WIFINAME", "PASSWORD"); 
  Serial.print("Waiting to connect...");

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) { 
  delay(500);
  Serial.print(".");
  }

  // Print to serial created IP address
  Serial.print("\nIP address: ");
  Serial.println(WiFi.localIP());  //Print the local IP to access the server
  
  // Create paths on server with handling functions that define what to do with those pages
  server.on("/", handleLedRequest);
  server.on("/genericArgs", handleGenericArgs);     
  server.on("/specificArgs", handleSpecificArg);    

  // Start the server
  server.begin();                                       
  Serial.println("Server listening");   

}

void loop() {

// Handling of incoming requests
server.handleClient();    

}

// Functions

void handleGenericArgs() { //Handler

  String message = "Number of args received:";
  // Get number of parameters
  message += server.args();  
  // Add a new line          
  message += "\n";                            

  // for loop to iterate through arguments and print name and value of each one
  for (int i = 0; i < server.args(); i++) {
    message += "Arg nº" + (String)i + " –> ";  
    message += server.argName(i) + ": ";    
    message += server.arg(i) + "\n";             
  } 
  // Send HTTP response is successful
  server.send(200, "text/plain", message);     
}

void handleSpecificArg() { 

  String message = "";

  // If an empty argument of "led" is found display message that argument was not found
  if (server.arg("led")== ""){    

    message = "Led Argument not found";

  }
  else{     
     message = "Led Argument = ";
     message += server.arg("led");     //Gets the value of the query parameter
  }
  // Send HTTP response is successful
  server.send(200, "text/plain", message);          
}

void handleLedRequest(){
  // If the arg "led" is found with the parameter ON then set GPIO high else set LOW
  if (server.arg("led") == "ON") 
                    {
                      digitalWrite(led_pin, HIGH);
                    }
                   else 
                     {
                       digitalWrite(led_pin, LOW);
                     }
   
  String message = "OK";
  // Send HTTP response is successful
  server.send(200, "text/plain", message);          //Returns the HTTP response
}
