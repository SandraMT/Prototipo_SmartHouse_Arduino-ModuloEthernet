  /* Adaptador ETHERNET */
#include <EtherCard.h>

static byte mymac[] = {0xDD,0xDD,0xDD,0x00,0x01,0x05};  // DIRECCIÓN MAC DEL DISPOSITIVO
static byte myip[] = {192,168,1,177};                   // DIRECCIÓN IP DEL DISPOSITIVO
byte Ethernet::buffer[700];                             // Tamaño del buffer 

  /*SENSOR TEMPERATURA Y HUMEDAD*/
  // Libreria para Sensor DHT
    #include <DHT.h>

  // Libreria para Display LCD
    #include  <LiquidCrystal.h>

/**/
const int pinBuzzer = 10;
/**/

  /*HABITACION*/
  int HABITACION = 13; // Pin digital para el LED  
  char encendido_habitacion; // Variable para guardar el valor del carácter enviado

  /*LUZ PRINCIPAL CONTROLADA POR LDR*/
   int principal = 12;
   int sensorReading;//Pin análogo en espera

  // Teperatura y humedad
    int SENSOR = 11;
    int temp, humedad;
    
    DHT dht (SENSOR, DHT11); //id del sensor. Puede ser DHT11 o DHT22

  //  LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // pantalla
  
   
  void setup()  {
    
    Serial.begin(9600); // Velocidad de comunicación en baudios.
 
   if (!ether.begin(sizeof Ethernet::buffer, mymac, 10))
      Serial.println("No se ha podido acceder a la controlador Ethernet");
   else
      Serial.println("Controlador Ethernet inicializado");
 
   if (!ether.staticSetup(myip))
      Serial.println("No se pudo establecer la dirección IP");
    Serial.println();
    
    pinMode(HABITACION, OUTPUT); // Pin digital del LED como salida.
    pinMode(principal,OUTPUT);  // habitacion principal

    /*HUMEDAD Y TEMPERATURA*/
     dht.begin(); 
    // lcd.begin(16, 2);

  }

  void loop()  {

  word len = ether.packetReceive();
    word pos = ether.packetLoop(len);

     if (pos) 
   {
      if (strstr((char *)Ethernet::buffer + pos, "GET /?foco=L") != 0) {
         Serial.println("Foco Apagado");
         digitalWrite(pinLed1, LOW);
         statusLed1 = "OFF";
      }
 
      if (strstr((char *)Ethernet::buffer + pos, "GET /?foco=L") != 0) {
         Serial.println("Foco Encendido");
         digitalWrite(pinLed1, HIGH);
         statusLed1 = "ON";
      }
 
      if (strstr((char *)Ethernet::buffer + pos, "GET /?puerta=C") != 0) {
         Serial.println("Puerta Cerrada");
         
         
      }
 
      if (strstr((char *)Ethernet::buffer + pos, "GET /?puerta=A") != 0) {
         Serial.println("Puerta abierta");
         
         
      }

      if (strstr((char *)Ethernet::buffer + pos, "GET /?LCD=E") != 0) {
         Serial.println("LCD Encendido");
         
         
      }

      if (strstr((char *)Ethernet::buffer + pos, "GET /?LCD=A") != 0) {
         Serial.println("LCD Apagado");
         
         
      }
 
 
      ether.httpServerReply(mainPage());
   }
    
    Habitacion();
    Principal ();

    Humedad();
  }
   
  void Habitacion ()  {
    
    if (Serial.available() > 0) // Si se recibe un carácter...
    {
      encendido_habitacion = Serial.read(); // Guardamos el valor del carácter en la variable entrada.
   
      if ((encendido_habitacion=='H')||(encendido_habitacion=='h')) // Si el carácter recibido es "D" o "d"
      {      
        digitalWrite(HABITACION, HIGH); // Se enciende el LED        
        //Serial.write(encendido_habitacion); // verifica valor recibido
        Serial.print("Luz de habitacion encendida");  //estado de la habitacion
        Serial.print("\n");
      }
      else if ((encendido_habitacion=='L')||(encendido_habitacion=='l')) // Si el carácter recibido es "I" o "i"    
      {
        digitalWrite(HABITACION, LOW); // Se apaga el LED    
       // Serial.write(encendido_habitacion); // verfica valor recibido
        Serial.print("Luz de habitacion apagada"); //estado de la habitacion
        Serial.print("\n");
      }
    }
  }

  void Principal(){
    /* Si esta recibiendo luz el led permanece a pagado, de lo contrario se enciende.*/
    
    sensorReading=analogRead(0); //Instrucción para obtener dato analógico
      if (sensorReading<200) {  // no recibe luz = es de noche
        digitalWrite(principal,HIGH);
            
    } else  // recibe luz = es de día
        digitalWrite(principal,LOW);
        Serial.println(sensorReading); 

  }


void Humedad (){
  //lcd.home(); // Iniciamos el curso del lcd en el inicio
  humedad = dht.readHumidity();  // lee temperatura
  temp = dht.readTemperature();   // lee humedad

  //lcd.print("Temperatura:");
  //lcd.print (temp);

//serial  
  Serial.print("Temperatura: ");
  Serial.println(temp);

  //lcd.setCursor(0,1);

  //lcd.print("Humedad: ");
  //lcd.print(humedad);
  
//serial
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.println("%");

  delay(500);

    // alarma para exceso de humedad
  if( humedad>=200){
    tone(pinBuzzer, 523, 300);
  }else 
  noTone(pinBuzzer);

}

static word mainPage()
{
   BufferFiller bfill = ether.tcpOffset();
   bfill.emit_p(PSTR("HTTP/1.0 200 OKrn"
      "Content-Type: text/htmlrnPragma: no-cachernRefresh: 5rnrn"
      "<html><head><title>Casa Inteligente</title></head>"
      "<body><center><div style='text-align:center; background-color: chartreuse; width: 30%; height: 280px ; box-shadow: 10px 10px 29px 0px rgba(0,0,0,0.75); border-radius: 15px; '>"
      "<h1 style='font-family: Century gothic'>Habitación</h1>"
      "<br />FOCO DE HABITACIÓN<br />"
      "<a href='./?foco=L'><input type='button' value='Apagado'></a>"
      "<a href='./?foco=H'><input type='button' value='Encendido'></a>"
      "<br /><br />PUERTA DE HABITACIÓN<br />"
      "<a href='./?puerta=C'><input type='button' value='Cerrada'></a>"
      "<a href='./?puerta=A'><input type='button' value='Abierta'></a>"
      "<br /><br />LCD<br />"
      "<a href='./?LCD=C'><input type='button' value='Desactivado'></a>"
      "<a href='./?LCD=A'><input type='button' value='Activado'></a>"
      "<br /></div>\n</center></body></html>"));
 
   return bfill.position();
}
