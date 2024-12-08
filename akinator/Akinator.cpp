#include <iostream>
#include <string>
#include <memory>
#include <fstream>

struct Nodo {
    std::string dato;
    std::shared_ptr<Nodo> izquierdo;
    std::shared_ptr<Nodo> derecho;
    int altura;

    Nodo(const std::string& d) : dato(d), izquierdo(nullptr), derecho(nullptr), altura(1) {}
};
int altura(std::shared_ptr<Nodo> nodo) {
    return nodo ? nodo->altura : 0;
}
int obtenerBalance(std::shared_ptr<Nodo> nodo) {
    return nodo ? altura(nodo->izquierdo) - altura(nodo->derecho) : 0;
}

// Funci�n para guardar el �rbol en un archivo
void guardarArbol(std::shared_ptr<Nodo> nodo, std::ofstream& archivo) {
    if (!nodo) {
        archivo << "#\n"; // Indicador de nodo nulo
        return;
    }
    archivo << nodo->dato << "\n";
    guardarArbol(nodo->izquierdo, archivo);
    guardarArbol(nodo->derecho, archivo);
}

// Funci�n para cargar el �rbol desde un archivo
std::shared_ptr<Nodo> cargarArbol(std::ifstream& archivo) {
    std::string linea;
    if (!std::getline(archivo, linea) || linea == "#") {
        return nullptr; // Nodo nulo
    }
    auto nodo = std::make_shared<Nodo>(linea);
    nodo->izquierdo = cargarArbol(archivo);
    nodo->derecho = cargarArbol(archivo);
    return nodo;
}

// Funci�n para obtener una respuesta v�lida de s� o no
std::string obtenerRespuestaValida() {
    std::string respuesta;
    while (true) {
        std::cin >> respuesta;
        if (respuesta == "si" || respuesta == "no") {
            return respuesta;
        }
        std::cout << "Respuesta no v�lida. Por favor, escribe 'si' o 'no': ";
    }
}

// Funci�n para hacer preguntas y permitir aprendizaje
void hacerPreguntas(std::shared_ptr<Nodo> nodo) {
    if (!nodo) return;

    // Si es una hoja, verifica si es la respuesta correcta
    if (!nodo->izquierdo && !nodo->derecho) {
        std::cout << "Es " << nodo->dato << "? (si/no): ";
        std::string respuesta = obtenerRespuestaValida();

        if (respuesta == "si") {
            std::cout << "Genial! Adivine.\n";
        }
        else {
            // Aprender nueva informaci�n
            std::cout << "Oh, no adivine. Que era?\n";
            std::cin.ignore(); // Limpia el buffer de entrada
            std::string nuevaRespuesta;
            std::getline(std::cin, nuevaRespuesta);

            std::cout << "Dame una pregunta para diferenciar entre \"" << nodo->dato
                << "\" y \"" << nuevaRespuesta << "\":\n";
            std::string nuevaPregunta;
            std::getline(std::cin, nuevaPregunta);

            std::cout << "Para \"" << nuevaRespuesta << "\"La respuesta a tu pregunta es s� o no? ";
            respuesta = obtenerRespuestaValida();

            // Reestructurar el nodo actual
            auto viejoDato = nodo->dato;
            nodo->dato = nuevaPregunta;
            if (respuesta == "si") {
                nodo->izquierdo = std::make_shared<Nodo>(nuevaRespuesta);
                nodo->derecho = std::make_shared<Nodo>(viejoDato);
            }
            else {
                nodo->izquierdo = std::make_shared<Nodo>(viejoDato);
                nodo->derecho = std::make_shared<Nodo>(nuevaRespuesta);
            }
        }
        return;
    }

    // Preguntar y avanzar por el �rbol
    std::cout << nodo->dato << " (si/no): ";
    std::string respuesta = obtenerRespuestaValida();

    if (respuesta == "si") {
        hacerPreguntas(nodo->izquierdo);
    }
    else {
        hacerPreguntas(nodo->derecho);
    }
}
std::shared_ptr<Nodo> rotacionDerecha(std::shared_ptr<Nodo> y) {
    auto x = y->izquierdo;
    auto T2 = x->derecho;

    x->derecho = y;
    y->izquierdo = T2;

    y->altura = std::max(altura(y->izquierdo), altura(y->derecho)) + 1;
    x->altura = std::max(altura(x->izquierdo), altura(x->derecho)) + 1;

    return x;
}
std::shared_ptr<Nodo> rotacionIzquierda(std::shared_ptr<Nodo> x) {
    auto y = x->derecho;
    auto T2 = y->izquierdo;

    y->izquierdo = x;
    x->derecho = T2;

    x->altura = std::max(altura(x->izquierdo), altura(x->derecho)) + 1;
    y->altura = std::max(altura(y->izquierdo), altura(y->derecho)) + 1;

    return y;
}


// Funci�n para imprimir el �rbol (en preorden)
void imprimirArbol(std::shared_ptr<Nodo> nodo, int nivel = 0) {
    if (!nodo) return;
    for (int i = 0; i < nivel; ++i) std::cout << "  ";
    std::cout << nodo->dato << "\n";
    imprimirArbol(nodo->izquierdo, nivel + 1);
    imprimirArbol(nodo->derecho, nivel + 1);
}
std::shared_ptr<Nodo> rebalancear(std::shared_ptr<Nodo> nodo) {
    int balance = obtenerBalance(nodo);

    // Caso Izquierda-Izquierda
    if (balance > 1 && obtenerBalance(nodo->izquierdo) >= 0) {
        return rotacionDerecha(nodo);
    }

    // Caso Izquierda-Derecha
    if (balance > 1 && obtenerBalance(nodo->izquierdo) < 0) {
        nodo->izquierdo = rotacionIzquierda(nodo->izquierdo);
        return rotacionDerecha(nodo);
    }

    // Caso Derecha-Derecha
    if (balance < -1 && obtenerBalance(nodo->derecho) <= 0) {
        return rotacionIzquierda(nodo);
    }

    // Caso Derecha-Izquierda
    if (balance < -1 && obtenerBalance(nodo->derecho) > 0) {
        nodo->derecho = rotacionDerecha(nodo->derecho);
        return rotacionIzquierda(nodo);
    }

    return nodo;
}
std::shared_ptr<Nodo> insertar(std::shared_ptr<Nodo> nodo, const std::string& dato) {
    if (!nodo) {
        return std::make_shared<Nodo>(dato);
    }

    if (dato < nodo->dato) {
        nodo->izquierdo = insertar(nodo->izquierdo, dato);
    }
    else if (dato > nodo->dato) {
        nodo->derecho = insertar(nodo->derecho, dato);
    }
    else {
        return nodo; // No se permiten duplicados
    }

    nodo->altura = 1 + std::max(altura(nodo->izquierdo), altura(nodo->derecho));

    return rebalancear(nodo);
}

int main() {
    std::shared_ptr<Nodo> arbol;
    std::ifstream archivoEntrada("arbol.txt");

    if (archivoEntrada) {
        arbol = cargarArbol(archivoEntrada);
        archivoEntrada.close();
    }
    else {
        std::cout << "No se encontr� un arbol previo. Creando uno nuevo.\n";
        arbol = std::make_shared<Nodo>("Es un animal?");
        arbol->izquierdo = std::make_shared<Nodo>("un ave");
        arbol->derecho = std::make_shared<Nodo>("una persona");
    }

    int opcion;
    do {
        std::cout << "\nMenu:\n";
        std::cout << "1. Jugar\n";
        std::cout << "2. Imprimir el arbol\n";
        std::cout << "3. Salir\n";
        std::cout << "Elige una opcion: ";

        // Validaci�n de entrada
        if (!(std::cin >> opcion)) {
            std::cin.clear(); // Limpia el estado de error de cin
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora la entrada incorrecta
            std::cout << "Entrada invalida. Por favor, introduce un numero.\n";
            continue; // Vuelve al inicio del bucle
        }

        switch (opcion) {
        case 1:
            hacerPreguntas(arbol);
            break;
        case 2:
            std::cout << "Arbol actual:\n";
            imprimirArbol(arbol);
            break;
        case 3:
            std::cout << "Guardando el arbol y saliendo...\n";
            {
                std::ofstream archivoSalida("arbol.txt");
                guardarArbol(arbol, archivoSalida);
            }
            break;
        default:
            std::cout << "Opcion no valida. Intenta de nuevo.\n";
        }
    } while (opcion != 3);

    return 0;
}
