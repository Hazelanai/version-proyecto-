#ifndef HEADER_H
#define HEADER_H

#include <string>
using namespace std;

const int MAX_JUGADORES = 5;   // Máximo jugadores por carrera
const int MAX_CARRERAS = 50;   // Historial máximo de carreras

// Estructura para jugadores
struct Jugador {
    string nombre;
    char simbolo;
    int victorias;
    int carrerasJugadas;
    int empates;
};

// Estructura para almacenar la carrera
struct Carrera {
    string fechaHora;
    string ganador;
    int numJugadores;
    string participantes[MAX_JUGADORES];
    int posiciones[MAX_JUGADORES]; // índice del jugador según orden de llegada
    int puntajes[MAX_JUGADORES];   // puntaje según orden de llegada
};

// Funciones de manejo
void gotoxy(int x, int y);
void ocultarCursor();
int avanceAleatorio();

void registrarJugadores(Jugador jugs[], int &numJugs);
void playRace(Jugador jugs[], int numJugs);

void mostrarEstadisticas(Jugador jugs[], int numJugs);
void mostrarResumenCarreras();
void guardarHistorialArchivo();
void mostrarOrdenLlegada();
void mostrarTop3(Jugador jugs[], int numJugs);

#endif
