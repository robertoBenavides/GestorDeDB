#include "Nodo.h"
#include <stack>
#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include "Fecha.h"
#include "StringAdapter.h"
using namespace std;
#pragma once
template <class T>
struct ArbolAVL {
	StringAdapter SA;
	string tipo;
	Nodo<T>* raiz = 0;
	int cantnodos = 0;
	ArbolAVL(string type) {
		tipo = type;
	}
	bool find(T val, Nodo<T>**& p, stack<pair<Nodo<T>**, bool> >& pila) {
		p = &raiz;
		pila.push(make_pair(&raiz, 0));

		if (tipo == "int") {
			while (*p && (*p)->val != val) {
				bool opt = stoi(val) >stoi((*p)->val);
				p = &((*p)->nodos[opt]);
				pila.push(make_pair(p, opt));
			}
		}
		else if (tipo == "date") {
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
	bool insert(T val,int nlinea) {

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
					(*p.first) = (*p.first)->nodos[!pos];
					w->nodos[!pos] = (*p.first)->nodos[pos];
					(*p.first)->nodos[pos] = w;
					swap(w->alt, (*p.first)->alt);
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
	
};

