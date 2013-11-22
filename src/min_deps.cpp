class ShaderObject {};

class AtlasedTexture {};

class BufferedBatch {
public:
	static const unsigned long	kFormatUsesTextureUnit0 = (1UL << 12);
	static const unsigned long	kFormatUsesTextureUnit1 = (1UL << 13);
	static const unsigned long	kFormatUsesTextureUnit2 = (1UL << 14);
	static const unsigned long	kFormatUsesTextureUnit3 = (1UL << 15);
};

class TextureManager {
public:
	static unsigned int getTextureWidth(unsigned int textureId) { return 0; };
	static unsigned int getTextureHeight(unsigned int textureId) { return 0; };

	class Atlas {
	public:
		bool willFit (const unsigned long textureID) {return false; };
		const AtlasedTexture* addTexture(const unsigned long textureID) { return NULL; };
	};
};

class SupportedTexture {
public:
	SupportedTexture (const unsigned int textureID, unsigned int width, unsigned int height) {};
	unsigned int getTextureID() {return 0;};
	void incrementTextureUsage() {};
};

class BatchableObject {
public:
	unsigned long getDataFormat() const { return 0; };
	bool isStatic() const {return false;};
	const ShaderObject* getVertexShader() const {return NULL;};
	const ShaderObject* getFragmentShader() const {return NULL;};
	unsigned int hasIndicies() const { return false;};
	unsigned int getTextureID(int textureNumber) const {return 0;};
};