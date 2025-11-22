#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <string>
using namespace std;

#define MAX_JUGADORES 5
#define MAX_CARRERAS 100
#define PISTA 50

struct Jugador {
    string nombre;
    char simbolo;
    int victorias = 0;
    int empates = 0;
    int carrerasJugadas = 0;
};

struct Carrera {
    string fechaHora;
    string ganador;
    string participantes[MAX_JUGADORES];
    int posiciones[MAX_JUGADORES];
    int puntajes[MAX_JUGADORES];
    int numJugadores = 0;
};

// variables globales 
extern Carrera histCarr[MAX_CARRERAS];
extern int numCarr;

// utilitarias
void gotoxy(int x, int y);
void ocultarCursor();
int avanceAleatorio();

// funciones
void registrarJugadores(Jugador jugs[], int &numJugs);
void playRace(Jugador jugs[], int numJugs);
void dibujarPista(Jugador jugs[], int numJugs, int pos[]);
void mostrarEstadisticas(Jugador jugs[], int numJugs);
void mostrarResumenCarreras();
void mostrarOrdenLlegada();
void guardarHistorialArchivo();

#endif
