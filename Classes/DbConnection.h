#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include <string>

#include <memory>

#include "SqlDataReader.h"

/**
 *
 */
class DbConnection
{
public:
	virtual ~DbConnection() {}

	virtual std::shared_ptr<SqlDataReader> execQuery(const std::string& query) = 0;
	virtual void execSql(const std::string& statement) = 0;
};

#endif
