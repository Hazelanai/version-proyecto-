#ifndef HEADER_H
#define HEADER_H

#define MAX_CARRERAS 100
#define MAX_JUGADORES 5
#define PISTA 30

#include <string>
using namespace std;

struct Jugador {
    string nombre;
    char simbolo;
    int victorias;
    int carrerasJugadas;
    int empates;
};

struct Carrera {
    string fechaHora;
    string ganador;
    int numJugadores;
    string participantes[MAX_JUGADORES];
    int posiciones[MAX_JUGADORES];
    int puntajes[MAX_JUGADORES];
};

extern Carrera histCarr[MAX_CARRERAS];
extern int numCarr;

void registrarJugadores(Jugador jugs[], int &numJugs);
void playRace(Jugador jugs[], int numJugs);
void mostrarEstadisticas(Jugador jugs[], int numJugs);
void mostrarResumenCarreras(Jugador jugs[]);
void guardarHistorialArchivo();

#endif
