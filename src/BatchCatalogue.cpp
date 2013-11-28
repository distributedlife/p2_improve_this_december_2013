#include <vector>
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
		m_textureUnit[0] = NULL;
		m_textureUnit[1] = NULL;
		m_textureUnit[2] = NULL;
		m_textureUnit[3] = NULL;
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
	std::vector<unsigned int> m_textures;
	TextureManager::Atlas* m_textureUnit[4];
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
	if (m_format == object->getDataFormat () && m_static == object->isStatic () && 
		m_vShader == object->getVertexShader () && m_fShader == object->getFragmentShader () && 
		m_indicies == object->hasIndicies ())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool BatchCatalogue::willFit (const BatchableObject* object)
{
	// _debug ("check to see if the texture matches an existing support texture");
	const size_t numTexures = m_textures.size ();
	for (size_t i = 0; i < numTexures; i++) 
	{
		// _debug ("we only check the first texture slot here, this is by design as batchable geometry only uses the fist texture as a distinguishing one");
		if (object->getTextureID (0) == m_textures[i]) 
		{
			return true;
		}
	}

	// _debug ("supported texture not found, attempt to add a new texture to the atlas");
	if (m_textureUnit[0]) 
	{
		if (!m_textureUnit[0]->willFit (object->getTextureID (0))) 
		{
			// _debug ("our texture won't fit, return false as the object is no longer a match");
			return false;
		}
	}

	return true;
}

void BatchCatalogue::addToCatalogue (const BatchableObject* object) {
	const size_t numTexures = m_textures.size ();
	for (size_t i = 0; i < numTexures; i++) 
	{
		// _debug ("we only check the first texture slot here, this is by design as batchable geometry only uses the fist texture as a distinguishing one");
		if (object->getTextureID (0) == m_textures[i]) 
		{
			return;
		}
	}

	if (object->getDataFormat () & BufferedBatch::kFormatUsesTextureUnit0) 
	{
		if (m_textureUnit[0]) 
		{
			m_textureUnit[0]->addTexture (object->getTextureID (0));
		}
	}
	if (object->getDataFormat () & BufferedBatch::kFormatUsesTextureUnit1) 
	{
		if (m_textureUnit[1]) 
		{
			m_textureUnit[1]->addTexture (object->getTextureID (1));
		}
	}
	if (object->getDataFormat () & BufferedBatch::kFormatUsesTextureUnit2) 
	{
		if (m_textureUnit[2]) 
		{
			m_textureUnit[2]->addTexture (object->getTextureID (2));
		}
	}
	if (object->getDataFormat () & BufferedBatch::kFormatUsesTextureUnit3) 
	{
		if (m_textureUnit[3]) 
		{
			m_textureUnit[3]->addTexture (object->getTextureID (3));
		}
	}

	m_textures.push_back (object->getTextureID (0));
}