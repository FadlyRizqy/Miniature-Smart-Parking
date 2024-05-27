// Library
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Servo.h>

// Define pinout
#define park1 D0
#define park2 D1
#define entry D2
// #define servo D3
#define led1 D5
#define led2 D4
#define led3 D6

// Start servo setup
Servo servo;
int pos = 0;
int condition;

/****** WiFi Connection Details *******/
const char* ssid = "";
const char* password = "";

/******* MQTT Broker Connection Details *******/
const char* mqtt_server = "broker.emqx.io";
const char* mqtt_username = "emqx";
const char* mqtt_password = "public";
const int mqtt_port =1883;

/**** Secure WiFi Connectivity Initialisation *****/
WiFiClient espClient;

/**** MQTT Client Initialisation Using WiFi Connection *****/
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

/************* Connect to WiFi ***********/
void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}

/************* Connect to MQTT Broker ***********/
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";   // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      client.subscribe("iot_red");   // subscribe the topics for red LED
      client.subscribe("iot_green");   // subscribe the topics for green LED
      client.subscribe("iot_white");   // subscribe the topics for white LED

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/***** Call back Method for Receiving MQTT messages and Switching LED ****/

void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];

  Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);

//--- check the incomming message
  if( strcmp(topic,"iot_red") == 0)
  {
    if (incommingMessage.equals("1")) {
      digitalWrite(led1, HIGH);   // Turn the LED on
    }
    else {
      digitalWrite(led1, LOW);  // Turn the LED off
    }
  }
  if( strcmp(topic,"iot_green") == 0)
  {
    if (incommingMessage.equals("1")) {
      digitalWrite(led2, HIGH);   // Turn the LED on
    }
    else {
      digitalWrite(led2, LOW);  // Turn the LED off
    }
  } 
  if( strcmp(topic,"iot_white") == 0)
  {
    if (incommingMessage.equals("1")) {
      digitalWrite(led3, HIGH);   // Turn the LED on
    }
    else {
      digitalWrite(led3, LOW);  // Turn the LED off
    }
  } 
}

/**** Method for Publishing MQTT Messages **********/
void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str(), true))
      Serial.println("Message publised ["+String(topic)+"]: "+payload);
}

/**** Application Initialisation Function******/
void setup() {

  // set servo at 0 degree
  servo.attach(D3);
  servo.write(0);

  // Pinout led
  pinMode(led1, OUTPUT); //set up LED
  pinMode(led2, OUTPUT); //set up LED
  pinMode(led3, OUTPUT); //set up LED

  Serial.begin(9600);
  while (!Serial) delay(1);
  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

/******** Main Function *************/
void loop() {

  if (!client.connected()) reconnect(); // check if client is connected
  client.loop();

// Sensor read distance, 1 if empty, 0 if there is a car
int slot1 = digitalRead(park1);
int slot2 = digitalRead(park2);
int gate = digitalRead(entry);

// gate closing
if (gate == 1 || condition == 0) {
  // if error, just make the gate equals to == 1
  servo.write(pos);
  Serial.println("Gate closed.");
  delay(15);
}  
else {
  // gate opening
  Serial.println("Gate open up.");
  for (pos = 0; pos <= 180; pos += 1) {
    servo.write(pos);
    delay(15);
  }

  delay(100);

  for (pos = 180; pos >= 0; pos -= 1) {
    servo.write(pos);
    delay(15);
  }
}
  // Check the values of slot1 and slot2
  if (slot1 == 0 && slot2 == 0) {
    condition = 0;
  } 
  else if (slot1 == 0 || slot2 == 0) {
    condition = 1;
  } 
  else {
    condition = 2;
  }

// Up to MQTT
  String msgStr = String(slot1) + "," + String(slot2) + "," + String(condition);
  byte arrSize = msgStr.length() + 1;
  char msg[arrSize];

  Serial.print("PUBLISH DATA:");
  Serial.println(msgStr);
  msgStr.toCharArray(msg, arrSize);
  publishMessage("iotUAS_arraydata", msg, true);
  msgStr = "";

  delay(1000);

}
