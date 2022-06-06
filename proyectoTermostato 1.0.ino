/*
 * Codigo para placa controladorea de heladera
 * desarrollado por Esteban Luis Orduna
*/
#include <SoftwareSerial.h>
//definiendo salidas dijitales:
#define ledA 13//salida pin 13 de la placa
#define ventilador 11//ventilador pin12
#define motor 10//motor compresor de gas
#define resistenciaDeshielo 12//resistencia de deshielo
#define powerRele 9 // para control de optoacoplador



//definiendo variables para implementar el netodo
//steinhart-hart  peimer metodo aun sin probar

int value = 0;
//definiendo variables para temporizador
//ncargado de secuensiar os prosesos de lectura
//y escritura debentradas y salidas
unsigned long tiempo1 = 0;//tiempo de milis
unsigned long tiempo2 = 0;//tiem trancurrido
unsigned long tiempoSegundos = 0;
boolean iniciar= true;// vatiable para el control del temporizador
 
int conteo=0;//op para el switch

//variables para el censor
int Vo; 
float R1 = 100000; // resistencia fija del divisor de tension
float logR2, R2, TEMPERATURA; 
float c1 = 0.1503269811e-03, c2 = 2.802574773e-04, c3 =-0.3689760732e-07; // coeficientes de S-H en pagina: // http://www.thinksrs.com/downloads/programs/Therm%20Calc/NTCCalibrator/NTCcalculator.htm 



//bloque inicio de comunicasiin serie   asignar comportamiento
// pinnes
void setup() {
  
 Serial.begin(9600);//nicia comunicasion serial

 tiempo1 = millis();//obtiene el tiempo del reloj interno
 pinMode(ledA, OUTPUT);//se pone pin13como salida para led testigo
 pinMode(ventilador,OUTPUT);//pin 12 como salida
 pinMode(motor,OUTPUT);//pin11como salida
 pinMode(resistenciaDeshielo,OUTPUT);//pin 10 como salida
delay(10000);
pinMode(powerRele,OUTPUT);
//digitalWrite(powerRele,LOW);
//delay(10000);
digitalWrite(powerRele,HIGH);

 Serial.print("ok \n"); //mensaje para provar comunicasion serial



  delay(1000);  
}
//siclo princial
void loop() {

//calculo de temperatura y lectura del sensor
Vo = analogRead(A0);			// lectura de A0 
//Serial.print(Vo);
R2 = R1 * (1023.0 / (float)Vo - 1.0);	// conversion de tension a resistencia 
//Serial.print("\n");
//Serial.print(R2);
logR2 = log(R2);			// logaritmo de R2 necesario para ecuacion 
TEMPERATURA = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); 	// ecuacion S-H 
TEMPERATURA = TEMPERATURA - 273.15; // Kelvin a Centigrados (Celsius) 
Serial.print("Temperatura: "); 	// imprime valor en monitor serie 
Serial.print(TEMPERATURA);
Serial.println(" C"); 

delay(500);
//BT.print(TEMPERATURA);


//condicional para iniciar temporizadir //(variable opcional)

    if(iniciar==true||TEMPERATURA>= -7){
      tiempo2 = millis();//verificasion del tiempo transcurrido desde que se inicio arduino
         if(tiempo2 > (tiempo1+1000)){  //Si ha pasado 1 segundo ejecuta el IF
             tiempo1 = millis(); //Actualiza el tiempo actual
             tiempoSegundos = tiempo1/1000;//c
            // Serial.print("Ha transcurrido: ");
             //Serial.print(tiempoSegundos);
            // Serial.println(" desde que se encendio el Arduino");
            // Serial.println(" conteo en: ");
             conteo++;// incremento la variable para el swtch
             //Serial.println(conteo);
 
             
       }
 
   }


 controlActuad();

}

void controlActuad(){
   switch(conteo){
    case 10: //10segundos despues de inicio
             Serial.print("motor HIGH \n");      
             digitalWrite(motor,HIGH);
     // iniciar= true;
         break;
    case 240: //4 minutos
          
             Serial.print("ventilador HIGH");
             digitalWrite(ventilador, HIGH);
             
         break;
    case 480: //4 minutos
  // digitalWrite(ventilador, LOW);
           
           if (TEMPERATURA>= -20){
                conteo=480;}
           
           else{
             Serial.print("motor LOW \n");
           digitalWrite(motor, LOW);
                    }
         
         break;
      
     case  9600:   //8 horas despues
              
           Serial.print("Deshielo HIGH\n");
           digitalWrite(resistenciaDeshielo, HIGH);
           Serial.print("VENTILADOR LOW\n");
           digitalWrite(ventilador, LOW);
           Serial.print("motor LOW \n");
           digitalWrite(motor, LOW);
        /*  if (TEMPERATURA>= -4){
                conteo=11100;
              
             }*/
          break;
              
    case 11100:   //25 minutos
                
            Serial.print("fin del siclo Low\n");        
            digitalWrite(resistenciaDeshielo, LOW);
            break;
                  
   case 11101:  //un segundo despues
             conteo=0;
            
           iniciar= false;
           break;
          }

 }

 
