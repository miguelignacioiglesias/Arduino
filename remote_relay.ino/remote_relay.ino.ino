#include <IRremote.h>      // importa libreria IRremote
#include <Wire.h>   // incluye libreria para interfaz I2C
#include <RTClib.h>   // incluye libreria para el manejo del modulo RTC

#define Boton_1 0xBA45FF00    // reemplaza cada instancia de Boton_1 por su valor HEX
#define Boton_2 0xB946FF00    // reemplaza cada instancia de Boton_2 por su valor HEX
#define Boton_3 0xB847FF00    // reemplaza cada instancia de Boton_3 por su valor HEX
RTC_DS3231 rtc;     // crea objeto del tipo RTC_DS3231
int SENSOR = 11;   // sensor KY-022 

int AUTO = 0;
int RELAY1 = 2;      
int RELAY2 = 3;     


unsigned long t1 = 100; //retardo para lectura IR
unsigned long t2 = 1000; //retardo para lectura RTC
unsigned long previousMillis = 0;
unsigned long previousMillist2 = 0;
unsigned long currentMillis = 0;


void setup() {
  Serial.begin(9600);
  if (! rtc.begin()) {       // si falla la inicializacion del modulo
  Serial.println("Modulo RTC no encontrado !");  // muestra mensaje de error
  while (1);         // bucle infinito que detiene ejecucion del programa
  }
  IrReceiver.begin(SENSOR, DISABLE_LED_FEEDBACK); 
  pinMode(RELAY1, OUTPUT);    // pin 2 como salida
  pinMode(RELAY2, OUTPUT);    // pin 3 como salida
  pinMode(LED_BUILTIN, OUTPUT); // initialize digital pin LED_BUILTIN as an output.
} 

void loop() {
  currentMillis = millis();
  if (currentMillis - previousMillis >= t1)
  {
  previousMillis = millis();
  DateTime fecha = rtc.now();
  if (IrReceiver.decode()) {        // si existen datos ya decodificados
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); // imprime valor en hexadecimal en monitor
    if (IrReceiver.decodedIRData.decodedRawData == Boton_1)   // si codigo recibido es igual a Boton_1
    digitalWrite(RELAY1, !digitalRead(RELAY1)); // enciende o apaga componente rojo

    if (IrReceiver.decodedIRData.decodedRawData == Boton_2)   // si codigo recibido es igual a Boton_2
    digitalWrite(RELAY2, !digitalRead(RELAY2)); // enciende o apaga componente verde

    if (IrReceiver.decodedIRData.decodedRawData == Boton_3)  // si codigo recibido es igual a Boton_2
    {
      if(AUTO == 0)
      { 
        AUTO = 1;
        Serial.println("Modo auto ON");
      }else
      {
        AUTO = 0;
        Serial.println("Modo auto OFF");
      }
    
     }  
    
    IrReceiver.resume();        // resume la adquisicion de datos
  }
  if (fecha.hour() == 21 && fecha.minute() == 50 && AUTO == 1)
    {
      Serial.println("Encendido de RELAY 2");
      digitalWrite(RELAY2, HIGH);
    }
  if (fecha.hour() == 22 && fecha.minute() == 10 && AUTO == 1)
    {
      Serial.println("Apagando de RELAY 2");
      digitalWrite(RELAY2, LOW);
    }
    
  if(currentMillis - previousMillist2 >= t2 && AUTO == 1)
  {
     digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
     previousMillist2 = millis();
     Serial.print(fecha.hour());
     Serial.println(fecha.minute());
  }
  }
}
