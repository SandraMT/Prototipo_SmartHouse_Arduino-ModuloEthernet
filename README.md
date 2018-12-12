# Prototipo_SmartHouse_Arduino-ModuloEthernet

###### Desarrollado por: 
###### * Angel Christian Alvarez Trujillo
###### * Jesica Irais Nicasio Hernandez
###### * Sandra Guadalupe Mireles Tovar

<center><b>
Tecnológico Nacional de México <br>
Instituto Tecnológico de León <br>
Ingeniería en Sistemas Computacionales <br>
Sistemas programables <br>
</b></center>

### Material utilizado
* Sensor DHT11
* Foto resistencia
* Arduino UNO
* Ethernet Shield
* 2 LEDs
* 1 Resistencia 220
* 1 Servomotor
* 1 Cable ethernet
* Jumpers Hembra-Macho
* Jumpers Macho-Macho
* ReadSwitch
* Cable usb a arduino
* Imán

### Sofware utilizado
* Arduino IDE
* Navegador Google Chrome
* ElementaryOS
* Kubuntu 18.04
* Git
* Github

### Librerias utilizadas
* Ethernet.h
* DHT.h
* Servo.h
* SPI.h

### Contenido del respositorio
* Diagramas de conexión de circuito
* Código compilado de Arduino
* Imágenes de muestra del circuito

### Funcion
El funcionamiento de la casa inteligente programada funciona de la siguiente manera:
* El usuario puede acceder desde un navegador a la placa para poder activar o desactivar la luz de la habitación.
* La habitación principal estará encendida cuando no haya iluminación en el exterior de la casa.
* Cuando se supera la temperatura encima del 90% el buzzer comienza a sonar, alertano a los habitantes.
* Cuando se abre la puerta, la luz exterior se enciende por un instante.
* La temperatura y la humedad se muestra en la interfaz serial de arduino.

###### NOTA: Por falta de tiempo se omitió la implementacion del servomotor, el cual su manera de operar seria por medio de la web en conjunto con la luz de la habitación y donde este haría que se abriera o cerrara la puerta de la habitacion.