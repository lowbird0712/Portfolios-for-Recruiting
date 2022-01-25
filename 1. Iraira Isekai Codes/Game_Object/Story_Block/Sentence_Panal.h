#pragma once

#include "../../Pieces/Pieces/Panal.h"

#define SENTENCE_PANAL_PROTOTYPE_KEY		"Sentence_Panal_Prototype"
#define SENTENCE_PANAL_TEXTURE_KEY			"Sentence_Panal_Texture"
#define SENTENCE_PANAL_TEXTURE_FILENAME		TEXT("Sentence_Panal_Image_Test.bmp")
#define SPEAKER_NAME_START_POS				POSITION(55, 15)
#define SENTENCE_START_POS					POSITION(30, 50)

class Sentence_Panal : public Panal {
	// Variables
	tstring m_speaker_name;
	tstring m_sentence;
public:
	// Functions
	Sentence_Panal();
	Sentence_Panal(const Sentence_Panal &_panal);
	virtual ~Sentence_Panal();

	virtual Sentence_Panal* Clone()								{ return new Sentence_Panal(*this); }
	bool					Clone_Init(const TCHAR * _speaker_name, const TCHAR * const _sentence);

	virtual bool			Init();
	virtual void			Render(const HDC &_hDC);
};

