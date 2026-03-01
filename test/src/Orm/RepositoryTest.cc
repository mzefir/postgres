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

} // namespace Zef::Testing::Orm
