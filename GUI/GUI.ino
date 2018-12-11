#include <VGAXUtils.h>
#include <VGAX.h>

#include <VGAX.h>

VGAX vga;

#define BACKGROUND 01  // set the background color
#define CIRCLE 00      // set the circle color

int x0; // coordenada x0 
int y0; // Coordenada y0
int next_x, next_y;
float R = 3.0; // Radio del circulo
int H, W;

void setup() {
  Serial.begin(9600);
  vga.begin();
  H = VGAX_HEIGHT;
  W = VGAX_WIDTH;
  Serial.println("Dimensions: ");
  Serial.print(W);
  Serial.print(", ");
  Serial.println(H);
  x0 = W / 2;    // estas coordenadas se pueden cambiar
  y0 = H / 2;
  
  vga.clear(BACKGROUND);
  put_circle(x0, y0);

}


void loop() {
  //next_x = x0 - 1 + random(1, 3);
  //next_y = y0 - 1 + random(1, 3);
  next_x = (x0 + 1) % W;
  next_y = (y0 + 1) % H;

  //next_x = min(next_x, (int) (W - R - 1));
  //next_x = max(next_x, (int) R);
  //next_y = min(next_y, (int) (H - R - 1));
  //next_y = min(next_y, (int) R);
  put_new_circle_v2(next_x, next_y, x0, y0);
  x0 = next_x;
  y0 = next_y;
  //delay(300);
  
}

void put_circle_background(int X, int Y){
    for (int y=0; y<H; y++) {
      for (int x=0; x<W; x++) {
        if ( (y - Y) * (y - Y) + (x - X) * (x - X) <= R * R )
          vga.putpixel(x, y, CIRCLE);
        else
          vga.putpixel(x, y, BACKGROUND);
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

void delete_circle(int X, int Y){
    for (int y=Y - R - 1; y<=Y + R + 1; y++) {
      for (int x=X - R - 1; x<=X + R + 1; x++) {
        if ( (y - Y) * (y - Y) + (x - X) * (x - X) <= R * R )
          vga.putpixel(x, y, BACKGROUND);
      }
    } 
}

void put_new_circle(int new_x, int new_y, int old_x, int old_y){
  int x_min = min(new_x, old_x);
  int x_max = max(new_x, old_x);
  int y_min = min(new_y, old_y);
  int y_max = max(new_y, old_y);
  for (int y= y_min - R - 1; y<= y_max + R + 1; y++){
    for (int x = x_min - R - 1; x<= x_max + R + 1; x++){
      if ((y - new_y)*(y - new_y) + (x - new_x)*(x - new_x) <= R * R)
        vga.putpixel(x, y, CIRCLE);
      else
        vga.putpixel(x, y, BACKGROUND);
    } 
  }
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
  
