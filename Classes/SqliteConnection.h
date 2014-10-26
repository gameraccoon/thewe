#ifndef SQLITE_CONNECTION_H
#define SQLITE_CONNECTION_H

#include "DbConnection.h"

struct sqlite3;

/**
 * Class able to read Textures and create and sprites.
 *
 * If texture load once that it unload only after destruction.
 */
class SqliteConnection : public DbConnection
{
public:
	SqliteConnection(const std::string& databaseFile);
	virtual ~SqliteConnection(void) override;

	virtual std::shared_ptr<SqlDataReader> execQuery(const std::string& query) override;
	virtual void execSql(const std::string& statement) override;

	bool IsTableExists(const std::string& table);

	std::string GetSqliteErrMessage();

private:
	sqlite3 *_database;
};

#endif
