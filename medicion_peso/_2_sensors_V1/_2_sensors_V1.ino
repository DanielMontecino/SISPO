#include "HX711.h"
#define DOUT  3
#define CLK  2
HX711 scale(DOUT, CLK);
HX711 scaleB(DOUT, CLK, 32);
float calibration_factor = -80; //-7050 worked for my 440lb max scale setup
float calibration_factorB = -80;

float tare, tareB;
boolean change = false;
float f = 1;

float distance_between_pivots;
float Afactor = 22400;
float Bfactor = 22400;

void calibrate(){	
	scaleA.set_scale();
	scaleA.tare();
	
	scaleB.set_scale();
	scaleB.tare();
	
	Serial.println("Measure the distance between the pivots and enter it");
	distance_between_pivots = Serial.read();
	Serial.println("Measure the distance between the object and the right pivot and enter it");
	float x = Serial.read();
	Serial.println(" from the left pivot.");
	Serial.print("Enter the object's mass: ");
	float mass = Serial.read();
	float Fa = scaleA.get_units(5);
	float Fb = scaleB.get_units(5);
	Afactor = mass * x / (Fa * distance_between_pivots);
	Bfactor = (mass + Afactor * Fa) / Fb;
	Serial.print("A calibration's factor: ");
	Serial.println(Afactor);
	Serial.print("B calibrations' factor: ");
	Serial.println(Bfactor);	
}

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");
  scale.set_scale();
  scaleB.set_scale();
  scale.tare(); //Reset the scale to 0
  scaleB.tare();
  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.print(zero_factor);
  Serial.print(" -- ");
  Serial.println(scaleB.read_average());
  tare = scale.get_units(5);
  tareB = scaleB.get_units(5);
  Serial.println();
  Serial.println();
  calibration()
}
void loop() {
  if (change){
    scale.tare();
    scaleB.tare();
    scale.set_scale(calibration_factor); //Adjust to this calibration factor
    scaleB.set_scale(calibration_factorB);
    tare = scale.get_units(5) * 0;
    tareB = scaleB.get_units(5) * 0;
    change = false;
  }
  
  float med = scale.get_units(5) - tare;
  float medB = scaleB.get_units(5) - tareB;
  Serial.print("Reading: ");
  Serial.print(med, 1);
  Serial.print(" || "); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(medB, 1);
  Serial.print(". Sum: ");
  Serial.print(med + medB, 1);
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.print(" || ");
  Serial.print(calibration_factorB);
  Serial.println();
  
  
  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == 'a'){
      calibration_factor += 1*f;
      scale.set_scale(calibration_factor); //Adjust to this calibration factor
    }
    else if(temp == 's'){
      calibration_factor -= 1*f;
      scale.set_scale(calibration_factor); //Adjust to this calibration factor
    }
    else if (temp == 'b'){
      calibration_factorB += 1*f;
      scaleB.set_scale(calibration_factorB);
    }
    else if (temp == 'n'){
      calibration_factorB -= 1*f;
      scaleB.set_scale(calibration_factorB);
    }
    else if (temp == '+')
      f = f * 10.0;
    else if (temp == '-')
      f = f / 10.0;
    else if (temp == 't')
      change = true;
  }
}
