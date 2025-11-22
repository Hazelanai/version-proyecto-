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
                mostrarEstadisticas(jugadores, numJugadores);
                break;

            case 4:
                cout << "Mostrando resumen de cada carrera..." << endl;
                mostrarResumenCarreras(jugadores);
                break;

            case 5:
                cout << "Mostrando orden de llegada..." << endl;
                if(numCarr == 0){
                    cout << "No se ha jugado ninguna carrera" << endl;
                } else {
                    for(int c=0; c<numCarr; c++){
                        cout << "Carrera " << c+1 << " - " << histCarr[c].fechaHora << ":" << endl;
                        for(int p=0; p<histCarr[c].numJugadores; p++){
                            int posJugador = histCarr[c].posiciones[p]-1; // índice real
                            cout << "  " << histCarr[c].posiciones[p] << "°: " 
                                 << histCarr[c].participantes[p] 
                                 << " - Puntos: " << histCarr[c].puntajes[p] << endl;
                        }
                        cout << endl;
                    }
                }
                break;

            case 6:
                cout << "Mostrando TOP 3 jugadores..." << endl;
                if(numJugadores > 0){
                    Jugador copia[MAX_JUGADORES];
                    for(int i=0;i<numJugadores;i++) copia[i] = jugadores[i];
                    sort(copia, copia+numJugadores, [](Jugador a, Jugador b){ return a.victorias > b.victorias; });
                    int top = (numJugadores < 3) ? numJugadores : 3;
                    for(int i=0;i<top;i++){
                        cout << i+1 << "° " << copia[i].nombre << " - Victorias: " << copia[i].victorias << endl;
                    }
                }
                break;

            case 7:
                cout << "Guardando resultados en archivo..." << endl;
                guardarHistorialArchivo();
                cout << "Historial guardado en 'historialCarreras.txt'" << endl;
                break;

            case 8:
                cout << "Saliendo del programa..." << endl;
                break;

            default:
                cout << "Error. Opcion invalida" << endl;
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
