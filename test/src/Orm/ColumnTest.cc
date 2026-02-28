#include <testing.hpp>

#include "Orm/Column.hpp"
#include "Orm/Defs.hpp"

using namespace ::testing;

namespace Zef::Testing::Orm {

class ColumnTest : public Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(ColumnTest, GetName_ReturnsConstructedName) {
  Zef::Orm::Column col("age", Zef::Orm::ColumnType::Integer, Zef::Orm::ColumnFlags::Nullable);
  EXPECT_EQ(col.GetName(), "age");
}

TEST_F(ColumnTest, GetType_ReturnsConstructedType) {
  Zef::Orm::Column col("active", Zef::Orm::ColumnType::Boolean, Zef::Orm::ColumnFlags::Nullable);
  EXPECT_EQ(col.GetType(), Zef::Orm::ColumnType::Boolean);
}

TEST_F(ColumnTest, GetFlags_ReturnsConstructedFlags) {
  Zef::Orm::Column col("score", Zef::Orm::ColumnType::Float, Zef::Orm::ColumnFlags::IgnoreOnRead);
  EXPECT_EQ(col.GetFlags(), Zef::Orm::ColumnFlags::IgnoreOnRead);
}

TEST_F(ColumnTest, GetFlags_CombinedFlags_ReturnsCombinedValue) {
  auto flags = Zef::Orm::ColumnFlags::Nullable | Zef::Orm::ColumnFlags::IgnoreOnWrite;
  Zef::Orm::Column col("description", Zef::Orm::ColumnType::Text, flags);
  EXPECT_TRUE(col.GetFlags() & Zef::Orm::ColumnFlags::Nullable);
  EXPECT_TRUE(col.GetFlags() & Zef::Orm::ColumnFlags::IgnoreOnWrite);
  EXPECT_FALSE(col.GetFlags() & Zef::Orm::ColumnFlags::IgnoreOnRead);
}

TEST_F(ColumnTest, GetType_TextColumn_ReturnsText) {
  Zef::Orm::Column col("username", Zef::Orm::ColumnType::Text, Zef::Orm::ColumnFlags::Nullable);
  EXPECT_EQ(col.GetType(), Zef::Orm::ColumnType::Text);
}

TEST_F(ColumnTest, GetType_IntegerColumn_ReturnsInteger) {
  Zef::Orm::Column col("id", Zef::Orm::ColumnType::Integer, Zef::Orm::ColumnFlags::IgnoreOnRead);
  EXPECT_EQ(col.GetType(), Zef::Orm::ColumnType::Integer);
}

TEST_F(ColumnTest, GetFlags_OneToMany_IsDetectedCorrectly) {
  Zef::Orm::Column col("items", Zef::Orm::ColumnType::Integer, Zef::Orm::ColumnFlags::OneToMany);
  EXPECT_TRUE(col.GetFlags() & Zef::Orm::ColumnFlags::OneToMany);
  EXPECT_FALSE(col.GetFlags() & Zef::Orm::ColumnFlags::Nullable);
}

} // namespace Zef::Testing::Orm
