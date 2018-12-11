#include "HX711.h"

// (clk = 6, dout = 5 --> -85
// clk = 2, dout = 3 --> -41

#define DOUT_A  9
#define CLK_A  8

#define DOUT_B 5
#define CLK_B 6

#define DOUT_C A2 
#define CLK_C A3

#define DOUT_D A0 
#define CLK_D A1

#define DOUT_Ap 
#define CLK_Ap

#define DOUT_Bp
#define CLK_Bp 

#define DOUT_Cp  
#define CLK_Cp 

#define DOUT_Dp  
#define CLK_Dp 

HX711 scaleA(DOUT_A, CLK_A);
HX711 scaleC(DOUT_C, CLK_C);

HX711 scaleB(DOUT_B, CLK_B);
HX711 scaleD(DOUT_D, CLK_D);

HX711 scaleAp(DOUT_Ap, CLK_Ap);
HX711 scaleCp(DOUT_Cp, CLK_Cp);

HX711 scaleBp(DOUT_Bp, CLK_Bp);
HX711 scaleDp(DOUT_Dp, CLK_Dp);



float A = -160; 
float C = 98;
float B = -74;
float D = -117;
float Ap = -125;
float Bp = 115;
float Cp = -78;
float Dp = -122;

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
  scaleAp.set_scale();
  scaleCp.set_scale();
  scaleBp.set_scale();
  scaleDp.set_scale();
  
  scaleA.tare(); //Reset the scale to 0
  scaleC.tare();
  scaleB.tare();
  scaleD.tare();
  scaleAp.tare(); 
  scaleCp.tare();
  scaleBp.tare();
  scaleDp.tare();
  
  long zero_factor = scaleA.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.print(zero_factor);
  Serial.print(" -- ");
  Serial.println(scaleB.read_average());
  tareA = scaleA.get_units(samples) * 0;
  tareC = scaleC.get_units(samples) * 0;
  tareB = scaleB.get_units(samples) * 0;
  tareD = scaleD.get_units(samples) * 0;
  tareAp = scaleAp.get_units(samples) * 0;
  tareCp = scaleCp.get_units(samples) * 0;
  tareBp = scaleBp.get_units(samples) * 0;
  tareDp = scaleDp.get_units(samples) * 0;
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
    
    scaleAp.tare(); //Reset the scale to 0
    scaleAp.set_scale(Ap);
    
    scaleCp.tare();
    scaleCp.set_scale(Cp);
    
    scaleBp.tare();
    scaleBp.set_scale(Bp);
    
    scaleDp.tare();
    scaleDp.set_scale(Dp);
        
    //tareA = scaleA.get_units(samples) * 0;
    //tareC = scaleC.get_units(samples) * 0;
    //tareB = scaleB.get_units(samples) * 0;
    //tareD = scaleD.get_units(samples) * 0;
    //tareAp = scaleAp.get_units(samples) * 0;
    //tareCp = scaleCp.get_units(samples) * 0;
    //tareBp = scaleBp.get_units(samples) * 0;
    //tareDp = scaleDp.get_units(samples) * 0;
    change = false;
  }
  
  float medA = (scaleA.get_units(samples) - tareA);
  float medC = (scaleC.get_units(samples) - tareC);
  float medB = (scaleB.get_units(samples) - tareB);
  float medD = (scaleD.get_units(samples) - tareD);
  float medAp = (scaleAp.get_units(samples) - tareAp);
  float medCp = (scaleCp.get_units(samples) - tareCp);
  float medBp = (scaleBp.get_units(samples) - tareBp);
  float medDp = (scaleDp.get_units(samples) - tareDp);
  
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
    
    Serial.print("Reading III: ");
    Serial.print(medAp, 1);
    Serial.print(" || "); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
    Serial.print(medBp, 1);
    Serial.print(", sum: ");
    Serial.print(medAp + medBp, 1);
    Serial.print(", factors: ");
    Serial.print(Ap);
    Serial.print(" | ");
    Serial.println(Bp);
    
    Serial.print("Reading IV: ");
    Serial.print(medCp, 1);
    Serial.print(" || "); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
    Serial.print(medDp, 1);
    Serial.print(", sum: ");
    Serial.print(medCp + medDp, 1);
    Serial.print(", factors: ");
    Serial.print(Cp);
    Serial.print(" | ");
    Serial.println(Dp);
    
    Serial.println();
  }
  else{
    float medI = medA + medB;
    float medII = medC + medD;
    float medIII = medAp + medBp;
    float medIV = medCp + medDp;
    
    float norm = medI + medII + medIII + medIV;
    ;
    float x = 100 * (medI + medIV) / norm;
    float y = 100 * (medIII + medIV) / norm;
    
    Serial.print(pos((int) x));
    Serial.print(" | ");
    Serial.println(pos((int) y));
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
