#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>
#include <fstream>
#include <windows.h>
#include "../include/header.h"

using namespace std;

Carrera histCarr[MAX_CARRERAS];
int numCarr = 0;

void gotoxy(int x, int y) {
    COORD c; c.X = x; c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void ocultarCursor() {
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 1;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

int rand1to5() { return (rand() % 5) + 1; }

void registrarJugadores(Jugador jugs[], int &numJugs) {
    cout << "Cuantos jugadores (2-5): ";
    cin >> numJugs;
    while(numJugs < 2 || numJugs > 5) {
        cout << "Error, intente de nuevo: ";
        cin >> numJugs;
    }

    for(int i=0;i<numJugs;i++){
        bool ok=false;
        while(!ok){
            cout << "Nombre jugador " << (i+1) << ": ";
            cin >> jugs[i].nombre;
            ok=true;
            for(char c: jugs[i].nombre){
                if(!((c>='A' && c<='Z') || (c>='a' && c<='z'))){
                    ok=false;
                    cout << "Solo letras. Intente de nuevo.\n";
                    break;
                }
            }
        }
        jugs[i].simbolo = jugs[i].nombre[0];
        jugs[i].victorias = jugs[i].carrerasJugadas = jugs[i].empates = 0;
    }
    cout << "Jugadores registrados.\n";
}

void playRace(Jugador jugs[], int numJugs) {
    const int startX=0, startY=5;
    const int finishX=50;
    int posAct[MAX_JUGADORES]={0};
    system("cls");
    ocultarCursor();

    // Dibujar meta
    for(int y=startY; y<startY+numJugs; y++){
        gotoxy(finishX, y);
        cout << "|";
    }

    // Mostrar jugadores al inicio
    for(int i=0;i<numJugs;i++){
        gotoxy(startX,startY+i);
        cout << jugs[i].simbolo;
    }

    srand(time(0));

    // Guardar fecha y hora de inicio
    time_t t=time(0);
    struct tm *lt = localtime(&t);
    char buf[20];
    strftime(buf,20,"%Y-%m-%d %H:%M:%S",lt);
    histCarr[numCarr].fechaHora = buf;

    histCarr[numCarr].numJugadores = numJugs;
    for(int i=0;i<numJugs;i++) histCarr[numCarr].participantes[i] = jugs[i].nombre;

    bool fin=false;
    while(!fin){
        Sleep(120);
        for(int i=0;i<numJugs;i++){
            if(posAct[i]>=finishX) continue;
            gotoxy(posAct[i], startY+i); cout << " "; // borrar
            posAct[i] += rand1to5();
            if(posAct[i]>=finishX){ posAct[i]=finishX; fin=true; }
            gotoxy(posAct[i], startY+i); cout << jugs[i].simbolo;
        }
    }

    // Determinar ganador y empates
    int maxPos=-1;
    for(int i=0;i<numJugs;i++) if(posAct[i]>maxPos) maxPos=posAct[i];

    int contGan=0, ganador=-1;
    for(int i=0;i<numJugs;i++){
        if(posAct[i]==maxPos){
            contGan++;
            ganador=i;
        }
    }

    if(contGan>1){
        histCarr[numCarr].ganador = "Empate";
        for(int i=0;i<numJugs;i++) if(posAct[i]==maxPos) jugs[i].empates++;
    } else {
        histCarr[numCarr].ganador = jugs[ganador].nombre;
        jugs[ganador].victorias++;
    }

    for(int i=0;i<numJugs;i++) jugs[i].carrerasJugadas++;

    // Guardar posiciones y puntajes
    bool jugadorAsignado[MAX_JUGADORES]={false};
    for(int p=0;p<numJugs;p++){
        int maxP=-1, idx=-1;
        for(int i=0;i<numJugs;i++){
            if(!jugadorAsignado[i] && posAct[i]>maxP){ maxP=posAct[i]; idx=i; }
        }
        histCarr[numCarr].posiciones[p] = p+1;
        histCarr[numCarr].puntajes[p] = (numJugs-p)*2; // puntos descendentes
        jugadorAsignado[idx]=true;
    }

    cout << "\nGanador: " << histCarr[numCarr].ganador << endl;

    numCarr++;
    cout << "\nPresione ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    cin.get();
}

void mostrarEstadisticas(Jugador jugs[], int numJugs){
    if(numJugs==0){ cout << "No hay jugadores registrados.\n"; return;}
    cout << "ESTADISTICAS DE JUGADORES\n";
    cout << "------------------------\n";
    for(int i=0;i<numJugs;i++){
        cout << "Jugador: " << jugs[i].nombre << " (" << jugs[i].simbolo << ")\n";
        cout << "  Carreras jugadas: " << jugs[i].carrerasJugadas << endl;
        cout << "  Victorias: " << jugs[i].victorias << endl;
        cout << "  Empates: " << jugs[i].empates << endl;
        cout << "------------------------\n";
    }
}

void mostrarResumenCarreras(Jugador jugs[]){
    if(numCarr==0){ cout << "No se ha jugado ninguna carrera.\n"; return;}
    for(int c=0;c<numCarr;c++){
        cout << "Carrera " << c+1 << " - " << histCarr[c].fechaHora << ":\n";
        cout << "  Ganador: " << histCarr[c].ganador << endl;
        for(int p=0;p<histCarr[c].numJugadores;p++){
            cout << "  " << histCarr[c].posiciones[p] << "°: " 
                 << histCarr[c].participantes[p] 
                 << " - Puntos: " << histCarr[c].puntajes[p] << endl;
        }
        cout << endl;
    }
}

void guardarHistorialArchivo(){
    ofstream archivo("historialCarreras.txt", ios::app);
    if(!archivo){ cout << "Error al abrir archivo.\n"; return; }

    for(int c=0;c<numCarr;c++){
        archivo << histCarr[c].fechaHora << ",";
        archivo << histCarr[c].ganador << ",";
        for(int j=0;j<histCarr[c].numJugadores;j++){
            archivo << histCarr[c].participantes[j] << "(" << histCarr[c].puntajes[j] << ")";
            if(j<histCarr[c].numJugadores-1) archivo << ";";
        }
        archivo << "\n";
    }
    archivo.close();
}
