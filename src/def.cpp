#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <limits>
#include "header.h"
using namespace std;

//Variables globales
Carrera histCarr[MAX_CARRERAS];
int numCarr = 0;

//Funciones auxiliares
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

// Registrar jugadores
void registrarJugadores(Jugador jugs[], int &numJugs) {
    cout << "Cuantos jugadores se desea inscribir (2-5): " << endl; 
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

// Simulación de carrera
void playRace(Jugador jugs[], int numJugs) {
    const int startX = 0, startY = 5;
    int finishX = 80;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        finishX = (startX+25 > csbi.srWindow.Right-7) ? startX+25 : csbi.srWindow.Right-7;

    int posAct[MAX_JUGADORES] = {0};
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

    // Guardar fecha y hora de inicio
    time_t t = time(0); 
    struct tm *lt = localtime(&t); 
    char buf[20];
    strftime(buf, 20, "%Y-%m-%d %H:%M:%S", lt);
    histCarr[numCarr].fechaHora = buf;

    // Guardar participantes y número
    histCarr[numCarr].numJugadores = numJugs;
    for(int i=0;i<numJugs;i++) histCarr[numCarr].participantes[i] = jugs[i].nombre;

    while (!fin) {
        Sleep(120);
        for (int i = 0; i < numJugs; i++) {
            if (startX + posAct[i] >= finishX) continue;
            gotoxy(startX+posAct[i], startY+i); 
            cout << " ";
            posAct[i] += rand1to5();
            if (startX + posAct[i] >= finishX) { 
                posAct[i] = finishX-startX; 
                if(ganador==-1) ganador=i;
            }
            gotoxy(startX+posAct[i], startY+i); 
            cout << jugs[i].simbolo;
        }
        // Revisar si terminó
        fin = true;
        for(int i=0;i<numJugs;i++) if(posAct[i]<finishX-startX) fin=false;
    }

    // Verificar empates
    int maxPos=-1, contGan=0;
    for(int i=0;i<numJugs;i++) if(posAct[i]>maxPos) maxPos=posAct[i];
    for(int i=0;i<numJugs;i++) if(posAct[i]==maxPos) contGan++;

    if(contGan>1){
        histCarr[numCarr].ganador="Empate";
        for(int i=0;i<numJugs;i++) if(posAct[i]==maxPos) jugs[i].empates++;
    } else {
        histCarr[numCarr].ganador=jugs[ganador].nombre;
        jugs[ganador].victorias++;
    }

    // Actualizar carreras jugadas
    for(int i=0;i<numJugs;i++) jugs[i].carrerasJugadas++;

    // Guardar posiciones y puntos
    bool jugAdd[MAX_JUGADORES] = {false};
    for(int pos=0; pos<numJugs; pos++){
        int maxP=-1, jugMax=-1;
        for(int i=0;i<numJugs;i++){
            if(!jugAdd[i] && posAct[i]>maxP){ maxP=posAct[i]; jugMax=i; }
        }
        histCarr[numCarr].posiciones[pos] = jugMax; 
        histCarr[numCarr].puntajes[pos] = (numJugs-pos)*2; 
        jugAdd[jugMax]=true;
    }

    numCarr++;

    gotoxy(0, startY+numJugs+4); 
    cout << "Presione ENTER para continuar..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    cin.get();
}

// Mostrar estadísticas
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

// Mostrar resumen de carreras
void mostrarResumenCarreras(Jugador jugs[]) {
    if (numCarr==0){ 
        cout<<"No se ha jugado ninguna carrera"<< endl; 
        return; 
    }
    for(int c=0;c<numCarr;c++){
        cout<<"Carrera "<<(c+1)<<" - "<<histCarr[c].fechaHora<<":"<< endl;
        cout<<"  Ganador: "<<histCarr[c].ganador<<endl;
        for(int p=0;p<histCarr[c].numJugadores;p++){
            int idx = histCarr[c].posiciones[p];
            cout<<"  "<<p+1<<"°: "<<histCarr[c].participantes[idx]<<" - Puntos: "<<histCarr[c].puntajes[p]<< endl;
        }
        cout<< endl;
    }
}

// Guardar historial en archivo
void guardarHistorialArchivo() {
    ofstream archivo("historialCarreras.txt", ios::out);
    if(!archivo){ cout << "Error al abrir archivo\n"; return; }

    for(int c=0;c<numCarr;c++){
        archivo << histCarr[c].fechaHora << ",";
        archivo << histCarr[c].ganador << ",";
        for(int j=0;j<histCarr[c].numJugadores;j++){
            int idx = histCarr[c].posiciones[j];
            archivo << histCarr[c].participantes[idx] << "(" << histCarr[c].puntajes[j] << ")";
            if(j < histCarr[c].numJugadores-1) archivo << ";";
        }
        archivo << "\n";
    }
    archivo.close();
}
