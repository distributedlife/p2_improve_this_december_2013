#include "../src/BatchCatalogue.cpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class BatchCatalogueWithStubTexture : public BatchCatalogue {
public:
	inline BatchCatalogueWithStubTexture(const unsigned long format, const bool isStatic, const ShaderObject* vShader, const ShaderObject* fShader, const bool indicies) : 
		BatchCatalogue(format, isStatic, vShader, fShader, indicies)
	{};

	void addSupportedTexture(SupportedTexture* texture) {
		m_textures.push_back(texture);
	}
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

using ::testing::Return;
using ::testing::ReturnNull;

TEST(BatchCatalogue, IsNotAMatchWhenNullBatchableObjectSupplied) {
	BatchCatalogue* catalogue = new BatchCatalogue(0, false, NULL, NULL, false);

	EXPECT_FALSE(catalogue->isMatch(NULL, false));
}

TEST(BatchCatalogue, IsNotAMatchWhenCheckOnlyAndWhenBatchableObjectFormatDoesNotMatchCatalogue) {
	BatchCatalogue* catalogue = new BatchCatalogue(0, false, NULL, NULL, false);
	
	MockBatchableObject batchableObject;
	ON_CALL(batchableObject, getDataFormat()).WillByDefault(Return(1));
	ON_CALL(batchableObject, isStatic()).WillByDefault(Return(false));
	ON_CALL(batchableObject, getVertexShader()).WillByDefault(ReturnNull());
	ON_CALL(batchableObject, getFragmentShader()).WillByDefault(ReturnNull());
	ON_CALL(batchableObject, hasIndicies()).WillByDefault(Return(false));
	
	EXPECT_FALSE(catalogue->isMatch(&batchableObject, true));
};

TEST(BatchCatalogue, IsNotAMatchWhenCheckOnlyAndWhenBatchableObjectStaticDoesNotMatchCatalogue) {
	BatchCatalogue* catalogue = new BatchCatalogue(0, false, NULL, NULL, false);
	
	MockBatchableObject batchableObject;
	ON_CALL(batchableObject, getDataFormat()).WillByDefault(Return(0));
	ON_CALL(batchableObject, isStatic()).WillByDefault(Return(true));
	ON_CALL(batchableObject, getVertexShader()).WillByDefault(ReturnNull());
	ON_CALL(batchableObject, getFragmentShader()).WillByDefault(ReturnNull());
	ON_CALL(batchableObject, hasIndicies()).WillByDefault(Return(false));
	
	EXPECT_FALSE(catalogue->isMatch(&batchableObject, true));
};

TEST(BatchCatalogue, IsNotAMatchWhenCheckOnlyAndWhenBatchableObjectVertexShaderDoesNotMatchCatalogue) {
	BatchCatalogue* catalogue = new BatchCatalogue(0, false, NULL, NULL, false);
	
	MockBatchableObject batchableObject;
	ON_CALL(batchableObject, getDataFormat()).WillByDefault(Return(0));
	ON_CALL(batchableObject, isStatic()).WillByDefault(Return(false));
	ON_CALL(batchableObject, getVertexShader()).WillByDefault(Return(new ShaderObject()));
	ON_CALL(batchableObject, getFragmentShader()).WillByDefault(ReturnNull());
	ON_CALL(batchableObject, hasIndicies()).WillByDefault(Return(false));
	
	EXPECT_FALSE(catalogue->isMatch(&batchableObject, true));
};

TEST(BatchCatalogue, IsNotAMatchWhenCheckOnlyAndWhenBatchableObjectFragmentShaderDoesNotMatchCatalogue) {
	BatchCatalogue* catalogue = new BatchCatalogue(0, false, NULL, NULL, false);
	
	MockBatchableObject batchableObject;
	ON_CALL(batchableObject, getDataFormat()).WillByDefault(Return(0));
	ON_CALL(batchableObject, isStatic()).WillByDefault(Return(false));
	ON_CALL(batchableObject, getVertexShader()).WillByDefault(ReturnNull());
	ON_CALL(batchableObject, getFragmentShader()).WillByDefault(Return(new ShaderObject()));
	ON_CALL(batchableObject, hasIndicies()).WillByDefault(Return(false));
	
	EXPECT_FALSE(catalogue->isMatch(&batchableObject, true));
};

TEST(BatchCatalogue, IsNotAMatchWhenCheckOnlyAndWhenBatchableObjectHasIndiciesDoesNotMatchCatalogue) {
	BatchCatalogue* catalogue = new BatchCatalogue(0, false, NULL, NULL, false);
	
	MockBatchableObject batchableObject;
	ON_CALL(batchableObject, getDataFormat()).WillByDefault(Return(0));
	ON_CALL(batchableObject, isStatic()).WillByDefault(Return(false));
	ON_CALL(batchableObject, getVertexShader()).WillByDefault(ReturnNull());
	ON_CALL(batchableObject, getFragmentShader()).WillByDefault(ReturnNull());
	ON_CALL(batchableObject, hasIndicies()).WillByDefault(Return(true));
	
	EXPECT_FALSE(catalogue->isMatch(&batchableObject, true));
};

TEST(BatchCatalogue, IsAMatchWhenCheckOnlyAndWhenBatchableObjectMeetsTheCriteria) {
	BatchCatalogue* catalogue = new BatchCatalogue(0, false, NULL, NULL, false);
	
	MockBatchableObject batchableObject;
	ON_CALL(batchableObject, getDataFormat()).WillByDefault(Return(0));
	ON_CALL(batchableObject, isStatic()).WillByDefault(Return(false));
	ON_CALL(batchableObject, getVertexShader()).WillByDefault(ReturnNull());
	ON_CALL(batchableObject, getFragmentShader()).WillByDefault(ReturnNull());
	ON_CALL(batchableObject, hasIndicies()).WillByDefault(Return(false));
	
	EXPECT_TRUE(catalogue->isMatch(&batchableObject, true));
};

TEST(BatchCatalogue, IsAMatchWhenNotCheckOnlyAndBatchableObjectMeetsTheCriteriaAndTextureIsInTheCatalogue) {
	BatchCatalogueWithStubTexture* catalogue = new BatchCatalogueWithStubTexture(0, false, NULL, NULL, false);
	catalogue->addSupportedTexture(new SupportedTexture(1, 0, 0));

	MockBatchableObject batchableObject;
	ON_CALL(batchableObject, getDataFormat()).WillByDefault(Return(0));
	ON_CALL(batchableObject, isStatic()).WillByDefault(Return(false));
	ON_CALL(batchableObject, getVertexShader()).WillByDefault(ReturnNull());
	ON_CALL(batchableObject, getFragmentShader()).WillByDefault(ReturnNull());
	ON_CALL(batchableObject, hasIndicies()).WillByDefault(Return(false));
	ON_CALL(batchableObject, getTextureID(0)).WillByDefault(Return(1));
	
	EXPECT_TRUE(catalogue->isMatch(&batchableObject, false));
}

TEST(BatchCatalogue, IsAMatchWhenNotCheckOnlyAndBatchableObjectMeetsTheCriteriaAndTextureIsAnywhereInTheCatalogue) {
	BatchCatalogueWithStubTexture* catalogue = new BatchCatalogueWithStubTexture(0, false, NULL, NULL, false);
	catalogue->addSupportedTexture(new SupportedTexture(1, 0, 0));
	catalogue->addSupportedTexture(new SupportedTexture(2, 0, 0));
	catalogue->addSupportedTexture(new SupportedTexture(3, 0, 0));

	MockBatchableObject batchableObject;
	ON_CALL(batchableObject, getDataFormat()).WillByDefault(Return(0));
	ON_CALL(batchableObject, isStatic()).WillByDefault(Return(false));
	ON_CALL(batchableObject, getVertexShader()).WillByDefault(ReturnNull());
	ON_CALL(batchableObject, getFragmentShader()).WillByDefault(ReturnNull());
	ON_CALL(batchableObject, hasIndicies()).WillByDefault(Return(false));
	ON_CALL(batchableObject, getTextureID(0)).WillByDefault(Return(2));
	
	EXPECT_TRUE(catalogue->isMatch(&batchableObject, false));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);

  return RUN_ALL_TESTS();
}