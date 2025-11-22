#ifndef HEADER_H
#define HEADER_H

#include <string>
using namespace std;

#define MAX_JUGADORES 5
#define MAX_CARRERAS 50
#define PISTA 30

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

// variables globales
extern Carrera histCarr[MAX_CARRERAS];
extern int numCarr;

// funciones
void registrarJugadores(Jugador jugadores[], int &numJugadores);
void playRace(Jugador jugadores[], int numJugadores);
void mostrarEstadisticas(Jugador jugadores[], int numJugadores);
void mostrarResumenCarreras();
void mostrarOrdenLlegada();
void guardarHistorialArchivo();
void mostrarOrdenLlegada();


#endif
