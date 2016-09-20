#include <iostream>
#include <vector>
#include <math.h>
#include "FreeImage.h"

using namespace std ;


class POINT{
    public:
        int x;
        int y;
        POINT(int c, int j): x(c), y(j) {}
        void print(){
            cout << x << "X " << y << "Y" << endl;
        }
        int distance(POINT c){
          int a = sqrt( pow(x - c.x,2) + pow(y - c.y, 2) );
          return a;
        }
};

typedef vector<POINT> pasos;

void printColor(RGBQUAD color){
    float r,g,b;
    r = color.rgbRed;
    g = color.rgbGreen;
    b = color.rgbBlue;
    cout << r << "R " << g << "G " << b << "B" << endl;
}

void newCoords(int * y){
    *y = 256 - *y;
}

void drawLine(FIBITMAP * map, POINT a, POINT b, RGBQUAD color){
    color.rgbRed = 255;
    color.rgbGreen = 0;
    color.rgbBlue = 0;
    for(int x = a.x; x < b.x; x++){
            FreeImage_SetPixelColor(map, x, b.y, &color);
    }
}

void printActual(FIBITMAP * map, POINT punto){
    RGBQUAD color;
    color.rgbRed = 255;
    color.rgbGreen = 0;
    color.rgbBlue = 0;
    punto.print();
    if ( !FreeImage_SetPixelColor(map, punto.x, punto.y, &color) ){
      cout <<"Failed printing actual." <<endl;
    }
}

void printPasos(FIBITMAP * map, pasos camino){
    for(int x = 0; x < camino.size(); x++){
      printActual(map, camino[x]);
   }
}

POINT start(195,109);
POINT end(187,126);
RGBQUAD lineColor;

pasos avaiableSteps(FIBITMAP * map, POINT punto){
    pasos camino;
    int auxX[] = {1,-1,0,0,0};
    int auxY[] = {0,0,1,-1,0};
    RGBQUAD color;

    for (int i = 0; i < 6; i++){
        FreeImage_GetPixelColor(map, punto.x+auxX[i], punto.y+auxY[i], &color);
        if (color.rgbRed == color.rgbBlue && color.rgbGreen == color.rgbRed){
            POINT r = POINT(punto.x+auxX[i], punto.y+auxY[i]);
            camino.push_back(r);
        }
    }

    return camino;
}


int main(int argc, char **argv) {

      FreeImage_Initialise();
      FIBITMAP * bitmap = FreeImage_Allocate(256, 256, 24);
      FIBITMAP * mapa = FreeImage_Load(FIF_PNG, "minimap.png", PNG_DEFAULT);
      mapa = FreeImage_ConvertTo24Bits(mapa);
      
      // Copying map.
      RGBQUAD color; 
      for(int x = 0; x < 256; x++){
        for(int y = 0; y < 256; y++){
            FreeImage_GetPixelColor(mapa, x, y, &color);
            FreeImage_SetPixelColor(bitmap, x, y, &color);
        }
      }

      newCoords(&start.y);

      pasos p = avaiableSteps(mapa, start);
      printPasos(bitmap, p);
     
      FreeImage_Save(FIF_PNG, bitmap, "Final.png", 0);
      FreeImage_DeInitialise();
      return 0;

}

