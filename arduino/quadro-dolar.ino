#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <FastLED.h>

#define LED_PIN     D4      // PORTA EM QUE O DISPLAY ESTÁ CONECTADO
#define COLOR_ORDER GRB     // SEQUENCIA DE CORES DO LED NÃO MODIFICAR
#define CHIPSET     WS2812  // TIPO DE LED USADO NO DISPLAY

#define MAXBRIGHTNESS   250 // BRILHO MÁXIMO

char string0[16] = "CONFIG  ";

int brilho = 30;  // BRILHO DO DISPLAY
int velocidade = 110; // VELOCIDADE DO TEXTO

int colorR = 200;
int colorG = 0;
int colorB = 0;

#include "effects.h"

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <ESP8266HTTPClient.h>

#include <ArduinoJson.h>

WiFiClient client;
HTTPClient http;

String URI = "http://dolar-real.herokuapp.com/informations/KKK"; //"http://economia.awesomeapi.com.br/json/all/USD-BRL"; // API DOLAR NÃO MODIFICAR

void httpRequest(String path);

DynamicJsonDocument doc(1024);

unsigned long previousMillis = 0;

const long interval = 3000; // INTERVALO DE REQUISIÇÕES NA API MÍNIMO ACONSELHÁVEL: 5000

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, LAST_VISIBLE_LED + 1);
  FastLED.setBrightness( scale8(brilho, MAXBRIGHTNESS) );

  Serial.begin(115200);

  WiFiManager wifiManager;

  // wifiManager.resetSettings(); // SE PRECISAR APAGAR AS CONFIGURAÇÕES DA EEPROM É SÓ DESCOMENTAR E COMPILAR O CÓDIGO, APÓS O CÓDIGO RODAR É SÓ COMENTAR E COMPILAR NOVAMENTE

  wifiManager.autoConnect("Dolar-Real"); // NOME DA REDE QUE É CRIADA

  // BUSCANDO OS PRIMEIROS DADOS:

  Serial.println("connected...yeey :)");

  Serial.println("-----------");
  Serial.println("[GET] / API - ...");
  Serial.println("");

  httpRequest(URI);

  Serial.println("");

  delay(1000);

}

void loop() {
  unsigned long currentMillis = millis();

  velocidade = 110;

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    Serial.println("[GET] / API - ...");
    Serial.println("");

    httpRequest(URI);

    Serial.println("");

    velocidade = 0;
 
  }

  scrollText(string0, charSpacing, CRGB(colorR, colorG, colorB), CRGB(0, 0, 8)); // PASSANDO O TEXTO PARA O DISPLAY
  FastLED.setBrightness( scale8(brilho, MAXBRIGHTNESS) );
  FastLED.show();

  delay(velocidade);
}

void httpRequest(String URI) {
  String payload = makeRequest(URI);

  Serial.println("DADOS:");
  Serial.println(payload);
  Serial.println();

  if (!payload) {
    return;
  }

  deserializeJson(doc, payload);
  JsonObject obj = doc.as<JsonObject>();

  String dolar = obj["USD"]["bid"];
  
  colorR = obj["user"]["deviceOptions"]["ledColor"]["r"];
  colorG = obj["user"]["deviceOptions"]["ledColor"]["g"];
  colorB = obj["user"]["deviceOptions"]["ledColor"]["b"];
  
  Serial.println("##[RESULT]## ==> " + String(colorR) + String(colorG) + String(colorB));

  //  dolar.remove(3, 1);
  //  dolar.concat("   ");
  //
  //  dolar.toCharArray(string0, 16);

  string0[0] = 'R';
  string0[1] = '$';
  string0[2] = ' ';
  string0[3] = dolar[0];
  string0[4] = dolar[1];
  string0[5] = dolar[2];
  string0[6] = dolar[3];
  string0[7] = ' ';
  string0[8] = ' ';
  string0[9] = ' ';


}

String makeRequest(String URI) {
  http.begin(URI);
  int httpCode = http.GET();

  if (httpCode < 0) {
    Serial.println("request error - " + httpCode);
    return "";

  }

  if (httpCode != HTTP_CODE_OK) {
    return "";
  }

  String response =  http.getString();
  http.end();

  return response;
}
