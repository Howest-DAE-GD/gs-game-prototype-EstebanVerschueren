#include "pch.h"
#include "CharacterManager.h"

CharacterManager& CharacterManager::GetInstance()
{
	static CharacterManager instance;
	return instance;
}

CharacterManager::CharacterManager(){}

CharacterManager::~CharacterManager()
{
	for (std::pair<const char, Texture*>& pair : m_CharacterTextures)
	{
		delete pair.second;
	}
}


void CharacterManager::LoadCharacterTextures(const std::string& fontPath, int fontSize)
{
	int firstASCIIChar = 32;
	int lastASCIIChar = 126;

	for (char charIndex = firstASCIIChar; charIndex <= lastASCIIChar; ++charIndex)
	{
		std::string charStr(1, charIndex);
		m_CharacterTextures[charIndex] = new Texture(charStr, fontPath, fontSize, Color4f(1, 1, 1, 1));
	}
}


Texture* CharacterManager::GetCharacterTexture(char character)
{
	if (m_CharacterTextures.find(character) != m_CharacterTextures.end())
	{
		return m_CharacterTextures[character];
	}
	return nullptr;
}