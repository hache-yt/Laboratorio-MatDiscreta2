#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

// Aporte de Jessica: Estructura de nodo y lista enlazada para ecuaciones
struct NodoEcuacion {
    int id;
    string expresion;
    string complejidad;
    NodoEcuacion* sig;
};

class ListaEcuaciones {
private:
    NodoEcuacion* inicio;
    int siguienteId;
public:
    ListaEcuaciones() : inicio(NULL), siguienteId(1) {}
    ~ListaEcuaciones() {
        NodoEcuacion* actual = inicio;
        while (actual != NULL) {
            NodoEcuacion* temp = actual;
            actual = actual->sig;
            delete temp;
        }
    }
    void insertarEcuacion(int id, string expresion, string complejidad) {
        if (complejidad != "Alta" && complejidad != "Baja") {
            cout << "Error: Complejidad debe ser 'Alta' o 'Baja'.\n";
            return;
        }
        NodoEcuacion* nueva = new NodoEcuacion{id, expresion, complejidad, NULL};
        if (inicio == NULL) {
            inicio = nueva;
        } else {
            NodoEcuacion* actual = inicio;
            while (actual->sig != NULL) actual = actual->sig;
            actual->sig = nueva;
        }
        cout << "Ecuacion agregada: " << expresion << " (" << complejidad << ")\n";
    }
    NodoEcuacion* buscarEcuacion(int id) {
        NodoEcuacion* actual = inicio;
        while (actual != NULL) {
            if (actual->id == id) return actual;
            actual = actual->sig;
        }
        return NULL;
    }
    void mostrarEcuaciones() {
        NodoEcuacion* actual = inicio;
        cout << "\nEcuaciones registradas:\n";
        while (actual != NULL) {
            cout << "ID: " << actual->id << " | " << actual->expresion
                 << " | Complejidad: " << actual->complejidad << endl;
            actual = actual->sig;
        }
    }
    void guardarArchivo() {
        ofstream archivo("ecuaciones.txt");
        NodoEcuacion* actual = inicio;
        while (actual != NULL) {
            archivo << actual->id << "," << actual->expresion << "," << actual->complejidad << endl;
            actual = actual->sig;
        }
        archivo.close();
        cout << "Ecuaciones guardadas correctamente.\n";
    }
    void cargarArchivo() {
        ifstream archivo("ecuaciones.txt");
        if (!archivo.is_open()) {
            cout << "No se encontro archivo de ecuaciones. Iniciando vacio.\n";
            return;
        }
        string linea;
        int maxId = 0;
        while (getline(archivo, linea)) {
            stringstream ss(linea);
            string idStr, expr, comp;
            getline(ss, idStr, ',');
            getline(ss, expr, ',');
            getline(ss, comp);
            int id = stoi(idStr);
            insertarEcuacion(id, expr, comp);
            if (id > maxId) maxId = id;
        }
        siguienteId = maxId + 1;
        archivo.close();
        cout << "Ecuaciones cargadas desde archivo.\n";
    }
    int obtenerSiguienteId() { return siguienteId; }
};

// Aporte de Shintaro: Cola de prioridad con gestión correcta de memoria
struct NodoCola {
    int id;
    string expresion;
    string complejidad;
    NodoCola* sig;
};

class ColaPrioridad {
private:
    NodoCola* frente;
public:
    ColaPrioridad() : frente(NULL) {}
    ~ColaPrioridad() {
        while (frente != NULL) {
            NodoCola* temp = frente;
            frente = frente->sig;
            delete temp;
        }
    }
    void encolar(NodoEcuacion eq) {
        NodoCola* nuevo = new NodoCola{eq.id, eq.expresion, eq.complejidad, NULL};
        if (frente == NULL || eq.complejidad == "Alta") {
            nuevo->sig = frente;
            frente = nuevo;
        } else {
            NodoCola* actual = frente;
            while (actual->sig != NULL && actual->sig->complejidad != "Baja")
                actual = actual->sig;
            nuevo->sig = actual->sig;
            actual->sig = nuevo;
        }
        cout << "Encolada ecuacion: " << eq.expresion << endl;
    }
    NodoEcuacion desencolar() {
        if (frente == NULL) {
            cout << "No hay ecuaciones en cola.\n";
            return NodoEcuacion{-1, "", "", NULL};
        }
        NodoCola* temp = frente;
        frente = frente->sig;
        NodoEcuacion eq = {temp->id, temp->expresion, temp->complejidad, NULL};
        delete temp;
        return eq;
    }
    bool estaVacia() { return frente == NULL; }
    void mostrarCola() {
        NodoCola* actual = frente;
        cout << "\nCola de prioridad:\n";
        while (actual != NULL) {
            cout << actual->expresion << " (" << actual->complejidad << ")\n";
            actual = actual->sig;
        }
    }
};

// Aporte de Rey Kino: Pila de memoria y función de resolución con historial
struct NodoPila {
    string variable;
    NodoPila* sig;
};

class PilaMemoria {
private:
    NodoPila* tope;
public:
    PilaMemoria() : tope(NULL) {}
    ~PilaMemoria() {
        while (tope != NULL) {
            NodoPila* temp = tope;
            tope = tope->sig;
            delete temp;
        }
    }
    void push(string variable) {
        NodoPila* nuevo = new NodoPila{variable, tope};
        tope = nuevo;
        cout << "Memoria asignada: " << variable << endl;
    }
    void pop() {
        if (tope != NULL) {
            cout << "Liberando memoria: " << tope->variable << endl;
            NodoPila* temp = tope;
            tope = tope->sig;
            delete temp;
        }
    }
    void mostrarPila() {
        NodoPila* actual = tope;
        cout << "\nVariables en memoria:\n";
        while (actual != NULL) {
            cout << actual->variable << endl;
            actual = actual->sig;
        }
    }
};

void resolverEcuacion(NodoEcuacion eq, PilaMemoria& pila, vector<string>& historial) {
    cout << "\nResolviendo: " << eq.expresion << endl;
    pila.push("Datos temporales de " + eq.expresion);
    string resultado;
    if (eq.complejidad == "Alta") {
        double a, b, c;
        cout << "Ingrese coeficientes a, b, c: ";
        cin >> a >> b >> c;
        double D = pow(b, 2) - 4 * a * c;
        if (D >= 0) {
            double x1 = (-b + sqrt(D)) / (2 * a);
            double x2 = (-b - sqrt(D)) / (2 * a);
            cout << "Raices reales: x1 = " << x1 << ", x2 = " << x2 << endl;
            resultado = "x1 = " + to_string(x1) + ", x2 = " + to_string(x2);
        } else {
            cout << "No existen soluciones reales.\n";
            resultado = "No hay soluciones reales";
        }
    } else {
        double a, b;
        cout << "Ingrese coeficientes a y b: ";
        cin >> a >> b;
        double x = -b / a;
        cout << "Solucion: x = " << x << endl;
        resultado = "x = " + to_string(x);
    }
    pila.pop();
    cout << "Ecuacion resuelta.\n";
    historial.push_back("ID: " + to_string(eq.id) + " | " + eq.expresion + " | Resultado: " + resultado);
}

// Aporte de Jessica, Shintaro y Rey Kino: Programa principal integrado
int main() {
    ListaEcuaciones lista;
    ColaPrioridad cola;
    PilaMemoria pila;
    vector<string> historial;

    lista.cargarArchivo();
    int id = lista.obtenerSiguienteId();
    int opcion;
    string expresion, complejidad;

    do {
        cout << "\n===== SISTEMA DE GESTION DE PROCESOS MATEMATICOS =====\n";
        cout << "1. Registrar ecuacion\n2. Mostrar ecuaciones\n3. Encolar ecuacion\n";
        cout << "4. Mostrar cola\n5. Resolver ecuacion\n6. Guardar ecuaciones\n";
        cout << "7. Mostrar historial\n8. Mostrar pila de memoria\n0. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            cout << "Ingrese la ecuacion: ";
            cin.ignore();
            getline(cin, expresion);
            cout << "Complejidad (Alta/Baja): ";
            cin >> complejidad;
            lista.insertarEcuacion(id, expresion, complejidad);
            id++;
            break;
        case 2: lista.mostrarEcuaciones(); break;
        case 3: {
            int idBuscar;
            cout << "Ingrese el ID de la ecuacion a encolar: ";
            cin >> idBuscar;
            NodoEcuacion* eqPtr = lista.buscarEcuacion(idBuscar);
            if (eqPtr != NULL) cola.encolar(*eqPtr);
            else cout << "Ecuacion no encontrada.\n";
            break;
        }
        case 4: cola.mostrarCola(); break;
        case 5:
            if (!cola.estaVacia()) {
                NodoEcuacion eq = cola.desencolar();
                resolverEcuacion(eq, pila, historial);
            } else cout << "No hay ecuaciones pendientes.\n";
            break;
        case 6: lista.guardarArchivo(); break;
        case 7:
            cout << "\n=== HISTORIAL DE RESOLUCIONES ===\n";
            if (historial.empty()) {
                cout << "No hay resoluciones en el historial.\n";
            } else {
                for (const auto& h : historial) {
                    cout << h << endl;
                }
            }
            break;
        case 8: pila.mostrarPila(); break;
        case 0: cout << "\nFin de la simulacion.\n"; break;
        default: cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);

    return 0;
}