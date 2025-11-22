#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include "header.h"
using namespace std;

void registrarJugadores(Jugador jugadores[], int &numJugadores) {

    cout << "Cuantos jugadores desea registrar? (2 - 5): ";
    cin >> numJugadores;

    while (numJugadores < 2 || numJugadores > 5) {
        cout << "Error. Solo se permiten entre 2 y 5 jugadores. Intente de nuevo: ";
        cin >> numJugadores;
    }

    for (int i = 0; i < numJugadores; i++) {
        cout << "Jugador #" << (i + 1) << endl;

        // Validar nombre
        bool nombreValido = false;
        while (!nombreValido) {
            cout << "Nombre: " << endl;
            cin >> jugadores[i].nombre;

            nombreValido = true;
            for (char letra : jugadores[i].nombre) {
                if (!((letra >= 'A' && letra <= 'Z') || 
                      (letra >= 'a' && letra <= 'z'))) {
                    nombreValido = false;
                    cout << "Error, el nombre solo debe contener letras. Intente de nuevo." << endl;
                    break;
                }
            }
        }

        cout << "Simbolo (1 caracter): " << endl;
        cin >> jugadores[i].simbolo;

        jugadores[i].victorias = 0;
        jugadores[i].carrerasJugadas = 0;
        jugadores[i].posicion = 0;
    }

    cout << "Jugadores registrados correctamente" << endl;
}

int generateRandom1to5() {
    return (rand() % 5) + 1;
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); 
}

void ocultarCursor() {
    HANDLE hCon; 
    CONSOLE_CURSOR_INFO cci; 
    hCon = GetStdHandle(STD_OUTPUT_HANDLE); 
    cci.dwSize = 1; 
    cci.bVisible = FALSE; 
    SetConsoleCursorInfo(hCon, &cci);
}


void playRace(Jugador jugadores[], int players) {

    const int startX = 0;
    const int startY = 5;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int finishX;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        finishX = (startX + 25 > consoleWidth - 10)
                    ? (startX + 25)
                    : (consoleWidth - 7);
    } else {
        finishX = 80;
    }

    int pos[5];
    for (int i = 0; i < players; i++)
        pos[i] = 0;

    system("cls");
    ocultarCursor();

    // Dibujar meta
    for (int y = startY - 1; y <= startY + players; y++) {
        gotoxy(finishX, y);
        cout << "||";
    }

    // Dibujar jugadores al inicio
    for (int i = 0; i < players; i++) {
        gotoxy(startX, startY + i);
        cout << jugadores[i].simbolo;
    }

    srand(time(0));

    bool finished = false;
    int winner = -1;

    while (!finished) {
        Sleep(120);

        for (int i = 0; i < players; i++) {

            if (startX + pos[i] >= finishX) continue;

            gotoxy(startX + pos[i], startY + i);
            cout << " ";

            pos[i] += generateRandom1to5();

            if (startX + pos[i] >= finishX) {
                pos[i] = finishX - startX;

                if (winner == -1) {
                    winner = i;
                    finished = true;
                }
            }

            gotoxy(startX + pos[i], startY + i);
            cout << jugadores[i].simbolo;
        }
    }

    gotoxy(0, startY + players + 2);
    cout << "Ganador: " << jugadores[winner].nombre 
         << " (" << jugadores[winner].simbolo << ")";

    jugadores[winner].victorias++;
    for (int i = 0; i < players; i++)
        jugadores[i].carrerasJugadas++;

    gotoxy(0, startY + players + 4);
    cout << "Presione ENTER para continuar..." << endl;
    cin.ignore();
}