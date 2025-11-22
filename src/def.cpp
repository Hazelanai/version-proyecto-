#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>
#include <fstream>
#include <windows.h>
#include "../include/header.h"
using namespace std;


Carrera histCarr[MAX_CARRERAS];
int numCarr = 0;

// posiciona cursor
void gotoxy(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// oculta cursor
void ocultarCursor() {
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 1;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hCon, &cci);
}

// avance aleatorio 1..3
int avanceAleatorio() {
    return (rand() % 3) + 1;
}

// registrar jugadores
void registrarJugadores(Jugador jugs[], int &numJugs) {
    cout << "Cuantos jugadores se desean registrar?" << endl;
    cin >> numJugs;
    if (numJugs < 2 || numJugs > 5) {
        cout << "Cantidad invalida. Debe ser entre 2 y 5." << endl;
        numJugs = 0;
        return;
    }

    for (int i = 0; i < numJugs; i++) {
        bool ok = false;
        while (!ok) {
            cout << "Nombre jugador " << (i + 1) << ":" << endl;
            cin >> jugs[i].nombre;
            ok = true;
            for (char c : jugs[i].nombre) {
                if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) {
                    ok = false;
                    cout << "Solo letras. Intente de nuevo." << endl;
                    break;
                }
            }
        }
        jugs[i].simbolo = toupper(jugs[i].nombre[0]);
        jugs[i].victorias = 0;
        jugs[i].carrerasJugadas = 0;
        jugs[i].empates = 0;
    }

    cout << "Jugadores registrados" << endl;
}

// imprime la pista (usa gotoxy para dibujar)
void dibujarPista(Jugador jugs[], int numJugs, int pos[]) {
    int startX = 2;
    int startY = 2;

    // limpiar pantalla
    system("cls");

    // dibujar cada fila de la pista
    for (int i = 0; i < numJugs; i++) {
        // imprimir símbolo del jugador al inicio
        gotoxy(0, startY + i);
        cout << jugs[i].simbolo << " " << endl;

        // imprimir pista con puntos y posición del jugador
        cout << "|";
        for (int c = 0; c < PISTA; c++) {
            if (c == pos[i])
                cout << jugs[i].simbolo;
            else
                cout << "." << endl;
        }
        cout << "|" << endl;
    }

    // dejar línea en blanco después de pistas
    cout << endl;
}

// playRace con gotoxy y movimiento visible
void playRace(Jugador jugs[], int numJugs) {
    ocultarCursor();
    srand((unsigned)time(0));

    int pos[MAX_JUGADORES] = {0};
    for (int i = 0; i < MAX_JUGADORES; i++) pos[i] = 0;

    // guardar fecha/hora y participantes en historial
    time_t t = time(0);
    string fecha = ctime(&t);
    if (!fecha.empty() && fecha.back() == '\n') fecha.pop_back();
    histCarr[numCarr].fechaHora = fecha;
    histCarr[numCarr].numJugadores = numJugs;
    for (int i = 0; i < numJugs; i++)
        histCarr[numCarr].participantes[i] = jugs[i].nombre;

    bool terminado = false;
    int primerCruceIdx = -1;

    // bucle de carrera
    while (!terminado) {
        // avanzar cada jugador
        for (int i = 0; i < numJugs; i++) {
            if (pos[i] >= PISTA - 1) continue;
            pos[i] += avanceAleatorio();
            if (pos[i] > PISTA - 1) pos[i] = PISTA - 1;
            if (pos[i] == PISTA - 1 && primerCruceIdx == -1) primerCruceIdx = i;
        }

        // dibujar pista actualizada
        dibujarPista(jugs, numJugs, pos);

        // comprobar si todos llegaron o al menos uno
        terminado = true;
        for (int i = 0; i < numJugs; i++) {
            if (pos[i] < PISTA - 1) {
                terminado = false;
                break;
            }
        }

        Sleep(150);
    }

    // determinar ganador/empate
    int mejor = 0;
    for (int i = 1; i < numJugs; i++) {
        if (pos[i] > pos[mejor]) mejor = i;
    }

    int cuentaMax = 0;
    for (int i = 0; i < numJugs; i++) if (pos[i] == pos[mejor]) cuentaMax++;

    if (cuentaMax > 1) {
        histCarr[numCarr].ganador = "Empate";
        for (int i = 0; i < numJugs; i++) if (pos[i] == pos[mejor]) jugs[i].empates++;
        cout << "Ganador: Empate" << endl;
    } else {
        histCarr[numCarr].ganador = jugs[mejor].nombre;
        jugs[mejor].victorias++;
        cout << "Ganador: " << histCarr[numCarr].ganador << endl;
    }

    // actualizar carreras jugadas
    for (int i = 0; i < numJugs; i++) jugs[i].carrerasJugadas++;

    // guardar posiciones como índices y puntajes (puntaje ejemplo = posición inversa)
    bool usados[MAX_JUGADORES] = {false};
    for (int rank = 0; rank < numJugs; rank++) {
        int best = -1;
        int bestIdx = -1;
        for (int i = 0; i < numJugs; i++) {
            if (!usados[i] && pos[i] > best) {
                best = pos[i];
                bestIdx = i;
            }
        }
        if (bestIdx == -1) bestIdx = 0;
        histCarr[numCarr].posiciones[rank] = bestIdx; // índice del jugador
        histCarr[numCarr].puntajes[rank] = (numJugs - rank) * 2;
        usados[bestIdx] = true;
    }

    numCarr++;

    cout << "Presione ENTER para continuar..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// mostrar estadisticas
void mostrarEstadisticas(Jugador jugs[], int numJugs) {
    if (numJugs == 0) {
        cout << "No hay jugadores registrados." << endl;
        return;
    }
    for (int i = 0; i < numJugs; i++) {
        cout << "Jugador: " << jugs[i].nombre << endl;
        cout << "  Victorias: " << jugs[i].victorias << endl;
        cout << "  Empates: " << jugs[i].empates << endl;
        cout << "  Carreras jugadas: " << jugs[i].carrerasJugadas << endl;
        cout << "--------------------------" << endl;
    }
}

// mostrar resumen de carreras
void mostrarResumenCarreras(Jugador jugs[]) {
    if (numCarr == 0) {
        cout << "No se ha jugado ninguna carrera." << endl;
        return;
    }
    for (int c = 0; c < numCarr; c++) {
        cout << "Carrera " << c + 1 << " - " << histCarr[c].fechaHora << endl;
        cout << "Ganador: " << histCarr[c].ganador << endl;
        for (int p = 0; p < histCarr[c].numJugadores; p++) {
            int idx = histCarr[c].posiciones[p];
            cout << (p + 1) << "°: " << histCarr[c].participantes[idx]
                 << " - Puntos: " << histCarr[c].puntajes[p] << endl;
        }
        cout << endl;
    }
}

// guardar historial en archivo
void guardarHistorialArchivo() {
    ofstream archivo("historialCarreras.txt", ios::app);
    if (!archivo) {
        cout << "Error al abrir archivo." << endl;
        return;
    }
    for (int c = 0; c < numCarr; c++) {
        archivo << "Carrera " << c + 1 << endl;
        archivo << "Fecha: " << histCarr[c].fechaHora << endl;
        archivo << "Ganador: " << histCarr[c].ganador << endl;
        for (int p = 0; p < histCarr[c].numJugadores; p++) {
            int idx = histCarr[c].posiciones[p];
            archivo << (p + 1) << "°: " << histCarr[c].participantes[idx]
                    << " - Puntos: " << histCarr[c].puntajes[p] << endl;
        }
        archivo << endl;
    }
    archivo.close();
}

void mostrarOrdenLlegada() {
    if (numCarr == 0) {
        cout << "No se ha jugado ninguna carrera." << endl;
        return;
    }

    for (int c = 0; c < numCarr; c++) {
        cout << "Carrera " << c + 1 << " - " << histCarr[c].fechaHora << endl;

        for (int p = 0; p < histCarr[c].numJugadores; p++) {
            int idx = histCarr[c].posiciones[p];
            cout << (p + 1) << "°: " 
                 << histCarr[c].participantes[idx]
                 << " - Puntos: " << histCarr[c].puntajes[p] 
                 << endl;
        }

        cout << endl;
    }
}
