#include "../DBFile.h"
#include "gtest/gtest.h"

TEST(CreateFile, CreateSucess) {
  DBFile file;
  int ret = file.Create("testfile.bin", heap, NULL);
  ASSERT_EQ(ret, 1);
}
TEST(CreateFile, FailMissingDirectory) {
  DBFile file;
  // providing not found directory as part of file path leads to exception that
  // causes file creation to fail
  int ret = file.Create("bruh/testfile.bin", heap, NULL);
  ASSERT_EQ(ret, 0);
}

TEST(CreateFile, FailBadType) {
  DBFile file;
  // providing not found directory as part of file path leads to exception that
  // causes file creation to fail
  int ret = file.Create("testfile.bin", sorted, NULL);
  ASSERT_EQ(ret, 0);
}


TEST(CloseFile, CreateAndCloseFile) {
  DBFile file;
  int ret = file.Create("testfile.bin", heap, NULL);
  ASSERT_EQ(ret, 1);

  ret = file.Close();
  ASSERT_EQ(ret, 1);
}

TEST(OpenFile, OpenAfterCreate) {
  DBFile file;
  int ret = file.Create("testfile.bin", heap, NULL);
  ASSERT_EQ(ret, 1);

  ret = file.Close();
  ASSERT_EQ(ret, 1);
  ret = file.Open("testfile.bin");
  ASSERT_EQ(ret, 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
