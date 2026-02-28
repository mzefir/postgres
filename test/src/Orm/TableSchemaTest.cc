#include <testing.hpp>

#include "Orm/TableSchema.hpp"
#include "Orm/ColumnMock.hpp"

using namespace ::testing;

namespace Zef::Testing::Orm {

class TableSchemaTest : public Test {
protected:
  void SetUp() override {}
  void TearDown() override {}

  static std::vector<std::unique_ptr<Zef::Orm::ColumnIf>> MakeColumns(int count) {
    std::vector<std::unique_ptr<Zef::Orm::ColumnIf>> cols;
    for (int i = 0; i < count; ++i)
      cols.push_back(std::make_unique<NiceMock<ColumnMock>>());
    return cols;
  }
};

TEST_F(TableSchemaTest, GetTableName_ReturnsConstructedName) {
  Zef::Orm::TableSchema schema("users", {});
  EXPECT_EQ(schema.GetTableName(), "users");
}

TEST_F(TableSchemaTest, GetColumns_ReturnsEmptyVector_WhenConstructedWithNoColumns) {
  Zef::Orm::TableSchema schema("orders", {});
  EXPECT_TRUE(schema.GetColumns().empty());
}

TEST_F(TableSchemaTest, GetColumns_ReturnsCorrectNumberOfColumns) {
  Zef::Orm::TableSchema schema("products", MakeColumns(3));
  EXPECT_EQ(schema.GetColumns().size(), 3u);
}

TEST_F(TableSchemaTest, GetColumns_ColumnDataIsAccessible) {
  const std::string colName{"id"};

  auto mockCol = std::make_unique<StrictMock<ColumnMock>>();
  auto *mockColPtr = mockCol.get();

  std::vector<std::unique_ptr<Zef::Orm::ColumnIf>> cols;
  cols.push_back(std::move(mockCol));

  Zef::Orm::TableSchema schema("accounts", std::move(cols));

  EXPECT_CALL(*mockColPtr, GetName()).WillOnce(ReturnRef(colName));
  EXPECT_EQ(schema.GetColumns()[0]->GetName(), colName);
}

TEST_F(TableSchemaTest, GetColumns_ColumnTypeIsAccessible) {
  auto mockCol = std::make_unique<StrictMock<ColumnMock>>();
  auto *mockColPtr = mockCol.get();

  std::vector<std::unique_ptr<Zef::Orm::ColumnIf>> cols;
  cols.push_back(std::move(mockCol));

  Zef::Orm::TableSchema schema("accounts", std::move(cols));

  EXPECT_CALL(*mockColPtr, GetType()).WillOnce(Return(Zef::Orm::ColumnType::Integer));
  EXPECT_EQ(schema.GetColumns()[0]->GetType(), Zef::Orm::ColumnType::Integer);
}

} // namespace Zef::Testing::Orm
