#include "HX711.h"
#include <VGAXUtils.h>
#include <VGAX.h>

#include <VGAX.h>

VGAX vga;

#define BACKGROUND 01  // set the background color
#define CIRCLE 00      // set the circle color

#define DOUT_A  6
#define CLK_A  8

#define DOUT_B 4
#define CLK_B 7

#define DOUT_C A2 
#define CLK_C A3

#define DOUT_D A0 
#define CLK_D A1

#define DOUT_Ap A4
#define CLK_Ap A5

#define DOUT_Bp 13
#define CLK_Bp 12

#define DOUT_Cp 11
#define CLK_Cp 10

#define DOUT_Dp 5
#define CLK_Dp 2

HX711 scaleA(DOUT_A, CLK_A);
HX711 scaleC(DOUT_C, CLK_C);

HX711 scaleB(DOUT_B, CLK_B);
HX711 scaleD(DOUT_D, CLK_D);

HX711 scaleAp(DOUT_Ap, CLK_Ap);
HX711 scaleCp(DOUT_Cp, CLK_Cp);

HX711 scaleBp(DOUT_Bp, CLK_Bp);
HX711 scaleDp(DOUT_Dp, CLK_Dp);



float A = -125; 
float C = -78;
float B = 115;
float D = -122;
float Ap = -160;
float Bp = -74;
float Cp = 98;
float Dp = -117;

boolean change = true;
float f = 1;

int samples = 1;

boolean show_meds = false;

int x0; // coordenada x0 
int y0; // Coordenada y0
float R = 3.0; // Radio del circulo
int H, W;

void setup() {

  vga.begin();
  H = VGAX_HEIGHT;
  W = VGAX_WIDTH;

  x0 = W / 2;    // estas coordenadas se pueden cambiar
  y0 = H / 2;
  
  vga.clear(BACKGROUND);
  
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
        
    change = false;
    put_circle(x0, y0);
  }
  
  float medA = (scaleA.get_units(samples)) ;
  float medC = (scaleC.get_units(samples) ) ;
  float medB = (scaleB.get_units(samples)) ;
  float medD = (scaleD.get_units(samples) ) ;
  float medAp = (scaleAp.get_units(samples) ) ;
  float medCp = (scaleCp.get_units(samples) ) ;
  float medBp = (scaleBp.get_units(samples) ) ;
  float medDp = (scaleDp.get_units(samples) ) ;
  

  
  //float medI = 15000;
  float medI = (medA + medB);
  float medII = (medC + medD);
  float medIII = (medAp + medBp);
  float medIV = (medCp + medDp);
  
  float norm = medI + medII + medIII + medIV + 0.00001;
  float x = W * (medI + medIV) / norm;
  float y = H * (medIII + medIV) / norm;
   
  put_new_circle_v2(x, y, x0, y0);
  x0 = x;
  y0 = y;

}

void put_new_circle_v2(int new_x, int new_y, int old_x, int old_y){
     for (int y=old_y - R ; y<=old_y + R ; y++) {
      for (int x=old_x - R ; x<=old_x + R ; x++) {
        if ( (y - new_y) * (y - new_y) + (x - new_x) * (x - new_x) <= R * R )
          continue;
        else
          vga.putpixel(x, y, BACKGROUND);
      }
    } 
    for (int y=new_y - R ; y<=new_y + R ; y++) {
      for (int x=new_x - R ; x<=new_x + R ; x++) {
        if ( (y - new_y) * (y - new_y) + (x - new_x) * (x - new_x) <= R * R )
          vga.putpixel(x, y, CIRCLE);
      }
    } 
}

void put_circle(int X, int Y){
    for (int y=Y - R ; y<=Y + R ; y++) {
      for (int x=X - R ; x<=X + R ; x++) {
        if ( (y - Y) * (y - Y) + (x - X) * (x - X) <= R * R )
          vga.putpixel(x, y, CIRCLE);
      } 
    }
}

