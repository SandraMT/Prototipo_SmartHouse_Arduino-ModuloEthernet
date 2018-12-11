
  /*HABITACION*/
  int HABITACION = 13; // Pin digital para el LED  
  char encendido_habitacion; // Variable para guardar el valor del carácter enviado

  /*LUZ PRINCIPAL CONTROLADA POR LDR*/
   int principal = 12;
   int sensorReading;//Pin análogo en espera
  
   
  void setup()  {
    
    Serial.begin(9600); // Velocidad de comunicación en baudios.
    pinMode(HABITACION, OUTPUT); // Pin digital del LED como salida.
    pinMode(principal,OUTPUT);  // habitacion principal

  }

  void loop()  {
    Habitacion();
    Principal ();
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
    sensorReading=analogRead(0); //Instrucción para obtener dato analógico
      if (sensorReading<200) {
        digitalWrite(principal,HIGH);
    } else 
        digitalWrite(principal,LOW);
        Serial.println(sensorReading); 
  }

