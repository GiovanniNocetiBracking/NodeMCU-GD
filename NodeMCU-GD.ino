#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#define FIREBASE_HOST "https://gasdetect-fe60e-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "XKKqf7QjLzIDa5rzSwjNbpG5wjufnxbbtFtoM5dS"
#define WIFI_SSID "KARIN."
#define WIFI_PASSWORD "89428942"
FirebaseData firebaseData;
SoftwareSerial nodemcu(D5, D6);
void setup()
{
    Serial.begin(9600);
    nodemcu.begin(9600);
    while (!Serial)
        continue;
    // connect to wifi.
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
}
void loop()
{
    StaticJsonBuffer<1000> jsonBuffer;
    JsonObject &data = jsonBuffer.parseObject(nodemcu);

    if (data == JsonObject::invalid())
    {
        Serial.println("invalid object");
        jsonBuffer.clear();
        return;
    }
    float LPG = data["LPG"];
    float CO = data["CO"];
    float SMOKE = data["SMOKE"];
    Firebase.setFloat(firebaseData, "Sensor1/lpg", LPG);
    Firebase.setFloat(firebaseData, "Sensor1/co", CO);
    Firebase.setFloat(firebaseData, "Sensor1/smoke", SMOKE);
    delay(1000);
}