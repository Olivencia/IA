#ifndef AGENT__
#define AGENT__

#include <string>
using namespace std;

// -----------------------------------------------------------
//				class Agent
// -----------------------------------------------------------
class Environment;
class Agent
{
public:
	Agent(){
        trufa_size_=-1;
        bump_=false;
        orientacion_=0;
        ult_accion_ = actIDLE;
        x=0; y=0;
        for(int i=0; i<10; i++){
            for(int j=0; j<10; j++){
                mapa_[i][j]=-1;
            }
        }
        mapa_[0][0]=1;

        for(int i=0; i<10; i++){
            for(int j=0; j<10; j++){
                prob_[i][j]=0;
            }
        }
        contador_=0;
        trufas_=0;
        inicio_=true;
        pt_=10;
	}

	enum ActionType
	{
	    actFORWARD,
	    actTURN_L,
	    actTURN_R,
	    actSNIFF,
	    actEXTRACT,
	    actIDLE
	};



	void Perceive(const Environment &env);
	ActionType Think();
	void ModifyCoordinates(int &xp, int &yp, int orientacion);
	void IncreaseProb_();
	ActionType GetMoveAction_();
	ActionType Exploration_();
private:
	int trufa_size_;
	bool bump_;
	int orientacion_;
	ActionType ult_accion_;
	double prob_[10][10];
	int mapa_[10][10];
	int x,y;
	int contador_;
	double trufas_;
	bool inicio_;
	int pt_;
};

string ActionStr(Agent::ActionType);

#endif
