#include "Nodo.h"
#include <stack>
#include <vector>
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
	ArbolAVL(Indice indx) {
		this->indx = indx;
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
		maxline = (nlinea > maxline) ?nlinea :maxline ;


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
				if (s.front() == nullptr) {
					f << "\n";
					s.pop();
				}
				else {
					if (s.front()->estado == 0) {
						s.front()->estado += 1;
						s.push((s.front()->nodos[0]) ? s.front()->nodos[0] : nullptr);
						s.push((s.front()->nodos[1]) ? s.front()->nodos[1] : nullptr);

					}
					else if (s.front()->estado == 1) {
						f << s.front()->toString();
						f << "\n";
						s.pop();
					}
				}
				
			}
			f.close();
		}
		else cout << "Error de apertura de archivo." << endl;
	}
};

