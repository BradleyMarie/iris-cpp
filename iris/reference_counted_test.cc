#include "iris/reference_counted.h"

#include "googletest/include/gtest/gtest.h"

const int kValue = 1337;

class Sharable : public iris::ReferenceCountable {
 public:
  Sharable(const bool* allow_deletion, bool* deleted)
      : allow_deletion_(allow_deletion), deleted_(deleted) {
    *deleted_ = false;
  }

  ~Sharable() {
    EXPECT_TRUE(*allow_deletion_);
    *deleted_ = true;
  }

  int GetValue() const { return kValue; }

 private:
  const bool* allow_deletion_;
  bool* deleted_;
};

std::unique_ptr<iris::ReferenceCounted<Sharable>> MakeSharable(
    const bool* allow_deletion, bool* deleted) {
  return std::make_unique<iris::ReferenceCounted<Sharable>>(
      std::make_unique<Sharable>(allow_deletion, deleted));
}

TEST(ReferenceCountedTest, Empty) {
  iris::ReferenceCounted<Sharable> ptr;
  EXPECT_EQ(nullptr, ptr.Get());
}

TEST(ReferenceCountedTest, Construct) {
  bool allow_deletion = false, deleted = false;
  auto ptr = MakeSharable(&allow_deletion, &deleted);
  EXPECT_FALSE(deleted);
  allow_deletion = true;
  ptr.reset();
  EXPECT_TRUE(deleted);
}

TEST(ReferenceCountedTest, CopyConstruct) {
  bool allow_deletion = false, deleted = false;
  auto ptr = MakeSharable(&allow_deletion, &deleted);
  EXPECT_FALSE(deleted);
  auto ptr2 = std::make_unique<iris::ReferenceCounted<Sharable>>(*ptr);
  ptr.reset();
  EXPECT_FALSE(deleted);
  allow_deletion = true;
  ptr2.reset();
  EXPECT_TRUE(deleted);
}

TEST(ReferenceCountedTest, MoveConstruct) {
  bool allow_deletion = false, deleted = false;
  auto ptr = MakeSharable(&allow_deletion, &deleted);
  EXPECT_FALSE(deleted);
  auto ptr2 =
      std::make_unique<iris::ReferenceCounted<Sharable>>(std::move(*ptr));
  allow_deletion = true;
  ptr2.reset();
  EXPECT_TRUE(deleted);
  ptr.reset();
}

TEST(ReferenceCountedTest, CopyAssign) {
  bool allow_deletion = false, deleted = false;
  auto ptr = MakeSharable(&allow_deletion, &deleted);
  EXPECT_FALSE(deleted);
  auto ptr2 = std::make_unique<iris::ReferenceCounted<Sharable>>();
  *ptr2 = *ptr;
  ptr.reset();
  EXPECT_FALSE(deleted);
  allow_deletion = true;
  ptr2.reset();
  EXPECT_TRUE(deleted);
}

TEST(ReferenceCountedTest, MoveAssign) {
  bool allow_deletion = false, deleted = false;
  auto ptr = MakeSharable(&allow_deletion, &deleted);
  EXPECT_FALSE(deleted);
  auto ptr2 = std::make_unique<iris::ReferenceCounted<Sharable>>();
  *ptr2 = std::move(*ptr);
  allow_deletion = true;
  ptr2.reset();
  EXPECT_TRUE(deleted);
  ptr.reset();
}

TEST(ReferenceCountedTest, OperatorBool) {
  iris::ReferenceCounted<Sharable> empty;
  EXPECT_FALSE(empty);

  bool allow_deletion = true, deleted = false;
  auto ptr = iris::MakeReferenceCounted<Sharable>(&allow_deletion, &deleted);
  EXPECT_TRUE(ptr);
}

TEST(ReferenceCountedTest, OperatorArrow) {
  bool allow_deletion = true, deleted = false;
  auto ptr = iris::MakeReferenceCounted<Sharable>(&allow_deletion, &deleted);
  EXPECT_TRUE(ptr);
  EXPECT_EQ(kValue, ptr->GetValue());
}

TEST(ReferenceCountedTest, OperatorDereference) {
  bool allow_deletion = true, deleted = false;
  auto ptr = iris::MakeReferenceCounted<Sharable>(&allow_deletion, &deleted);
  EXPECT_TRUE(ptr);
  EXPECT_EQ(kValue, (*ptr).GetValue());
}

TEST(ReferenceCountedTest, Get) {
  bool allow_deletion = true, deleted = false;
  auto unique = std::make_unique<Sharable>(&allow_deletion, &deleted);
  auto raw = unique.get();
  iris::ReferenceCounted<Sharable> ptr(std::move(unique));
  EXPECT_EQ(raw, ptr.Get());
}

TEST(ReferenceCountedTest, Swap) {
  bool allow_deletion = true, deleted = false;
  auto unique0 = std::make_unique<Sharable>(&allow_deletion, &deleted);
  auto raw0 = unique0.get();
  iris::ReferenceCounted<Sharable> ptr0(std::move(unique0));
  auto unique1 = std::make_unique<Sharable>(&allow_deletion, &deleted);
  auto raw1 = unique1.get();
  iris::ReferenceCounted<Sharable> ptr1(std::move(unique1));
  std::swap(ptr0, ptr1);
  EXPECT_EQ(raw0, ptr1.Get());
  EXPECT_EQ(raw1, ptr0.Get());
}

TEST(ReferenceCountedTest, Compare) {
  bool allow_deletion = true, deleted = false;
  auto ptr0 = iris::MakeReferenceCounted<Sharable>(&allow_deletion, &deleted);
  auto ptr1 = ptr0;
  EXPECT_EQ(ptr0, ptr1);
  EXPECT_LE(ptr0, ptr1);
  EXPECT_GE(ptr0, ptr1);
  EXPECT_FALSE(ptr0 < ptr1);
  EXPECT_FALSE(ptr0 > ptr1);

  auto ptr2 = iris::MakeReferenceCounted<Sharable>(&allow_deletion, &deleted);
  EXPECT_NE(ptr0, ptr2);
  EXPECT_TRUE(ptr0 < ptr2 || ptr1 > ptr2);
  EXPECT_FALSE(ptr0 < ptr2 && ptr1 > ptr2);
  EXPECT_TRUE(ptr0 <= ptr2 || ptr1 >= ptr2);
  EXPECT_FALSE(ptr0 <= ptr2 && ptr1 >= ptr2);
}

TEST(ReferenceCountedTest, Hash) {
  std::hash<iris::ReferenceCounted<Sharable>> hasher;

  bool allow_deletion = true, deleted = false;
  auto ptr0 = iris::MakeReferenceCounted<Sharable>(&allow_deletion, &deleted);
  iris::ReferenceCounted<Sharable> empty;
  EXPECT_NE(hasher(empty), hasher(ptr0));

  auto ptr1 = ptr0;
  EXPECT_EQ(hasher(ptr0), hasher(ptr1));
}