#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>      
#include "header.h"
using namespace std;

Carrera histCarr[MAX_CARRERAS];
int numCarr = 0;

void gotoxy(int x, int y) {
    COORD c; c.X = x; c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c); 
}

void ocultarCursor() {
    CONSOLE_CURSOR_INFO cci; 
    cci.dwSize = 1; cci.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

int rand1to5() { return (rand() % 5) + 1; }

void registrarJugadores(Jugador jugs[], int &numJugs) {
    cout << "Cuantos jugadores (2-5): " << endl; 
    cin >> numJugs;
    while (numJugs < 2 || numJugs > 5) { 
        cout << "Error, intente de nuevo " << endl; 
        cin >> numJugs; 
    }

    for (int i = 0; i < numJugs; i++) {
        bool ok = false;
        while (!ok) {
            cout << "Nombre jugador " << (i+1) << ": " << endl; 
            cin >> jugs[i].nombre;
            ok = true;
            for (char c : jugs[i].nombre) {
                if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) { 
                    ok = false; 
                    cout << "Solo letras. Intente de nuevo" << endl; 
                    break; 
                }
            }
        }
        jugs[i].simbolo = jugs[i].nombre[0];
        jugs[i].victorias = jugs[i].carrerasJugadas = jugs[i].posicion = jugs[i].empates = 0;
    }
    cout << "Jugadores registrados" << endl;
}

void playRace(Jugador jugs[], int numJugs) {
    const int startX = 0, startY = 5;
    int finishX = 80;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        finishX = (startX+25 > csbi.srWindow.Right-7) ? startX+25 : csbi.srWindow.Right-7;

    int posAct[5] = {0};
    system("cls"); 
    ocultarCursor();

    for (int y = startY-1; y <= startY+numJugs; y++) { 
        gotoxy(finishX, y); 
        cout << "||"; 
    }
    for (int i = 0; i < numJugs; i++) { 
        gotoxy(startX, startY+i); 
        cout << jugs[i].simbolo; 
    }

    srand(time(0));
    bool fin = false;
    int ganador = -1;

    while (!fin) {
        Sleep(120);
        for (int i = 0; i < numJugs; i++) {
            if (startX + posAct[i] >= finishX) continue;
            gotoxy(startX+posAct[i], startY+i); 
            cout << " ";
            posAct[i] += rand1to5();
            if (startX + posAct[i] >= finishX) { 
                posAct[i] = finishX-startX; 
                if (ganador==-1){ ganador=i; fin=true; } 
            }
            gotoxy(startX+posAct[i], startY+i); 
            cout << jugs[i].simbolo;
        }
    }

    gotoxy(0, startY+numJugs+2);
    cout << "Ganador: " << jugs[ganador].nombre << " (" << jugs[ganador].simbolo << ")" << endl;

    jugs[ganador].victorias++;
    for (int i = 0; i < numJugs; i++) jugs[i].carrerasJugadas++;

    // Empates
    int maxPos = -1;
    for (int i = 0; i < numJugs; i++) if (posAct[i] >= maxPos) maxPos = posAct[i];
    for (int i = 0; i < numJugs; i++) if (posAct[i] >= maxPos) jugs[i].empates++;

    // Guardar historial
    if (numCarr < MAX_CARRERAS) {
        histCarr[numCarr].numJugadores = numJugs;
        bool jugAdd[5] = {false};
        int pos = 0;

        while (pos < numJugs) {
            int maxP = -1, jugMax = -1;
            for (int i = 0; i < numJugs; i++) if (!jugAdd[i] && posAct[i] > maxP) { maxP=posAct[i]; jugMax=i; }
            histCarr[numCarr].posiciones[pos] = jugMax;
            jugAdd[jugMax] = true;

            time_t t = time(0); 
            struct tm *lt = localtime(&t); 
            char buf[9];
            strftime(buf, 9, "%H:%M:%S", lt);
            histCarr[numCarr].horaLlegada[pos] = buf;

            pos++;
        }
        numCarr++;
    }

    gotoxy(0, startY+numJugs+4); 
    cout << "Presione ENTER para continuar..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    cin.get();
}

void mostrarEstadisticas(Jugador jugs[], int numJugs) {
    if (numJugs==0){ 
        cout<<"No hay jugadores registrados"<< endl; 
        return; 
    }
    cout<<"ESTADISTICAS DE JUGADORES"<< endl;
    cout<<"---------------------------"<< endl;
    for(int i=0;i<numJugs;i++){
        cout<<"Jugador: "<<jugs[i].nombre<<" ("<<jugs[i].simbolo<<")"<< endl;
        cout<<"  Carreras jugadas: "<<jugs[i].carrerasJugadas<< endl;
        cout<<"  Victorias: "<<jugs[i].victorias<< endl;
        cout<<"  Empates: "<<jugs[i].empates<< endl;
        cout<<"---------------------------"<< endl;
    }
}

void mostrarResumenCarreras(Jugador jugs[]) {
    if (numCarr==0){ 
        cout<<"No se ha jugado ninguna carrera"<< endl; 
        return; 
    }
    for(int c=0;c<numCarr;c++){
        cout<<"Carrera "<<(c+1)<<":"<< endl;
        for(int p=0;p<histCarr[c].numJugadores;p++){
            int idx = histCarr[c].posiciones[p];
            cout<<"  "<<(p+1)<<"°: "<<jugs[idx].nombre<<" ("<<jugs[idx].simbolo<<") - Hora: "<<histCarr[c].horaLlegada[p]<< endl;
        }
        cout<< endl;
    }
}
