#include "nodoFib.hpp"
#include "math.h"
#include <cstddef>
#include <iostream>
#include <string>

template <class T> 
class monticuloFib {
	private:
		int _num;
		nodo<T>* _minimo;
		nodo<T>* _crearNodo(T valor);
		nodo<T>* _unir(nodo<T>* m1, nodo<T>* m2);
		void _declecerClave(nodo<T>* nod, T valor);
		void _cortar(nodo<T>* hijo, nodo<T>* padre);
		void _cortarCascada(nodo<T>* padre);
		void _borrarMinimo();
		void _consolidate();
		void _link(nodo<T>* y, nodo<T>* x);
		void _insertar(nodo<T>* x);
		void swap(nodo<T>*& a, nodo<T>*& b)
		{
		   nodo<T>* aux = a;
		   a = b;
		   b = aux;
		}
		
	public:
		monticuloFib() { 
			_minimo=nullptr;
			_num = 0;
		}
		
		void insertar(T valor){
			_insertar(_crearNodo(valor));
		}
	
		nodo<T>* getMinimo() const{
			return _minimo;
		}
		
		void unir(monticuloFib monticulo) {
			//Sacas el minimo de la union de los dos monticulos
			_minimo=_unir(this->getMinimo(),monticulo.getMinimo());
			//H1.n += H2.n
			_num += monticulo.getNumNodos();
		}
	
		void decrecerClave(nodo<T>* nod, T valor) {
			_declecerClave(nod, valor);
		}
		
		void borrarMinimo(){
			_borrarMinimo();
		}
		
		int getNumNodos(){
			return _num;
		}
		
		std::string static leerMonticulo(nodo<T>* m){
			std::string buffer = "(";
			
			nodo<T>* aux = m->getSig();
			buffer += std::to_string(m->getValor())+" ";
			if(m->getHijo() != nullptr){
					buffer += leerMonticulo(m->getHijo());
			}

			while(aux != m) {
				buffer += std::to_string(aux->getValor())+" ";
				if(aux->getHijo() != nullptr){
					buffer += leerMonticulo(aux->getHijo());
				}
				aux = aux->getSig();
			}
			buffer +=")";
			
			return buffer;
		}
		
		friend std::ostream& operator<< (std::ostream& os, const monticuloFib& mon) {
			std::string buffer = leerMonticulo(mon.getMinimo());
			
			os << buffer;
			return os;
		}
	};
/*
 * 
 *  _crearNodo(T valor) => crea un nodo con un valor de tipo T
 * 
 */
template<class T>
nodo<T>* monticuloFib<T>::_crearNodo(T valor){
			nodo<T>* s = new nodo<T>;
			s->setValor(valor);
			s->setAnt(s);
			s->setSig(s);
			s->setHijo(nullptr);
			s->setPadre(nullptr);
			return s;
	}

/*
 * 
 *  _unir(nodo<T>* m1,nodo<T>* m2) => une el monticulo m2 al m1
 * 
 */	
template<class T>
nodo<T>* monticuloFib<T>::_unir(nodo<T>* m1, nodo<T>* m2){
		//Si un monticulo esta a null, cogemos el otro como resultado de la union
		if(m1==nullptr || m2==nullptr) return (m1==nullptr) ? m2 : m1;
		nodo<T>* sigm1 = m1->getSig();
		nodo<T>* antm2 = m2->getAnt();
		m1->setSig(m2);
		m2->setAnt(m1);
		antm2->setSig(sigm1);
		sigm1->setAnt(antm2);
		//Si el minimo del m2 es menor al del m1, entonces
		// el minimo de la union de m2 en m1 es m2
		if(m1->getValor() > m2->getValor()) { swap(m1,m2); }
		return m1;
}

/*
 * 
 *  _insertar(nodo<T>* x) => insertar un nodo en el monticulo
 * 
 */	
template<class T>
void monticuloFib<T>::_insertar(nodo<T>* x){
	//si H.min == NIL
	if(_minimo == nullptr){
		//crea una lista de raices en la que este solo x
		//H.min = x
		_minimo = x;
	} else { //si no inserta x en la lista de raices
			nodo<T>* antMin = _minimo->getAnt();
			antMin->setSig(x);
			x->setAnt(antMin);
			_minimo->setAnt(x);
			x->setSig(_minimo);
			//si x.key < H.min.key
			//H.min = x
			_minimo = (x->getValor() < _minimo->getValor()) ? x : _minimo;
	}
	//H.n = H.n + 1
	_num++;
}


/*
 * 
 *  _decrecerClave(nodo<T>* hijo, T valor) => cambia el valor de hijo a otro valor menor
 * 
 */
template<class T>
void monticuloFib<T>::_declecerClave(nodo<T>* hijo, T valor) {
		//si valor > key[hijo]
		if(hijo->getValor() < valor) return;
		//key[hijo] <- valor
		//padre <- p[hijo]
		hijo->setValor(valor);
		nodo<T>* padre = hijo->getPadre();
		//si padre != NIL y key[hijo] < key[padre]
		if(padre != nullptr && hijo->getValor() < padre->getValor()){
			_cortar(hijo,padre);
			_cortarCascada(padre);
		}
		//si key[hijo] < key[min[H]]
		if(hijo->getValor() < _minimo->getValor()){
			//then min[H] <- hijo			
			_minimo = hijo;
		}
	}

/*
 * 
 *  _cortar(nodo<T>* hijo, nodo<T>* padre) => quita a 'hijo' de la lista de hijos del padre
 *						para añadirlo a la lista de root
 * 
 */	
template<class T>
void monticuloFib<T>::_cortar(nodo<T>* hijo, nodo<T>* padre) {
	//quita a 'hijo' de la lista de hijos del padre
	nodo<T>* sig = hijo->getSig();
	nodo<T>* ant = hijo->getAnt();
	if(sig == hijo) {
		padre->setHijo(nullptr);
	}else{
		sig->setAnt(hijo->getAnt());
		ant->setSig(hijo->getSig());
		if(padre->getHijo() == hijo) padre->setHijo(hijo->getSig());
	}
	//decrementar degree[padre]
	padre->decrecerNumHij();
	//añadir 'hijo' a la lista de raices de H
	nodo<T>* sigMin = _minimo->getSig();
	sigMin->setAnt(hijo);
	hijo->setSig(sigMin);
	_minimo->setSig(hijo);
	hijo->setAnt(_minimo);
	
	//p[hijo] <- NIL
	hijo->setPadre(nullptr);
	//mark[hijo] <- FALSE
	hijo->setMarcado(false);
	}

/*
 * 
 *  _cortarCascada(nodo<T>* padre) => quita a padre o no, dependiendo de si ha perdido hijos antes
 *		
 * 
 */	
template<class T>
void monticuloFib<T>::_cortarCascada(nodo<T>* padre){
	//abuelo <- p[padre]
	nodo<T>* abuelo = padre->getPadre();
	//si abuelo != NIL
	if(padre != nullptr) {
		//si mark[padre] = FALSE
		if(padre->getMarcado() == false){
			//mark[padre] <- TRUE
			padre->setMarcado(true);
		}else{
			_cortar(padre,abuelo);
			_cortarCascada(abuelo);
		}
	}	
}

/*
 * 
 *  _borrarMinimo() => borrar el minimo
 *		
 * 
 */
template<class T>
void monticuloFib<T>::_borrarMinimo(){
	//si _minimo != NIL
	if(_minimo != nullptr) {
		nodo<T>* minimo = _minimo;
		int numHijos = minimo->getNumHij();
		nodo<T>* antMin = _minimo->getAnt();
		nodo<T>** hijosMin = new nodo<T>*[numHijos];
		nodo<T>* aux = minimo->getHijo();
		int i = 0;
		while(i < numHijos){
			hijosMin[i] = aux;
			aux = aux->getSig();
			i++;
		}
		//para cada hijo de minimo
		int j = 0;
		while(j < numHijos){
			aux = hijosMin[j];
			
			antMin->setSig(aux);
			aux->setAnt(antMin);
			_minimo->setAnt(aux);
			aux->setSig(minimo);
			
			aux->setPadre(nullptr);
			j++;
		}
		delete [] hijosMin;
		
		minimo->getAnt()->setSig(minimo->getSig());
		minimo->getSig()->setAnt(minimo->getAnt());
		
		if(minimo == minimo->getSig()) _minimo = nullptr;
		else { 
			_minimo = minimo->getSig();
			_consolidate();
		}
		_num--;
	}
}

/*
 * 
 *  _consolidate() => unimos todos los nodos que tengan el mismo num de hijos
 *			con ayuda de un array auxiliar al cual llamaremos 'perchero? (en este caso)
 *		
 * 
 */
template<class T>
void monticuloFib<T>::_consolidate(){
	nodo<T>* x, *w, *auxPerchero;
	int grado;
	//maxGrado <- floor(log con base de la número áureo de _num)
	int maxGrado = floor(log(_num)/log((1+sqrt(5))/2));
	//Por si acaso, el tamaño del perchero va a ser el maxGrados+2
	//por temas de arrastres a la hora de unir 2 nodos
	nodo<T>** perchero = new nodo<T>*[maxGrado+2];
	int numRaices = 1;
	nodo<T>* aux = _minimo->getSig();
	//Conseguimos el numRaices
	while(aux != _minimo){
		numRaices++;
		aux = aux->getSig();
	}
	
	nodo<T>** raices = new nodo<T>*[numRaices];
	int i = 0;
	//Metemos las raices en el array raices[numRaices]
	while(i < numRaices){
		raices[i] = aux;
		aux = aux->getSig();
		i++;
	}
	
	i = 0;
	//Para cada nodo de la lista de raices
	while(i < numRaices){
		//x <- raices[i]
		w = raices[i];
		x = w;
		//grado <- degree[x]
		grado = x->getNumHij();
		//mientras perchero[grado] != NIL
		while(perchero[grado] != nullptr)
		{
			auxPerchero = perchero[grado];
			//Si key[x] > key[auxPerchero] entonces que cambien los valores
			if(x->getValor() > auxPerchero->getValor()) swap(x,auxPerchero);
			//unimos estos dos nodos			
			_link(auxPerchero,x);
			//perchero[grado] <- NIL			
			perchero[grado] = nullptr;
			//grado = grado + 1
			grado++;
		}
		//la union de estos dos nodos se guardara en el siguiente
		//hueco del 'perchero'
		perchero[grado] = x;
		i++;
	}
	delete [] raices;
	//min[H]  <- NIL
	_minimo = nullptr;
	
	i = 0;
	//recorremos todos los nodos del perchero
	while(i < maxGrado+2){
		if(perchero[i] != nullptr) {
			//si min[H] <- NIL entonces min[H] <- perchero[i]
			if(_minimo == nullptr){
				_minimo = perchero[i]; 
				perchero[i]->setAnt(perchero[i]);
				perchero[i]->setSig(perchero[i]);
			}else {
				//añadir perchero[i] a la lista de raices de H
				nodo<T>* antMin = _minimo->getAnt();
				antMin->setSig(perchero[i]);
				perchero[i]->setAnt(antMin);
				_minimo->setAnt(perchero[i]);
				perchero[i]->setSig(_minimo);
				//si key[perchero[i]] < key[min[H]] entonces min[H] <- perchero[i]
				if(perchero[i]->getValor() < _minimo->getValor()) _minimo = perchero[i];
			}
		}
		i++;
	}
	
	delete [] perchero;
}

/*
 * 
 *  _link(nodo<T>* y, nodo<T>* x) => convierte 'y' en un hijo de 'x'
 *		
 * 
 */
template<class T>
void monticuloFib<T>::_link(nodo<T>* y, nodo<T>* x){
	//quita 'y' de la lista de raices de H
	nodo<T>* antY = y->getAnt();
	nodo<T>* sigY = y->getSig();
	nodo<T>* hijoX = x->getHijo();
	nodo<T>* antHijoX;
	
	antY->setSig(sigY);
	sigY->setAnt(antY);
	
	//hacer 'y' un hijo de 'x'
	if(hijoX != nullptr){
		antHijoX = hijoX->getAnt();
		antHijoX->setSig(y);
		y->setAnt(antHijoX);
		hijoX->setAnt(y);
		y->setSig(hijoX);
	}else{
		y->setAnt(y);
		y->setSig(y);
	}
	x->setHijo(y);
	y->setPadre(x);

	//incrementar degree[x]
	x->aumentarNumHij();
	
	//mark[y] <- FALSE
	y->setMarcado(false);
}
