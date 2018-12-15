#include "HX711.h"

// (clk = 6, dout = 5 --> -85
// clk = 2, dout = 3 --> -41

#define DOUT_A  A4
#define CLK_A  A5

#define DOUT_B 13
#define CLK_B 12

#define DOUT_C 11
#define CLK_C 10

#define DOUT_D 5
#define CLK_D 2

HX711 scaleA(DOUT_A, CLK_A);
HX711 scaleC(DOUT_C, CLK_C);

HX711 scaleB(DOUT_B, CLK_B);
HX711 scaleD(DOUT_D, CLK_D);

float A = -160; 
float C = 98;
float B = -74;
float D = -117;

boolean change = false;
float f = 1;

float tareA, tareC, tareB, tareD;
int samples = 1;

boolean show_meds = false;

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");
  scaleA.set_scale();
  scaleC.set_scale();
  scaleB.set_scale();
  scaleD.set_scale();
  
  scaleA.tare(); //Reset the scale to 0
  scaleC.tare();
  scaleB.tare();
  scaleD.tare();
  long zero_factor = scaleA.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.print(zero_factor);
  Serial.print(" -- ");
  Serial.println(scaleB.read_average());
  tareA = scaleA.get_units(samples) * 0;
  tareC = scaleC.get_units(samples) * 0;
  tareB = scaleB.get_units(samples) * 0;
  tareD = scaleD.get_units(samples) * 0;
  Serial.println();
  Serial.println();
  // calibrate();
}
void loop() {
  if (change){
    scaleA.tare(); //Reset the scale to 0
    scaleA.set_scale(A);
    
    scaleC.tare();
    scaleC.set_scale(C);
    
    scaleB.tare();
    scaleB.set_scale(B);
    
    scaleD.tare();
    scaleD.set_scale(D);
        
    //tareA = scaleA.get_units(samples) * 0;
    //tareC = scaleC.get_units(samples) * 0;
    //tareB = scaleB.get_units(samples) * 0;
    //tareD = scaleD.get_units(samples) * 0;
    change = false;
  }
  
  float medA = (scaleA.get_units(samples) - tareA);
  float medC = (scaleC.get_units(samples) - tareC);
  float medB = (scaleB.get_units(samples) - tareB);
  float medD = (scaleD.get_units(samples) - tareD);
  
  if (show_meds){
    Serial.print("Reading I: ");
    Serial.print(medA, 1);
    Serial.print(" || "); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
    Serial.print(medB, 1);
    Serial.print(", sum: ");
    Serial.print(medA + medB, 1);
    Serial.print(", factors: ");
    Serial.print(A);
    Serial.print(" | ");
    Serial.println(B);
    
    Serial.print("Reading II: ");
    Serial.print(medC, 1);
    Serial.print(" || "); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
    Serial.print(medD, 1);
    Serial.print(", sum: ");
    Serial.print(medC + medD, 1);
    Serial.print(", factors: ");
    Serial.print(C);
    Serial.print(" | ");
    Serial.println(D);
    Serial.println();
  }
  else{
    float norm = medA + medB + medC + medD;
    float x = 100 * (medA + medB) / norm;
    
    Serial.println(pos((int) x));
  } 
  
  if(Serial.available())
  {
    char temp = Serial.read();
    
    if(temp == 'a'){
      A += 1*f;
      scaleA.set_scale(A); 
    }
    else if(temp == 's'){
      A -= 1*f;
      scaleA.set_scale(A); 
    }
    if(temp == 'q'){
      C += 1*f;
      scaleC.set_scale(C); 
    }
    else if(temp == 'w'){
      C -= 1*f;
      scaleC.set_scale(C); 
    }
    
    if(temp == 'b'){
      B += 1*f;
      scaleB.set_scale(B); 
    }
    else if(temp == 'n'){
      B -= 1*f;
      scaleB.set_scale(B); 
    }
    if(temp == 'g'){
      D += 1*f;
      scaleD.set_scale(D); 
    }
    else if(temp == 'h'){
      D -= 1*f;
      scaleD.set_scale(D); 
    }
    else if (temp == '+')
      f = f * 10.0;
    else if (temp == '-')
      f = f / 10.0;
    else if (temp == 't')
      change = true;
  }
}

String pos(int k){  
  String a = "A";
  String row = "";
  for (int i=0; i<100; i++){
    if (i==k){
      row += "A";
    }
    else{
      row += "_";
    }
  }
  return row;
}
