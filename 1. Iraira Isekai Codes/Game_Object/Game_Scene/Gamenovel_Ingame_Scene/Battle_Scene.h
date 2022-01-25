#pragma once

#include "Gamenovel_Ingame_Scene.h"

#define BATTLE_BGM_KEY											"Battle_BGM"
#define BATTLE_BGM_FILENAME										TEXT("Battle_BGM.mp3")

#define BATTLE_BACKGROUND_TEXTURE_KEY							"Battle_Background_Texture_Key"
#define BATTLE_BACKGROUND_TEXTURE_FILENAME						TEXT("Background_BattleStage_01.bmp")
#define BATTLE_BACKGROUND_CLONE_KEY								"Battle_Background_Clone_Key"

#define HP_BARS_ICON_TEXTURE_KEY								"HP_Bars_Icon_Texture_Key"
#define HP_BARS_ICON_TEXTURE_FILENAME							TEXT("HP_Bars_Icons.bmp")
#define HP_BARS_ICON_SIZE										FLOATSIZE(29, 26)
#define HP_BARS_HEART_IMAGE_LEFTTOP								POSITION(0, 0)
#define HP_BARS_SKULL_IMAGE_LEFTTOP								POSITION(HP_BARS_ICON_SIZE.x, 0)
#define HP_BARS_ICON_X_SHIFT									5

#define PLAYER_NAME												"Lowbird_Test"
#define BATTLE_PLAYER_CLONE_KEY									"Battle_Player_Clone_Key"
#define BATTLE_PLAYER_CLONE_INIT_LEFTTOP						POSITION(70, 250)

#define MONSTER_CLONE_KEY										"Monster_Clone_Key"
#define MONSTER_CLONE_INIT_LEFTTOP								POSITION(350, -50)

class Background;
class Player;
class Monster;

class Battle_Scene : public Gamenovel_Ingame_Scene {
	// Variables
	Background*		m_background = NULL;
	Player*			m_player = NULL;
	Monster*		m_monster = NULL;
public:
	// Functions
	Battle_Scene();
	~Battle_Scene();

	bool	Init();
	void	Input(const float _deltatime)		{ Gamenovel_Ingame_Scene::Input(_deltatime); }
	void	Update(const float _deltatime);
	void	Add()								{ /*Gamenovel_Ingame_Scene::Add();*/ }
	void	Late_Update()						{ Gamenovel_Ingame_Scene::Late_Update(); }
	void	Render(const HDC &_hDC);
};