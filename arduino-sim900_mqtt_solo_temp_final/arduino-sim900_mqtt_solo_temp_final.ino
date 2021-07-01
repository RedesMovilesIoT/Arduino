//Se determina el Modulo usado
#define TINY_GSM_MODEM_SIM900
//Se declaran las librerías necesarias 
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>
#include "ThingsBoard.h"
#include <OneWire.h>
#include <DallasTemperature.h>
const int pinDatosDQ = 4;
// Instancia a las clases OneWire y DallasTemperature
OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);

//Se definen credenciales del APN
const char apn[]  = "Your APN";
const char user[] = "";
const char pass[] = "";

//Se define el token y el servidor
#define TOKEN               "Your token"
#define THINGSBOARD_SERVER  "demo.thingsboard.io"

//Se definen los pines para la comunicación serial y la velocidad
#define SERIAL_DEBUG_BAUD   19200

// Serial port for GSM shield
SoftwareSerial serialGsm(7, 8); // RX, TX pins for communicating with modem

// Initialize GSM modem
TinyGsm modem(serialGsm);

// Initialize GSM client
TinyGsmClient client(modem);

// Initialize ThingsBoard instance
ThingsBoard tb(client);

// Set to true, if modem is connected
bool modemConnected = false;

void setup() {
  // Se setea la velocidad
  Serial.begin(SERIAL_DEBUG_BAUD);

  // Set GSM module baud rate
  serialGsm.begin(19200);
  delay(3000);


  //Se inicia el modem
  Serial.println(F("Initializing modem..."));
  modem.restart();

  String modemInfo = modem.getModemInfo();
  Serial.print(F("Modem: "));
  Serial.println(modemInfo);
  //Se inicia el sensor de temperatura
  sensorDS18B20.begin();
}

void loop() {
  delay(1000);
    //Inicia la conexión a la red 
  if (!modemConnected) {
    Serial.print(F("Waiting for network..."));
    if (!modem.waitForNetwork()) {
        Serial.println(" fail");
        delay(10000);
        return;
    }
    Serial.println(" OK");
    Serial.print(F("Connecting to "));
    Serial.print(apn);
      //Se conecta a internet
    if (!modem.gprsConnect(apn, user, pass)) {
        Serial.println(" fail");
        delay(10000);
        return;
    }
    modemConnected = true;
    Serial.println(" OK");
  }
  // Se conecta a la pagina de ThingBoard
  if (!tb.connected()) {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
  }
 //Se envía la temperatura
   Serial.println("Sending data...");
   sensorDS18B20.requestTemperatures();
  tb.sendTelemetryFloat("temperature", sensorDS18B20.getTempCByIndex(0));
  tb.loop();
}
