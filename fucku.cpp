  // Puede morir, no lo necesito.
  Nodo inicio(&start, 0);
  Nodo finals(&final, 0);
  abiertos.push_back(inicio);
  int i = 0;
  while(!abiertos.empty()){
    // Busco el menor F
    // vector<Nodo>::iterator minimo = min_element(abiertos.begin(), abiertos.end(), cmpNodo);
     
    Nodo * actual = minimoAbiertos(abiertos);
    cout << actual->f << endl ;
    
    actual->colorear(nuevo,0);
    //cout << "Actual: " << &(*minimo) <<endl;

    if (*(actual->coordenadas) == final){
      cout << "LLegamos al destino" << endl;
      actual->imprimirCamino(nuevo,1);
      //actual->colorear(bitmap,0);
      inicio.colorear(nuevo,1);
      finals.colorear(nuevo,1);
      return;
    }
    cout << actual->f << endl ;
    
    actual->print();
    // Busco vecinos. Quiero una lista de Nodo.
    cerrados.push_back(*actual);
    actual = &cerrados.back();
    vecinos = avaiableSteps(bitmap, *actual, final);
    for (vector<Nodo>::iterator it = vecinos.begin(); it != vecinos.end(); ++it) {
      vector<Nodo>::iterator resultado;
     // cout << "Hijo:" << endl;
      //(*it).print() ;
      if ( (find(cerrados.begin(), cerrados.end(), *it ) != cerrados.end()) ){
        continue;
      }
      resultado = find(abiertos.begin(), abiertos.end(), *it );
      if (resultado == abiertos.end()){

        //cout << "Abriendo camino" << endl;
        abiertos.push_back(*it);
        if ((*it).coordenadas->x == 194 && (*it).coordenadas->y == 142){
          // cout << " METIOENDOLO" << endl;
          // for (vector<Nodo>::iterator ut = abiertos.begin(); ut != abiertos.end(); ++ut){
          //   (*ut).print();
          // }
          // cout << "EMPEZANDO LISTA" << endl;
        }
      } else if ((*resultado).f > (*it).f) {
        (*resultado).g = (*it).g;
        (*resultado).h = (*it).h;
        (*resultado).f = (*it).f;
        cout << " THIS IS HAPPENING " << endl << endl << endl;
        (*resultado).padre = (*it).padre;
      }

    }
    abiertos.erase(find(abiertos.begin(), abiertos.end(), *actual));
    i++;
    if (i == 5){
      return;
    }

  }
  // No hay camino