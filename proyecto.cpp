// v1.cpp
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
using namespace std;

struct NodoEcuacion {
    int id;
    string expresion;
    string complejidad;
    NodoEcuacion* sig;
};

class ListaEcuaciones {
private:
    NodoEcuacion* inicio;
public:
    ListaEcuaciones() : inicio(NULL) {}
    void insertarEcuacion(int id, string expresion, string complejidad) {
        NodoEcuacion* nueva = new NodoEcuacion{id, expresion, complejidad, NULL};
        if (inicio == NULL) {
            inicio = nueva;
        } else {
            NodoEcuacion* actual = inicio;
            while (actual->sig != NULL) actual = actual->sig;
            actual->sig = nueva;
        }
        cout << "? Ecuacion agregada: " << expresion << " (" << complejidad << ")\n";
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
        cout << "\n?? Ecuaciones registradas:\n";
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
        cout << "?? Ecuaciones guardadas correctamente.\n";
    }
};

struct NodoCola {
    NodoEcuacion ecuacion;
    NodoCola* sig;
};

class ColaPrioridad {
private:
    NodoCola* frente;
public:
    ColaPrioridad() : frente(NULL) {}
    void encolar(NodoEcuacion eq) {
        NodoCola* nuevo = new NodoCola{eq, NULL};
        if (frente == NULL || eq.complejidad == "Alta") {
            nuevo->sig = frente;
            frente = nuevo;
        } else {
            NodoCola* actual = frente;
            while (actual->sig != NULL && actual->sig->ecuacion.complejidad != "Baja")
                actual = actual->sig;
            nuevo->sig = actual->sig;
            actual->sig = nuevo;
        }
        cout << "?? Encolada ecuacion: " << eq.expresion << endl;
    }
    NodoEcuacion desencolar() {
        if (frente == NULL) {
            cout << "?? No hay ecuaciones en cola.\n";
            return NodoEcuacion{-1, "", "", NULL};
        }
        NodoCola* temp = frente;
        frente = frente->sig;
        NodoEcuacion eq = temp->ecuacion;
        delete temp;
        return eq;
    }
    bool estaVacia() { return frente == NULL; }
    void mostrarCola() {
        NodoCola* actual = frente;
        cout << "\n?? Cola de prioridad:\n";
        while (actual != NULL) {
            cout << actual->ecuacion.expresion << " (" << actual->ecuacion.complejidad << ")\n";
            actual = actual->sig;
        }
    }
};

struct NodoPila {
    string variable;
    NodoPila* sig;
};

class PilaMemoria {
private:
    NodoPila* tope;
public:
    PilaMemoria() : tope(NULL) {}
    void push(string variable) {
        NodoPila* nuevo = new NodoPila{variable, tope};
        tope = nuevo;
        cout << "?? Memoria asignada: " << variable << endl;
    }
    void pop() {
        if (tope != NULL) {
            cout << "?? Liberando memoria: " << tope->variable << endl;
            NodoPila* temp = tope;
            tope = tope->sig;
            delete temp;
        }
    }
    void mostrarPila() {
        NodoPila* actual = tope;
        cout << "\n?? Variables en memoria:\n";
        while (actual != NULL) {
            cout << actual->variable << endl;
            actual = actual->sig;
        }
    }
};

void resolverEcuacion(NodoEcuacion eq, PilaMemoria& pila) {
    cout << "\n?? Resolviendo: " << eq.expresion << endl;
    pila.push("Datos temporales de " + eq.expresion);
    if (eq.complejidad == "Alta") {
        double a, b, c;
        cout << "Ingrese coeficientes a, b, c: ";
        cin >> a >> b >> c;
        double D = pow(b, 2) - 4 * a * c;
        if (D >= 0) {
            double x1 = (-b + sqrt(D)) / (2 * a);
            double x2 = (-b - sqrt(D)) / (2 * a);
            cout << "Raices reales: x1 = " << x1 << ", x2 = " << x2 << endl;
        } else {
            cout << "No existen soluciones reales.\n";
        }
    } else {
        double a, b;
        cout << "Ingrese coeficientes a y b: ";
        cin >> a >> b;
        cout << "Solucion: x = " << -b / a << endl;
    }
    pila.pop();
    cout << "? Ecuacion resuelta.\n";
}

int main() {
    ListaEcuaciones lista;
    ColaPrioridad cola;
    PilaMemoria pila;
    int opcion, id = 1;
    string expresion, complejidad;

    do {
        cout << "\n===== SISTEMA DE GESTION DE PROCESOS MATEMATICOS =====\n";
        cout << "1. Registrar ecuacion\n2. Mostrar ecuaciones\n3. Encolar ecuacion\n";
        cout << "4. Mostrar cola\n5. Resolver ecuacion\n6. Guardar ecuaciones\n0. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            cout << "Ingrese la ecuacion: ";
            cin.ignore();
            getline(cin, expresion);
            cout << "Complejidad (Alta/Baja): ";
            cin >> complejidad;
            lista.insertarEcuacion(id++, expresion, complejidad);
            break;
        case 2: lista.mostrarEcuaciones(); break;
        case 3: {
            int idBuscar; cout << "ID a encolar: "; cin >> idBuscar;
            NodoEcuacion* eq = lista.buscarEcuacion(idBuscar);
            if (eq) cola.encolar(*eq); else cout << "? No encontrada.\n";
            break;
        }
        case 4: cola.mostrarCola(); break;
        case 5:
            if (!cola.estaVacia()) {
                NodoEcuacion eq = cola.desencolar();
                resolverEcuacion(eq, pila);
            } else cout << "?? Cola vacia.\n";
            break;
        case 6: lista.guardarArchivo(); break;
        case 0: cout << "\n?? Fin.\n"; break;
        default: cout << "Opcion invalida.\n";
        }
    } while (opcion != 0);

    return 0;
}