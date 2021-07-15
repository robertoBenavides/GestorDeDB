#include "Nodo.h"
#include <stack>
#include <vector>
#include <chrono>
#include <string>
#include <iostream>
#include <queue>
#include "Fecha.h"
#include "StringAdapter.h"
#include "Indice.h"
using namespace std;
#pragma once
template <class T>
struct ArbolAVL {
	int maxline=0;
	Indice indx;
	StringAdapter SA;
	bool changed=false;

	Nodo<T>* raiz = 0;
	int cantnodos = 0;
	ArbolAVL() {
	};
	ArbolAVL(Indice indx) {
		this->indx = indx;
	}
	
	void insertfromTxt(string linea) {
		vector<string>data = SA.split(linea,"/");
		string val = SA.trim(data[0]);
		vector<string> lines = SA.split(string(data[1].begin() + 1, data[1].end() - 1),",");
		for (string l : lines) {
			insert(val,stoi(l));
		}
	}
	
	bool find(T val, Nodo<T>**& p, stack<pair<Nodo<T>**, bool> >& pila) {
		p = &raiz;
		pila.push(make_pair(&raiz, 0));

		if (indx.tipo == "int") {
			while (*p && (*p)->val != val) {
				bool opt = stoi(val) >stoi((*p)->val);
				p = &((*p)->nodos[opt]);
				pila.push(make_pair(p, opt));
			}
		}
		else if (indx.tipo == "date") {
			while (*p && (*p)->val != val) {
				bool opt = Fecha(SA.split(val, "-")) > Fecha(SA.split((*p)->val, "-"));
				p = &((*p)->nodos[opt]);
				pila.push(make_pair(p, opt));
			}
		}
		else {
			while (*p && (*p)->val != val) {
				bool opt = val >(*p)->val;
				p = &((*p)->nodos[opt]);
				pila.push(make_pair(p, opt));
			}
		}
		
		return *p;
	}

	vector<int> buscar(T dat) {
		Nodo<T>** p;
		if (indx.tipo == "int") {
			if (buscarInt(stoi(dat),p)) {
				return (*p)->lineas;
			}
			else {
				vector<int> vacia;
				return vacia;
			}
		}
		else {
			if (buscarString(dat, p)) {
				return (*p)->lineas;
			}
			else {
				vector<int> vacia;
				return vacia;
			}
		}
	}

	bool buscarString(T dat, Nodo<T>**& p)
	{
		p = &raiz;
		for (p; *p && (*p)->val != dat; p = &((*p)->nodos[dat > (*p)->val]));
		return *p;
	}

	bool buscarInt(int dat, Nodo<T>**& p)
	{
		p = &raiz;
		for (p; *p && stoi((*p)->val) != dat; p = &((*p)->nodos[dat > stoi((*p)->val)]));
		return *p;
	}

	void borrarPuntero(T dat, int puntero)
	{
		Nodo<T>** p;

		if (indx.tipo == "int") {
			buscarInt(stoi(dat), p);
		}
		else {
			buscarString(dat, p);
		}
		int i = 0;
		if (*p) {
			for (int j : (*p)->lineas) {
				if (j == puntero) {
					break;
				}
				i++;
			}
			((*p)->lineas).erase(((*p)->lineas).begin() + i);
		}
	}

	void menorRigth(Nodo<T>**& p) {
		for (p = &((*p)->nodos[0]); (*p)->nodos[0]; p = &((*p)->nodos[0]));

	}
	
	bool insert(T val,int nlinea) {
		maxline = (nlinea > maxline) ? nlinea : maxline;
		Nodo<T>** p;
		stack< pair< Nodo<T>**, bool> > pila;

		if (find(val, p, pila)) {
			(*p)->lineas.push_back(nlinea);
			return 0;
		}
		*p = new Nodo<T>(val, nlinea);

		if (pila.size() > 1) {
			balanceo(pila);
		}
		


		cantnodos++;
		return 1;
	}
	
	void balanceo(stack< pair< Nodo<T>**, bool> > pila) {
		pair <Nodo<T>**, bool> h, p;
		int val = 0;
		while (pila.size() > 1) {
			p = pila.top();
			pila.pop();
			Nodo<T>* t = *(pila.top().first);
			if (t->nodos[0] && t->nodos[1]) {
				val = t->nodos[0]->alt - t->nodos[1]->alt;
			}
			else if (t->nodos[0]) val = t->nodos[0]->alt;
			else if (t->nodos[1]) val = t->nodos[1]->alt;

			if (val == 0) return;
			if (val == 1 || val == -1) {
				if ((*p.first)->alt + 1 > (*pila.top().first)->alt) (*pila.top().first)->alt++;
				h = p;
			}
			if (val == 2 || val == -2) {
				if (p.second == h.second) {
					Nodo<T>** a = (pila.top()).first;
					(*a)->alt--;

					Nodo<T>* w = (*a);
					bool pos = p.second;
					//a->p->h to p->->h
					(*a) = (*(p.first));
					//p.second es la direccion del hijo
					w->nodos[pos] = (*(p.first))->nodos[!pos];
					//si p era hijo izq de a se agrega a la der y viceversa
					(*a)->nodos[!pos] = w;
				}
				else {
					Nodo<T>* w = (*p.first);
					//si p era hijo izq de a se cambia a la der y viceversa
					bool pos = p.second;
					(*p.first) = (*p.first)->nodos[!pos];//p apunta al hijo
					w->nodos[!pos] = (*p.first)->nodos[pos];//w q ahora es pe apunta a
					(*p.first)->nodos[pos] = w;
					swap(w->alt, (*p.first)->alt);

					Nodo<T>** a = (pila.top()).first;
					(*a)->alt--;
					w = (*a);
					(*a) = (*(p.first));
					//p.second es la direccion del hijo
					w->nodos[pos] = (*(p.first))->nodos[!pos];
					//si p era hijo izq de a se agrega a la der y viceversa
					(*a)->nodos[!pos] = w;
					
				}
			}
		}
	}
	
	bool remove(T val) {
		stack< pair< Nodo<T>**, bool> > pila;
		Nodo<T>** p;
		if (!find(val, p, pila)) return 0;
		if ((*p)->nodos[0] && (*p)->nodos[1]) {
			Nodo<T>** q = p;
			menorRigth(q);

			(*p)->val = (*q)->val;
			p = q;
		}
		Nodo<T>* t = *p;
		*p = (*p)->nodos[(*p)->nodos[1] != 0];
		delete t;
		return 1;

	}
	
	void recorrerNiveles(stack<Nodo<T>*> s) {
		int niveles = 0, alt = niveles;
		while (!s.empty())
		{
			if (s.top()->estado == 0) {
				s.top()->estado += 1;
				niveles++;
				if ((s.top()->nodos[0])) s.push(s.top()->nodos[0]);

			}
			else if (s.top()->estado == 1) {
				s.top()->estado += 1;
				alt = (niveles > alt) ? niveles : alt;
				//cout << s.top()->val << " ";
			}
			else if (s.top()->estado == 2) {
				s.top()->estado += 1;
				if ((s.top()->nodos[1])) s.push(s.top()->nodos[1]);
			}
			else if (s.top()->estado == 3) {
				niveles--;
				s.top()->estado = 0;
				s.pop();
			}
		}
		int finalAlt = alt - 1;
		int maxval;
		/*Nodo<T>* max = raiz;
		while (max->nodos[1]) {
			max = max->nodos[1];
		}*/
		maxval = 10;

		queue<Nodo<T>*> q;
		q.push(raiz);

		vector<string> levelarbol;
		int l = 0, maxhojas = pow(2, finalAlt);
		int maxspaceline = to_string(maxval).length();
		int maxAncho = maxhojas * maxspaceline + (maxhojas - 1) * maxspaceline;
		int altto1 = alt;


		while (!q.empty())
		{
			string lvl;
			int numNodes = pow(2, l);
			int tam = pow(2, altto1) - 1;
			while (numNodes && l <= finalAlt && !q.empty()) {
				if (q.front() != nullptr) {
					if (q.front()->nodos[0] != nullptr) {
						q.push(q.front()->nodos[0]);
					}
					else
					{
						q.push(nullptr);
					}
					if (q.front()->nodos[1] != nullptr) {
						q.push(q.front()->nodos[1]);
					}
					else
					{
						q.push(nullptr);
					}

					levelarbol.push_back(to_string(q.front()->val));

				}
				else {
					levelarbol.push_back(string(maxspaceline, ' '));
					q.push(nullptr);
					q.push(nullptr);
				}
				q.pop();
				numNodes--;
			}
			if (l > finalAlt) {
				while (!q.empty()) q.pop();
			}
			//if (levelarbol.size() != 0) centerLine(lvl, maxAncho, levelarbol, tam);
			cout << lvl << endl;
			l++;
			altto1--;
			levelarbol.clear();


		}
		cout << '\n';

	}
	
	void print() {
		if (!raiz) {
			cout << "el arbol esta vacio" << endl;
			return;
		}
		stack<Nodo<T>*> s;
		s.push(raiz);
		recorrerNiveles(s);

		cout << endl;
	}
	
	void save() {
		ofstream f(indx.nombre + ".txt");
		if (f.is_open()) {
			queue<Nodo<T>*> s;
			s.push(raiz);
			while (!s.empty())
			{
				if (s.front()->estado == 0) {
					s.front()->estado += 1;
					if (s.front()->nodos[0])s.push(s.front()->nodos[0]);
					if (s.front()->nodos[1])s.push(s.front()->nodos[1]);

				}
				else if (s.front()->estado == 1) {
					s.front()->estado = 0;
					f << s.front()->toString();
					f << "\n";
					s.pop();
				}
				
				
			}
			f.close();
		}
		else cout << "Error de apertura de archivo." << endl;
	}
	
	void print_hijos(Nodo<T>* p)
	{
		if (p)
		{
			print_hijos((p)->nodos[0]);
			if ((p)->nodos[0])
			{
				cout << "\t" << (p)->val << " -> " << (p)->nodos[0]->val << ";" << endl;
			}
			if ((p)->nodos[1])
			{
				cout << "\t" << (p)->val << " -> " << (p)->nodos[1]->val << ";" << endl;
			}
			print_hijos(p->nodos[1]);
		}
	}
	
	void hijos()
	{
		Nodo<T>* p = raiz;
		cout << "digraph G {" << endl;
		print_hijos(p);
		cout << "}" << endl;
	}
};

