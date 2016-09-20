#include <iostream>
#include <vector>
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
};

typedef vector<POINT> pasos;

void printColor(RGBQUAD color){
    float r,g,b;
    r = color.rgbRed;
    g = color.rgbGreen;
    b = color.rgbBlue;
    cout << r << "R " << g << "G " << b << "B" << endl;
}

void newCoords(int * x , int * y){
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

void printPasos(FIBITMAP * map, pasos camino){
    RGBQUAD color;
    color.rgbRed = 255;
    color.rgbGreen = 0;
    color.rgbBlue = 0;
    for(int x = 0; x < camino.size(); x++){
      FreeImage_SetPixelColor(map, camino[x].x, camino[x].y, &color);
   }
}

void printActual(FIBITMAP * map, POINT punto){
    RGBQUAD color;
    color.rgbRed = 0;
    color.rgbGreen = 55;
    color.rgbBlue = 255;
    punto.print();
    FreeImage_SetPixelColor(map, punto.x, punto.y, &color);
}

POINT start(100,126);
POINT end(187,126);
RGBQUAD lineColor;

pasos avaiableSteps(FIBITMAP * map, POINT punto){
    pasos camino;
    int auxX[] = {1,-1,0,0};
    int auxY[] = {0,0,1,-1};
    RGBQUAD color;

    printActual(map, punto);
    cout << "Trying";

    for (int i = 0; i < 4; i++){
        FreeImage_GetPixelColor(map, punto.x+auxX[i], punto.y+auxY[i], &color);
        if (color.rgbRed == color.rgbBlue && color.rgbGreen == color.rgbRed){
            cout << punto.x << "punto.x" << endl;
            POINT r = POINT(punto.x+auxX[i], punto.y+auxY[i]);
            r.print();
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
     // cout << FreeImage_GetWidth(mapa) << " Width " << endl << FreeImage_GetHeight(mapa) << " Height " << FreeImage_GetBPP(mapa) << " BPP" << endl ;
      RGBQUAD color; 
      for(int x = 0; x < 256; x++){
        for(int y = 0; y < 256; y++){
            FreeImage_GetPixelColor(mapa, x, y, &color);
            FreeImage_SetPixelColor(bitmap, x, y, &color);
        }
      }
      start.y = 256-start.y;
      pasos p = avaiableSteps(mapa, start);
      printPasos(bitmap, p);
      //drawLine(bitmap, start, end, lineColor);
      FreeImage_Save(FIF_PNG, bitmap, "Final.png", 0);
      FreeImage_DeInitialise();
      return 0;

}

