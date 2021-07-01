//Se incluye la librería de Software serial
#include <SoftwareSerial.h>;
SoftwareSerial SIM900(7, 8); // Configura el puerto serial para el SIM900
char incoming_char=0; //Variable que guarda los caracteres que envia el SIM GSM
void setup(){
SIM900.begin(19200); //Configura velocidad serial para el SIM900
delay(25000); //Retardo para que encuentre la red
Serial.begin(19200); //Configura velocidad serial para el Arduino
Serial.println("OK"); //Mensaje OK en el arduino, solo comprobación 
}
void modo_recibe_mensaje(){
Serial.println("Modo recepción SMS activado");
//Configura el modo texto para enviar o recibir mensajes
SIM900.print("AT+CMGF=1\r"); //Formato de mensaje se configura en SMS
delay(100);
SIM900.print("AT+CNMI=2,2,0,0,0\r"); //Activa la recepción de mensajes
delay(15000);// Se da un tiempo de espera de 15 segundos
}
void llamar(){
// Función que permite llamar a un celular local
Serial.println("Hacer llamada");
SIM900.println("ATD 0900000004;"); //Llamar al número de celular
delay(100);
SIM900.println();
delay(15000); //Se espera por 15 segundos
SIM900.println("ATH"); // Cuelta el telefono
//Serial.println("Esperar llamada");
//delay(25000);
//SIM900.println("ATH"); // Cuelta el telefono
delay(1000);
}
void mensaje_sms(){
//Funcion para mandar mensaje de texto
SIM900.print("AT+CMGF=1\r"); // Comando AT que configura mensaje a SMS
delay(100);
SIM900.println("AT+CMGS=\"0900000004\""); //Enviar mensaje al número
delay(100);
SIM900.println("Hola"); //Mensaje a enviar
delay(100);
SIM900.println((char)26); //Comando AT que cierra modo mensaje SMS siendo ^Z a ASCII code 26
delay(100);
SIM900.println();
delay(5000); // Tiempo para que se envie el mensaje
Serial.println("Mensaje SMS enviado");
}
void loop(){
//mensaje_sms(); //Envia mensaje
llamar(); //Hace la llamada
modo_recibe_mensaje();//Activa la recepción de mensajes SMS
for(;;){
if(SIM900.available()>0){
incoming_char=SIM900.read(); //Obtiene los datos que se han enviado al SIM900
Serial.print(incoming_char); //Imprimi los valores obtenidos.
}
if(Serial.available()>0){
if(Serial.read() == 'A') break;
}
}
Serial.println("OK-2");
delay(100);
SIM900.println();
delay(30000);
while(1); // Esperar por tiempo indefinido
}
