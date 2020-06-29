#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

char auth[] = "*****";
char ssid[] = "****";
char pass[] = "******";
char server[] = "blynk-cloud.com";  // Cloud Server address
int port = 8080;  //  MCU Hardware Device port

int outerTimer; // Timer ID
int innerTimer; // timer ID
int onTime = 30000; // Initial on time value for slider input
int offTime = 150000; // Initial off time value for slider input
int btnState = HIGH; // Flag
int VbtnPin = LOW; // Flag
int UpdatePin = LOW; // Flag
int buttonBlock = LOW; // Flag
const int relayPin = 4;  // Pin ID
WidgetLED led1(V7);


BlynkTimer timer;

void setup() {
  Serial.begin(115200);  
  WiFi.begin(ssid, pass);  // Connect to WiFi 
  Blynk.config(auth, server, port);
  Blynk.connect();  // Initialise Connection to Server

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(relayPin, OUTPUT);

  // "Sacrificial" Timer - needed when deleting timers as using that feature will also kill first timer created...
  timer.setTimeout(10L, []() {
    Blynk.virtualWrite(V0, "START");
  });  // END sacrificial Function

  // Timed Lambda Function - UpTime 
  timer.setInterval(1000L, []() { // Run every second
    Blynk.virtualWrite(V0, millis() / 1000);  // Display the UpTime in seconds
  });  // END UpTime Function

  // Timed Button watcher...
  timer.setInterval(500L, buttonScan);

  ArduinoOTA.setHostname("Shelly1 Mist TImer"); 
  ArduinoOTA.setPassword((const char *)"water");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });

  ArduinoOTA.begin();
}



BLYNK_CONNECTED() {
Blynk.virtualWrite(V1, "OFF");  // display timer as OFF
}


void loop() {
  timer.run();
  ArduinoOTA.handle();
  Blynk.run();
}

void printValues() {
  Serial.print("Update Pin: ");
  Serial.println(UpdatePin);
  Serial.print("On/Off Pin: ");
  Serial.println(VbtnPin);
  Serial.print("Button Block: ");
  Serial.println(buttonBlock);
  Serial.print("On time: ");
  Serial.println(onTime);
  Serial.print("Off time: ");
  Serial.println(offTime);
}


void buttonScan() {  // Change interval time via Physical Button
  
  if (buttonBlock == LOW && UpdatePin == HIGH  ) {
      if (btnState != LOW) {  // btnState is used to avoid sequential toggles
        timer.deleteTimer(outerTimer);
        timer.deleteTimer(innerTimer);
        Blynk.virtualWrite(V4, onTime/1000); // display the on time interval
        Blynk.virtualWrite(V6, offTime/1000); // display the off time interval
        Blynk.virtualWrite(V2, LOW);
        printValues();
        repeatTimerFunction();
      }
      btnState = LOW;
    } else {
      btnState = HIGH;
    }
}

BLYNK_WRITE(V1) {  // Turn timer on/off
  int old_value = VbtnPin;
  VbtnPin = param.asInt();  // Set the virtual button flag
  if (old_value != VbtnPin){
    Blynk.virtualWrite(V2, HIGH);
  }
  buttonScan();  // Run the button processing function.
}

BLYNK_WRITE(V2) {  // Refresh timer intervals
  UpdatePin = param.asInt();  // Set the virtual button flag
  buttonScan();  // Run the button processing function.
}

BLYNK_WRITE(V3) {  // Change interval time via Blynk button
  onTime = param.asInt()*1000;  // Set the virtual button flag
  buttonScan();  // Run button processing function.
}

BLYNK_WRITE(V5) {  // Change interval time via Blynk button
  offTime = param.asInt()*1000;  // Set the virtual button flag
  buttonScan();  // Run the button processing function.
}


void repeatTimerFunction() {
  buttonBlock = HIGH;  // Lockout flag for preventing further button press until done, else can cause timer duplication
  
  if (VbtnPin == HIGH){
    // This dual nested timer routine will constantly blink both an LED and the onboard LED of an ESP8266 Dev Board
    outerTimer == timer.setInterval(offTime+onTime, []() {  // Start 1st Timed Lambda Function - Turn ON LEDs

      
      digitalWrite(relayPin, HIGH); // Turn relay switch on
      led1.on(); // Turn on virtual LED

      innerTimer == timer.setTimeout(onTime, []() {  // Start 2nd Timed Lambda Function - Turn OFF LEDs (once per loop)
        digitalWrite(relayPin, LOW); // Turn relay switch off
        led1.off(); // Turn off virtual LED
      });  // END inner Timer Function

      
    });  // END outer Timer Function
  }
  
  buttonBlock = LOW;
}
