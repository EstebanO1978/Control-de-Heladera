/*
 * Codigo para placa controladorea de heladera
 * desarrollado por Esteban Luis Orduna
*/

//definiendo salidas dijitales:
#define ledA 13//salida pin 13 de la placa
#define ventilador 11//ventilador pin12
#define motor 10//motor compresor de gas
#define resistenciaDeshielo 12//resistencia de deshielo
# define powerRele 4                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    // para control de optoacoplador


//definiendo variables para temporizador
unsigned long tiempo1 = 0;//tiempo de milis
unsigned long tiempo2 = 0;//tiem trancurrido
unsigned long tiempoSegundos = 0;
// vatiable para el control del siclo de inicio
bool start=true; 
bool estabilizar= false;
int conteo=0;//op para el switch
int stateVentilador= 0;
//definiendo variables para implementar el netodo
//steinhart-hart 
//variables para el censor
int Vo; 
float R1 = 100000; // resistencia fija del divisor de tension
float logR2, R2, TEMPERATURA; 
float c1 = 0.1503269811e-03, c2 = 2.802574773e-04, c3 =-0.3689760732e-07; // coeficientes de S-H en pagina: // http://www.thinksrs.com/downloads/programs/Therm%20Calc/NTCCalibrator/NTCcalculator.htm 

void setup() {
  Serial.begin(9600);//nicia comunicasion serial
 tiempo1 = millis();//obtiene el tiempo del reloj interno
 pinMode(ledA, OUTPUT);//se pone pin13como salida para led testigo
 pinMode(ventilador,OUTPUT);//pin 12 como salida
 pinMode(motor,OUTPUT);//pin11como salida
 pinMode(resistenciaDeshielo,OUTPUT);//pin 10 como salida
//delay(10000);
pinMode(powerRele,OUTPUT);
//digitalWrite(powerRele,LOW);
delay(10000);
digitalWrite(powerRele,HIGH);

  delay(1000);   
}
//siclo princial
void loop() {

//calculo de temperatura y lectura del sensor
Vo = analogRead(A0);			// lectura de A0 
R2 = R1 * (1023.0 / (float)Vo - 1.0);	// conversion de tension a resistencia 
logR2 = log(R2);			// logaritmo de R2 necesario para ecuacion 
TEMPERATURA = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); 	// ecuacion S-H 
TEMPERATURA = TEMPERATURA - 273.15; // Kelvin a Centigrados (Celsius) 
Serial.println(TEMPERATURA);
contadorEnSegubdos();//inicio del contador  de 8 hs
inicio();//secuencia inicial de ensedido de actuadores
sicloEnEstabilizado();// siclo que mantiene la temperatura entre -17 y -20
sicloDeDeshielo(); //siclo de deshielo que se activa cada 8 hs
}
//funcion de control de motor
void setMOTOR(int state){
  switch(state){
    case 0: 
      digitalWrite(motor,LOW);
        Serial.println("motor LOW");
      break;
    case 1:
      digitalWrite(motor,HIGH);
       Serial.println("motor HIGH"); 
      break;
    }
  }
  //funcion de control de ventilador
void setVENTILADOR(int state){
  switch(state){
    case 0:
      digitalWrite(ventilador,LOW);
        Serial.println("ventilador LOW");
      break;
    case 1:
      digitalWrite(ventilador,HIGH);
        
      break;
  }
}

//funcion de control resistencia de deshielo
void setRESDESHIELO(int state){
  switch(state){
    case 0: 
      digitalWrite(resistenciaDeshielo,LOW);
             
         break;
    case 1:
      digitalWrite(resistenciaDeshielo,HIGH);
                 
         break;
    }
  }
//funcion de secuencia  de inicio
void inicio(){
    if(start == true){
        if(conteo == 10){//cuando conteo 10 segundos deespues de el inicio
          setMOTOR(1);
          }
        if(conteo == 240){//a los 4 minutos
          setVENTILADOR(1);   
            start = false;
          }
         }
      } 
  //funcion  para estabilizar temperatura y mantenerla entre -17 y -20
void sicloEnEstabilizado(){
  
  if (TEMPERATURA  <= -20 ){// limite de temperatura cota inferior
    setMOTOR(0); //llamado a la fun//llamado a la funcion mototr paso por parametro 0 apaga MOTORcion mototr paso por parametro 0 apaga MOTOR
     estabilizar= true;   // variable para la abilitar el condicional de los -16
       
  }
  if(TEMPERATURA  >= -16 && estabilizar==true){ //limite de temperatura  cota superior
        setMOTOR(1); //llamado a la funcion mototr paso por parametro 1enciende MOTOR
        setVENTILADOR(1);//llamado a la funcion ventilador paso por parametro 1enciende ventilador MOTOR
        setRESDESHIELO(0); //llamado a la funcion resistencia de deshielo paso por parametro 0  desactiva resistencia
        estabilizar==false;
                
  }
}
//funcion funcion para el deshielo
void sicloDeDeshielo(){
  if(conteo == 28800){//a las 8hs
        setMOTOR(0);
        setVENTILADOR(0);
        setRESDESHIELO(1);
  }   
   if(conteo == 29016 || estabilizar==true){//8hs 6minutos
        setRESDESHIELO(0);
    estabilizar==true;
       }
}
//funsion temporizador de 8 hs
void contadorEnSegubdos(){

        tiempo2 = millis();//verificasion del tiempo transcurrido desde que se inicio arduino
         if(tiempo2 > (tiempo1+1000)){  //Si ha pasado 1 segundo ejecuta el IF
             tiempo1 = millis(); //Actualiza el tiempo actual
             tiempoSegundos = tiempo1/1000;//c
          
             conteo++;// incremento la variable para el swtch
             
                 if (conteo >=29052){//8.07horas reinicia conteo
               conteo=0;
               }
             }
        }
 



 
