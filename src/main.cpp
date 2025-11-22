#include <iostream>
#include <cstdlib>
#include "header.h"
using namespace std;

int main() {

    int opcion;
    Jugador jugadores[5];
    int numJugadores = 0;

    do {
        cout << "|---------------- MENU  ----------------|" << endl;
        cout << "| 1. Registrar jugadores                |" << endl;
        cout << "| 2. Iniciar carrera                    |" << endl;
        cout << "| 3. Mostrar estadisticas               |" << endl;
        cout << "| 4. Mostrar resumen de cada carrera    |" << endl;
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
                    cout << "Error, deben registrarse 2 o mas jugadores para iniciar la carrera" << endl;
                } else {
                    cout << "Iniciando carrera..." << endl;
                    playRace(jugadores, numJugadores);
                }
                break;

            case 3:
                cout << "Mostrando estadisticas..." << endl;
                break;

            case 4:
                cout << "Mostrando resumen de cada carrera..." << endl;
                break;

            case 5:
                cout << "Mostrando orden de llegada..." << endl;
                break;

            case 6:
                cout << "Mostrando TOP 3 jugadores..." << endl;
                break;

            case 7:
                cout << "Guardando resultados en archivo..." << endl;
                break;

            case 8:
                cout << "Saliendo del programa..." << endl;
                break;

            default:
                cout << "Error. Opcion invalida" << endl;
        }

        if (opcion != 8)  {
            cout << "Presione ENTER para continuar..." << endl;
            cin.ignore();
            cin.get();
            system("cls");
        }

    } while (opcion != 8);

    return 0;
}
