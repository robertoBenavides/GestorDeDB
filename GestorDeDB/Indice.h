#pragma once
#include <vector>
#include <string>
using namespace std;
class Indice
{
	string val;
	vector<int> lineas;

	

public:
	Indice(string val, vector<int> l) {
		this->val = val;
		this->lineas = l;
	}
	string toString() {
		string a= val + ",[";
		for (int l : lineas) {
			a += to_string(l) + ",";
		}
		a[a.size() - 1] = ']';
		return a;
	}

};

