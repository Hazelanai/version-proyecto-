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

//Definicion de las funciones

void registrarJugadores(Jugador jugadores[], int &numJugadores);

int generateRandom1to5();
void gotoxy(int x, int y);
void ocultarCursor();

void playRace(Jugador jugadores[], int players);

#endif