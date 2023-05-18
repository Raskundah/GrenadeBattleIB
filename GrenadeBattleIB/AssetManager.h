#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>

class AssetManager
{
public:

	static sf::Texture& RequestTexture(std::string assetName);
	static sf::Font& RequestFont(std::string assetName);
	static sf::SoundBuffer& RequestSoundBuffer(std::string assetName);

	static void DestroyAllAssets();

private:

	static std::map<std::string, sf::Texture> textures;
	static std::map<std::string, sf::Font> fonts;
	static std::map<std::string, sf::SoundBuffer> soundBuffers;

};

