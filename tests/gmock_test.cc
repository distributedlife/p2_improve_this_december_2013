#include "../src/BatchCatalogue.cpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

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

TEST(BatchCatalogue, IsNotAMatchWhenCheckOnlyAndWhenBatchableObjectMeetsTheCriteria) {
	BatchCatalogue* catalogue = new BatchCatalogue(0, false, NULL, NULL, false);
	
	MockBatchableObject batchableObject;
	ON_CALL(batchableObject, getDataFormat()).WillByDefault(Return(0));
	ON_CALL(batchableObject, isStatic()).WillByDefault(Return(false));
	ON_CALL(batchableObject, getVertexShader()).WillByDefault(ReturnNull());
	ON_CALL(batchableObject, getFragmentShader()).WillByDefault(ReturnNull());
	ON_CALL(batchableObject, hasIndicies()).WillByDefault(Return(false));
	
	EXPECT_TRUE(catalogue->isMatch(&batchableObject, true));
};

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);

  return RUN_ALL_TESTS();
}