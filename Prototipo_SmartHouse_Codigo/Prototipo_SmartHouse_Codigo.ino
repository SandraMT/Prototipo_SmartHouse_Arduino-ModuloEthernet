#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,177); 
EthernetServer server(80);  

String HTTP_req;          // Para guardar la peticion del cliente
boolean LED2_status = 0;   

/**********CONTROLES AUTONOMOS DE LA CASA********************/
/**/
const int pinBuzzer = 10;
/**/

  /*HABITACION*/
  int HABITACION = 2; // Pin digital para el LED  /*CAMBIO A PIN 2, ANTES 13*/
  char estado_habitacion; // Variable para guardar el valor del carácter enviado

  /*LUZ PRINCIPAL CONTROLADA POR LDR*/
   int principal = 12;
   int sensorReading;//Pin análogo en espera

  /*SENSOR TEMPERATURA Y HUMEDAD*/
  // Libreria para Sensor DHT
    #include <DHT.h>

  // Teperatura y humedad
    int SENSOR = 11;
    int temp, humedad;
    
    DHT dht (SENSOR, DHT11); //id del sensor. Puede ser DHT11 o DHT22

  //  LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // pantalla

  /*CONTROL ENTRADA PRINCIPAL*/
    const int sensor_puerta = 9;
    const int ENTRADA = 8;

 /*CONTROL PUERTA HABITACION*/
    #include <Servo.h>
 
    Servo myservo;  // crea el objeto servo
 
    int pos = 0;    // posicion del servo
    char puerta;

/********FIN DE CONTROLES AUTONOMOS DE LA CASA*************/

void setup(){
    Ethernet.begin(mac, ip);  
    server.begin();           
    Serial.begin(9600);       
    pinMode(2, OUTPUT);    

/*************CONTROLES AUTONOMOS DE LA CASA*********************/
 Serial.begin(9600); // Velocidad de comunicación en baudios.
    pinMode(HABITACION, OUTPUT); // Pin digital del LED como salida.
    pinMode(principal,OUTPUT);  // habitacion principal

    /*HUMEDAD Y TEMPERATURA*/
     dht.begin(); 
    // lcd.begin(16, 2);

    /*CONTROL ENTRADA PRINCIPAL*/
      pinMode(sensor_puerta, INPUT_PULLUP);
      pinMode(ENTRADA, OUTPUT);

    /*CONTROL DE PUERTA HABITACION*/
    myservo.attach(7);  // vincula el servo al pin digital 9

/***************CONTROLES AUTONOMOS DE LA CASA****************************/
}

void loop(){
    EthernetClient client = server.available();  // Comprobamos si hay peticiones

    if (client) {                                // En caso afirmativo
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // Hay algo pendiente de leer
                char c = client.read(); // Leemos los caracteres de uno en uno
                HTTP_req += c;          // Los añadimos al String
                if (c == '\n' && currentLineIsBlank) 
                  {
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    // send web page
                    client.println("<!DOCTYPE html>");
                    client.println("<html>");
                    client.println("<head>");
                    client.println("<title>Control de LEDs en Arduino</title>");
                    client.println("</head>");
                    client.println("<body>");
                    client.println("<h1>LED</h1>");
                    client.println("<p>Haz click para conmutar el LED.</p>");
                    client.println("<form method=\"get\">");
                    ProcessCheckbox(client);
                    client.println("</form>");
                    client.println("</body>");
                    client.println("</html>");
                    Serial.print(HTTP_req);
                    HTTP_req = "";    // Una vez procesador, limpiar el string
                    break;
                  }
                if (c == '\n') 
                    currentLineIsBlank = true;
                else if (c != '\r') 
                    currentLineIsBlank = false;
            } 
        } 
     delay(10);      // dar tiempo
     client.stop(); // Cerra conexion
    }

    Habitacion();
    Principal ();
    Entrada();
    Humedad();
    
     
}

void ProcessCheckbox(EthernetClient cl){
    if (HTTP_req.indexOf("LED2=2") > -1)    // LED2 pinchado?
        LED2_status = !LED2_status ;         // Si pichado invertimos el valor

    digitalWrite(2, LED2_status);
    if (LED2_status) 
          cl.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" \\ onclick=\"submit();\">LED2");
    else 
          cl.println("<input type=\"checkbox\" name=\"LED2\" value=\"2\" \\  onclick=\"submit();\">LED2");
  
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
  if( humedad>=90){
    tone(pinBuzzer, 523, 300);
  }else 
  noTone(pinBuzzer);

}

void Entrada(){
 int value = digitalRead(sensor_puerta);
 
  if (value == LOW) {
    /* si el sensor está activado, quiere decir que la puerta está cerrada,
    por lo tanto el led debe de estar apagado, al abrirse la puera, se enciende el led*/
    digitalWrite(ENTRADA, LOW);   
  } else {
    digitalWrite(ENTRADA, HIGH);
  }
 
  delay(1000);  
}

void Habitacion ()  {
    
    if (Serial.available() > 0) // Si se recibe un carácter...
    {
   if ((estado_habitacion=='C')||(estado_habitacion=='c')) // 
      {      
            //varia la posicion de 0 a 180, con esperas de 15ms
            for (pos = 0; pos <= 80; pos += 1) {
                myservo.write(pos);              
                //delay(15);                       
            }              
        Serial.print("Habitacion abierta");  //estado de la habitacion
        Serial.print("\n");
      }
      else if ((estado_habitacion=='A')||(estado_habitacion=='a')) 
      {
      for (pos = 80; pos >= 0; pos -= 1) {
            myservo.write(pos);              
          //delay(15);                       
        }

        Serial.print("Habitacion cerrada"); //estado de la habitacion
        Serial.print("\n");
      }
    }//fin if serial
  }




