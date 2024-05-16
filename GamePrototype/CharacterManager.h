#pragma once
#include "Texture.h"
#include <string>
#include <map>


class CharacterManager
{
public:
	~CharacterManager();


	static CharacterManager& GetInstance();

	void LoadCharacterTextures(const std::string& fontPath, int fontSize);

	Texture* GetCharacterTexture(char character);

	CharacterManager(const CharacterManager&) = delete;
	CharacterManager& operator=(const CharacterManager&) = delete;

private:

	CharacterManager();



	std::map<char, Texture*> m_CharacterTextures;


};

