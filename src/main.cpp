#include <iostream>
#include <cstdlib>  
#include <ctime>    
#include <algorithm>
#include <limits>
#include "../include/header.h"
using namespace std;


int main() {

    srand((unsigned)time(0)); 

    int opcion;
    Jugador jugadores[MAX_JUGADORES];
    int numJugadores = 0;

    do {
    cout << "|---------------- MENU  ----------------|" << endl;
    cout << "| 1. Registrar jugadores                |" << endl;
    cout << "| 2. Iniciar carrera                    |" << endl;
    cout << "| 3. Mostrar resumen de carreras        |" << endl;  
    cout << "| 4. Mostrar estadisticas               |" << endl;  
    cout << "| 5. Mostrar orden de llegada           |" << endl;
    cout << "| 6. Mostrar top 3 jugadores            |" << endl;
    cout << "| 7. Guardar resultados en archivo      |" << endl;
    cout << "| 8. Salir                              |" << endl;
    cout << "|---------------------------------------|" << endl;
    cin >> opcion;

    switch (opcion) {
        case 1:
            registrarJugadores(jugadores, numJugadores);
            break;

        case 2:
            if (numJugadores < 2) {
                cout << "Error, Se necesitan al menos 2 jugadores" << endl;
            } else {
                cout << "Iniciando carrera..." << endl;
                playRace(jugadores, numJugadores);
            }
            break;

        case 3:  
            mostrarResumenCarreras();
            break;

        case 4: 
            mostrarEstadisticas(jugadores, numJugadores);
            break;

        case 5:
            mostrarOrdenLlegada();
            break;

        case 6:
            mostrarTop3(jugadores, numJugadores);
            break;


        case 7:
            guardarHistorialArchivo();
            cout << "Historial guardado en 'historialCarreras.txt'" << endl;
            break;

        case 8:
            cout << "Saliendo del programa..." << endl;
            break;

        default:
            cout << "Error, opcion invalida" << endl;
    }

    if (opcion != 8) {
        cout << "Presione ENTER para continuar..." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        system("cls");
    }

} while (opcion != 8);

    return 0;
}
