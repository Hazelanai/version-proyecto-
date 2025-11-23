#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>
#include <fstream>
#include <windows.h>   
#include "../include/header.h"
using namespace std;

// definicion de las variables globales 
Carrera histCarr[MAX_CARRERAS];
int numCarr = 0;

// posiciona cursor 
void gotoxy(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void ocultarCursor() {
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 1;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hCon, &cci);
}

// avance aleatorio 
int avanceAleatorio() {
    return (rand() % 3) + 1;
}

void registrarJugadores(Jugador jugs[], int &numJugs) {
    cout << "Cuantos jugadores se desean registrar? " << MAX_JUGADORES << ": " << endl;
    cin >> numJugs;
    if (numJugs < 2 || numJugs > MAX_JUGADORES) {
        cout << "Cantidad invalida. Debe ser entre 2 y 5 " << MAX_JUGADORES << "." << endl;
        numJugs = 0;
        return;
    }

    for (int i = 0; i < numJugs; i++) {
        bool ok = false;
        while (!ok) {
            cout << "Nombre jugador " << (i + 1) << ": " << endl;
            cin >> jugs[i].nombre;
            ok = true;
            for (unsigned char c : jugs[i].nombre) {
                if (!isalpha(c)) {
                    ok = false;
                    cout << "Solo letras. Intente de nuevo." << endl;
                    break;
                }
            }
        }
        jugs[i].simbolo = static_cast<char>(toupper(static_cast<unsigned char>(jugs[i].nombre[0])));
        jugs[i].victorias = 0;
        jugs[i].carrerasJugadas = 0;
        jugs[i].empates = 0;
    }

    cout << "Jugadores registrados" << endl;
}

void dibujarPista(Jugador jugs[], int numJugs, int pos[], int pistaLength) {
    system("cls");
    for (int i = 0; i < numJugs; i++) {
        gotoxy(0, i + 2);
        for (int c = 0; c < pistaLength; c++) {
            if (c == pos[i])
                cout << jugs[i].simbolo;
            else
                cout << " ";
        }
        cout << "|"; // meta
    }
    cout << endl;
}

void playRace(Jugador jugs[], int numJugs) {
    ocultarCursor();
    srand(static_cast<unsigned>(time(0)));

    // obtener ancho de pantalla para la pista
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int pistaLength = 80;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        pistaLength = csbi.srWindow.Right - csbi.srWindow.Left - 2;
    }

    int pos[MAX_JUGADORES] = {0}; 

    // guardar fecha y hora de la carrera
    time_t t = time(0);
    string fecha = ctime(&t);
    if (!fecha.empty() && fecha.back() == '\n') fecha.pop_back();

    histCarr[numCarr].fechaHora = fecha;
    histCarr[numCarr].numJugadores = numJugs;
    for (int i = 0; i < numJugs; i++)
        histCarr[numCarr].participantes[i] = jugs[i].nombre;

    bool terminado = false;

    while (!terminado) {
        terminado = true;
        for (int i = 0; i < numJugs; i++) {
            if (pos[i] < pistaLength - 1) {
                pos[i] += avanceAleatorio();
                if (pos[i] >= pistaLength - 1) pos[i] = pistaLength - 1;
                terminado = false;
            }
        }

        dibujarPista(jugs, numJugs, pos, pistaLength);
        Sleep(80);
    }

    // guardar posiciones y puntajes
    bool usados[MAX_JUGADORES] = {false};
    for (int rank = 0; rank < numJugs; rank++) {
        int best = -1, bestIdx = -1;
        for (int i = 0; i < numJugs; i++) {
            if (!usados[i] && pos[i] > best) { 
                best = pos[i]; 
                bestIdx = i; 
            }
        }
        if (bestIdx == -1) bestIdx = 0;
        histCarr[numCarr].posiciones[rank] = bestIdx;
        histCarr[numCarr].puntajes[rank] = (numJugs - rank) * 2;
        usados[bestIdx] = true;
    }

    // determinar ganador
    int maxPuntos = 0;
    for (int i = 0; i < numJugs; i++) {
        if (histCarr[numCarr].puntajes[i] > maxPuntos)
            maxPuntos = histCarr[numCarr].puntajes[i];
    }

    int cuentaMax = 0;
    int ganadorIdx = -1;
    for (int i = 0; i < numJugs; i++) {
        if (histCarr[numCarr].puntajes[i] == maxPuntos) {
            cuentaMax++;
            ganadorIdx = histCarr[numCarr].posiciones[i];
        }
    }

    if (cuentaMax > 1) {
        histCarr[numCarr].ganador = "Empate";
        for (int i = 0; i < numJugs; i++) {
            if (histCarr[numCarr].puntajes[i] == maxPuntos)
                jugs[histCarr[numCarr].posiciones[i]].empates++;
        }
        cout << "Ganador: Empate" << endl;
    } else {
        histCarr[numCarr].ganador = jugs[ganadorIdx].nombre;
        jugs[ganadorIdx].victorias++;
        cout << "Ganador: " << histCarr[numCarr].ganador << endl;
    }

    // actualizar carreras jugadas
    for (int i = 0; i < numJugs; i++)
        jugs[i].carrerasJugadas++;

    // Mostrar orden de llegada
    cout << "--- Orden de llegada ---" << endl;
    for (int p = 0; p < numJugs; p++) {
        int idx = histCarr[numCarr].posiciones[p];
        cout << (p + 1) << "°: " << jugs[idx].nombre
             << " - Puntos: " << histCarr[numCarr].puntajes[p] << endl;
    }
    cout << "------------------------" << endl;

    numCarr++;

    cout << "Presione ENTER para continuar..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}


void mostrarEstadisticas(Jugador jugs[], int numJugs) {
    if (numJugs == 0) {
        cout << "No hay jugadores registrados" << endl;
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

void mostrarResumenCarreras() {
    if (numCarr == 0) {
        cout << "No se ha jugado ninguna carrera" << endl;
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
        cout << "Error al abrir archivo" << endl;
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
        cout << "No se ha jugado ninguna carrera" << endl;
        return;
    }
    for (int c = 0; c < numCarr; c++) {
        cout << "Carrera " << c + 1 << " - " << histCarr[c].fechaHora << endl;
        for (int p = 0; p < histCarr[c].numJugadores; p++) {
            int idx = histCarr[c].posiciones[p];
            cout << (p + 1) << "°: " << histCarr[c].participantes[idx]
                 << " - Puntos: " << histCarr[c].puntajes[p] << endl;
        }
        cout << endl;
    }
}
