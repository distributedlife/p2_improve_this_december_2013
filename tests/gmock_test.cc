#include "../src/BatchCatalogue.cpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class BatchCatalogueWithStubTexture : public BatchCatalogue {
public:
	inline BatchCatalogueWithStubTexture(const unsigned long format, const bool isStatic, const ShaderObject* vShader, const ShaderObject* fShader, const bool indicies) : 
		BatchCatalogue(format, isStatic, vShader, fShader, indicies)
	{};

	void addSupportedTexture(unsigned int textureId) {
		m_textures.push_back(textureId);
	}

	std::vector<unsigned int> getTextures() {
		return m_textures;
	}
};

class BatchCatalogueWithAtlas : public BatchCatalogue {
public:
	inline BatchCatalogueWithAtlas(const unsigned long format, const bool isStatic, const ShaderObject* vShader, const ShaderObject* fShader, const bool indicies, 
		TextureManager::Atlas* atlas0, TextureManager::Atlas* atlas1, TextureManager::Atlas* atlas2, TextureManager::Atlas* atlas3) 
	: 
		BatchCatalogue(format, isStatic, vShader, fShader, indicies)
	{
		m_textureUnit[0] = atlas0;
		m_textureUnit[1] = atlas1;
		m_textureUnit[2] = atlas2;
		m_textureUnit[3] = atlas3;
	};
};

class MockBatchableObject : public BatchableObject {
public:
	MOCK_CONST_METHOD0(getDataFormat, unsigned long());
	MOCK_CONST_METHOD0(isStatic, bool());
	MOCK_CONST_METHOD0(getVertexShader, ShaderObject*());
	MOCK_CONST_METHOD0(getFragmentShader, ShaderObject*());
	MOCK_CONST_METHOD0(hasIndicies, bool());
	MOCK_CONST_METHOD1(getTextureID, unsigned int(int textureNumber));
};

class MockAtlas : public TextureManager::Atlas {
public:
	MOCK_METHOD1(willFit, bool(const unsigned long textureId));
	MOCK_METHOD1(addTexture, const AtlasedTexture*(const unsigned long textureId));
};

using ::testing::Return;
using ::testing::ReturnNull;
using ::testing::Mock;

TEST(BatchCatalogue, IsNotAMatchWhenCheckOnlyAndWhenBatchableObjectFormatDoesNotMatchCatalogue) {
	BatchCatalogue catalogue(0, false, NULL, NULL, false);
	
	MockBatchableObject batchableObject;
	EXPECT_CALL(batchableObject, getDataFormat()).WillRepeatedly(Return(1));
	EXPECT_CALL(batchableObject, isStatic()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getVertexShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, getFragmentShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, hasIndicies()).WillRepeatedly(Return(false));
	
	EXPECT_FALSE(catalogue.isMatch(&batchableObject, true));
};

TEST(BatchCatalogue, IsNotAMatchWhenCheckOnlyAndWhenBatchableObjectStaticDoesNotMatchCatalogue) {
	BatchCatalogue catalogue(0, false, NULL, NULL, false);
	
	MockBatchableObject batchableObject;
	EXPECT_CALL(batchableObject, getDataFormat()).WillRepeatedly(Return(0));
	EXPECT_CALL(batchableObject, isStatic()).WillRepeatedly(Return(true));
	EXPECT_CALL(batchableObject, getVertexShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, getFragmentShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, hasIndicies()).WillRepeatedly(Return(false));
	
	EXPECT_FALSE(catalogue.isMatch(&batchableObject, true));
};

TEST(BatchCatalogue, IsNotAMatchWhenCheckOnlyAndWhenBatchableObjectVertexShaderDoesNotMatchCatalogue) {
	BatchCatalogue catalogue(0, false, NULL, NULL, false);
	
	MockBatchableObject batchableObject;
	EXPECT_CALL(batchableObject, getDataFormat()).WillRepeatedly(Return(0));
	EXPECT_CALL(batchableObject, isStatic()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getVertexShader()).WillRepeatedly(Return(new ShaderObject()));
	EXPECT_CALL(batchableObject, getFragmentShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, hasIndicies()).WillRepeatedly(Return(false));
	
	EXPECT_FALSE(catalogue.isMatch(&batchableObject, true));
};

TEST(BatchCatalogue, IsNotAMatchWhenCheckOnlyAndWhenBatchableObjectFragmentShaderDoesNotMatchCatalogue) {
	BatchCatalogue catalogue(0, false, NULL, NULL, false);
	
	MockBatchableObject batchableObject;
	EXPECT_CALL(batchableObject, getDataFormat()).WillRepeatedly(Return(0));
	EXPECT_CALL(batchableObject, isStatic()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getVertexShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, getFragmentShader()).WillRepeatedly(Return(new ShaderObject()));
	EXPECT_CALL(batchableObject, hasIndicies()).WillRepeatedly(Return(false));
	
	EXPECT_FALSE(catalogue.isMatch(&batchableObject, true));
};

TEST(BatchCatalogue, IsNotAMatchWhenCheckOnlyAndWhenBatchableObjectHasIndiciesDoesNotMatchCatalogue) {
	BatchCatalogue catalogue(0, false, NULL, NULL, false);
	
	MockBatchableObject batchableObject;
	EXPECT_CALL(batchableObject, getDataFormat()).WillRepeatedly(Return(0));
	EXPECT_CALL(batchableObject, isStatic()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getVertexShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, getFragmentShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, hasIndicies()).WillRepeatedly(Return(true));
	
	EXPECT_FALSE(catalogue.isMatch(&batchableObject, true));
};

TEST(BatchCatalogue, IsAMatchWhenCheckOnlyAndWhenBatchableObjectMeetsTheCriteria) {
	BatchCatalogue catalogue(0, false, NULL, NULL, false);
	
	MockBatchableObject batchableObject;
	EXPECT_CALL(batchableObject, getDataFormat()).WillRepeatedly(Return(0));
	EXPECT_CALL(batchableObject, isStatic()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getVertexShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, getFragmentShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, hasIndicies()).WillRepeatedly(Return(false));
	
	EXPECT_TRUE(catalogue.isMatch(&batchableObject, true));
};

TEST(BatchCatalogue, IsAMatchWhenNotCheckOnlyAndBatchableObjectMeetsTheCriteriaAndTextureIsInTheCatalogue) {
	BatchCatalogueWithStubTexture catalogue(0, false, NULL, NULL, false);
	catalogue.addSupportedTexture(1);

	MockBatchableObject batchableObject;
	EXPECT_CALL(batchableObject, getDataFormat()).WillRepeatedly(Return(0));
	EXPECT_CALL(batchableObject, isStatic()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getVertexShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, getFragmentShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, hasIndicies()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getTextureID(0)).WillRepeatedly(Return(1));
	
	EXPECT_TRUE(catalogue.isMatch(&batchableObject, false));
}

TEST(BatchCatalogue, IsAMatchWhenNotCheckOnlyAndBatchableObjectMeetsTheCriteriaAndTextureIsAnywhereInTheCatalogue) {
	BatchCatalogueWithStubTexture catalogue(0, false, NULL, NULL, false);
	catalogue.addSupportedTexture(1);
	catalogue.addSupportedTexture(2);
	catalogue.addSupportedTexture(3);

	MockBatchableObject batchableObject;
	EXPECT_CALL(batchableObject, getDataFormat()).WillRepeatedly(Return(0));
	EXPECT_CALL(batchableObject, isStatic()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getVertexShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, getFragmentShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, hasIndicies()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getTextureID(0)).WillRepeatedly(Return(2));
	
	EXPECT_TRUE(catalogue.isMatch(&batchableObject, false));
}

TEST(BatchCatalogue, IsAMatchWhenNotCheckOnlyAndBatchableObjectMeetsTheCriteriaAndTextureIsNotTheCatalogueAndAtlasesAreNotBeingUsed) {
	BatchCatalogue catalogue(0, false, NULL, NULL, false);

	MockBatchableObject batchableObject;
	EXPECT_CALL(batchableObject, getDataFormat()).WillRepeatedly(Return(0));
	EXPECT_CALL(batchableObject, isStatic()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getVertexShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, getFragmentShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, hasIndicies()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getTextureID(0)).WillRepeatedly(Return(2));
	
	EXPECT_TRUE(catalogue.isMatch(&batchableObject, false));
}

TEST(BatchCatalogue, IsAMatch_AddsNewSupportedTexture_WhenNotCheckOnlyAndBatchableObjectMeetsTheCriteriaAndTextureIsNotInTheCatalogueAndAtlasesAreNotBeingUsed) {
	BatchCatalogueWithStubTexture catalogue(0, false, NULL, NULL, false);

	MockBatchableObject batchableObject;
	EXPECT_CALL(batchableObject, getDataFormat()).WillRepeatedly(Return(0));
	EXPECT_CALL(batchableObject, isStatic()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getVertexShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, getFragmentShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, hasIndicies()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getTextureID(0)).WillRepeatedly(Return(2));
	
	EXPECT_TRUE(catalogue.isMatch(&batchableObject, false));
	EXPECT_EQ(catalogue.getTextures().size(), 1);
	EXPECT_EQ(catalogue.getTextures()[0], 2);
}

TEST(BatchCatalogue, IsAMatch_WhenNotCheckOnlyAndBatchableObjectMeetsTheCriteriaAndTextureIsNotInTheCatalogueAndAtlasesAreBeingUsedAndTextureDoesNotFit) {
	MockAtlas atlas;
	EXPECT_CALL(atlas, willFit(2)).WillRepeatedly(Return(false));

	BatchCatalogueWithAtlas catalogue(0, false, NULL, NULL, false, &atlas, NULL, NULL, NULL);

	MockBatchableObject batchableObject;
	EXPECT_CALL(batchableObject, getDataFormat()).WillRepeatedly(Return(0));
	EXPECT_CALL(batchableObject, isStatic()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getVertexShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, getFragmentShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, hasIndicies()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getTextureID(0)).WillRepeatedly(Return(2));
	
	EXPECT_FALSE(catalogue.isMatch(&batchableObject, false));
}

TEST(BatchCatalogue, IsAMatch_WhenNotCheckOnlyAndBatchableObjectMeetsTheCriteriaAndTextureIsNotInTheCatalogueAndAtlasesAreBeingUsed_AndTextureFits) {
	MockAtlas atlas;
	EXPECT_CALL(atlas, willFit(2)).WillRepeatedly(Return(true));
	EXPECT_CALL(atlas, addTexture(2)).WillRepeatedly(ReturnNull());

	unsigned long dataFormat = BufferedBatch::kFormatUsesTextureUnit0;

	BatchCatalogueWithAtlas catalogue(dataFormat, false, NULL, NULL, false, &atlas, NULL, NULL, NULL);

	MockBatchableObject batchableObject;
	EXPECT_CALL(batchableObject, getDataFormat()).WillRepeatedly(Return(dataFormat));
	EXPECT_CALL(batchableObject, isStatic()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getVertexShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, getFragmentShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, hasIndicies()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getTextureID(0)).WillRepeatedly(Return(2));
	
	EXPECT_TRUE(catalogue.isMatch(&batchableObject, false));
}

TEST(BatchCatalogue, IsAMatch_WhenTextureFitsInAtlasAndTextureUnit0IsUsed) {
	MockAtlas atlas;
	EXPECT_CALL(atlas, willFit(2)).WillRepeatedly(Return(true));
	EXPECT_CALL(atlas, addTexture(2)).WillRepeatedly(ReturnNull());

	unsigned long dataFormat = BufferedBatch::kFormatUsesTextureUnit0;

	BatchCatalogueWithAtlas catalogue(dataFormat, false, NULL, NULL, false, &atlas, NULL, NULL, NULL);

	MockBatchableObject batchableObject;
	EXPECT_CALL(batchableObject, getDataFormat()).WillRepeatedly(Return(dataFormat));
	EXPECT_CALL(batchableObject, isStatic()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getVertexShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, getFragmentShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, hasIndicies()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getTextureID(0)).WillRepeatedly(Return(2));
	
	EXPECT_TRUE(catalogue.isMatch(&batchableObject, false));
	Mock::VerifyAndClearExpectations(&atlas);
}

TEST(BatchCatalogue, IsAMatch_WhenTextureFitsInAtlasAndAllTextureUnitsUsedWithDataFormat) {
	MockAtlas atlas0;
	EXPECT_CALL(atlas0, willFit(2)).WillRepeatedly(Return(true));
	EXPECT_CALL(atlas0, addTexture(2)).WillRepeatedly(ReturnNull());

	MockAtlas atlas1;
	EXPECT_CALL(atlas1, addTexture(3)).WillRepeatedly(ReturnNull());
	MockAtlas atlas2;
	EXPECT_CALL(atlas2, addTexture(4)).WillRepeatedly(ReturnNull());
	MockAtlas atlas3;
	EXPECT_CALL(atlas3, addTexture(5)).WillRepeatedly(ReturnNull());

	unsigned long dataFormat = BufferedBatch::kFormatUsesTextureUnit0 + BufferedBatch::kFormatUsesTextureUnit1 + BufferedBatch::kFormatUsesTextureUnit2 + BufferedBatch::kFormatUsesTextureUnit3;

	BatchCatalogueWithAtlas catalogue(dataFormat, false, NULL, NULL, false, &atlas0, &atlas1, &atlas2, &atlas3);

	MockBatchableObject batchableObject;
	EXPECT_CALL(batchableObject, getDataFormat()).WillRepeatedly(Return(dataFormat));
	EXPECT_CALL(batchableObject, isStatic()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getVertexShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, getFragmentShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, hasIndicies()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getTextureID(0)).WillRepeatedly(Return(2));
	EXPECT_CALL(batchableObject, getTextureID(1)).WillRepeatedly(Return(3));
	EXPECT_CALL(batchableObject, getTextureID(2)).WillRepeatedly(Return(4));
	EXPECT_CALL(batchableObject, getTextureID(3)).WillRepeatedly(Return(5));
	
	EXPECT_TRUE(catalogue.isMatch(&batchableObject, false));
	Mock::VerifyAndClearExpectations(&atlas0);
	Mock::VerifyAndClearExpectations(&atlas1);
	Mock::VerifyAndClearExpectations(&atlas2);
	Mock::VerifyAndClearExpectations(&atlas3);
}

TEST(BatchCatalogue, IsAMatch_WhenTextureFitsInAtlasAndAllTextureUnitsUsedWithoutDataFormat) {
	MockAtlas atlas0;
	EXPECT_CALL(atlas0, willFit(2)).WillRepeatedly(Return(true));
	EXPECT_CALL(atlas0, addTexture(2)).Times(0);
	MockAtlas atlas1;
	EXPECT_CALL(atlas1, addTexture(3)).Times(0);
	MockAtlas atlas2;
	EXPECT_CALL(atlas2, addTexture(4)).Times(0);
	MockAtlas atlas3;
	EXPECT_CALL(atlas3, addTexture(5)).Times(0);

	unsigned long dataFormat = 0;

	BatchCatalogueWithAtlas catalogue(dataFormat, false, NULL, NULL, false, &atlas0, &atlas1, &atlas2, &atlas3);

	MockBatchableObject batchableObject;
	EXPECT_CALL(batchableObject, getDataFormat()).WillRepeatedly(Return(dataFormat));
	EXPECT_CALL(batchableObject, isStatic()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getVertexShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, getFragmentShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, hasIndicies()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getTextureID(0)).WillRepeatedly(Return(2));
	EXPECT_CALL(batchableObject, getTextureID(1)).WillRepeatedly(Return(3));
	EXPECT_CALL(batchableObject, getTextureID(2)).WillRepeatedly(Return(4));
	EXPECT_CALL(batchableObject, getTextureID(3)).WillRepeatedly(Return(5));
	
	EXPECT_TRUE(catalogue.isMatch(&batchableObject, false));
	Mock::VerifyAndClearExpectations(&atlas0);
	Mock::VerifyAndClearExpectations(&atlas1);
	Mock::VerifyAndClearExpectations(&atlas2);
	Mock::VerifyAndClearExpectations(&atlas3);
}

TEST(BatchCatalogue, IsAMatch_WhenTextureFitsInAtlasAndAllTextureUnitsUsedWithDataFormatButNullAtlases) {
	MockAtlas atlas0;
	EXPECT_CALL(atlas0, willFit(2)).WillRepeatedly(Return(true));
	EXPECT_CALL(atlas0, addTexture(2));

	unsigned long dataFormat = BufferedBatch::kFormatUsesTextureUnit0 + BufferedBatch::kFormatUsesTextureUnit1 + BufferedBatch::kFormatUsesTextureUnit2 + BufferedBatch::kFormatUsesTextureUnit3;;

	BatchCatalogueWithAtlas catalogue(dataFormat, false, NULL, NULL, false, &atlas0, NULL, NULL, NULL);

	MockBatchableObject batchableObject;
	EXPECT_CALL(batchableObject, getDataFormat()).WillRepeatedly(Return(dataFormat));
	EXPECT_CALL(batchableObject, isStatic()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getVertexShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, getFragmentShader()).WillRepeatedly(ReturnNull());
	EXPECT_CALL(batchableObject, hasIndicies()).WillRepeatedly(Return(false));
	EXPECT_CALL(batchableObject, getTextureID(0)).WillRepeatedly(Return(2));
	
	EXPECT_TRUE(catalogue.isMatch(&batchableObject, false));
	Mock::VerifyAndClearExpectations(&atlas0);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);

  return RUN_ALL_TESTS();
}