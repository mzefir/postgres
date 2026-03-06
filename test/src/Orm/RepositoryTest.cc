#include <testing.hpp>

#include "Entity/Category.hpp"
#include "Orm/DbConnectionMock.hpp"
#include "Orm/RawResultMock.hpp"
#include "Orm/Repository.hpp"

using namespace ::testing;

namespace Zef::Testing::Orm {

using CategoryRepo = Zef::Orm::Repository<Zef::Entity::Category>;

class RepositoryInsertTest : public Test {
protected:
  void TearDown() override { CategoryRepo::SetConnectionFactory(nullptr); }
};

TEST_F(RepositoryInsertTest, Insert_ReturnsTrue_AndSetsId_OnSuccess) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id"};
  static const std::vector<std::vector<std::string>> data{{"42"}};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(HasSubstr("RETURNING id"), _, _))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  Zef::Entity::Category cat;
  cat.Set("title", std::string{"Test"});

  EXPECT_TRUE(CategoryRepo::Save(cat));
  EXPECT_EQ(cat.Id(), 42);
}

TEST_F(RepositoryInsertTest, Insert_ReturnsFalse_WhenConnectionFails) {
  CategoryRepo::SetConnectionFactory([]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return nullptr; });

  Zef::Entity::Category cat;
  cat.Set("title", std::string{"Test"});

  EXPECT_FALSE(CategoryRepo::Save(cat));
}

TEST_F(RepositoryInsertTest, Insert_ReturnsFalse_WhenQueryFails) {
  auto mockConn = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr = mockConn.get();

  EXPECT_CALL(*connPtr, GetAll(_, _, _)).WillOnce(Return(std::nullopt));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  Zef::Entity::Category cat;
  cat.Set("title", std::string{"Test"});

  EXPECT_FALSE(CategoryRepo::Save(cat));
}

TEST_F(RepositoryInsertTest, Insert_ReturnsFalse_WhenResultIsEmpty) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{};
  static const std::vector<std::vector<std::string>> data{};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(_, _, _))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  Zef::Entity::Category cat;
  cat.Set("title", std::string{"Test"});

  EXPECT_FALSE(CategoryRepo::Save(cat));
}

TEST_F(RepositoryInsertTest, Insert_ReturnsFalse_WhenResultIdValueIsEmpty) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id"};
  static const std::vector<std::vector<std::string>> data{{""}};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(_, _, _))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  Zef::Entity::Category cat;
  cat.Set("title", std::string{"Test"});

  EXPECT_FALSE(CategoryRepo::Save(cat));
}

TEST_F(RepositoryInsertTest, Insert_BuildsQuery_WithCorrectInsertStatement) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id"};
  static const std::vector<std::vector<std::string>> data{{"7"}};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(AllOf(HasSubstr("INSERT INTO category"), HasSubstr("title"), HasSubstr("RETURNING id")), _, _))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  Zef::Entity::Category cat;
  cat.Set("title", std::string{"Electronics"});

  EXPECT_TRUE(CategoryRepo::Save(cat));
  EXPECT_EQ(cat.Id(), 7);
}

class RepositorySaveTest : public Test {
protected:
  void TearDown() override { CategoryRepo::SetConnectionFactory(nullptr); }
};

TEST_F(RepositorySaveTest, Save_CallsInsert_WhenIdIsZero) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id"};
  static const std::vector<std::vector<std::string>> data{{"5"}};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(HasSubstr("RETURNING id"), _, _))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  Zef::Entity::Category cat;
  cat.Set("id", 0);
  cat.Set("title", std::string{"Books"});

  EXPECT_TRUE(CategoryRepo::Save(cat));
  EXPECT_EQ(cat.Id(), 5);
}

TEST_F(RepositorySaveTest, Save_CallsInsert_WhenIdIsNotSet) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id"};
  static const std::vector<std::vector<std::string>> data{{"9"}};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(HasSubstr("RETURNING id"), _, _))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  Zef::Entity::Category cat;
  cat.Set("title", std::string{"Music"});

  EXPECT_TRUE(CategoryRepo::Save(cat));
  EXPECT_EQ(cat.Id(), 9);
}

TEST_F(RepositorySaveTest, Save_CallsUpdate_WhenIdIsNonZero) {
  auto mockConn = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr = mockConn.get();

  EXPECT_CALL(*connPtr, Exec(AllOf(HasSubstr("UPDATE category"), HasSubstr("title"), HasSubstr("WHERE id =")), _, _))
    .WillOnce(Return(true));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  Zef::Entity::Category cat;
  cat.Set("id", 7);
  cat.Set("title", std::string{"Sports"});

  EXPECT_TRUE(CategoryRepo::Save(cat));
}

TEST_F(RepositorySaveTest, Save_Update_BuildsQuery_WithCorrectStatementAndParams) {
  auto mockConn = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr = mockConn.get();

  EXPECT_CALL(*connPtr, Exec(
    AllOf(HasSubstr("UPDATE category"), HasSubstr("title = $1"), HasSubstr("WHERE id = $2")),
    _,
    ElementsAre("Electronics", "7")))
    .WillOnce(Return(true));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  Zef::Entity::Category cat;
  cat.Set("id", 7);
  cat.Set("title", std::string{"Electronics"});

  EXPECT_TRUE(CategoryRepo::Save(cat));
}

TEST_F(RepositorySaveTest, Save_Update_ReturnsFalse_WhenConnectionFails) {
  CategoryRepo::SetConnectionFactory([]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return nullptr; });

  Zef::Entity::Category cat;
  cat.Set("id", 3);
  cat.Set("title", std::string{"Tools"});

  EXPECT_FALSE(CategoryRepo::Save(cat));
}

TEST_F(RepositorySaveTest, Save_Update_ReturnsFalse_WhenExecFails) {
  auto mockConn = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr = mockConn.get();

  EXPECT_CALL(*connPtr, Exec(_, _, _)).WillOnce(Return(false));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  Zef::Entity::Category cat;
  cat.Set("id", 3);
  cat.Set("title", std::string{"Tools"});

  EXPECT_FALSE(CategoryRepo::Save(cat));
}

class RepositoryDeleteTest : public Test {
protected:
  void TearDown() override { CategoryRepo::SetConnectionFactory(nullptr); }
};

TEST_F(RepositoryDeleteTest, Delete_ReturnsTrue_AndSetsIdToZero_OnSuccess) {
  auto mockConn = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr = mockConn.get();

  EXPECT_CALL(*connPtr, Exec(AllOf(HasSubstr("DELETE FROM category"), HasSubstr("WHERE id =")), _, ElementsAre("7")))
    .WillOnce(Return(true));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  Zef::Entity::Category cat;
  cat.Set("id", 7);
  cat.Set("title", std::string{"Sports"});

  EXPECT_TRUE(CategoryRepo::Delete(cat));
  EXPECT_EQ(cat.Id(), 0);
}

TEST_F(RepositoryDeleteTest, Delete_ReturnsFalse_WhenIdIsZero) {
  Zef::Entity::Category cat;
  cat.Set("id", 0);

  EXPECT_FALSE(CategoryRepo::Delete(cat));
}

TEST_F(RepositoryDeleteTest, Delete_ReturnsFalse_WhenIdIsNotSet) {
  Zef::Entity::Category cat;
  EXPECT_FALSE(CategoryRepo::Delete(cat));
}

TEST_F(RepositoryDeleteTest, Delete_ReturnsFalse_WhenConnectionFails) {
  CategoryRepo::SetConnectionFactory([]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return nullptr; });

  Zef::Entity::Category cat;
  cat.Set("id", 5);

  EXPECT_FALSE(CategoryRepo::Delete(cat));
}

TEST_F(RepositoryDeleteTest, Delete_ReturnsFalse_WhenExecFails) {
  auto mockConn = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr = mockConn.get();

  EXPECT_CALL(*connPtr, Exec(_, _, _)).WillOnce(Return(false));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  Zef::Entity::Category cat;
  cat.Set("id", 5);

  EXPECT_FALSE(CategoryRepo::Delete(cat));
  EXPECT_EQ(cat.Id(), 5); // id must not be cleared on failure
}

class RepositoryGetAllTest : public Test {
protected:
  void TearDown() override { CategoryRepo::SetConnectionFactory(nullptr); }
};

TEST_F(RepositoryGetAllTest, GetAll_ReturnsNullopt_WhenConnectionFails) {
  CategoryRepo::SetConnectionFactory([]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return nullptr; });

  EXPECT_EQ(CategoryRepo::GetAll(), std::nullopt);
}

TEST_F(RepositoryGetAllTest, GetAll_ReturnsNullopt_WhenQueryFails) {
  auto mockConn = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr = mockConn.get();

  EXPECT_CALL(*connPtr, GetAll(_)).WillOnce(Return(std::nullopt));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  EXPECT_EQ(CategoryRepo::GetAll(), std::nullopt);
}

TEST_F(RepositoryGetAllTest, GetAll_ReturnsEmptyVector_WhenNoRows) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id", "title"};
  static const std::vector<std::vector<std::string>> data{};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(_))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  const auto result = CategoryRepo::GetAll();
  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(result->empty());
}

TEST_F(RepositoryGetAllTest, GetAll_ReturnsMappedEntities_OnSuccess) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id", "title"};
  static const std::vector<std::vector<std::string>> data{{"1", "Books"}, {"2", "Music"}};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(_))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  const auto result = CategoryRepo::GetAll();
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 2u);
  EXPECT_EQ((*result)[0].Id(), 1);
  EXPECT_EQ((*result)[0].GetString("title"), "Books");
  EXPECT_EQ((*result)[1].Id(), 2);
  EXPECT_EQ((*result)[1].GetString("title"), "Music");
}

TEST_F(RepositoryGetAllTest, GetAll_BuildsQuery_WithSelectAndOrderBy) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{};
  static const std::vector<std::vector<std::string>> emptyData{};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(emptyData));
  EXPECT_CALL(*connPtr, GetAll(AllOf(HasSubstr("SELECT"), HasSubstr("FROM category"), HasSubstr("ORDER BY"))))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  EXPECT_TRUE(CategoryRepo::GetAll().has_value());
}

TEST_F(RepositoryGetAllTest, GetAll_WithFilters_BuildsWhereClause) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{};
  static const std::vector<std::vector<std::string>> emptyData{};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(emptyData));
  EXPECT_CALL(*connPtr, GetAll(AllOf(HasSubstr("WHERE"), HasSubstr("title = $1")), _, ElementsAre("Books")))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  EXPECT_TRUE(CategoryRepo::GetAll({{"title", "Books"}}).has_value());
}

TEST_F(RepositoryGetAllTest, GetAll_WithLimit_BuildsLimitClause) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{};
  static const std::vector<std::vector<std::string>> emptyData{};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(emptyData));
  EXPECT_CALL(*connPtr, GetAll(HasSubstr("LIMIT 5")))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  EXPECT_TRUE(CategoryRepo::GetAll(std::string{}, 5).has_value());
}

class RepositoryGetTest : public Test {
protected:
  void TearDown() override { CategoryRepo::SetConnectionFactory(nullptr); }
};

TEST_F(RepositoryGetTest, Get_ReturnsNullopt_WhenConnectionFails) {
  CategoryRepo::SetConnectionFactory([]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return nullptr; });

  EXPECT_EQ(CategoryRepo::Get(42), std::nullopt);
}

TEST_F(RepositoryGetTest, Get_ReturnsNullopt_WhenQueryFails) {
  auto mockConn = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr = mockConn.get();

  EXPECT_CALL(*connPtr, GetAll(_, _, _)).WillOnce(Return(std::nullopt));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  EXPECT_EQ(CategoryRepo::Get(42), std::nullopt);
}

TEST_F(RepositoryGetTest, Get_ReturnsNullopt_WhenNoMatchingRow) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id", "title"};
  static const std::vector<std::vector<std::string>> data{};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(_, _, _))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  EXPECT_EQ(CategoryRepo::Get(42), std::nullopt);
}

TEST_F(RepositoryGetTest, Get_ReturnsEntity_OnSuccess) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id", "title"};
  static const std::vector<std::vector<std::string>> data{{"42", "Electronics"}};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(_, _, _))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  const auto result = CategoryRepo::Get(42);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->Id(), 42);
  EXPECT_EQ(result->GetString("title"), "Electronics");
}

TEST_F(RepositoryGetTest, Get_BuildsQuery_WithIdFilter_AndLimit1) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id", "title"};
  static const std::vector<std::vector<std::string>> data{{"42", "Books"}};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(AllOf(HasSubstr("WHERE"), HasSubstr("id = $1"), HasSubstr("LIMIT 1")), _, ElementsAre("42")))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  EXPECT_TRUE(CategoryRepo::Get(42).has_value());
}

} // namespace Zef::Testing::Orm

namespace Zef::Testing::Orm {

static const Zef::Orm::TableSchema *CategorySchema() {
  static Zef::Entity::Category proto;
  return proto.GetSchema();
}

class RepositorySaveWithSchemaTest : public Test {
protected:
  void TearDown() override { CategoryRepo::SetConnectionFactory(nullptr); }
};

TEST_F(RepositorySaveWithSchemaTest, Save_WithSchema_CallsInsert_WhenIdIsZero) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id"};
  static const std::vector<std::vector<std::string>> data{{"5"}};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(HasSubstr("RETURNING id"), _, _))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  Zef::Entity::Category cat;
  cat.Set("title", std::string{"Books"});

  EXPECT_TRUE(CategoryRepo::Save(CategorySchema(), cat));
  EXPECT_EQ(cat.Id(), 5);
}

TEST_F(RepositorySaveWithSchemaTest, Save_WithSchema_CallsUpdate_WhenIdIsNonZero) {
  auto mockConn = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr = mockConn.get();

  EXPECT_CALL(*connPtr, Exec(AllOf(HasSubstr("UPDATE category"), HasSubstr("WHERE id =")), _, _))
    .WillOnce(Return(true));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  Zef::Entity::Category cat;
  cat.Set("id", 7);
  cat.Set("title", std::string{"Sports"});

  EXPECT_TRUE(CategoryRepo::Save(CategorySchema(), cat));
}

TEST_F(RepositorySaveWithSchemaTest, Save_WithNullSchema_ReturnsFalse) {
  Zef::Entity::Category cat;
  cat.Set("title", std::string{"Books"});

  EXPECT_FALSE(CategoryRepo::Save(nullptr, cat));
}

class RepositoryGetWithSchemaTest : public Test {
protected:
  void TearDown() override { CategoryRepo::SetConnectionFactory(nullptr); }
};

TEST_F(RepositoryGetWithSchemaTest, Get_WithSchema_ReturnsEntity_OnSuccess) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id", "title"};
  static const std::vector<std::vector<std::string>> data{{"42", "Electronics"}};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(_, _, _))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  const auto result = CategoryRepo::Get(CategorySchema(), 42);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->Id(), 42);
  EXPECT_EQ(result->GetString("title"), "Electronics");
}

TEST_F(RepositoryGetWithSchemaTest, Get_WithNullSchema_ReturnsNullopt) {
  EXPECT_EQ(CategoryRepo::Get(nullptr, 42), std::nullopt);
}

class RepositoryGetAllWithSchemaTest : public Test {
protected:
  void TearDown() override { CategoryRepo::SetConnectionFactory(nullptr); }
};

TEST_F(RepositoryGetAllWithSchemaTest, GetAll_WithSchema_ReturnsMappedEntities_OnSuccess) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id", "title"};
  static const std::vector<std::vector<std::string>> data{{"1", "Books"}, {"2", "Music"}};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(_))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  const auto result = CategoryRepo::GetAll(CategorySchema());
  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result->size(), 2u);
  EXPECT_EQ((*result)[0].Id(), 1);
  EXPECT_EQ((*result)[1].Id(), 2);
}

TEST_F(RepositoryGetAllWithSchemaTest, GetAll_WithNullSchema_ReturnsNullopt) {
  EXPECT_EQ(CategoryRepo::GetAll(static_cast<const Zef::Orm::TableSchema *>(nullptr)), std::nullopt);
}

TEST_F(RepositoryGetAllWithSchemaTest, GetAll_WithSchema_WithFilters_BuildsWhereClause) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{};
  static const std::vector<std::vector<std::string>> emptyData{};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(emptyData));
  EXPECT_CALL(*connPtr, GetAll(AllOf(HasSubstr("WHERE"), HasSubstr("title = $1")), _, ElementsAre("Books")))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  EXPECT_TRUE(CategoryRepo::GetAll(CategorySchema(), {{"title", "Books"}}, {}, 0).has_value());
}

TEST_F(RepositoryGetAllWithSchemaTest, GetAll_WithSchema_WithLimit_BuildsLimitClause) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{};
  static const std::vector<std::vector<std::string>> emptyData{};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(emptyData));
  EXPECT_CALL(*connPtr, GetAll(HasSubstr("LIMIT 5")))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  EXPECT_TRUE(CategoryRepo::GetAll(CategorySchema(), std::string{}, 5).has_value());
}

} // namespace Zef::Testing::Orm

namespace Zef::Testing::Orm {

class RepositoryGetFirstTest : public Test {
protected:
  void TearDown() override { CategoryRepo::SetConnectionFactory(nullptr); }
};

TEST_F(RepositoryGetFirstTest, GetFirst_ReturnsEntity_OnSuccess) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id", "title"};
  static const std::vector<std::vector<std::string>> data{{"1", "Books"}, {"2", "Music"}};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(_))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  const auto result = CategoryRepo::GetFirst();
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->Id(), 1);
  EXPECT_EQ(result->GetString("title"), "Books");
}

TEST_F(RepositoryGetFirstTest, GetFirst_ReturnsNullopt_WhenNoRows) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id", "title"};
  static const std::vector<std::vector<std::string>> data{};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(_))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  EXPECT_EQ(CategoryRepo::GetFirst(), std::nullopt);
}

TEST_F(RepositoryGetFirstTest, GetFirst_ReturnsNullopt_WhenConnectionFails) {
  CategoryRepo::SetConnectionFactory([]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return nullptr; });

  EXPECT_EQ(CategoryRepo::GetFirst(), std::nullopt);
}

TEST_F(RepositoryGetFirstTest, GetFirst_BuildsQuery_WithLimit1) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{};
  static const std::vector<std::vector<std::string>> emptyData{};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(emptyData));
  EXPECT_CALL(*connPtr, GetAll(HasSubstr("LIMIT 1")))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  CategoryRepo::GetFirst();
}

TEST_F(RepositoryGetFirstTest, GetFirst_WithFilters_BuildsWhereClause) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{};
  static const std::vector<std::vector<std::string>> emptyData{};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(emptyData));
  EXPECT_CALL(*connPtr, GetAll(AllOf(HasSubstr("WHERE"), HasSubstr("title = $1"), HasSubstr("LIMIT 1")), _, ElementsAre("Books")))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  CategoryRepo::GetFirst({{"title", "Books"}});
}

TEST_F(RepositoryGetFirstTest, GetFirst_WithSortBy_BuildsOrderByClause) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{};
  static const std::vector<std::vector<std::string>> emptyData{};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(emptyData));
  EXPECT_CALL(*connPtr, GetAll(AllOf(HasSubstr("ORDER BY title DESC"), HasSubstr("LIMIT 1"))))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  CategoryRepo::GetFirst("title DESC");
}

TEST_F(RepositoryGetFirstTest, GetFirst_WithNullSchema_ReturnsNullopt) {
  EXPECT_EQ(CategoryRepo::GetFirst(static_cast<const Zef::Orm::TableSchema *>(nullptr)), std::nullopt);
}

TEST_F(RepositoryGetFirstTest, GetFirst_WithSchema_ReturnsEntity_OnSuccess) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id", "title"};
  static const std::vector<std::vector<std::string>> data{{"7", "Electronics"}};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(_))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  const auto result = CategoryRepo::GetFirst(CategorySchema());
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->Id(), 7);
  EXPECT_EQ(result->GetString("title"), "Electronics");
}

class RepositoryGetFirstRawTest : public Test {
protected:
  void TearDown() override { CategoryRepo::SetConnectionFactory(nullptr); }
};

TEST_F(RepositoryGetFirstRawTest, GetFirstRaw_ReturnsEntity_OnSuccess) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id", "title"};
  static const std::vector<std::vector<std::string>> data{{"3", "Music"}};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(_))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  const auto result = CategoryRepo::GetFirstRaw("SELECT id, title FROM category ORDER BY id ASC");
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->Id(), 3);
  EXPECT_EQ(result->GetString("title"), "Music");
}

TEST_F(RepositoryGetFirstRawTest, GetFirstRaw_AppendsLimit1_ToQuery) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{};
  static const std::vector<std::vector<std::string>> emptyData{};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(emptyData));
  EXPECT_CALL(*connPtr, GetAll(HasSubstr("LIMIT 1")))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  CategoryRepo::GetFirstRaw("SELECT id, title FROM category ORDER BY id ASC");
}

TEST_F(RepositoryGetFirstRawTest, GetFirstRaw_WithParams_AppendsLimit1_AndPassesParams) {
  auto mockConn   = std::make_unique<NiceMock<ConnectionMock>>();
  auto *connPtr   = mockConn.get();
  auto mockResult = std::make_unique<NiceMock<RawResultMock>>();

  static const std::vector<std::string>              colNames{"id", "title"};
  static const std::vector<std::vector<std::string>> data{{"5", "Sports"}};

  EXPECT_CALL(*mockResult, GetColumnNames()).WillRepeatedly(ReturnRef(colNames));
  EXPECT_CALL(*mockResult, GetData()).WillRepeatedly(ReturnRef(data));
  EXPECT_CALL(*connPtr, GetAll(AllOf(HasSubstr("WHERE title = $1"), HasSubstr("LIMIT 1")), _, ElementsAre("Sports")))
    .WillOnce(Return(ByMove(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>(std::move(mockResult)))));

  CategoryRepo::SetConnectionFactory([&]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return std::move(mockConn); });

  const auto result = CategoryRepo::GetFirstRaw("SELECT id, title FROM category WHERE title = $1", {"Sports"});
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->Id(), 5);
}

TEST_F(RepositoryGetFirstRawTest, GetFirstRaw_ReturnsNullopt_WhenConnectionFails) {
  CategoryRepo::SetConnectionFactory([]() -> std::unique_ptr<Zef::Orm::DbConnectionIf> { return nullptr; });

  EXPECT_EQ(CategoryRepo::GetFirstRaw("SELECT id, title FROM category"), std::nullopt);
}

} // namespace Zef::Testing::Orm
