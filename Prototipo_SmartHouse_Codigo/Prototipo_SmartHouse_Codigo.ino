
  /*HABITACION*/
  int HABITACION = 13; // Pin digital para el LED  
  char encendido_habitacion; // Variable para guardar el valor del carácter enviado

  /*LUZ PRINCIPAL CONTROLADA POR LDR*/
   int principal = 12;
   int sensorReading;//Pin análogo en espera

  /*SENSOR TEMPERATURA Y HUMEDAD*/
  // Libreria para Sensor DHT
    #include <DHT.h>

  // Libreria para Display LCD
    #include  <LiquidCrystal.h>

  // Teperatura y humedad
    int SENSOR = 11;
    int temp, humedad;
    
    DHT dht (SENSOR, DHT11); //id del sensor. Puede ser DHT11 o DHT22

  //  LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // pantalla
  
   
  void setup()  {
    
    Serial.begin(9600); // Velocidad de comunicación en baudios.
    pinMode(HABITACION, OUTPUT); // Pin digital del LED como salida.
    pinMode(principal,OUTPUT);  // habitacion principal

    /*HUMEDAD Y TEMPERATURA*/
     dht.begin(); 
    // lcd.begin(16, 2);

  }

  void loop()  {
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
      if (sensorReading<200) {
        digitalWrite(principal,HIGH);
    } else 
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
}

