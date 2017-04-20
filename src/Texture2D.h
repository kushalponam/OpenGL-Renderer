#ifndef _TEXTURE2D_H
#define _TEXTURE2D_H

#include "Core.h"
#include "LoadPNG.h"
#include <map>




class Texture2D {

public:
	Texture2D(std::string texName , unsigned int i_width, unsigned int i_height);
	std::vector<unsigned char> GetTextureData() { return image; }

	unsigned int GetTexWidth() const { return width; }
	unsigned int GetTexHeight() const { return height; }
	std::string GetName()const { return name; }
private:
	unsigned int width, height;
	std::vector<unsigned char>image;
	std::string name;
};


class TexLibrary {
public:
	static TexLibrary* getInstance() {
		if (t_Instance == nullptr) {
			t_Instance = new TexLibrary();
		}
		return t_Instance;
	}
	~TexLibrary();
	Texture2D* GetTexture(std::string texName, unsigned int i_width, unsigned int i_height);
	GLuint GetTextureBuffer(std::string texName) {return TextureBuffers[lib_id[texName]];}
	int GetTextureID(std::string texName) { return lib_id[texName]; }
	void BindTexture(std::string textureName);
	void UnBind(std::string texName);
private:
	TexLibrary() {};
	static TexLibrary* t_Instance;
	int texNumber = 10;

	GLuint TextureBuffers[80];
	std::map<std::string, Texture2D*> lib_tex;
	std::map<std::string, int> lib_id;
};




#endif // !_TEXTURE2D_H

