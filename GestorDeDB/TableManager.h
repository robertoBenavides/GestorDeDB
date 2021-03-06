#pragma once
#include <iostream>
#include <fstream>
#include <chrono>
#include <Windows.h>
#include "StringAdapter.h"
#include "Tabla.h"
#include "Fecha.h"
#include "Indice.h"
#include "ArbolAVL.h"

#define rojo 4
#define verde 2
#define blanco 15
#define plomo 8

using namespace std;
struct TableManager
{
    vector<Tabla> tablas;
    StringAdapter SA;
    vector<Indice> indices;
    vector<ArbolAVL<string>> indxTrees;
    
    TableManager();
    
    bool tableexist(string, Tabla*& );
    void writeTable(Tabla );
    void rewriteInfo(vector<string>, string);
    void createTable(string);

    //data MAnager
    void insertValue(string);
    void updateValue(string);
    void deleteValue(string);
    void select(string, string);

    //reading methods
    vector<vector<string>> getfrontxtbyline(string, vector<int>);
    




    //index methods
    void indexTable(string);
    void getArbol(string, ArbolAVL<string>*&);
    bool indexexist(string);
    void saveIndex(Indice);

    //helper functions
    vector<vector<string>> getall(string tablename);
    int getIndexColum(vector<Col> colums, string data);
    vector<int> getbycol(Tabla tb, vector<vector<string>>datos, int i, string val, string condicional);
    vector<int> findInAVL(string val, int pos);
    //print setting
    void print(string, int);
};

