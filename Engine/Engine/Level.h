#ifndef LEVEL_H
#define LEVEL_H
#include "modelclass.h"
#include "Bot.h"
#include "Player.h"
#include "D3D10Shaders.h"
#include <random>
#include "globals.h"
#include "ModelLoader.h"

class Level
{
public:
	Level(void);
	~Level(void);
	Player* player;
	Player* player2;
	Player* player3;
	Player* player4;
	bool Initialize(ModelLoader*);
	unsigned int playerCount;
	unordered_map<unsigned int,Bot*> Bots;
	unsigned int botCount;
	void UpdatePlayers(unsigned int fps);
	void UpdateBots(unsigned int fps);
	unsigned int GetRand(unsigned int,unsigned int);
	std::default_random_engine gen;
	const char* player_name;
	Ray down;
	Model target;
	Model* model;
	bool IsMultiplayer();
private:
	void UpdatePlayersS(unsigned int fps);
	bool multiplayer;
};

#endif