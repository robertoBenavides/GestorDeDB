#include "TableManager.h"

TableManager::TableManager()
{
    //reading all tables
    ifstream myfile("Tablas.txt");
    string line;
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            size_t pos = line.find("[");
            vector<string> tbd = SA.split(string(line.begin(), line.begin() + pos - 1), "/");
            vector<string> cd = SA.split(string(line.begin() + pos + 1, line.end() - 1), ",");
            vector<Col> colums;
            for (string c : cd) {
                vector<string> ds = SA.split(c, "/");
                if (ds.size() == 3) {
                    colums.push_back(Col(ds[0], ds[1], ds[2]));
                }
                else {
                    colums.push_back(Col(ds[0], ds[1]));
                }
            }
            tablas.push_back(Tabla(tbd[0], stoi(tbd[1]), colums));
        }
        myfile.close();
    }

    else cout << "Unable to open file";

    //reading indexes

    ifstream indxmyfile("Indices.txt");
    string indxline;
    if (indxmyfile.is_open())
    {
        while (getline(indxmyfile, indxline))
        {
            vector<string> valores = SA.split(indxline, ",");
            indices.push_back(Indice(valores[0], valores[1], valores[2], valores[3], stoi(valores[4])));
        }
        indxmyfile.close();
    }

    //generating trees
    for (Indice i : indices) {
        ifstream arbolmyfile(i.nombre+".txt");
        string arbolline;
        ArbolAVL<string> tree(i);
        if (arbolmyfile.is_open())
        {
            while (getline(arbolmyfile, arbolline))
            {
                
                tree.insertfromTxt(arbolline);
            }
            arbolmyfile.close();
        }
        indxTrees.push_back(tree);
        char a = 'd';
    }

}
bool TableManager::tableexist(string tbname, Tabla*& a)
{
    for (Tabla& t : tablas)
        if (t.nombre == tbname) {
            a = &t;
            return true;
        }
    return false;
}

void TableManager::writeTable(Tabla tb)
{
    {
        ofstream f("Tablas.txt", ios::app);
        if (f.is_open()) {
            f << tb.toString();
            f << "\n";
            f.close();
        }
        else  cout << "Error de apertura de archivo." << endl;

    }
}

void TableManager::rewriteInfo(vector<string> finald, string tablename)
{
    {
        ofstream f(tablename + ".txt", ios::out);

        if (f.is_open()) {
            for (string a : finald) f << a;
            f.close();
        }
        else cout << "Error de apertura de archivo." << endl;
    }
}

void TableManager::createTable(string data)
{
    {
        string tablename = SA.trim(string(data.begin() + 1, data.begin() + data.find("(")));
        Tabla* tb;
        if (!tableexist(tablename, tb)) {
            string args = string(data.begin() + data.find("(") + 1, data.begin() + data.find(")"));
            vector<string> campos = SA.split(SA.trim(args), ",");
            const int c = campos.size();
            vector<Col> columnas;

            for (string a : campos) {
                vector<string> val = SA.split(SA.trim(a));
                size_t pos = val[1].find("char");
                if (pos != string::npos) {
                    vector<string> tam = SA.split(SA.trim(val[1]), "[");
                    size_t postam = tam[1].find("]");
                    string tamanio = string(tam[1].begin(), tam[1].begin() + postam);
                    columnas.push_back(Col(val[0], tam[0], tamanio));
                }
                else {
                    columnas.push_back(Col(val[0], val[1]));
                }
            }
            Tabla t(tablename, columnas.size(), columnas);
            writeTable(t);
            tablas.push_back(t);
            print("tabla \"" + tablename + "\" creada", verde);
        }
        else {
            print("tabla \"" + tablename + "\" ya existe", rojo);
        }
    }
}

void TableManager::insertValue(string val)
{
    size_t pos = (val.find(" ") > val.find("(")) ? val.find("(") : val.find(" ");

    string tablename = SA.trim(string(val.begin(), val.begin() + pos));
    size_t posidx = tablename.find("_idx");

    Tabla* t;
    string finalvalues;
    if (tableexist(tablename, t)) {
        val = string(val.begin() + val.find("(") + 1, val.begin() + val.find(")"));
        Tabla tb = *t;
        vector<string> valores = SA.split(val, ",");

        if (valores.size() != tb.numcols) {
            cout << "numero incorrecto de columnas" << endl;
        }
        else {

            for (int i = 0; i < valores.size(); i++) {

                if (tb.colums[i].tamanio != "0") {
                    finalvalues += SA.fixCadsize(SA.trim(valores[i]), stoi(tb.colums[i].tamanio));
                }
                else {
                    vector<string> fecha = SA.split(SA.trim(valores[i]), "-");
                    if (fecha[0].size() == 1) {
                        fecha[0] = "0" + fecha[0];
                    }
                    if (fecha[1].size() == 1) {
                        fecha[1] = "0" + fecha[1];
                    }
                    finalvalues += fecha[0] + "-" + fecha[1] + "-" + fecha[2];
                }
                finalvalues += ",";
            }
            finalvalues = string(finalvalues.begin(), finalvalues.end() - 1);
            ofstream f(tablename + ".txt", ios::app);
            if (f.is_open()) {
                f << finalvalues;
                f << "\n";
                f.close();
            }
            else cout << "Error de apertura de archivo." << endl;
            //print("se inserto correctamente", verde);

        }

    }
    else {
        print("tabla \"" + tablename + "\" no existe", rojo);
    }

    if (posidx != string::npos) {
        ArbolAVL<string>* arbol = nullptr;
        getArbol(tablename, arbol);
        if (arbol != nullptr) {
            string val = SA.split(finalvalues, ",")[(*arbol).indx.colnumber];
            (*arbol).insert(val, (*arbol).maxline + 1);
        }

    }

}

void TableManager::getArbol(string tname, ArbolAVL<string>* ind) {
    for (Indice i : indices) {
        if (i.tablename == tname) {
            *ind = i;
            return;
        }
    }
}

bool TableManager::indexexist(string indexname)
{
    for (Indice ind : indices) {
        if (ind.nombre == indexname) {
            return true;
        }
    }
    return false;
}

void TableManager::saveIndex(Indice i)
{
    ofstream f("Indices.txt", ios::app);
    if (f.is_open()) {
        f << i.toString();
        f << "\n";
        f.close();
    }
    else  cout << "Error de apertura de archivo." << endl;

}

void TableManager::deleteValue(string value)
{
    int pos = value.find(" ");
    if (string(value.begin(), value.begin() + pos) == "from") {
        value.erase(value.begin(), value.begin() + pos + 1);
        pos = value.find(" ");
        string tablename = string(value.begin(), value.begin() + pos);
        pos = value.find("where");
        value.erase(value.begin(), value.begin() + pos + 5);
        string condicion = SA.trim(string(value.begin(), value.end() - 1));


        string condicional;
        if (condicion.find("=") != string::npos) {
            condicional = "=";
        }
        if (condicion.find("<") != string::npos) {
            condicional = "<";
        }
        if (condicion.find(">") != string::npos) {
            condicional = ">";
        }

        vector<string> cond = SA.split(SA.trim(string(value.begin(), value.end() - 1)), condicional);

        Tabla* t;
        if (tableexist(tablename, t)) {
            Tabla tb = *t;
            vector<vector<string>> data = getall(tablename);
            int colnumcond = getIndexColum(tb.colums, SA.trim(cond[0]));
            vector<int> indexdata;
            bool tablaindexada = false;
            bool colindexada = false;
            int pos = 0;
            for (ArbolAVL<string> e : indxTrees) {
                if (e.indx.tablename == tablename) {
                    tablaindexada = true;
                    if (e.indx.colnumber == colnumcond) {
                        colindexada = true;
                    }
                    break;
                }
                pos += 1;
            }

            if (tablaindexada && colindexada) {
                indexdata = findInAVL(SA.trim(cond[1]), pos);
                indxTrees[pos].remove(SA.trim(cond[1]));
                indxTrees[pos].save();
            }
            else {
                indexdata = getbycol(tb, data, colnumcond, SA.trim(cond[1]), condicional);
            }



            if (indexdata.size()) {
                for (int i = indexdata.size() - 1; i >= 0; i--) {
                    if (tablaindexada) {
                        string borrar = SA.trim(data[indexdata[i]][indxTrees[pos].indx.colnumber]);
                        vector<int> punteros = indxTrees[pos].buscar(borrar);
                        if (punteros.size() == 1) {
                            indxTrees[pos].remove(borrar);
                        }
                        else {
                            indxTrees[pos].borrarPuntero(borrar, indexdata[i]);
                        }
                    }
                    data.erase(data.begin() + indexdata[i]);
                }

                vector<string>finald = SA.toFlatString(data);
                if (tablaindexada) {
                    indxTrees[pos].save();
                }
                rewriteInfo(finald, tablename);
                print("se elimino correctamente", verde);
            }
            else {
                print("dato(s) no encontrado(s)", rojo);
            }

        }
        else {
            print("la tabla \"" + tablename + "\" no existe", rojo);
        }


    }
}

void TableManager::select(string value, string campos)
{
    auto start = std::chrono::system_clock::now();
   
    string tablename = SA.trim(string(value.begin(), value.end() - 1));
    int pos = value.find("where");
    string condicional;
    vector<string> cond;
    if (pos != string::npos) {
        tablename = SA.trim(string(value.begin(), value.begin() + pos));
        value.erase(value.begin(), value.begin() + pos + 5);
        string condicion = SA.trim(string(value.begin(), value.end() - 1));

        if (condicion.find("=") != string::npos) {
            condicional = "=";
        }
        if (condicion.find("<") != string::npos) {
            condicional = "<";
        }
        if (condicion.find(">") != string::npos) {
            condicional = ">";
        }

        cond = SA.split(SA.trim(string(value.begin(), value.end() - 1)), condicional);
    }
    Tabla* t;
    if (tableexist(tablename, t)) {
        Tabla tb = *t;
        vector<vector<string>>data = getall(tablename);

        if (pos != string::npos) {
            int colnumcond = getIndexColum(tb.colums, SA.trim(cond[0]));
            vector<int> indexdata;
            bool indexado = false;
            int pos=0;
            for (ArbolAVL<string> e : indxTrees) {
                if (e.indx.tablename == tablename && e.indx.colnumber == colnumcond) {
                    indexado = true;
                    break;
                }
                pos += 1;
            }
            if (condicional != "=") {
                indexado = false;
            }

            if (indexado) {
                indexdata = findInAVL(SA.trim(cond[1]), pos);
            }
            else {
                indexdata = getbycol(tb, data, colnumcond, SA.trim(cond[1]), condicional);
            }
            vector<vector<string>> newData;
            for (int i : indexdata) {
                newData.push_back(data[i]);
            }
            data = newData;
        }
        auto end = std::chrono::system_clock::now();

        std::chrono::duration<float, std::milli> duration = end - start;
        
        vector<string>fullTable;
        string charval = "row\t";
        if (SA.trim(campos) == "*") {
            for (Col c : tb.colums) {

                if (c.tamanio != "0") {
                    charval += SA.fixCadsize(c.nombre, stoi(c.tamanio)) + "\t";

                }
                else {
                    charval += c.nombre + "\t";
                }
            }
            fullTable.push_back(charval);
            
            print(charval, plomo);
            int i = 1;
            for (vector<string>camp : data) {

                cout << i << "\t";
                for (string c : camp) cout << c << "\t";
                cout << endl;
                i++;
            }
            cout << "ejecutado en:" << duration.count() << "ms" << endl;
        }
        else {
            vector<string> camps = SA.split(campos, ",");
            vector<int> indices;
            for (string c : camps) {
                indices.push_back(getIndexColum(tb.colums, SA.trim(c)));
            }

            for (int c : indices) {
                if (tb.colums[c].tamanio != "0") {
                    charval += SA.fixCadsize(tb.colums[c].nombre, stoi(tb.colums[c].tamanio)) + "\t";

                }
                else {
                    charval += tb.colums[c].nombre + "\t";
                }
            }
            auto end = std::chrono::system_clock::now();

            std::chrono::duration<float, std::milli> duration = end - start;
            
            print(charval + "\t", plomo);
            for (vector<string>camp : data) {
                for (int c : indices) cout << camp[c] << "\t";
                cout << endl;
            }
            cout << "ejecutado en:" << duration.count() << "ms" << endl;
        }
    }
    else {
       print("tabla no existe",rojo);
    }
    
}

void TableManager::updateValue(string value)
{
    int pos = value.find(" ");
    string tablename = string(value.begin(), value.begin() + pos);
    value.erase(value.begin(), value.begin() + pos + 1);
    Tabla* t;
    if (tableexist(tablename, t)) {
        Tabla tb = *t;
        pos = value.find(" ");
        if (string(value.begin(), value.begin() + pos) == "set") {
            value.erase(0, 4);
            pos = value.find("where");
            string updateds = SA.trim(string(value.begin(), value.begin() + pos));
            value.erase(0, pos);

            vector<vector<string>> data = getall(tablename);


            string condicion = SA.trim(string(value.begin() + value.find("where") + 6, value.end() - 1));
            string condicional;
            if (condicion.find("=") != string::npos) {
                condicional = "=";
            }
            if (condicion.find("<") != string::npos) {
                condicional = "<";
            }
            if (condicion.find(">") != string::npos) {
                condicional = ">";
            }
            vector<string> cond = SA.split(condicion, condicional);

            int colnumcond = getIndexColum(tb.colums, SA.trim(cond[0]));
            vector<int> indexdata = getbycol(tb, data, colnumcond, SA.trim(cond[1]), condicional);

            vector<string> parametros = SA.split(updateds, ",");
            for (string p : parametros) {
                vector<string> datos = SA.split(p, "=");
                int concaten = datos[1].find("concat");
                int columnumber = getIndexColum(tb.colums, SA.trim(datos[0]));
                for (int i = 0; i < indexdata.size(); i++) {
                    if (tb.colums[columnumber].tamanio != "0") {
                        string charval = SA.fixCadsize(datos[1], stoi(tb.colums[columnumber].tamanio));
                        data[indexdata[i]][columnumber] = charval;
                    }
                    else {
                        data[indexdata[i]][columnumber] = SA.trim(datos[1]);
                    }
                }

            }
            vector<string>finald = SA.toFlatString(data);


            rewriteInfo(finald, tablename);

            print("tabla \"" + tablename + "\" actualizada", verde);
        }
        else {
            print("la tabla \"" + tablename + "\"no se actualizo", rojo);
        }
    }
    else {
        print("tabla \"" + tablename + "\" no existe", rojo);
    }
}

void TableManager::indexTable(string value)
{
    int pos = value.find(" ");
    string indexname = string(value.begin(), value.begin() + pos);
    value.erase(value.begin(), value.begin() + pos + 4);
    pos = value.find(".");
    string tablename = string(value.begin(), value.begin() + pos);
    value.erase(value.begin(), value.begin() + pos + 1);
    pos = value.find(";");
    string atributo = string(value.begin(), value.begin() + pos);
    Tabla* t;
    if (tableexist(tablename, t)) {
        Tabla tb = *t;
        vector<vector<string>>data = getall(tablename);
        int colnumcond = getIndexColum(tb.colums, atributo);
        string datatype = tb.colums[colnumcond].tipo;
        if (!indexexist(indexname)) {
            Indice in(indexname, tb.colums[colnumcond].nombre, datatype, tablename, colnumcond);
            saveIndex(in);
            indices.push_back(in);
            ArbolAVL<string> arbol(in);
            for (int i = 0; i < data.size(); i++) {
                arbol.insert(SA.trim(data[i][colnumcond]), i);
            }
            arbol.save();
            indxTrees.push_back(arbol);
            print("indice \"" + indexname + "\" creado", verde);
        }
        else {
            print("el indice \"" + indexname + "\" ya existe", rojo);
        }
        
    }
}

vector<vector<string>> TableManager::getall(string tablename)
{
    fstream file;
    file.open(tablename + ".txt");
    string line;
    vector<vector<string>> data;
    while (std::getline(file, line)) {
        data.push_back(SA.split(line, ","));
    }
    file.close();
    return data;
}

int TableManager::getIndexColum(vector<Col> colums, string data)
{
    for (int i = 0; i < colums.size(); i++) {
        if (colums[i].nombre == SA.trim(data)) return i;
    }
    return -1;
}

vector<int> TableManager::getbycol(Tabla tb, vector<vector<string>> datos, int i, string val, string condicional)
{
    vector<int> coinciden;
    for (int j = 0; j < datos.size(); j++) {
        if (condicional == "=") {
            string newval = SA.trim(datos[j][i]);
            if (newval == val)  coinciden.push_back(j);
        }
        else if (condicional == "<") {
            string tipo = tb.colums[i].tipo;
            if (tipo == "int") {
                int cond1 = stoi(SA.trim(datos[j][i]));
                int cond2 = stoi(val);
                if (cond1 < cond2)  coinciden.push_back(j);
            }
            else if (tipo == "date") {
                Fecha cond1(SA.split(datos[j][i], "-"));
                Fecha cond2(SA.split(val, "-"));
                if (cond1 < cond2)  coinciden.push_back(j);
            }
            else {
                if (SA.trim(datos[j][i]) < val)  coinciden.push_back(j);
            }
        }
        else if (condicional == ">") {
            string tipo = tb.colums[i].tipo;
            if (tipo == "int") {
                int cond1 = stoi(SA.trim(datos[j][i]));
                int cond2 = stoi(val);
                if (cond1 > cond2)  coinciden.push_back(j);
            }
            else if (tipo == "date") {
                Fecha cond1(SA.split(datos[j][i], "-"));
                Fecha cond2(SA.split(val, "-"));
                if (cond1 > cond2)  coinciden.push_back(j);
            }
            else {
                if (SA.trim(datos[j][i]) > val)  coinciden.push_back(j);
            }
        }
    }
    return coinciden;
}

vector<int> TableManager::findInAVL(string val, int pos)
{
    return indxTrees[pos].buscar(val);
}

void TableManager::print(string msg, int color)
{
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    cout << msg << endl;
    SetConsoleTextAttribute(hConsole, 15);
}