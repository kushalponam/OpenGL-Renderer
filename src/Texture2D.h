#ifndef _TEXTURE2D_H
#define _TEXTURE2D_H

#include "Core.h"
#include "LoadPNG.h"
#include <map>




class Texture2D {

public:
	Texture2D(std::string texName);
	std::vector<unsigned char> GetTextureData() { return image; }

	unsigned int GetTexWidth() { return width; }
	unsigned int GetTexHeight() { return height; }
	std::string GetName() { return name; }
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
	Texture2D* GetTexture(std::string texName);
	GLuint GetTextureBuffer(std::string texName) {return TextureBuffers[lib_id[texName]];}
	int GetTextureID(std::string texName) { return lib_id[texName]; }

private:
	TexLibrary() {};
	static TexLibrary* t_Instance;
	int texNumber = 1;

	GLuint TextureBuffers[10];
	std::map<std::string, Texture2D*> lib_tex;
	std::map<std::string, int> lib_id;
};




#endif // !_TEXTURE2D_H

