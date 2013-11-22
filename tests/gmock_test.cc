#include "../src/BatchCatalogue.cpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::AtLeast;      

TEST(BatchCatalogue, NullBatchableObjectSupplied) {
	BatchCatalogue* catalogue = new BatchCatalogue(0, false, NULL, NULL, false);
	
	EXPECT_FALSE(catalogue->isMatch(NULL, false));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);

  return RUN_ALL_TESTS();
}