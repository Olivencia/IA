#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;

// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

// Función que devuelve la puntuación del tablero
double Puntuacion(int jugador, const Environment &estado){
    double suma=2;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}


/*********************FUNCIONES USADAS POR EL ALUMNO**************************************/

//Damos mayor prioridad a las 5 columnas que más cerca están del centro y a la que hay justo en el centro el doble.
double PrioridadCentral(const Environment & tablero, int jug){
    double puntos=0;
    for(int i=0; i<7; i++){
        for(int j=1; j<6; j++){
            if(tablero.See_Casilla(i,j) == jug)
                puntos+=50;


            if(j==3 && tablero.See_Casilla(i,j) == jug){
                puntos+=50;
            }
        }
    }
    return puntos;
}

//Tapar las casillas verticales
double TaparVerticales(const Environment & tablero, int jug){
    int bomba=4, opuesto=2, bomba_opuesto=5;
    double puntos=0;
    if(jug==2) {
        bomba=5;
        opuesto=1;
        bomba_opuesto=4;
    }
    for(int i=0; i<6; i++){
        for(int j=0; j<7; j++){
            if(((tablero.See_Casilla(i,j) == opuesto || tablero.See_Casilla(i,j) == bomba_opuesto)
            && (tablero.See_Casilla(i+1,j) == jug || tablero.See_Casilla(i+1,j) == bomba)))
                puntos+=20;

        }
    }
    return puntos;
}

/******************************************************************************************/


// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador,int prof){
    int ganador = estado.RevisarTablero();
    int jugador_opuesto=2;
    if(jugador==2) jugador_opuesto=1;

    if (ganador==jugador)
       return 99999999.0-prof; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0+prof; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 9999999.0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
    {
        double puntuacion_final;
        puntuacion_final = PrioridadCentral(estado,jugador)+TaparVerticales(estado,jugador);
        puntuacion_final -= PrioridadCentral(estado,jugador_opuesto)+TaparVerticales(estado,jugador_opuesto);
        return puntuacion_final;
    }

}


//Algortimo MiniMax
double MiniMax(const Environment & tablero, int jug, int prof, int limite, Environment::ActionType & accion){
    const double INF = 100000000.0, mINF = -100000000.0, gano = 99999999, pierdo = -99999999;
    Environment::ActionType accion_anterior;
    bool opciones[5];
    double mejor, aux;

    if(prof==limite){
        return Valoracion(tablero,jug,prof);
    }
    else{
        int n_act=tablero.possible_actions(opciones);

        if(prof%2==0)
                mejor=mINF;
        else
                mejor=INF;

        int ult_act=-1;
        Environment hijo = tablero.GenerateNextMove(ult_act);

        while(!(hijo==tablero)){
            aux=MiniMax(hijo,jug,prof+1,limite,accion_anterior);

            if(prof%2==0){
                if(aux>mejor){
                    mejor=aux;
                    accion = static_cast <Environment::ActionType > (ult_act);
                }
            }
            else{
                if(aux<mejor){
                    mejor=aux;
                    accion = static_cast <Environment::ActionType > (ult_act);
                }
            }

            hijo = tablero.GenerateNextMove(ult_act);
        }
        return mejor;
    }
}



//Algoritmo poda AlfaBeta
double Poda_AlfaBeta(Environment &tablero, int jugador,int prof,int PROFUNDIDAD_ALFABETA,Environment::ActionType &accion,double alpha,double beta){
    double minMax;
    int act = -1;
    Environment hijo;
    if(tablero.JuegoTerminado() || PROFUNDIDAD_ALFABETA == prof) return Valoracion(tablero, jugador,prof);
    else
    {
        hijo = tablero.GenerateNextMove(act);
        while(act<8){
            if(prof%2==0)
            {
                minMax = Poda_AlfaBeta(hijo,jugador,prof+1,PROFUNDIDAD_ALFABETA,accion,alpha,beta);
                if(minMax>alpha)
                {
                    alpha =  minMax;
                    if (prof==0)
                        accion=static_cast< Environment::ActionType > (act);
                }
            }
            else
            {
                minMax = Poda_AlfaBeta(hijo,jugador,prof+1,PROFUNDIDAD_ALFABETA,accion,alpha,beta);
                if(minMax<beta)
                {
                    beta =  minMax;
                }
            }
            if (alpha>=beta){
                    return alpha;
            }
            hijo = tablero.GenerateNextMove(act);
        }
        if (prof%2==0){
          return alpha;
        }else{
            return beta;
        }
    }
}

// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // accion que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable



    double valor,valor1; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = -9999999999.0, beta = 9999999999.0; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual

    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;

    //Si todas las casillas estan vacias colocamos en el centro
    int cnt=2;
    bool inicio=true;
    while(cnt<6){
        if(actual_.See_Casilla(0,cnt)!=0)
            inicio=false;
        cnt++;
    }
    if(inicio) return Environment::PUT4;
    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa
    valor = Poda_AlfaBeta(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion, alpha, beta);
    cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;


    return accion;
}

