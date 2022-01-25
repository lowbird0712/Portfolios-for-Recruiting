#pragma once

#include "../../Pieces/Bases/Unmoving.h"
#include "../../Singleton/Path_Manager.h"

#define STORYBLOCK_PROTOTYPE_KEY				"Storyblock_Prototype"
#define STORYBLOCK_BGM_STOP_KEY					"Storyblock_BGM_Stop"
#define STORYBLOCK_NEXTNODE_EFFECT_KEY			"Storyblock_Nextnode_Effect"
#define STORYBLOCK_NEXTNODE_EFFECT_FILENAME		TEXT("standrews.wav")

#define STORYBLOCK_NEXTPAGE_KEY					"Storyblock_Nextpage"

class Scene;
class Panal;
class Sentence_Panal;

struct STORY_NODE {
	Scene*			scene;

	vector<string>	sound_key_vector;
	Panal*			cutscene_panal;
	vector<Panal*>	character_panal_vector;
	Sentence_Panal*	sentence_panal;

	STORY_NODE();

	bool	Add_Sound(const string &_sound_key, const bool _loop, const TCHAR * const _filename, const string &_path_key = SOUND_PATH_KEY);
	bool	Add_Loaded_Sound(const string &_sound_key);
	void	Add_BGM_Stop_Sound();
	bool	Add_Cutscene_Panal(const string &_texture_key, const TCHAR * const _filename,
				const COLORREF &_color = TEXTURE_COLORKEY_UNUSE, const string &_path_key = IMAGE_PATH_KEY);
	bool	Add_Loaded_Cutscene_Panal(const string &_texture_key);
	bool	Add_Character_Panal(const POSITION &_pos, const string &_texture_key, const TCHAR * const _filename,
				const COLORREF &_color = TEXTURE_COLORKEY_UNUSE, const string &_path_key = IMAGE_PATH_KEY);
	bool	Add_Loaded_Character_Panal(const POSITION &_pos, const string &_texture_key);
	bool	Add_Sentence_Panal(const TCHAR * const _speaker_name, const TCHAR * const _sentence);
};

class Story_Block : public Unmoving {
	// Variables
	POSITION			m_sentence_panal_pos;
	vector<STORY_NODE*>	m_story_node_vector;
	size_t				m_current_index;
public:
	// Functions
	Story_Block();
	Story_Block(const Story_Block &_block);
	~Story_Block();

	virtual Story_Block*	Clone()								{ return new Story_Block(*this); }

	bool					Goto_Next_Node();
	STORY_NODE*				Add_Node(const TCHAR * _speaker_name, const TCHAR *_sentence);

	void					Set_Enable(const bool _enable);
	POSITION				Get_Sentence_Panal_Pos() const		{ return m_sentence_panal_pos; }

	virtual bool			Init();
	virtual void			Input(const float _deltatime);
	virtual void			Update(const float _deltatime)		{ Unmoving::Update(_deltatime); }
	virtual void			Late_Update()						{ Unmoving::Late_Update(); }
	virtual void			Render(const HDC &_hDC);
};

