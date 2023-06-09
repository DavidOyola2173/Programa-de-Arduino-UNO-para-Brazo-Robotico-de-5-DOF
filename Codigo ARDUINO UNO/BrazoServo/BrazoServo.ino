/*
 * Programa de Arduino UNO para Brazo Robotico de 5 DOF:
 * Hecho por: David Santiago Oyola Lozano
 * 
 * Tabla de conexiones servos a arduino:
 * | Número de Servo | Número de Pin |
 * |       3         |       3       |
 * |       5         |       5       |
 * |       6         |       6       |
 * |       9         |       9       |
 * |       10        |       10      |
 * |       11        |       11      |
 * 
 * Otras conexiones:
 * Pin GND arduino a la tierra física del circuito.
 * 
 * Tipos de Sermotor utilizados:
 * Tipo servomotores 5, 6, 9 y 10: SG90, link documentacion: http://www.ee.ic.ac.uk/pcheung/teaching/DE1_EE/stores/sg90_datasheet.pdf 
 * Tipo servomotores 3 y 11: MG90S, link documentacion: https://pdf1.alldatasheet.com/datasheet-pdf/view/1132104/ETC2/MG90S.html
 * 
 * Aclaraciones:
 * Los servomotores 5 y 6 son gemelos y van anclados al mismo eje por lo que a pesar de que se declaren diferentes ambos se programaron simultaneamente en el codigo.
 * 
 * Link del Github del proyecto:
 * https://github.com/DavidOyola2173/Programa-de-Arduino-UNO-para-Brazo-Robotico-de-5-DOF.git
 * 
 */

//INICIO DEL CODIGO
//==========================================================================================================================================================================================================================================

//LIBRERIAS:

//Se incluyen las librerias respectivas:
#include <Servo.h>

//==========================================================================================================================================================================================================================================

//DEFINICION DE PINES Y CONEXIONES DEL SISTEMA:

//Se declaran cada servomotor del brazo robotico con un respectivo nombre con la funcion "Servo" de la librería Servo de Arduino:
Servo servo3;
Servo servo5;
Servo servo6;
Servo servo9;
Servo servo10;
Servo servo11;

//==========================================================================================================================================================================================================================================

//CREACION E INICIALIZACION DE VARIABLES PARA CALCULOS INTERNOS:

//Declaración de variables:
int PULSOMIN = 1000;                        //Pulso mínimo indicativo de 0° para el servo
int PULSOMAX = 2000;                        //Pulso máximo indicativo de 180° para el servo
int pos = 0;                                //Variable para guardar la posicion del servo

int tiempoRetardo = 15;                     //Tiempo de retardo para los movimientos de SweepIn y SweepOut:
boolean mensaje;                            //Variable booleana para mostrar el mensaje de la funcion seleccionada en el monitor serial
boolean comprobante[3] = {false, false, false}; //Lista de las variables para el sistema Poka-Yoke del programa.

//==========================================================================================================================================================================================================================================

//PROGRAMA PRINCIPAL:

void setup() {
  Serial.begin(9600);                       //Inicialización del monitor serial a 9600 Baudios
  
  servo3.attach(3, PULSOMIN, PULSOMAX);     //Adjuntamos cada servo declarado con su respectivo pin de arduino donde está conectado el servo.
  servo5.attach(5, PULSOMIN, PULSOMAX);     //(Y se le incluyen el rango de movimiento de este mediante PULSOMIN y PULSOMAX, oasea de 0° a 180°)
  servo6.attach(6, PULSOMIN, PULSOMAX);
  servo9.attach(9, PULSOMIN, PULSOMAX);
  servo10.attach(10, PULSOMIN, PULSOMAX);
  servo11.attach(11, PULSOMIN, PULSOMAX);

//  servo3.write(45);                       //Esta seccion es para cambiar la posicion inicial de los servomotores del brazo robotico
//  servo5.write(75);                       //arduino por default define la posicion de cualquier servomotor conectado a 90°
//  servo6.write(75);                       //pero a veces ese angulo default de 90° puede alterar el mecanismo del brazo
//  servo9.write(180);                      //o llevarlo a una posición incorrecta que pueda dañar los servomotores involucrados
//  servo10.write(180);                     //por lo que esta seccion se incluye opcional para cambiar los servomotores a una posicion mas comoda
  servo11.write(85);                        //si asi lo decide el usuario (Eso si debe ubicar empíricamente el nuevo angulo que quiera indicar)
  
  mensaje = true;
}

void loop() {
  //En esta seccion se incluyen tres funciones creadas en el código, SOLO SE PUEDE USAR UNA DE LAS TRES
  //Para usar alguna funcion se debe comentar las demás y descomentar la funcion que quiera utilizar el usuario
  //(La explicacion de lo que hace cada funcion esta mas abajo en el codigo, leerlo antes de usar alguna funcion)
   
  //ejecutar();
  secuencia();
  //verificar();
}

//==============================================================================================================================================================================================

//FUNCIONES EXTERNAS:

/*
 * Funcion Ejecutar():
 * Esta funcion se encarga de mover solo 1 servo al angulo que le indique el usuario por medio del monitor serial del arduino
 * 
 * Primero el arduino recibe el mensaje enviado por el monitor serial
 * El mensaje debe ser de la siguiente forma:
 * 
 * FORMA:   "|Número de Servo a mover| |Angulo a mover|"
 *                                    ↑ ESTE ESPACIO SE DEBE PONER TAMBIEN
 * EJEMPLO: "3 90"     
 *            ↑ESPACIO
 *            
 * Es muy importante conservar esa forma del mensaje a enviar del monitor serial, ya que el primer numero indica el servomotor a mover
 * y el segundo numero indica el angulo destino donde tendra que moverse el servomotor
 * ambos van separados por un espacio y es el caracter separador para que el arduino reciba el mensaje entero y separe ambos numeros
 * para que los procese en su respectivo tipo
 * 
 * Ya con el mensaje del usuario recibido por el monitor serial el arduino sabe que servomotor mover y a que angulo moverlo
 */
void ejecutar(){
  //Parte del sistema Poka-Yoke del programa que evita que más de una funcion se ejecute a la vez en el codigo:
  comprobante[0] = true;
  if((comprobante[1] == true) || (comprobante[2] == true)){
    Serial.println("ATENCION! Tiene más de una función activada, verifique el codigo y evite usar más de una funcion");
    Serial.println("PROGRAMA DETENIDO POR SEGURIDAD");
    delay(55156);
  }
  
  //Parte donde se indica por unos segundos la funcion activada:
  if(mensaje == true){
    Serial.println("Funcion EJECUTAR() ACTIVADO, revise codigo si no es la funcion deseada...");
    delay(2000);
    Serial.println("Iniciando en... 3");
    delay(1000);
    Serial.println("............... 2");
    delay(1000);
    Serial.println("............... 1");
    delay(1000);
    Serial.println();
    mensaje = false;
  }
  
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');  //Leer el texto hasta encontrar un salto de línea
    
    int espacioIndex = input.indexOf(' ');  //Encontrar el índice del espacio
    
    String valor1 = input.substring(0, espacioIndex);  //Extraer el primer valor
    String valor2 = input.substring(espacioIndex + 1);  //Extraer el segundo valor
    
    int numeroServo = valor1.toInt();  //Convertir el primer valor a entero
    int anguloEnvio = valor2.toInt();  //Convertir el segundo valor a entero
    
    //Se hace un switch case para clasificar que servomotor usar segun el servo especificado por el usuario
    switch(numeroServo){
      case 3:
        servo3.write(anguloEnvio);
        Serial.println("Servo 3: "); Serial.println(servo3.read());
      break;
      case 5:
        servo5.write(180-anguloEnvio);
        servo6.write(anguloEnvio);
        Serial.println("Servo 5 (GEMELO 6): "); Serial.println(servo5.read());
        Serial.println("Servo 6: "); Serial.println(servo6.read()); 
      break;
      case 6:
        servo5.write(180-anguloEnvio);
        servo6.write(anguloEnvio);
        Serial.println("Servo 5 (GEMELO 6): "); Serial.println(servo5.read());
        Serial.println("Servo 6: "); Serial.println(servo6.read()); 
      break;
      case 9:
        servo9.write(anguloEnvio);
        Serial.println("Servo 9: "); Serial.println(servo9.read());
      break;
      case 10:
        servo10.write(anguloEnvio);
        Serial.println("Servo 10: "); Serial.println(servo10.read());
      break;
      case 11:
        servo11.write(anguloEnvio);
        Serial.println("Servo 11: "); Serial.println(servo11.read());
      break;
      default:
        Serial.println("No se ha encontrado el servo especificado");  //Si el usuario digita un numero que no corresponde a ningun servo
      break;
    }
  }
}

/*
 * Funcion Secuencia():
 * Esta funcion es experimental
 * Se usa las subfunciones SweepIn y SweepOut para mover el brazo mecanico a las diferentes posiciones para generar una secuencia
 * 
 * Hay una secuencia de prueba implementada pero si el usuario desea cambiar alguna posicion puede cambiar los angulos de las subfunciones y asi cambiar la posicion
 */
void secuencia(){
  //Parte del sistema Poka-Yoke del programa que evita que más de una funcion se ejecute a la vez en el codigo:
  comprobante[1] = true;
  if((comprobante[0] == true) || (comprobante[2] == true)){
    Serial.println("ATENCION! Tiene más de una función activada, verifique el codigo y evite usar más de una funcion");
    Serial.println("PROGRAMA DETENIDO POR SEGURIDAD");
    delay(55156);
  }
  
  //Parte donde se indica por unos segundos la funcion activada:
  if(mensaje == true){
    Serial.println("Funcion SECUENCIA() ACTIVADO, revise codigo si no es la funcion deseada...");
    delay(2000);
    Serial.println("Iniciando en... 3");
    delay(1000);
    Serial.println("............... 2");
    delay(1000);
    Serial.println("............... 1");
    delay(1000);
    Serial.println();
    mensaje = false;
  }
  
  //El brazo se ubica en la posicion Inicial:
  sweepOut(3, 45);
  sweepOut(6, 20);
  sweepIn(9, 180);
  sweepIn(10, 180);

  delay(500);
  sweepOut(11, 50);  //El brazo espera medio segundo y abre la pinza
  sweepOut(6, 0);
  delay(1500);
  sweepIn(11, 90);  //El brazo espera 1.5 segundos y cierra la pinza
  sweepIn(6, 20);
  delay(3000);      //Espera de 3 segundos para asegurar la carga en la pinza (Se puede acortar este tiempo si se desea)

  //El brazo se levanta en una posicion superior:
  sweepOut(10, 90);
  sweepOut(9, 90);
  sweepIn(6, 90);
  sweepIn(3, 90);
  
  delay(250);

  //El brazo luego de esperar 250 milisegundos se ubica en la posicion destino:
  sweepIn(3, 180);
  sweepOut(6, 75);
  sweepIn(9, 180);
  sweepIn(10, 180);

  delay(500);
  sweepOut(11, 0);  //El brazo espera medio segundo y abre la pinza
  delay(1500);
  sweepIn(11, 90);  //El brazo espera 1.5 segundos y cierra la pinza
  delay(3000);      //Espera de 3 segundos para asegurar la carga en la pinza (Se puede acortar este tiempo si se desea)

  //El brazo se levanta en una posicion superior:
  sweepOut(10, 90);
  sweepOut(9, 90);
  sweepIn(6, 90);
  sweepIn(3, 90);
  
  delay(250);
}

/*
 * Funcion Verificar():
 * Funcion dedicada solo a comprobar el estado de movimiento de un solo servomotor
 * 
 * La funcion Verificar() sirve para comprobar el movimiento de un servomotor, si observa algun comportamiento raro de algun servomotor del brazo utilice esta funcion
 * Si se quiere usar esta funcion se debe programar el servo a analizar al interior de la funcion, de la siguiente manera:
 * 
 * EJEMPLO:
 * El servo numero 3 no se mueve o se esta moviendo de manera incorrecta en el brazo, por lo que tengo que usar esta funcion
 * si la quiero usar debo cambiar todos los servo.read() y servo.write() ubicados en la funcion de la siguiente manera:
 * 
 *                 ↓ Se debe cambiar aqui el nombre del servo a utilizar, en este caso va a ser el servo numero 3, si se quiere otro servo, por ejemplo el 5, se pone servo5.read() y así con los demas servos.
 * for (pos = servo3.read(); pos <= 180; pos += 1) { // Va desde 0 grados hasta 180 en pasos de 1 grado por 15 milisegundos
 *  servo3.write(pos);              // Enviamos el angulo al servomotor
 *       ↑Se debe cambiar aqui el nombre del servo a utilizar, en este caso va a ser el servo numero 3, si se quiere otro servo, por ejemplo el 5, se pone servo5.read() y así con los demas servos.
 *  delay(tiempoRetardo);                       // Esperamos 15 milisegundos
 *}
 *
 * Revise bien si todos los servo.read() y servo.write() estan seleccionando al servo correcto, porque si no estarán analizando y moviendo el servo incorrecto.
 */
void verificar(){
  //Parte del sistema Poka-Yoke del programa que evita que más de una funcion se ejecute a la vez en el codigo:
  comprobante[2] = true;
  if((comprobante[0] == true) || (comprobante[1] == true)){
    Serial.println("ATENCION! Tiene más de una función activada, verifique el codigo y evite usar más de una funcion");
    Serial.println("PROGRAMA DETENIDO POR SEGURIDAD");
    delay(55156);
  }
  
  //Parte donde se indica por unos segundos la funcion activada:
  if(mensaje == true){
    Serial.println("Funcion VERIFICAR() ACTIVADO, revise codigo si no es la funcion deseada...");
    delay(2000);
    Serial.println("Iniciando en... 3");
    delay(1000);
    Serial.println("............... 2");
    delay(1000);
    Serial.println("............... 1");
    delay(1000);
    Serial.println();
    mensaje = false;
  }
  
  for (pos = servo11.read(); pos <= 180; pos += 1) { // Va desde 0 grados hasta 180 en pasos de 1 grado por 15 milisegundos
    servo11.write(pos);              // Enviamos el angulo al servomotor
    Serial.println(pos);
    delay(tiempoRetardo);                       // Esperamos 15 milisegundos
  }
  
  for (pos = servo11.read(); pos >= 0; pos -= 1) { // Va desde 180 grados hasta 0 en pasos de -1 grado por 15 milisegundos
    servo11.write(pos);              // Enviamos el angulo al servomotor
    Serial.println(pos);
    delay(tiempoRetardo);                       // Esperamos 15 milisegundos
  }
}

//==============================================================================================================================================================================================

//SUBFUNCIONES ESPECIALES:

/*
 * Subfuncion SweepIn:
 * Funcion dedicada a mover suavemente el servomotor del brazo robotico que se le indique a un angulo especificado.
 * 
 * La funcion necesita de dos valores que le programa el usuario
 * El primer valor es el numero del servo a mover
 * El segundo valor es el angulo a mover
 * 
 * SweepIn se encarga de mover el servomotor desde una posicion inicial hasta una posicion final POSITIVA, es decir:
 * 
 * EJEMPLO:
 * Si se quiere mover el servo numero 3 de 90° a 180° se usa la funcion SweepIn ya que el angulo final es mayor al angulo de inicio, osea 90°<180°
 * Y se programaría de la siguiente manera:
 * 
 * FORMA DE PROGRAMAR: sweepIn(|Numero del Servo|, |Angulo a mover|)
 * EJEMPLO: sweepIn(3, 180);
 * 
 * Esto esta definido de esta manera ya que internamente la funcion usa ciclos For crecientes,
 * si se quisiera un caso contrario, por ejemplo llevar el servomotor de 90° a 0° se debe usar la funcion SweepOut ya que SweepIn solo permite aumentar el angulo del servo, no reducirlo.
 * 
 */
void sweepIn(int numeroServo, int anguloServo){
  switch(numeroServo){
      case 3:
        for (pos = servo3.read(); pos <= anguloServo; pos += 1) {
          servo3.write(pos);
          Serial.println("Servo 3: "); Serial.println(servo3.read());            
          delay(tiempoRetardo);                       
        }
      break;
      case 6:
        for (pos = servo6.read(); pos <= anguloServo; pos += 1) { 
          servo6.write(pos);
          servo5.write(180-pos);
          Serial.println("Servo 5 (GEMELO 6): "); Serial.println(servo5.read());
          Serial.println("Servo 6: "); Serial.println(servo6.read());        
          delay(tiempoRetardo);                       
        }
      break;
      case 9:
        for (pos = servo9.read(); pos <= anguloServo; pos += 1) { 
          servo9.write(pos);
          Serial.println("Servo 9: "); Serial.println(servo9.read());             
          delay(tiempoRetardo);                       
        }
      break;
      case 10:
        for (pos = servo10.read(); pos <= anguloServo; pos += 1) { 
          servo10.write(pos);
          Serial.println("Servo 10: "); Serial.println(servo10.read());      
          delay(tiempoRetardo);                       
        }
      break;
      case 11:
        for (pos = servo11.read(); pos <= anguloServo; pos += 1) { 
          servo11.write(pos);
          Serial.println("Servo 11: "); Serial.println(servo11.read());           
          delay(tiempoRetardo);                       
        }
      break;
      default:
        Serial.println("No se ha encontrado el servo especificado");
      break;
  }
}

/*
 * Subfuncion SweepOut:
 * Funcion dedicada a mover suavemente el servomotor del brazo robotico que se le indique a un angulo especificado.
 * 
 * La funcion necesita de dos valores que le programa el usuario
 * El primer valor es el numero del servo a mover
 * El segundo valor es el angulo a mover
 * 
 * SweepOut se encarga de mover el servomotor desde una posicion inicial hasta una posicion final NEGATIVA, es decir:
 * 
 * EJEMPLO:
 * Si se quiere mover el servo numero 3 de 90° a 0° se usa la funcion SweepOut ya que el angulo final es menor al angulo de inicio, osea 90°>0°
 * Y se programaría de la siguiente manera:
 * 
 * FORMA DE PROGRAMAR: sweepOut(|Numero del Servo|, |Angulo a mover|)
 * EJEMPLO: sweepOut(3, 0);
 * 
 * Esto esta definido de esta manera ya que internamente la funcion usa ciclos For decrecientes,
 * si se quisiera un caso contrario, por ejemplo llevar el servomotor de 90° a 180° se debe usar la funcion SweepIn ya que SweepOut solo permite disminuir el angulo del servo, no aumentarlo.
 * 
 */
void sweepOut(int numeroServo, int anguloServo){
   switch(numeroServo){
      case 3:
        for (pos = servo3.read(); pos >= anguloServo; pos -= 1) { 
          servo3.write(pos);
          Serial.println("Servo 3: "); Serial.println(servo3.read());             
          delay(tiempoRetardo);                       
        }
      break;
      case 6:
        for (pos = servo6.read(); pos >= anguloServo; pos -= 1) { 
          servo6.write(pos);
          servo5.write(180-pos);
          Serial.println("Servo 5 (GEMELO 6): "); Serial.println(servo5.read());
          Serial.println("Servo 6: "); Serial.println(servo6.read());             
          delay(tiempoRetardo);                       
        }
      break;
      case 9:
        for (pos = servo9.read(); pos >= anguloServo; pos -= 1) { 
          servo9.write(pos);
          Serial.println("Servo 9: "); Serial.println(servo9.read());            
          delay(tiempoRetardo);                       
        }
      break;
      case 10:
        for (pos = servo10.read(); pos >= anguloServo; pos -= 1) {
          servo10.write(pos);
          Serial.println("Servo 10: "); Serial.println(servo10.read());           
          delay(tiempoRetardo);                       
        }
      break;
      case 11:
        for (pos = servo11.read(); pos >= anguloServo; pos -= 1) {
          servo11.write(pos);
          Serial.println("Servo 11: "); Serial.println(servo11.read());           
          delay(tiempoRetardo);                      
        }
      break;
      default:
        Serial.println("No se ha encontrado el servo especificado");
      break;
  }
}
