#include "agent.h"
#include "environment.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>

using namespace std;

void Agent::ModifyCoordinates(int &xp, int &yp, int orientacion){

    switch(orientacion){
        case 0:
            xp-=1;
            if(xp<0) xp=9;

        break;

        case 1:
            yp=(yp+1)%10;
        break;

        case 2:
            xp=(xp+1)%10;
        break;

        case 3:
            yp--;
            if(yp<0) yp=9;
        break;
    }

}

void Agent::IncreaseProb_(){
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            if(prob_[i][j]>=0){
                prob_[i][j]+=0.1;
            }
        }
    }

    if(ult_accion_==actEXTRACT){
        prob_[x][y]=0;
    }

}


Agent::ActionType Agent::GetMoveAction_(){
    ActionType accion;
    int xn=x, yn=y;
    int xe=x, ye=y;
    int xs=x, ys=y;
    int xo=x, yo=y;

    //norte
    xn-=1;
    if(xn<0) xn=9;
    //este
    ye=(ye+1)%10;
    //sur
    xs=(xs+1)%10;
    //oeste
    yo--;
    if(yo<0) yo=9;

    double aux;

    switch(orientacion_){
        case 0:
            aux=prob_[xn][yn];
            accion=actFORWARD;
            if(aux<prob_[xe][ye]){
                aux=prob_[xe][ye];
                accion=actTURN_R;
            }
            if(aux<prob_[xo][yo]){
                aux=prob_[xo][yo];
                accion=actTURN_L;
            }

            if(aux == -1) {
                if(rand()%2==0) accion = actTURN_L;
            else accion=actTURN_R;
            }
        break;

        case 1:
            aux=prob_[xe][ye];
            accion=actFORWARD;
            if(aux<prob_[xs][ys]){
                aux=prob_[xs][ys];
                accion=actTURN_R;
            }
            if(aux<prob_[xn][yn]){
                aux=prob_[xn][yn];
                accion=actTURN_L;
            }

            if(aux == -1) {
                if(rand()%2==0) accion = actTURN_L;
            else accion=actTURN_R;
            }

        break;

        case 2:
            aux=prob_[xs][ys];
            accion=actFORWARD;
            if(aux<prob_[xo][yo]){
                aux=prob_[xo][yo];
                accion=actTURN_R;
            }

            if(aux<prob_[xe][ye]){
                aux=prob_[xe][ye];
                accion=actTURN_L;
            }

           if(aux == -1) {
            if(rand()%2==0) accion = actTURN_L;
            else accion=actTURN_R;
            }
        break;

        case 3:
            aux=prob_[xo][yo];
            accion=actFORWARD;
            if(aux<prob_[xn][yn]){
                aux=prob_[xn][yn];
                accion=actTURN_R;
            }

            if(aux<prob_[xs][ys]){
                aux=prob_[xs][ys];
                accion=actTURN_L;
            }

            if(aux == -1) {
                if(rand()%2==0) accion = actTURN_L;
            else accion=actTURN_R;
            }
        break;
    }
    return accion;
}

Agent::ActionType Agent::Exploration_(){

    ActionType accion;
    int xn=x, yn=y;
    int xe=x, ye=y;
    int xs=x, ys=y;
    int xo=x, yo=y;
    int n,s,e,o;
    //norte
    xn-=1;
    if(xn<0) xn=9;
    //este
    ye=(ye+1)%10;
    //sur
    xs=(xs+1)%10;
    //oeste
    yo--;
    if(yo<0) yo=9;
    int aux;
    n=mapa_[xn][yn];
    e=mapa_[xe][ye];
    s=mapa_[xs][ys];
    o=mapa_[xo][yo];

    switch(orientacion_){
        case 0:

            if(n==-1) accion=actFORWARD;
            else if(o==-1) accion=actTURN_L;
            else if(e==-1) accion=actTURN_R;
            else if(e!=-1 && o!=-1 && n!=-1){
                if(n == 1) accion = actFORWARD;
                else if(e==1) accion = actTURN_R;
                else accion = actTURN_L;
            }

        break;

        case 1:
            if(e==-1) accion=actFORWARD;
            else if(n==-1) accion=actTURN_L;
            else if(s==-1) accion=actTURN_R;
            else if(e!=-1 && s!=-1 && n!=-1){
                if(e == 1) accion = actFORWARD;
                else if(s==1) accion = actTURN_R;
                else accion = actTURN_L;
            }

        break;

        case 2:
            if(s==-1) accion=actFORWARD;
            else if(e==-1) accion=actTURN_L;
            else if(o==-1) accion=actTURN_R;
            else if(e!=-1 && o!=-1 && s!=-1){
                if(s == 1) accion = actFORWARD;
                else if(o==1) accion = actTURN_R;
                else accion = actTURN_L;
            }

        break;

        case 3:
            if(o==-1) accion=actFORWARD;
            else if(s==-1) accion=actTURN_L;
            else if(n==-1) accion=actTURN_R;
            else if(s!=-1 && o!=-1 && n!=-1){
                if(o == 1) accion = actFORWARD;
                else if(n==1) accion = actTURN_R;
                else accion = actTURN_L;
            }

        break;
    }
    return accion;

}

// -----------------------------------------------------------
Agent::ActionType Agent::Think()
{

	ActionType accion;
/*****************************************************************************/
/*                           ACTUALIZACIÓN                                   */
/*****************************************************************************/
    switch(ult_accion_){
        case actFORWARD:
            if(bump_){
                int xp=x, yp=y;
                ModifyCoordinates(xp,yp,orientacion_);
                mapa_[xp][yp]=0;
                prob_[xp][yp]=-1;
            }
            else{
                ModifyCoordinates(x,y,orientacion_);
                mapa_[x][y]=1;
            }
        break;
        case actTURN_R:
            orientacion_=(orientacion_+1)%4;
        break;

        case actTURN_L:
            orientacion_--;
            if(orientacion_<0) orientacion_=3;
        break;
        case actSNIFF:
            if(trufa_size_<8) prob_[x][y]=0;
        }

/*****************************************************************************/

        IncreaseProb_();

        switch(contador_){
            case 200: inicio_=false;
            break;

            case 101:
            case 151: if(trufa_size_>=6) inicio_=false;
            break;

            case 300: if(trufas_/contador_ > 0.3) pt_/=2;
            break;

            case 1900: if(pt_==10) pt_/=2;
            break;
        }

/*****************************************************************************/
/*                           DECISIÓN                                        */
/*****************************************************************************/

        if(contador_==100 || contador_==150) accion=actSNIFF;

        else if(inicio_)
            accion=Exploration_();

       else if(prob_[x][y]>=pt_ && contador_ >= 1900) accion=actEXTRACT;

        else if(trufa_size_>=8){
            accion=actEXTRACT;
            trufas_+=trufa_size_;
        }

        else if(prob_[x][y]>=pt_ && trufa_size_==-1){
            accion=actSNIFF;
        }

        else{
            accion=GetMoveAction_();
        }
/*****************************************************************************/

        ult_accion_=accion;
        contador_++;

	return accion;
}
// -----------------------------------------------------------
void Agent::Perceive(const Environment &env)
{
	trufa_size_ = env.TrufaAmount();
	bump_ = env.isJustBump();
}
// -----------------------------------------------------------
string ActionStr(Agent::ActionType accion)
{
	switch (accion)
	{
	case Agent::actFORWARD: return "FORWARD";
	case Agent::actTURN_L: return "TURN LEFT";
	case Agent::actTURN_R: return "TURN RIGHT";
	case Agent::actSNIFF: return "SNIFF";
	case Agent::actEXTRACT: return "EXTRACT";
	case Agent::actIDLE: return "IDLE";
	default: return "???";
	}
}
