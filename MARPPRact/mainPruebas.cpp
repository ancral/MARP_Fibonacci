#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <map>
#include <cmath>
#include <vector>
#include "gnuplot-iostream/gnuplot-iostream.h"
#include "monticuloFib.hpp"
using namespace std;
using namespace std::chrono;

// Compilar con:
//   g++ -o mainGrafica mainPruebas.cpp -lboost_iostreams -lboost_system -lboost_filesystem


int main(int argc, char* argv[]){
	//Leemos el fichero 'tam.txt', pero se puede poner cualquier otro
	ifstream file("tam.txt"); 
	string linebuffer;
	int tam;

	//Vectores formados por el tamaño de la estructura y por el tiempo que tarda en hacer la operacion Z de xy_pts_Z;
	vector<pair<double, long long> > xy_pts_insertar;
	vector<pair<double, long long> > xy_pts_borrar;
	vector<pair<double, long long> > xy_pts_decrecer;
	vector<pair<double, long long> > xy_pts_union;
	while (file && getline(file, linebuffer)){
		if (linebuffer.length() == 0) continue;
		istringstream buffer(linebuffer);
		buffer >> tam;
		long long tiempoBorrar = 0;
		long long tiempoInsertar = 0;
		long long tiempoDecrecer = 0;
		long long tiempoUnion = 0;
		for(int i = 0; i < 1000; i++){
			monticuloFib<int> a;
			monticuloFib<int> b;

			//Insertamos tam numeros a los monticulos
			for(int i = 0; i < tam; i++) a.insertar(i);
			for(int i = 0; i < tam; i++) b.insertar(i);
			
			//Comprobacion de lo que tarda la operacion insertar
			auto start_timeInsertar = high_resolution_clock::now();
			a.insertar(1);
			auto end_timeInsertar = high_resolution_clock::now();

			//Comprobacion de lo que tarda la operacion borrarMinimo
			auto start_timeBorrar = high_resolution_clock::now();
			a.borrarMinimo();
			auto end_timeBorrar = high_resolution_clock::now();

			//Comprobacion de lo que tarda la operacion decrecerClave
			auto start_timeDecrecer = high_resolution_clock::now();
			if((a.getMinimo() != nullptr) && (a.getMinimo()->getHijo() != nullptr)){
				//std::cout << "Decreciendo la clave: " << a.getMinimo()->getHijo()->getValor() <<" a 0" << endl;
				a.decrecerClave(a.getMinimo()->getHijo(),0);
			}
			auto end_timeDecrecer  = high_resolution_clock::now();
		
			//Comprobacion de lo que tarda la operacion unir
			auto start_timeUnion = high_resolution_clock::now();
			//std::cout << "Uniendo monticulo B con monticulo A" << endl;
			b.unir(a);
			auto end_timeUnion = high_resolution_clock::now();
		
			//Borramos tam numeros a los monticulos
			for(int i = 0; i < tam; i++) a.borrarMinimo();
			for(int i = 0; i < tam; i++) b.borrarMinimo();
		
			//Suma de las 1000 pruebas para despues dividirlas entre 1000
			tiempoBorrar += duration_cast<microseconds>(end_timeBorrar - start_timeBorrar).count();
			tiempoInsertar += duration_cast<microseconds>(end_timeInsertar - start_timeInsertar).count();
			tiempoDecrecer += duration_cast<microseconds>(end_timeDecrecer - start_timeDecrecer).count();
			tiempoUnion += duration_cast<microseconds>(end_timeUnion - start_timeUnion).count();
		}	
		xy_pts_insertar.push_back(make_pair(tam,(tiempoInsertar/1000)));
		xy_pts_borrar.push_back(make_pair(tam,(tiempoBorrar/1000)));
		xy_pts_decrecer.push_back(make_pair(tam,(tiempoDecrecer/1000)));
		xy_pts_union.push_back(make_pair(tam,(tiempoUnion/1000)));
	};

	//Utilizacion de la interfaz de gnuplot a traves de gnuplot-iostream
	Gnuplot gp;
	gp 	<< "set xrange [0:6500]\nset yrange [0:500]\nset ylabel 'Tiempo (ms)'\nset xlabel 'Tamaño mont. Fibon.'\n";

	gp 	<< "plot" << gp.file1d(xy_pts_insertar) << "with lines title 'insertar',"
	<< gp.file1d(xy_pts_borrar) << "with lines title 'borrar',"
	<< gp.file1d(xy_pts_decrecer) << "with lines title 'decrecer'," 
	<< gp.file1d(xy_pts_union) << "with lines title 'union'"<< std::endl;
	
	#ifdef _WIN32
		std::cout << "Press enter to exit." << std::endl;
		std::cin.get();
	#endif
}
