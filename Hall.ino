#include <SD.h>
#include <Wire.h>
#include <ADXL345.h>
 //acelerometro
ADXL345 accelerometer;
//sd
File myFile;
String nombre;
//RuedaDelantera
const float pinSensor = A0;
int estadoSensor   = 0;
int estadoAnterior = 0;
int contador;
float v; 
//ruedaTrasera
const float pinSensor2 = A1;
int estadoSensor2   = 0;
int estadoAnterior2 = 0;
int contador2; 
float v2;
//delay
unsigned long tiempo = 0;
unsigned long t_actualizado = 0;
unsigned long t_delay = 2500;

//metodos

void setup()
{
  Serial.begin(9600);
  Serial.println("Inicializando ADXL345");
  nombre = "m.csv";
  if (!accelerometer.begin())
  {
    Serial.println("ADXL345 no se encuentra , compruebe la conexion!");
    delay(500);
  }

  
  accelerometer.setRange(ADXL345_RANGE_2G); 
  pinMode(pinSensor, INPUT);  
  pinMode(pinSensor2, INPUT); 
  Serial.print("Iniciando SD ...");
  if (!SD.begin(4)) {
    Serial.println("No se pudo inicializar");
    return;
  }
  Serial.println("inicializacion exitosa");
  
}

void loop()
{
  Vector norm = accelerometer.readNormalize();
  estadoSensor = analogRead(A0)*5/1023;
  estadoSensor2 = analogRead(A1)*5/1023;
  tiempo = millis();
   if (estadoSensor!=2&&estadoSensor!=estadoAnterior)
   {
     contador++;
   }
   if (estadoSensor2!=2&&estadoSensor2!=estadoAnterior2)
   {
     contador2++;
   }
    estadoAnterior = estadoSensor;
  estadoAnterior2 = estadoSensor2;
  if( tiempo >= t_actualizado + t_delay)
   {
     v=(((contador*6.28*1000)/(tiempo-t_actualizado))*0.33)*3.6;
     v2=(((contador2*6.28*1000)/(tiempo-t_actualizado))*0.33*3.6);
   double sensor1 = v;
   double sensor2 = v2;
   double sensor3 = norm.XAxis+5.5;
   myFile = SD.open(nombre, FILE_WRITE);
  if (myFile) { 
        Serial.print("Escribiendo SD: ");
        
        myFile.print(tiempo);
        myFile.print(",");
        myFile.print(sensor1);
        myFile.print(",");
        myFile.print(sensor2);
        myFile.print(",");
        myFile.println(sensor3);
        
        myFile.close(); //cerramos el archivo
        
        Serial.print("Tiempo(ms)=");
        Serial.print(tiempo);
        Serial.print("\t");
        Serial.print("Rueda delantera");
        Serial.print(sensor1);
        Serial.print("\t");
        Serial.print("Rueda Trasera");
        Serial.print(sensor2);
        Serial.print("\t");
        Serial.print("Acelerometro");
        Serial.println(sensor3);
  
  } else {
    Serial.println("Error al abrir el archivo");
  }
   t_actualizado = tiempo;
    contador=0;
    contador2=0;
   }
 
}

