
// Definimos el pin que utilizará nuestro LED, en este caso 13.
const int LED_PIN = 13; 

void setup() {
  // Definimos LED_PIN como un pin de salida
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Prendemos el led!
  digitalWrite(LED_PIN, HIGH);

  // Detenemos la ejecución durante 500ms, durante los cuales
  // el led permanecerá encendido.
  delay(500);

  // Apagamos el led
  digitalWrite(LED_PIN, LOW);

  // Detenemos la ejecución durante 500ms, durante los cuales
  // el led permanecerá apagado.
  delay(500);
}
