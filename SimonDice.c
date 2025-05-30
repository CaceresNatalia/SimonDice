

#define TONE_USE_INT
#define TONE_PITCH 440
#include <TonePitch.h>


#define trig 12
#define echo A5
#define parlante 3
#define boton1 4
#define boton2 5
#define boton3 6
#define boton4 7
#define led1 8
#define led2 9
#define led3 10
#define led4 11
#define APRETADO HIGH
#define NOAPRETADO LOW




/*Todo este bloque declara e inicializa las variables
referentes a manejar el rebote de los botones*/
unsigned long millisAnteriores = 0;
unsigned long millisActuales;
int estadoBoton1Anterior = LOW;
int estadoBoton2Anterior = LOW;
int estadoBoton3Anterior = LOW;
int estadoBoton4Anterior = LOW;
unsigned long millisBoton1 = 0;
unsigned long millisBoton2 = 0;
unsigned long millisBoton3 = 0;
unsigned long millisBoton4 = 0;


int nivelActual;
//Declaramos una variable donde asignar la cantidad máxima de notas/leds que tendrá la secuencia.
int nivelMax = 4;
//Creamos un array para guardar la secuencia de notas.
long melodia[4];
//Este array es para generar una secuencia al azar y luego cargamos en el anterior las
//notas.
int secuenciaRandom[4];
//Otro array para guardar la secuencia de luces asociadas a las notas
int leds[4];
//uno para los botones
int botones[4];
int error;
//Un array para guardar los botones presionados por el usuario para luego compararlos con la secuencia generada.
int secuenciaUsuario[4];
/*guardamos en un array los botones para pasarlos a la función
que evalúa si están presionados.*/
int botonPresionado[4]={boton1,boton2,boton3,boton4};


void setup() {
   
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    digitalWrite(trig, LOW);//Inicializamos en 0
    pinMode(parlante, OUTPUT);
    pinMode(boton1, INPUT);
    pinMode(boton2, INPUT);
    pinMode(boton3, INPUT);
    pinMode(boton4, INPUT);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);
  Serial.begin(9600);
    /*Generamos una semilla de numeros al azar para que no repita siempre la misma secuencia.
    Al pasarle analogRead(0) recibe ruido de la placa del Arduino.*/
    randomSeed(analogRead(0));


}


void loop() {
    Serial.print("Nivel:");
    Serial.println(nivelActual);
    nivelActual = 1;
    error = 0;  
 
    digitalWrite(led1,HIGH);
    digitalWrite(led2,HIGH);
    digitalWrite(led3,HIGH);
    digitalWrite(led4,HIGH);
    delay(1000);  
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(led4,LOW);
    delay(1000);
     
     
    long t; //tiempo que demora en llegar el eco
    long d; //distancia en centimetros


    digitalWrite(trig, HIGH);
    delayMicroseconds(10);//Enviamos un pulso de 10us
    digitalWrite(trig, LOW);
 
  t = pulseIn(echo, HIGH);
    d = t/59;//escalamos el tiempo a una distancia en cm segun ficha tecnica.
 
    Serial.print("Distancia: ");
    Serial.print(d);      
    Serial.print("cm");
    Serial.println();
    delay(100);
    /*Cuando el sensor de acercamiento detecta una distancia
    menor a 5 cm, inicia el juego */
 
        if(d < 5){
      generarSecuencia(); //Cambia la secuencia cada vez que se reinicia el juego.
           
      while ((nivelActual <= nivelMax)&&(error==0)){
            delay(500);
          mostrarSecuencia();
            lecturaSecuencia();
           
          if(nivelActual == (nivelMax+1)){
              victoria();
          }
               
        }if(error!=0){
                gameOver();
                         
            }        
   
    }
}


void generarSecuencia(){


  for(int i=0; i<nivelMax; i++){
        //Llenamos el array con 4 nros al azar
        secuenciaRandom[i] = random(1,5);
    }
    //Recorremos el array y reemplazamos el nro del 1 al 4 con una nota y le asignamos
    //a los otros vectores, el led y botón correspondientes a esa nota, en la misma ubicación.
    for(int j=0; j<nivelMax; j++){
        switch(secuenciaRandom[j]){
            case 1:
                melodia[j]= NOTE_C5;
                leds[j]= led1;
                botones[j]= boton1;
                break;
            case 2:
                melodia[j]= NOTE_E6;
                leds[j]= led2;
                botones[j]= boton2;
                break;
            case 3:
                melodia[j]= NOTE_G7;
                leds[j]= led3;
                botones[j]= boton3;
                break;
            case 4:
                melodia[j]= NOTE_C8;
                leds[j]= led4;
                botones[j]= boton4;
                break;


        }
        Serial.println(melodia[j]);
        Serial.println(leds[j]);
        Serial.println(botones[j]);
    }
 
}


void mostrarSecuencia(){
    for(int i=0; i< nivelActual; i++){


        tone(parlante, melodia[i], 500);
        digitalWrite(leds[i], HIGH);
        delay(700);
        digitalWrite(leds[i], LOW);
        noTone(parlante);
        delay(500);
   
    }  
 
   
}


void lecturaSecuencia(){
//Le pasamos uno a uno los botones para que evalúe cuál está presionado.
    int bandera=0;
    int k=0, j=0;
    while(bandera==0){
        for(int i = 0; i < 4; i++){
           
            //Si la función devuelve que el botón que le pasé está
            //presionado, lo guardo en el array.
            if(evaluarBoton(botonPresionado[i])){
                int capturaBoton = botonPresionado[i];
                if(capturaBoton != botones[k]){//Comprueba que el boton apretado sea el mismo al random.
                      error = 1;
                      bandera = 1;
                      return;
            }
                k++;
               
                for(j=0; j < nivelActual; j++){
                    secuenciaUsuario[nivelActual - 1] = capturaBoton;
                    Serial.println(capturaBoton);
                    Serial.println(secuenciaUsuario[nivelActual - 1]);
                    if(k==nivelActual){//comparacion cantidad botones apretados con botones requeridos segun nivel.
                    bandera=1;
                    }
                }                          
                if(bandera==1){
                    nivelActual++;
                    return;
                }
            }                
        }
    }
       
}


//La función recibe los botones y evalúa si está presionado.
bool evaluarBoton(int i){
    millisActuales = millis();
   
   
    int estadoBoton1 = digitalRead(boton1);
    int estadoBoton2 = digitalRead(boton2);
    int estadoBoton3 = digitalRead(boton3);
    int estadoBoton4 = digitalRead(boton4);
   
    switch(i){
   
        case 4:    
            if((estadoBoton1 == APRETADO) && (estadoBoton1Anterior == NOAPRETADO)){


                if((millisActuales - millisBoton1) >= 10) {//Los dos if chequean que no haya rebote en el botón
                    tone(parlante, NOTE_C5, 500);
                    digitalWrite(led1,HIGH);
                    delay(700);
                    digitalWrite(led1,LOW);
                    noTone(parlante);


                    return true;


                    estadoBoton1Anterior = estadoBoton1;
                }else{


                    estadoBoton1Anterior = estadoBoton1;
                    millisBoton1 = millisActuales;
                    return false;
                }
            }
            return false;
            break;
 
        case 5:
   
            if((estadoBoton2 == APRETADO) && (estadoBoton2Anterior == NOAPRETADO)){


                if((millisActuales - millisBoton2) >= 10) {
                    tone(parlante, NOTE_E6, 500);  
                    digitalWrite(led2,HIGH);
                    delay(700);
                    digitalWrite(led2,LOW);
                    return true;
                    estadoBoton2Anterior = estadoBoton2;


                }else{
                    estadoBoton2Anterior = estadoBoton2;
                    millisBoton2 = millisActuales;
                    return false;
                }
            }
            return false;
            break;


        case 6:
 
   
            if((estadoBoton3 == APRETADO) && (estadoBoton3Anterior == NOAPRETADO)){


                if((millisActuales - millisBoton3) >= 10) {
                    tone(parlante, NOTE_G7, 500);  
                    digitalWrite(led3,HIGH);
                    delay(700);
                    digitalWrite(led3,LOW);
                    return true;


                    estadoBoton3Anterior = estadoBoton3;
                }else{


                    estadoBoton3Anterior = estadoBoton3;
                    millisBoton3 = millisActuales;
                    return false;
                }
            }
            return false;
            break;
               
        case 7:
   
   
            if((estadoBoton4 == APRETADO) && (estadoBoton4Anterior == NOAPRETADO)){


                if((millisActuales - millisBoton4) >= 10) {
                    tone(parlante, NOTE_C8, 500);  
                    digitalWrite(led4,HIGH);
                    delay(700);
                    digitalWrite(led4,LOW);
                    return true;


                    estadoBoton4Anterior = estadoBoton4;
                }else{


                    estadoBoton4Anterior = estadoBoton4;
                    millisBoton4 = millisActuales;
                    return false;
                }
            }
            return false;
            break;
    }
}




void gameOver(){
 
    long perdiste[] = {NOTE_C6, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_B5, NOTE_C6};
    digitalWrite(led1,HIGH);
    digitalWrite(led2,HIGH);
    digitalWrite(led3,HIGH);
    digitalWrite(led4,HIGH);
         
 
    for (int i = 0; i < 6; i++) {
      tone(parlante, perdiste[i],300);
      delay(300);
      noTone(12);
           
    }
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(led4,LOW);
     
}


void victoria(){
 
  long victoria[]= {NOTE_G6, NOTE_C6, NOTE_C6, NOTE_E6, NOTE_G6, NOTE_C7};
    digitalWrite(led1,HIGH);
    delay(300);
    digitalWrite(led1,LOW);
    digitalWrite(led2,HIGH);
    delay(300);
    digitalWrite(led2,LOW);
    digitalWrite(led3,HIGH);
    delay(300);
    digitalWrite(led3,LOW);
    digitalWrite(led4,HIGH);
    delay(300);
    digitalWrite(led4,LOW);
 
  for (int i = 0; i < 6; i++){
    tone(parlante, victoria[i],400);
    delay(300);
    noTone(8);
     
  }
    digitalWrite(led1,HIGH);
    delay(300);
    digitalWrite(led1,LOW);
    digitalWrite(led2,HIGH);
    delay(300);
    digitalWrite(led2,LOW);
    digitalWrite(led3,HIGH);
    delay(300);
    digitalWrite(led3,LOW);
    digitalWrite(led4,HIGH);
    delay(300);
    digitalWrite(led4,LOW);


}








