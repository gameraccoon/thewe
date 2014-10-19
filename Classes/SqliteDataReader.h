#ifndef SQLITE_DATA_READER_H
#define SQLITE_DATA_READER_H

#include <string>

#include <sqlite3.h>

#include "SqlDataReader.h"
#include "DbConnection.h"

class SqliteValue : public DbValue
{
public:
	SqliteValue(sqlite3_stmt* ppStmt, int columnIndex);
	virtual ~SqliteValue() override;
	virtual bool asBool() override;
	virtual int asInt() override;
	virtual float asFloat() override;
	virtual std::string asString() override;
	virtual const void* asVariant() override;
private:
	sqlite3_stmt* ppStmt;
	int columnIndex;
};

/**
 * Class able to read Textures and create and sprites.
 *
 * If texture load once that it unload only after destruction.
 */
class SqliteDataReader : public SqlDataReader
{
public:
	SqliteDataReader(const std::string& query, sqlite3* db);
	virtual ~SqliteDataReader();
	/**
	 * @throws TextureNotFoundException
	 */
	virtual bool next() override;

	virtual DbValue::Ptr getValueByName(const std::string& columnName) override;
	
	virtual DbValue::Ptr getValueByIndex(int columnIndex) override;

private:
	sqlite3_stmt *ppStmt;
};

#endif
