template <class T> class nodo {
	private:
		T _valor;
		
		nodo<T>* _anterior;
		nodo<T>* _siguiente;
		nodo<T>* _hijo;
		nodo<T>* _padre;

		int _numHijos;
		bool _marcado;
		
	public:
		nodo(){
			_numHijos = 0;
		}
	
		//Getters
		T getValor() { return _valor; }
	
		nodo<T>* getAnt() { return  _anterior; }
		nodo<T>* getSig() { return _siguiente; }
		nodo<T>* getHijo() { return _hijo; }
		nodo<T>* getPadre() { return _padre; }
		
		const int& getNumHij() { return _numHijos; }
		const bool& getMarcado() { return _marcado; }
		
		//Setters
		void setValor(const T& valor) { _valor = valor; }
		
		void setAnt(nodo<T>* anterior) { _anterior = anterior; }
		void setSig(nodo<T>* siguiente) { _siguiente = siguiente; }
		void setHijo(nodo<T>* hijo) { _hijo = hijo; }
		void setPadre(nodo<T>* padre) { _padre = padre; }
		
		void aumentarNumHij() { _numHijos++; }
		void decrecerNumHij() { _numHijos--; }
		void setMarcado(const bool& marcado) { _marcado = marcado; }
		
	};
