#include <iostream>
#include <vector>
#include <math.h>     // pow , sqrt
#include <algorithm>  // std::min_element
#include <iterator>  // std::begin, std::end
#include <set>
#include "FreeImage.h"
//#include <windows.h>

using namespace std ;

#define DEBUG 0

class POINT{
    public:
        int x;
        int y;
        POINT(int c, int j): x(c), y(j) {}
        void print(){
            cout << x << "X " <<  255-y << "Y" << endl;
        }
        bool operator==(const POINT &other) const {
          return (x == other.x && y == other.y);
        }

        float distance(POINT c){
          float a = sqrt( pow(x - c.x,2) + pow(y - c.y, 2) );
          return a;
        }
};


    void printActual(FIBITMAP * map, POINT punto, bool p){
        RGBQUAD color;
        color.rgbRed = 255;
        color.rgbGreen = 0;
        color.rgbBlue = 0;
        if (p){
          color.rgbRed = 0;
          color.rgbGreen = 255;
          color.rgbBlue = 0;
        }
        
        //punto.print();
        if ( !FreeImage_SetPixelColor(map, punto.x, punto.y, &color) ){
          cout <<"Failed printing actual." <<endl;
        } 
    }

class Nodo{
  public:
    Nodo * padre;
    POINT * coordenadas;
    int g;
    float h,f;

    Nodo(POINT * p, int d): coordenadas(p), g(d), padre(NULL), f(0) ,h(0){}

    void print(){
      //cout << coordenadas << endl;
      coordenadas->print();
      cout << "YO " << this <<endl <<"Padre: " << padre << endl << g << "G " << f << "F " << h << "H"<< endl<< endl; 
    }

    bool operator==(const Nodo &other) const {
          return (*coordenadas) == *(other.coordenadas);
    }

    void colorear(FIBITMAP * map, bool p){
        //cout << " COLOREANDO " << endl;
        printActual(map, *coordenadas, p);
    }

    void imprimirCamino(FIBITMAP * map, bool p){
      //this->print();
      this->colorear(map,1);
      if (padre != NULL){
        if (this == this->padre){
          return;
        }
        padre->imprimirCamino(map,1);
      }
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
    *y = 255 - *y;
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
    for(int x = 0; x < camino.size(); x++){
      printActual(map, camino[x], 0);
   }
}



vector<Nodo *> avaiableSteps(FIBITMAP * map, Nodo & start, POINT final){
    vector<Nodo *> camino;
    POINT punto = *start.coordenadas;
    int auxX[] = {1,-1,0,0};
    int auxY[] = {0,0,1,-1};
    RGBQUAD color;

    // if (punto.y == 159 && punto.x == 217){
    //   start.print();
    // }
    for (int i = 0; i < 4; i++){
        FreeImage_GetPixelColor(map, punto.x+auxX[i], punto.y+auxY[i], &color);
        if (color.rgbRed == color.rgbBlue && color.rgbGreen == color.rgbRed){
            POINT * r = new POINT(punto.x+auxX[i], punto.y+auxY[i]);
            Nodo * temp = new Nodo(r, start.g+1);
            temp->h = r->distance(final);
            temp->f = temp->g + temp->h;
            temp->padre = &start;
            camino.push_back(temp);
            
        }
    }

    return camino;
}


void imprimirFs(vector<Nodo> minimo){
  for (vector<Nodo>::iterator it = minimo.begin(); it != minimo.end(); ++it){
    cout << (*it).f << " ";
  } 
  cout << endl << endl;
}

Nodo * minimoAbiertos(vector<Nodo *>& abiertos){
  if (DEBUG) cout << "Buscando minimo" << endl ;
  
  Nodo * p = abiertos.front();
  for (vector<Nodo *>::iterator it = abiertos.begin(); it != abiertos.end(); ++it){
   // cout << (*it)->f << " ";
    if(p->f > (*it)->f){
      p = (*it);
    }
  } 
  if (DEBUG) cout << "Resultado: " << p->f << endl << endl ; 
  return p;
}

//POINT start(187,126);
POINT start(115,81);
//POINT end(129,126);
POINT end(155,41);
RGBQUAD lineColor;

bool estaEn(vector <Nodo *> vec, Nodo * obj){
  for(vector<Nodo *>::iterator it = vec.begin(); it != vec.end(); ++it){
    if( *(*it) == (*obj) ){
      return true;
    }
  }
  return false;
};

void pathFinder(FIBITMAP * bitmap, FIBITMAP * nuevo, POINT start, POINT final){
  vector<Nodo *> cerrados;
  vector<Nodo *> abiertos;
  vector<Nodo *> vecinos;

  Nodo inicio(&start, 0);
  Nodo finals(&final, 0);

  abiertos.push_back(&inicio);

int i = 0;
  while(!abiertos.empty()){
    // Minimo F
    Nodo * actual = minimoAbiertos(abiertos);
    //cout << "Actual:" << endl;
    actual->colorear(nuevo,0);
    //actual->print();
    // Se encontro un camino
    if (*actual == finals){
      cout << "Se ha encontrado un camino." << endl;
      inicio.colorear(nuevo, 1);
      finals.colorear(nuevo, 1);
      actual->imprimirCamino(nuevo, 1);
      break;
    }

    // Cierro nodo
    cerrados.push_back(actual);
    // Busco vecinos
    vecinos = avaiableSteps(bitmap, *actual, final);

    // Chequear cada vecino.
    for (vector<Nodo * >::iterator it = vecinos.begin(); it != vecinos.end(); ++it) {
      if ( estaEn(cerrados, *it) ) {
        continue;
      }

      if ( ! estaEn(abiertos, *it) ) {
        abiertos.push_back((*it));
      }
    }

    abiertos.erase(find(abiertos.begin(), abiertos.end(), actual));
    // i++;
    // if (i == 100){
    //   break;
    // }
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

      // Setup

      newCoords(&start.y);
      newCoords(&end.y);
      pathFinder(mapa, bitmap, start, end);

      // Save
     
      FreeImage_Save(FIF_PNG, bitmap, "Final.png", 0);
      FreeImage_DeInitialise();
      return 0;

}

