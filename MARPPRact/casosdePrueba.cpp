#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <map>
#include <cmath>
#include <vector>
#include "monticuloFib.hpp"
using namespace std;

int main(int argc, char* argv[]){
	monticuloFib<int> a;
	a.insertar(2);
	a.insertar(3);
	a.insertar(1);
	a.insertar(4);
	a.insertar(5);
	a.insertar(6);
	a.insertar(0);

	cout << a << endl; //MOSTRAR COMO INSERTA
	
	a.borrarMinimo();
	
	cout << a << endl; //DESPUES DE BORRAR MINIMO

	a.decrecerClave(a.getMinimo()->getHijo()->getHijo(),0);
	
	cout << a << endl; //DESPUES DE DECREMENTAR A 0 EL HIJO DEL HIJO DEL MINIMO
	
	a.decrecerClave(a.getMinimo()->getSig()->getSig()->getHijo(), 0);
	
	cout << a << endl; //DESPUES DE DECREMENTAR A 0 EL HIJO DEL 1

	monticuloFib<int> b;
	b.insertar(2);
	b.insertar(1);
	b.insertar(7);
	b.insertar(12);
	b.insertar(3);

	monticuloFib<int> c;
	c.insertar(21);
	c.insertar(0);
	
	b.unir(c); 
	
	cout << b << endl; //DESPUES DE UNIR C a B

	return 0;
}
