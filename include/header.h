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
    int empates;
    int posicion;
};

struct Carrera {
    string fechaHora;        
    string ganador;         
    int numJugadores;
    string participantes[MAX_JUGADORES]; 
    int posiciones[MAX_JUGADORES];     
    int puntajes[MAX_JUGADORES];        
};

// Variables globales 
extern Carrera histCarr[MAX_CARRERAS];
extern int numCarr;

// Funciones
void registrarJugadores(Jugador jugs[], int &numJugs);
void playRace(Jugador jugs[], int numJugs);
void mostrarEstadisticas(Jugador jugs[], int numJugs);
void mostrarResumenCarreras(Jugador jugs[]);
void guardarHistorialArchivo();

#endif
