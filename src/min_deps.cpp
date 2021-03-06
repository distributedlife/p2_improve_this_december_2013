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
	class Atlas {
	public:
		virtual bool willFit (const unsigned long textureID) = 0;
		virtual const AtlasedTexture* addTexture(const unsigned long textureID) = 0;
	};
};

class BatchableObject {
public:
	virtual unsigned long getDataFormat() const = 0;
	virtual bool isStatic() const = 0;
	virtual const ShaderObject* getVertexShader() const = 0;
	virtual const ShaderObject* getFragmentShader() const = 0;
	virtual bool hasIndicies() const = 0;
	virtual unsigned int getTextureID(int textureNumber) const = 0;
	unsigned int getPrimaryTextureID() const {
		return getTextureID(0);
	}
};