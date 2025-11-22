#ifndef HEADER_H
#define HEADER_H

#include <string>
using namespace std;

#define MAX_CARRERAS 100
#define MAX_JUGADORES 5

struct Jugador {
    string nombre;
    char simbolo;
    int victorias;
    int carrerasJugadas;
    int posicion;
    int empates;
};

struct Carrera {
    int numJugadores;
    int posiciones[MAX_JUGADORES];
    string horaLlegada[MAX_JUGADORES];
};

// Funciones del juego
void gotoxy(int x, int y);
void ocultarCursor();
int rand1to5();

void registrarJugadores(Jugador jugs[], int &numJugs);
void playRace(Jugador jugs[], int numJugs);
void mostrarEstadisticas(Jugador jugs[], int numJugs);
void mostrarResumenCarreras(Jugador jugs[]);

#endif
