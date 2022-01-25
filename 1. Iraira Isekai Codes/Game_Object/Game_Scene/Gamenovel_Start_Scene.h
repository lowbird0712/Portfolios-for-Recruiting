#pragma once

#include "../../Singleton/Scene_Manager/Scene.h"

#define STARTSCENE_BGM_KEY						"Startscene_BGM"
//#define STARTSCENE_BGM_FILENAME					TEXT("NOTYET")

#define STARTSCENE_MAINPANAL_CLONE_KEY			"Startscene_Panal_Clone"
#define STARTSCENE_MAINPANAL_IMAGE_KEY			"Startscene_Panal_Image"
//#define STARTSCENE_MAINPANAL_IMAGE_FILENAME		TEXT("NOTYET")

//#define STARTSCENE_EDIT_BUTTON_CLONE_KEY		"Startscene_Edit_Button_Clone"
//#define STARTSCENE_EDIT_BUTTON_IMAGE_LEFTTOP	POSITION("NOTYET")
//#define STARTSCENE_EDIT_BUTTON_SIZE				FLOATSIZE("NOTYET")
//#define STARTSCENE_EDIT_BUTTON_POS				POSITION("NOTYET")

//#define STARTSCENE_START_BUTTON_CLONE_KEY		"Startscene_Start_Button_Clone"
//#define STARTSCENE_START_BUTTON_IMAGE_LEFTTOP	POSITION(0.f, 0.f)
//#define STARTSCENE_START_BUTTON_SIZE			FLOATSIZE(138.f, 39.f)
//#define STARTSCENE_START_BUTTON_POS				POSITION(100.f, 480.f)

class Panal;
class Rect_Button;

class Gamenovel_Start_Scene : public Scene {
	// Variables
	Panal*			m_mainpanal;
	Rect_Button*	m_edit_button;
	Rect_Button*	m_start_button;
public:
	// Functions
	Gamenovel_Start_Scene();
	virtual ~Gamenovel_Start_Scene();

	void			Start_Editing();
	void			Start_Game();

	virtual bool	Init();
	virtual void	Input(const float _deltatime) { Scene::Input(_deltatime); }
	virtual void	Update(const float _deltatime) { Scene::Update(_deltatime); }
	virtual void	Add() { /*Scene::Add();*/ }
	virtual void	Late_Update() { Scene::Late_Update(); }
	virtual void	Render(const HDC &_hDC) { Scene::Render(_hDC); }
};