#pragma once
#include <vector>
#include <string>
using namespace std;
struct Indice
{
	string nombre;
	string campo;
	string tipo;
	string tablename;
	int colnumber;
	Indice() {}
	Indice(string nombre,string campo, string tipo,string tablename,int colnumber) {
		this->nombre= nombre;
		this->tipo=tipo;
		this->campo = campo;
		this->tablename = tablename;
		this->colnumber = colnumber;
	}

	string toString() {
		return nombre + "," + campo + "," + tipo;
	}

};