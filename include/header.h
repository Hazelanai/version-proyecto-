#ifndef HEADER_H
#define HEADER_H

#include <string>
using namespace std;

struct Jugador {
    string nombre;       
    char simbolo;        
    int victorias;       
    int carrerasJugadas; 
    int posicion;        
};

// Funciones
void registrarJugadores(Jugador jugadores[], int &numJugadores);
void playRace(Jugador jugadores[], int players);
void gotoxy(int x, int y);
void ocultarCursor();
int generateRandom1to5();

#endif
