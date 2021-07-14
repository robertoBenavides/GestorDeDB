#pragma once
#include <vector>
#include <string>
using namespace std;
template <class T>
struct Nodo {
	T val;
	int alt;
	vector<int> lineas;
	Nodo<T>* nodos[2];
	int estado = 0;
	Nodo(T a,int l) {
		val = a;
		alt = 1;
		nodos[0] = nodos[1] = 0;
		lineas.push_back(l);
	}
	Nodo(T a, vector<int> l) {
		val = a;
		alt = 1;
		nodos[0] = nodos[1] = 0;
		lineas = l;
	}
	string toString() {
		string a = val + ",[";
		for (int l : lineas) {
			a += to_string(l) + ",";
		}
		a[a.size() - 1] = ']';
		return a;
	}
};

