#include "HX711.h"

// (clk = 6, dout = 5 --> -85
// clk = 2, dout = 3 --> -41

#define DOUT  5
#define CLK  6
HX711 scale(DOUT, CLK);
HX711 scaleB(DOUT, CLK, 32);
float calibration_factor = -140; //-7050 worked for my 440lb max scale setup
float calibration_factorB = -28;

float tare = 0;
float tareB = 0;;
boolean change = false;
float f = 1;

float distance_between_pivots = 5;
float Afactor = 22400;
float Bfactor = 22400;

int samples = 5;

void calibrate(){	
	scale.set_scale();
	scale.tare();
	scale.set_scale(calibration_factor);
	scaleB.set_scale();
	scaleB.tare();
        scaleB.set_scale(calibration_factorB);
	Serial.println("Put the object");
        delay(10000);
        Serial.println("Continue...");
	// Serial.println("Measure the distance between the object and the right pivot and enter it");
	float x = 2.5;
	//Serial.println(" from the left pivot.");
	//Serial.print("Enter the object's mass: ");
	float mass = 1000;
	float Fa = scale.get_units(samples);
	float Fb = scaleB.get_units(samples);
        Serial.print("Forces A and B: ");
        Serial.print(Fa);
        Serial.print(" | ");
        Serial.println(Fb);
	Afactor = mass * x / (abs(Fa) * distance_between_pivots);
	Bfactor = (mass + Afactor * abs(Fa)) / abs(Fb);
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
  tare = scale.get_units(samples);
  tareB = scaleB.get_units(samples);
  Serial.println();
  Serial.println();
  // calibrate();
}
void loop() {
  if (change){
    //scale.set_scale();
    //scaleB.set_scale();
    //scale.tare();
    //scaleB.tare();
    scale.set_scale(calibration_factor); //Adjust to this calibration factor
    scaleB.set_scale(calibration_factorB);
    tare = scale.get_units(samples);
    tareB = scaleB.get_units(samples);
    Serial.print("Tare setted: ");
    Serial.print(tare);
    Serial.print(" | ");
    Serial.println(tareB);
    change = false;
  }
  
  float med = (scale.get_units(samples) - tare);
  float medB = (scaleB.get_units(samples) - tareB);
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
