#include <IRremote.h>      // importa libreria IRremote
#define Boton_1 0xBA45FF00    // reemplaza cada instancia de Boton_1 por su valor HEX
#define Boton_2 0xB946FF00    // reemplaza cada instancia de Boton_2 por su valor HEX
#define Boton_3 0xB847FF00    // reemplaza cada instancia de Boton_3 por su valor HEX

int SENSOR = 11;   // sensor KY-022 

int RELAY1 = 2;      
int RELAY2 = 3;     


unsigned long t1 = 100; //retardo para lectura IR
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;


void setup() {
  Serial.begin(9600);     
  IrReceiver.begin(SENSOR, DISABLE_LED_FEEDBACK); 
  pinMode(RELAY1, OUTPUT);    // pin 2 como salida
  pinMode(RELAY2, OUTPUT);    // pin 3 como salida
} 

void loop() {
  currentMillis = millis();
  if (currentMillis - previousMillis >= t1)
  {
  if (IrReceiver.decode()) {        // si existen datos ya decodificados
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); // imprime valor en hexadecimal en monitor
    if (IrReceiver.decodedIRData.decodedRawData == Boton_1)   // si codigo recibido es igual a Boton_1
    digitalWrite(RELAY1, !digitalRead(RELAY1)); // enciende o apaga componente rojo

    if (IrReceiver.decodedIRData.decodedRawData == Boton_2)   // si codigo recibido es igual a Boton_2
    digitalWrite(RELAY2, !digitalRead(RELAY2)); // enciende o apaga componente verde
    
    IrReceiver.resume();        // resume la adquisicion de datos
  }
  }
}
