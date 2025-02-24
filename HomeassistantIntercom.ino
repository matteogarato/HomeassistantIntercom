#include <WiFi.h>
#include <PubSubClient.h>

#define ssid F("")
#define password F("")
#define GATE_RELAY_PIN 15
#define BIG_GATE_RELAY_PIN 16
#define LED_PIN 2
#define ON_TIME 1000
#define mqttPort 1883
#define mqttServer ""
#define mqttUser ""
#define mqttPassword ""
#define aliveTopic "home/electrical/intercom/get"
const char *gateTopic = "home/electrical/gaterelay/set";
const char *bigGateTopic = "home/electrical/biggaterelay/set";
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(921600);
  delay(200);
  Serial.println(F("Home assistant integrated Gate opener - Welcome!"));
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  pinMode(GATE_RELAY_PIN, OUTPUT);
  pinMode(BIG_GATE_RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(GATE_RELAY_PIN, LOW);
  digitalWrite(BIG_GATE_RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  Serial.print(F("Connecting to WiFi"));
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(F("."));
    delay(200);
  }
  Serial.println(F(""));
  Serial.println(F("Connected to WiFi"));
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  client.setKeepAlive(60);
  connectMqtt();
}

void loop() {
  if (!client.connected()) {
    connectMqtt();
  }
  client.loop();
}

void connectMqtt() {
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    if (client.connect(client_id.c_str(), mqttUser, mqttPassword)) {
      Serial.println(F("Reconnected to MQTT broker"));
      client.subscribe(gateTopic);
      client.subscribe(bigGateTopic);
    } else {
      Serial.print(F("Failed to connect, state "));
      Serial.print(client.state());
      delay(5000);  // wait for 5 seconds before retrying
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.println(F("homeassistant callback recived"));
  Serial.println(topic);
  if (strcmp(topic, gateTopic) == 0) {
    openGate();
  } else if (strcmp(topic, bigGateTopic) == 0) {
    openBigGate();
  }
}

void openGate() {
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(GATE_RELAY_PIN, HIGH);
  delay(ON_TIME);
  digitalWrite(GATE_RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  Serial.printf("GATE opened \n");
}

void openBigGate() {
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(BIG_GATE_RELAY_PIN, HIGH);
  delay(ON_TIME);
  digitalWrite(BIG_GATE_RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  Serial.printf("BIG GATE opened \n");
}
