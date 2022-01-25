#pragma once

#include "Sentence_Panal.h"
#include "../../Singleton/Core.h"
#include "../../Singleton/Resource_Manager/Texture.h"

Sentence_Panal::Sentence_Panal() : m_speaker_name(""), m_sentence("") { }
Sentence_Panal::Sentence_Panal(const Sentence_Panal & _panal) : Panal(_panal), m_speaker_name(_panal.m_speaker_name), m_sentence(_panal.m_sentence) {
	m_pos = _panal.m_pos;
}
Sentence_Panal::~Sentence_Panal() { }

bool Sentence_Panal::Clone_Init(const TCHAR * _speaker_name, const TCHAR * const _sentence) {
	m_speaker_name = _speaker_name;
	m_sentence = _sentence;

	return TRUE;
}

bool Sentence_Panal::Init() {
	if (!Panal::Init())
		return FALSE;

	if (!Set_Texture(SENTENCE_PANAL_TEXTURE_KEY, SENTENCE_PANAL_TEXTURE_FILENAME))
		return FALSE;

	m_pos = POSITION(0, GET_RESOLUTION().height - m_texture->Get_Frame_Size().y);
	m_size = m_texture->Get_Frame_Size();

	return TRUE;
}

void Sentence_Panal::Render(const HDC & _hDC) {
	Panal::Render(_hDC);

	const TCHAR* speaker_name = m_speaker_name.c_str();
	TextOut(_hDC, m_pos.x + SPEAKER_NAME_START_POS.x, m_pos.y + SPEAKER_NAME_START_POS.y, speaker_name, strlen(speaker_name));

	const TCHAR* sentence = m_sentence.c_str();
	TextOut(_hDC, m_pos.x + SENTENCE_START_POS.x, m_pos.y + SENTENCE_START_POS.y, sentence, strlen(sentence));
}
