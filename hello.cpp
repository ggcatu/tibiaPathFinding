#include <iostream>
#include <vector>
#include <math.h>     // pow , sqrt
#include <algorithm>  // std::min_element
#include <iterator>  // std::begin, std::end
#include <set>
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
        bool operator==(const POINT &other) const {
          return (x == other.x && y == other.y);
        }

        int distance(POINT c){
          int a = sqrt( pow(x - c.x,2) + pow(y - c.y, 2) );
          return a;
        }
};

class Nodo{
  public:
    Nodo * padre;
    POINT * coordenadas;
    int g, f, h;
    Nodo(POINT * p, int d): coordenadas(p), g(d) {}

    void print(){
      //cout << coordenadas << endl;
      coordenadas->print();
      cout << g << "G " << f << "F " << h << "H"<< endl<< endl; 
    }
};

bool cmpNodo(Nodo a, Nodo b){
  return a.f < b.f;
}

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

vector<Nodo> avaiableSteps(FIBITMAP * map, Nodo start, POINT final){
    vector<Nodo> camino;
    POINT punto = *start.coordenadas;
    int auxX[] = {1,-1,0,0};
    int auxY[] = {0,0,1,-1};
    RGBQUAD color;

    for (int i = 0; i < 4; i++){
        FreeImage_GetPixelColor(map, punto.x+auxX[i], punto.y+auxY[i], &color);
        if (color.rgbRed == color.rgbBlue && color.rgbGreen == color.rgbRed){
            POINT * r = new POINT(punto.x+auxX[i], punto.y+auxY[i]);
            Nodo temp(r, start.g+1);
            temp.h = r->distance(final);
            temp.f = temp.g + temp.h;
            camino.push_back(temp);
        }
    }

    return camino;
}

void pathFinder(FIBITMAP * bitmap, POINT start, POINT final){
  vector<Nodo> cerrados;
  vector<Nodo> abiertos;
  vector<Nodo> vecinos;
  Nodo inicio(&start, 0);
  abiertos.push_back(inicio);
  while(!abiertos.empty()){
    // Busco el menor F
    vector<Nodo>::iterator minimo = min_element(abiertos.begin(), abiertos.end(), cmpNodo);
    Nodo actual = *minimo;
    if (*actual.coordenadas == final){
      cout << "LLegamos al destino" << endl;
      return;
    }
    abiertos.erase(minimo);
    cerrados.push_back(actual);
    // Busco vecinos. Quiero una lista de Nodo.
    vecinos = avaiableSteps(bitmap, actual, final);
    for (vector<Nodo>::iterator it = vecinos.begin(); it != vecinos.end(); ++it) {
          it->print();
    }
    // cout <<"Camino preferido:" <<endl;
    // vector<Nodo>::iterator minips = min_element(vecinos.begin(), vecinos.end(), cmpNodo);
    // (*minips).print();


  }
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
      vector<Nodo> abiertos;
      newCoords(&start.y);
      newCoords(&end.y);
      Nodo a(&start, 3);
      vector<Nodo> prueba = avaiableSteps(mapa, a, end);

      // for (vector<Nodo>::iterator it = prueba.begin(); it != prueba.end(); ++it) {
      //     it->print();
      // }


      Nodo b(&end, 5);
      abiertos.push_back(a);
      abiertos.push_back(b);
      pathFinder(mapa, start, end);
      //cout << "WENO: " <<  min.distancia << endl;
      //cout <<"Camino preferido:" <<endl;
      vector<Nodo>::iterator minips = min_element(prueba.begin(), prueba.end(), cmpNodo);
      (*minips).print();
      prueba.erase(minips);
      //Nodo min = * min_element(abiertos.begin(), abiertos.end(), cmpNodo);
      //cout << "WENO: " <<  (*minips).distancia << endl;

      newCoords(&start.y);

      // pasos p = avaiableSteps(mapa, start);
      // printPasos(bitmap, p);
     
      FreeImage_Save(FIF_PNG, bitmap, "Final.png", 0);
      FreeImage_DeInitialise();
      return 0;

}

