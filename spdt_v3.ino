#include <Ethernet.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include "RestClient.h"

#define IP "192.168.0.4"
#define PORT 3000

int pushbutton1 = 22;
int pushbutton2 = 24;

int led_spdt1 = 26;
int spdt1 = 28;

int led_spdt2 = 30;
int spdt2 = 32;

RestClient client = RestClient(IP, PORT);

void setup()
{    
  Serial.begin(9600);

  pinMode(pushbutton1,INPUT);
  pinMode(pushbutton2,INPUT);

  pinMode(spdt1,INPUT);
  pinMode(led_spdt1, OUTPUT);

  pinMode(spdt2,INPUT);
  pinMode(led_spdt2, OUTPUT);
  
  Serial.println("Conectado a la red:");
  client.dhcp();

  Serial.print("IP de Placa Ethernet Shield:");
  Serial.println(Ethernet.localIP());
  
  Serial.println("Listo :D!");
}

void loop()
{
  boton_izquierdo();
  boton_derecho();

  int spdt_izquierdo();
  int spdt_derecho();
}

int boton_izquierdo()
  {
    int estado = 0;
    int salida = 0;
    int estado_anterior = 0;
    
    estado = digitalRead(pushbutton1);
  
    if((estado == HIGH) && (estado_anterior == LOW))
    {
      salida = 1 - salida;
    }
  
    estado_anterior = estado;
    delay(80);
    
    if(salida == 1)
    {
      Serial.print("Boton izquierdo presionado");
      Serial.println(F(" \n "));
      Serial.println(F("Preparando peticion"));
      Serial.println(F(" \n "));

      digitalWrite(led_spdt1, HIGH);

      spdt_izquierdo(salida);

      return salida;
    }
      
    else
      {
        Serial.print("\n .");
      }
  }

int boton_derecho()
  {
    int estado2 = 0;
    int salida2 = 0;
    int estado_anterior2 = 0;
    
    estado2 = digitalRead(pushbutton2);
  
    if((estado2 == HIGH) && (estado_anterior2 == LOW))
    {
      salida2 = 2 - salida2;
    }
  
    estado_anterior2 = estado2;
    delay(80);
    
    if(salida2 == 2)
    {
      Serial.print("Boton derecho presionado");
      Serial.println(F(" \n "));
      Serial.println(F("Preparando peticion"));
      Serial.println(F(" \n "));

      digitalWrite(led_spdt2, HIGH);

      spdt_derecho(salida2);

      return salida2;
    }
      
    else
      {
        Serial.print("\n . ");
      }
  }

int spdt_izquierdo(int salida)
{  
  int estadospdt = 0;
  int salidaspdt = 0;
  int estado_anteriorspdt = 0;
  
  estadospdt = digitalRead(spdt1);

  if((estadospdt == HIGH) && (estado_anteriorspdt == LOW))
  {
    salidaspdt = 1 - salidaspdt;
  }

  estado_anteriorspdt = estadospdt;
  delay(80);

    if((estadospdt == 1) && (salida == 1))
    {      
      Serial.println("Boton y Switch izquierdo presionado");
      digitalWrite(led_spdt1, LOW);

      /*postSPDTizquierdo(salida, estadospdt);

      return estadospdt;*/
    }

    else
    {
      Serial.println("switch NO presionado");
    }

    postSPDTizquierdo(salida, estadospdt);
    
    return estadospdt;
}

int spdt_derecho(int salida2)
{
  int estadospdt2 = 0;
  int salidaspdt2 = 0;
  int estado_anteriorspdt2 = 0;

  estadospdt2 = digitalRead(spdt2);

  if((estadospdt2 == HIGH) && (estado_anteriorspdt2 == LOW))
  {
    salidaspdt2 = 1 - salidaspdt2;
  }

  estado_anteriorspdt2 = estadospdt2;
  delay(80);

    if((estadospdt2 == 1) && (salida2 == 2))
    {      
      Serial.println("Boton y Switch derecho presionado");
      digitalWrite(led_spdt2, LOW);

      estadospdt2 = estadospdt2 + 1;

      //postSPDTderecho(salida2, estadospdt2);

      //return estadospdt2;
    }

    else
    {
      Serial.println("switch NO presionado");
    }

    postSPDTderecho(salida2, estadospdt2);

    return estadospdt2;
}

void postSPDTizquierdo(int salida, int estadospdt)
{
  String responseIzquierdo = "";

  client.setHeader("Content-Type: application/json");
  
  StaticJsonBuffer<200> jsonBuffer;
  char json[256];  
  JsonObject& root = jsonBuffer.createObject();
  root["boton"] = salida;
  root["palanca"] = estadospdt;
  root.printTo(json, sizeof(json));
  Serial.println(json);
  
  int statusCode = client.post("http://192.168.0.4:3000/api/product/insert", json, &responseIzquierdo);
  
  Serial.print("Status code from server: ");
  Serial.println(statusCode);
  Serial.print("responseIzquierdo body from server: ");
  Serial.println(responseIzquierdo);
  
  delay(3000);
}

void postSPDTderecho(int salida2, int estadospdt2)
{
  String responseDerecho = "";

  client.setHeader("Content-Type: application/json");
  
  StaticJsonBuffer<200> jsonBuffer;
  char json[256];  
  JsonObject& root = jsonBuffer.createObject();
  root["boton"] = salida2;
  root["palanca"] = estadospdt2;
  root.printTo(json, sizeof(json));
  Serial.println(json);
  
  int statusCode = client.post("http://192.168.0.4:3000/api/product/insert", json, &responseDerecho);
  
  Serial.print("Status code from server: ");
  Serial.println(statusCode);
  Serial.print("responseDerecho body from server: ");
  Serial.println(responseDerecho);
  
  delay(3000);
}
