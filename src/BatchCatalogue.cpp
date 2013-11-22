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
	{};
	const bool isMatch (const BatchableObject* object, const bool checkOnly);

protected:
	const unsigned long m_format;
	const bool m_static;
	const ShaderObject* m_vShader;
	const ShaderObject* m_fShader;
	const bool m_indicies;
	std::vector<SupportedTexture*> m_textures;
	TextureManager::Atlas* m_textureUnit[4];
};

const bool BatchCatalogue::isMatch (const BatchableObject* object, const bool checkOnly) 
{
	if (!object) 
	{
		return false;
	}

	// _debug ("check to see if the geometry and shaders match");
	if (m_format == object->getDataFormat () && m_static == object->isStatic () && 
		m_vShader == object->getVertexShader () && m_fShader == object->getFragmentShader () && 
		m_indicies == object->hasIndicies ())
	{
		if (checkOnly) 
		{
			return true;
		}

		// _debug ("check to see if the texture matches an existing support texture");
		const size_t numTexures = m_textures.size ();
		for (size_t i = 0; i < numTexures; i++) 
		{
			SupportedTexture* catalogueItemTexture = m_textures[i];

			// _debug ("we only check the first texture slot here, this is by design as batchable geometry only uses the fist texture as a distinguishing one");
			if (object->getTextureID (0) == catalogueItemTexture->getTextureID ()) 
			{
				catalogueItemTexture->incrementTextureUsage ();
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

			if (object->getDataFormat () & BufferedBatch::kFormatUsesTextureUnit0) 
			{
				m_textureUnit[0]->addTexture (object->getTextureID (0));
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
		} 

		// _debug ("add our texture to our batch-catalogue support list");
		SupportedTexture* texture = new SupportedTexture (object->getTextureID (0), TextureManager::getTextureWidth (object->getTextureID (0)), TextureManager::getTextureHeight (object->getTextureID (0)));

		if (texture) 
		{
			m_textures.push_back (texture);
		}

		return true;
	}

	// _debug ("object does not meet the batch catalogue requirements");
	return false;
}