#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include "header.h"
using namespace std;

int main() {

    int opcion;
    Jugador jugadores[MAX_JUGADORES];
    int numJugadores = 0;

    do {
        cout << "|---------------- MENU  ----------------|" << endl;
        cout << "| 1. Registrar jugadores                |" << endl;
        cout << "| 2. Iniciar carrera                    |" << endl;
        cout << "| 3. Mostrar estadisticas               |" << endl;
        cout << "| 4. Mostrar resumen de carreras        |" << endl;
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
                    cout << "Error: Se necesitan al menos 2 jugadores." << endl;
                } else {
                    cout << "Iniciando carrera..." << endl;
                    playRace(jugadores, numJugadores);
                }
                break;

            case 3:
                mostrarEstadisticas(jugadores, numJugadores);
                break;

            case 4:
                mostrarResumenCarreras();
                break;

            case 5:
                mostrarOrdenLlegada();
                break;

            case 6:
                if (numJugadores > 0) {
                    cout << "TOP 3 jugadores:" << endl;

                    Jugador copia[MAX_JUGADORES];
                    for (int i = 0; i < numJugadores; i++)
                        copia[i] = jugadores[i];

                    sort(copia, copia + numJugadores, [](Jugador a, Jugador b) {
                        return a.victorias > b.victorias;
                    });

                    int top = (numJugadores < 3) ? numJugadores : 3;

                    for (int i = 0; i < top; i++) {
                        cout << i + 1 << "° " << copia[i].nombre
                             << " - Victorias: " << copia[i].victorias << endl;
                    }

                } else {
                    cout << "No hay jugadores registrados." << endl;
                }
                break;

            case 7:
                guardarHistorialArchivo();
                cout << "Historial guardado en 'historialCarreras.txt'" << endl;
                break;

            case 8:
                cout << "Saliendo del programa..." << endl;
                break;

            default:
                cout << "Error: Opcion invalida." << endl;
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
