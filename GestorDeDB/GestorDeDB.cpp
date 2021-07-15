#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Tabla.h"
#include "Col.h"
#include "Fecha.h"
#include "StringAdapter.h"
#include "TableManager.h"
#include "ArbolAVL.h"

using namespace std;

StringAdapter SA;
TableManager TM;

string readComand() {
    string comando=" ";
    string line;
    do {
        getline(cin, line); 
        comando += line;
    } while (comando[comando.size() - 1] != ';');
    return SA.trim(comando);
}

void insertMasivo(int n, string nombreTabla) {
    for (int i = 0; i < n; i++)
    {
        string dni = to_string(11111111 + i);
        string edad = to_string(rand() % 100 + 1);
        string dia = to_string(rand() % 31 + 1);
        string mes = to_string(rand() % 12 + 1);
        string anio = to_string(rand() % 21 + 1970);
        string fecha = dia + "-" + mes + "-" + anio;
        string linea = nombreTabla + "(" + dni + ", Nombre-" + dni + ", Apellido-" + dni + ", " + edad + ", " + fecha + ")";
        TM.insertValue(linea);
    }
}

void insertMasivo2(int n, string nombreTabla) {
    for (int i = 0; i < n; i++)
    {
        string id = to_string(11111111 + (rand() % 1111111));
        string edad = to_string(rand() % 100 + 1);
        string dia = to_string(rand() % 31 + 1);
        string mes = to_string(rand() % 12 + 1);
        string anio = to_string(rand() % 21 + 1970);
        string fecha = dia + "-" + mes + "-" + anio;
        string linea = nombreTabla + "(" + id + ", Nombre-" + id + ", Apellido-" + id + ", " + edad + ", " + fecha + ")";
        TM.insertValue(linea);
    }
}

void run() {
    string comando = readComand();
    size_t pos = comando.find(" ");
    string ident = string(comando.begin(), comando.begin() + pos);
    string value = string(comando.begin() + pos + 1, comando.end());
    bool executed = false;
    if (ident == "create_index") {
        executed = true;
        TM.indexTable(value);
    }
    if (ident == "create") {
        pos = value.find(" ");
        size_t pospar = value.find("(");
        if (pospar != string::npos) {
            executed = true;
            string a = (pospar < pos) ? value.substr(0, pospar) : value.substr(0, pos);
            pos = (pospar < pos) ? pospar : pos;
            if (value.substr(0, pos) == "table")
                TM.createTable(string(value.begin() + pos, value.end()));
        }

    }
    if (ident == "insert") {
        pos = value.find(" ");
        if (pos != string::npos && string(value.begin(), value.begin() + pos) == "into") {

            value = string(value.begin() + pos + 1, value.end());

            executed = true;
            TM.insertValue(value);


        }
    }
    if (ident == "update") {
        TM.updateValue(value);

    }
    if (ident == "delete") {
        TM.deleteValue(value);
        executed = true;
    }
    if (ident == "select") {
        pos = value.find("from");
        if (pos != string::npos) {
            executed = true;
            string campos = string(value.begin(), value.begin() + pos);
            value.erase(value.begin(), value.begin() + pos + 4);
            TM.select(value, campos);
        }

    }
    if (!executed) {
         TM.print("comando incorrecto",rojo);
     }
}

int main()
{
    bool running = true;
    while (running) {
        run();
    }
}