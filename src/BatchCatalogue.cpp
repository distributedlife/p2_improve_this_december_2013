#include <vector>
#include <algorithm>
#include "min_deps.cpp"

class BatchCatalogue {
public:
	inline BatchCatalogue(const unsigned long format, const bool isStatic, const ShaderObject* vShader, const ShaderObject* fShader, const bool indicies) :
		m_format(format),
		m_static(isStatic),
		m_vShader(vShader),
		m_fShader(fShader),
		m_indicies(indicies)
	{
		m_textureAtlas[0] = NULL;
		m_textureAtlas[1] = NULL;
		m_textureAtlas[2] = NULL;
		m_textureAtlas[3] = NULL;
	};
	bool isMatch (const BatchableObject* object, const bool checkOnly);
	bool isEligible (const BatchableObject* object);
	bool willFit (const BatchableObject* object);
	void addToCatalogue (const BatchableObject* object);

protected:
	const unsigned long m_format;
	const bool m_static;
	const ShaderObject* m_vShader;
	const ShaderObject* m_fShader;
	const bool m_indicies;
	std::vector<unsigned int> m_texturesAlreadyInCatalogue;
	TextureManager::Atlas* m_textureAtlas[4];

	void addToTextureUnit(TextureManager::Atlas* textureUnit, unsigned int textureId);
	bool catalogueContainsTexture(unsigned int textureId);
};

bool BatchCatalogue::isMatch (const BatchableObject* object, const bool checkOnly) {
	if (!isEligible(object)) {
		return false;
	}
	if (checkOnly) {
		return true;
	}
	if (!willFit(object)) {
		return false;
	}

	addToCatalogue(object);
	return true;
}

bool BatchCatalogue::isEligible (const BatchableObject* object) 
{
	if (m_format != object->getDataFormat()) { return false; }
	if (m_static != object->isStatic()) { return false; }
	if (m_vShader != object->getVertexShader()) { return false; } 
	if (m_fShader != object->getFragmentShader()) { return false; }
	if (m_indicies != object->hasIndicies()) { return false; }
	
	return true;
}

bool BatchCatalogue::willFit (const BatchableObject* object)
{
	if (catalogueContainsTexture(object->getPrimaryTextureID()))
	{
		return true;
	}
	if (m_textureAtlas[0]) 
	{
		return m_textureAtlas[0]->willFit(object->getPrimaryTextureID());
	}

	return true;
}

bool BatchCatalogue::catalogueContainsTexture(unsigned int textureId)
{
	return std::find(m_texturesAlreadyInCatalogue.begin(), m_texturesAlreadyInCatalogue.end(), textureId) != m_texturesAlreadyInCatalogue.end();
}

void BatchCatalogue::addToCatalogue (const BatchableObject* object) {
	if (catalogueContainsTexture(object->getPrimaryTextureID()))
	{
		return;
	}

	if (object->getDataFormat() & BufferedBatch::kFormatUsesTextureUnit0) 
	{
		addToTextureUnit(m_textureAtlas[0], object->getTextureID(0));
	}
	if (object->getDataFormat() & BufferedBatch::kFormatUsesTextureUnit1) 
	{
		addToTextureUnit(m_textureAtlas[1], object->getTextureID(1));
	}
	if (object->getDataFormat() & BufferedBatch::kFormatUsesTextureUnit2) 
	{
		addToTextureUnit(m_textureAtlas[2], object->getTextureID(2));
	}
	if (object->getDataFormat() & BufferedBatch::kFormatUsesTextureUnit3) 
	{
		addToTextureUnit(m_textureAtlas[3], object->getTextureID(3));
	}

	m_texturesAlreadyInCatalogue.push_back(object->getPrimaryTextureID());
}

void BatchCatalogue::addToTextureUnit(TextureManager::Atlas* textureUnit, unsigned int textureId)
{
	if (textureUnit) 
	{
		textureUnit->addTexture(textureId);
	}
}