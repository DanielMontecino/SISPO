#include "HX711.h"

// (clk = 6, dout = 5 --> -85
// clk = 2, dout = 3 --> -41

#define DOUT_A  3
#define CLK_A  2
#define DOUT_B 5
#define CLK_B 6

HX711 scaleA1(DOUT_A, CLK_A);
HX711 scaleA2(DOUT_A, CLK_A, 32);

HX711 scaleB1(DOUT_B, CLK_B);
HX711 scaleB2(DOUT_B, CLK_B, 32);

float A1 = -41; 
float A2 = -85;
float B1 = -41;
float B2 = -85;

boolean change = false;
float f = 1;

float tareA1, tareA2, tareB1, tareB2;
int samples = 1;

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");
  scaleA1.set_scale();
  scaleA2.set_scale();
  scaleB1.set_scale();
  scaleB2.set_scale();
  
  scaleA1.tare(); //Reset the scale to 0
  scaleA2.tare();
  scaleB1.tare();
  scaleB2.tare();
  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.print(zero_factor);
  Serial.print(" -- ");
  Serial.println(scaleB.read_average());
  tareA1 = scaleA1.get_units(samples);
  tareA2 = scaleA2.get_units(samples);
  tareB1 = scaleB1.get_units(samples);
  tareB2 = scaleB2.set_units(samples);
  Serial.println();
  Serial.println();
  // calibrate();
}
void loop() {
  if (change){
    scaleA1.tare(); //Reset the scale to 0
    scaleA2.tare();
    scaleB1.tare();
    scaleB2.tare();
    
    scaleA1.set_scale(A1);
    scaleA2.set_scale(A2);
    scaleB1.set.scale(B1);
    scaleB2.set_scale(B2);
    
    tareA1 = scaleA1.get_units(samples) * 0;
    tareA2 = scaleA2.get_units(samples) * 0;
    tareB1 = scaleB1.get_units(samples) * 0;
    tareB2 = scaleB2.set_units(samples) * 0;
    change = false;
  }
  
  float medA1 = (scaleA1.get_units(3) - tareA1);
  float medA2 = (scaleA2.get_units(3) - tareA2);
  float medB1 = (scaleB1.get_units(3) - tareB1);
  float medB2 = (scaleB2.get_units(3) - tareB2);
  
  Serial.print("Reading A: ");
  Serial.print(medA1, 1);
  Serial.print(" || "); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(medA2, 1);
  Serial.print(", sum: ");
  Serial.print(medA1 + medA2, 1);
  Serial.print(", factors: ");
  Serial.print(A1);
  Serial.print(" | ");
  Serial.println(A2);
  
  Serial.print("Reading B: ");
  Serial.print(medB1, 1);
  Serial.print(" || "); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  Serial.print(medB2, 1);
  Serial.print(", sum: ");
  Serial.print(medB1 + medB2, 1);
  Serial.print(", factors: ");
  Serial.print(B1);
  Serial.print(" | ");
  Serial.println(B2);
  
   
  
  if(Serial.available())
  {
    char temp = Serial.read();
    
    if(temp == 'a'){
      A1 += 1*f;
      scaleA1.set_scale(A1); 
    }
    else if(temp == 's'){
      A1 -= 1*f;
      scaleA1.set_scale(A1); 
    }
    if(temp == 'q'){
      A2 += 1*f;
      scaleA2.set_scale(A2); 
    }
    else if(temp == 'w'){
      A2 -= 1*f;
      scaleA2.set_scale(A2); 
    }
    
    if(temp == 'b'){
      B1 += 1*f;
      scaleB1.set_scale(B1); 
    }
    else if(temp == 'n'){
      B1 -= 1*f;
      scaleB1.set_scale(B1); 
    }
    if(temp == 'g'){
      B2 += 1*f;
      scaleB2.set_scale(B2); 
    }
    else if(temp == 'h'){
      B2 -= 1*f;
      scaleB2.set_scale(B2); 
    }
    else if (temp == '+')
      f = f * 10.0;
    else if (temp == '-')
      f = f / 10.0;
    else if (temp == 't')
      change = true;
  }
}
