#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>
#include <fstream>
#include <windows.h>
#include "../include/header.h"
using namespace std;

// Variables globales
Carrera histCarr[MAX_CARRERAS];
int numCarr = 0;

// Posicionar cursor en consola
void gotoxy(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// Ocultar cursor
void ocultarCursor() {
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 1;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hCon, &cci);
}

// Avance aleatorio
int avanceAleatorio() {
    return (rand() % 3) + 1;
}

// Registrar jugadores
void registrarJugadores(Jugador jugs[], int &numJugs) {
    int cantidad;
    cout << "Cuantos jugadores se desean registrar? (2 a 5): ";
    cin >> cantidad;

    if (cantidad < 2 || cantidad > 5) {
        cout << "Cantidad invalida. Debe ser entre 2 y 5." << endl;
        return;
    }

    for (int i = 0; i < cantidad; i++) {
        string nombre;
        bool ok = false;
        while (!ok) {
            cout << "Nombre jugador " << (i + 1) << ": ";
            cin >> nombre;
            ok = true;
            for (unsigned char c : nombre) {
                if (!isalpha(c)) {
                    ok = false;
                    cout << "Solo letras. Intente de nuevo." << endl;
                    break;
                }
            }
        }

        // Verificar si ya existe
        bool existe = false;
        for (int j = 0; j < numJugs; j++) {
            if (jugs[j].nombre == nombre) {
                cout << "El jugador " << nombre << " ya está registrado, se conservarán sus victorias" << endl;
                existe = true;
                break;
            }
        }

        if (!existe) {
            jugs[numJugs].nombre = nombre;
            jugs[numJugs].simbolo = static_cast<char>(toupper(nombre[0]));
            jugs[numJugs].victorias = 0;
            jugs[numJugs].carrerasJugadas = 0;
            jugs[numJugs].empates = 0;
            numJugs++;
        }
    }

    cout << "Registro completado" << endl;
}

// Dibujar pista
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
        cout << "|";
    }
    cout << endl;
}

// Función para calcular ganador, retorna -1 si hay empate
int calcularGanador(int pos[], int numJugs) {
    int maxPos = pos[0];
    int ganadorIdx = 0;
    bool empate = false;

    for (int i = 1; i < numJugs; i++) {
        if (pos[i] > maxPos) {
            maxPos = pos[i];
            ganadorIdx = i;
            empate = false;
        } else if (pos[i] == maxPos) {
            empate = true;
        }
    }

    if (empate) return -1;
    return ganadorIdx;
}

// Jugar carrera
void playRace(Jugador jugs[], int numJugs) {
    ocultarCursor();
    int pos[MAX_JUGADORES] = {0};
    int pistaLength = 80;

    // Fecha y hora
    time_t t = time(0);
    string fecha = ctime(&t);
    if (!fecha.empty() && fecha.back() == '\n') fecha.pop_back();
    histCarr[numCarr].fechaHora = fecha;
    histCarr[numCarr].numJugadores = numJugs;

    for (int i = 0; i < numJugs; i++)
        histCarr[numCarr].participantes[i] = jugs[i].nombre;

    // Determinar ancho de la pista
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hConsole, &csbi))
        pistaLength = csbi.srWindow.Right - csbi.srWindow.Left - 2;

    // Simulación de la carrera
    bool carreraTerminada = false;
    while (!carreraTerminada) {
        carreraTerminada = true;
        for (int i = 0; i < numJugs; i++) {
            if (pos[i] < pistaLength - 1) {
                pos[i] += avanceAleatorio();
                if (pos[i] >= pistaLength - 1) pos[i] = pistaLength - 1;
                carreraTerminada = false;
            }
        }
        dibujarPista(jugs, numJugs, pos, pistaLength);
        Sleep(80);
    }

    // --- Calcular orden de llegada ---
    bool usados[MAX_JUGADORES] = {false};
    for (int rank = 0; rank < numJugs; rank++) {
        int mejorPos = -1;
        int mejorIdx = -1;
        for (int i = 0; i < numJugs; i++) {
            if (!usados[i] && pos[i] > mejorPos) {
                mejorPos = pos[i];
                mejorIdx = i;
            }
        }
        histCarr[numCarr].posiciones[rank] = mejorIdx;
        histCarr[numCarr].puntajes[rank] = (numJugs - rank) * 2;
        usados[mejorIdx] = true;
    }

    // --- Determinar ganador real ---
    int primerLugarIdx = histCarr[numCarr].posiciones[0];
    int puntosMax = histCarr[numCarr].puntajes[0];
    bool hayEmpate = false;
    for (int i = 1; i < numJugs; i++) {
        if (histCarr[numCarr].puntajes[i] == puntosMax) {
            hayEmpate = true;
            break;
        }
    }

    if (hayEmpate) {
        histCarr[numCarr].ganador = "Empate";
        for (int i = 0; i < numJugs; i++) {
            if (histCarr[numCarr].puntajes[i] == puntosMax)
                jugs[histCarr[numCarr].posiciones[i]].empates++;
        }
        cout << "Ganador: Empate" << endl;
    } else {
        histCarr[numCarr].ganador = jugs[primerLugarIdx].nombre;
        jugs[primerLugarIdx].victorias++;
        cout << "Ganador: " << histCarr[numCarr].ganador << endl;
    }

    // Actualizar carreras jugadas
    for (int i = 0; i < numJugs; i++)
        jugs[i].carrerasJugadas++;

    // Mostrar orden de llegada
    cout << "\n--- Orden de llegada ---" << endl;
    for (int i = 0; i < numJugs; i++) {
        int idx = histCarr[numCarr].posiciones[i];
        cout << (i + 1) << "# " << jugs[idx].nombre
             << " - Puntos: " << histCarr[numCarr].puntajes[i] << endl;
    }
    cout << "------------------------" << endl;

    numCarr++;
    cout << "\nPresione ENTER para continuar..." << endl;
    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    cin.get();
}


// Mostrar estadísticas
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

// Mostrar resumen de carreras
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
            cout << (p + 1) << "#: " << histCarr[c].participantes[idx]
                 << " - Puntos: " << histCarr[c].puntajes[p] << endl;
        }
        cout << endl;
    }
}

// Guardar historial en archivo
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
            archivo << (p + 1) << "#: " << histCarr[c].participantes[idx]
                    << " - Puntos: " << histCarr[c].puntajes[p] << endl;
        }
        archivo << endl;
    }
    archivo.close();
}

// Mostrar orden de llegada
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

// Top 3 jugadores
bool compararVictorias(const Jugador &a, const Jugador &b) {
    return a.victorias > b.victorias;
}

void mostrarTop3(Jugador jugs[], int numJugs) {
    if (numJugs == 0) {
        cout << "No hay jugadores registrados" << endl;
        return;
    }

    cout << "--- Top 3 jugadores ---" << endl;

    Jugador copia[MAX_JUGADORES];
    for (int i = 0; i < numJugs; i++) copia[i] = jugs[i];

    for (int i = 0; i < numJugs - 1; i++) {
        for (int j = i + 1; j < numJugs; j++) {
            if (copia[i].victorias < copia[j].victorias) {
                Jugador tmp = copia[i];
                copia[i] = copia[j];
                copia[j] = tmp;
            }
        }
    }

    int top = (numJugs < 3) ? numJugs : 3;
    for (int i = 0; i < top; i++) {
        cout << i + 1 << "# " << copia[i].nombre
             << " - Victorias: " << copia[i].victorias
             << " | Empates: " << copia[i].empates
             << " | Carreras jugadas: " << copia[i].carrerasJugadas << endl;
    }
    cout << "---------------------------------------------" << endl;
}
