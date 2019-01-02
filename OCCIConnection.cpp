/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Portions created by the Initial Developer are
 * Copyright (C) 2012 the Initial Developer.
 * All Rights Reserved.
 *
 * The Original Code is this.
 *
 * The Initial Developer of the Original Code is the author below.
 */
/**
 * @file OcciConnection.cpp
 * @brief auto generated.
 * @author K.Ozaki
 * @date 2011/03/11
 */

#include "semi/db/OcciConnection.h"

namespace semi {
namespace db {

OcciConnection OcciConnection::instance = OcciConnection();

OcciConnection::OcciConnection() :
	semi::db::DatabaseConnection() {
}

//int fo_callback(oracle::occi::Environment *env, oracle::occi::Connection *conn,
//        void *ctx, oracle::occi::Connection::FailOverType foType,
//        oracle::occi::Connection::FailOverEventType foEvent) {
//    std::cout << "[Callback] Called " << std::endl;
//
//    std::cout << "Failover Event : ";
//    switch (foEvent) {
//    case oracle::occi::Connection::FO_END:
//        std::cout << "FO_END" << std::endl;
//        break;
//    case oracle::occi::Connection::FO_ABORT:
//        std::cout << "FO_ABORT" << std::endl;
//        break;
//    case oracle::occi::Connection::FO_REAUTH:
//        std::cout << "FO_REAUTH" << std::endl;
//        break;
//    case oracle::occi::Connection::FO_BEGIN:
//        std::cout << "FO_BEGIN" << std::endl;
//        break;
//    case oracle::occi::Connection::FO_ERROR:
//        std::cout << "FO_ERROR" << std::endl;
//        break;
//    default:
//        std::cout << "Default " << std::endl;
//        break;
//    }
//    std::cout << "Failover Type : ";
//    switch (foType) {
//    case oracle::occi::Connection::FO_NONE:
//        std::cout << "FO_NONE" << std::endl;
//        break;
//    case oracle::occi::Connection::FO_SESSION:
//        std::cout << "FO_SESSION" << std::endl;
//        break;
//    case oracle::occi::Connection::FO_SELECT:
//        std::cout << "FO_SELECT" << std::endl;
//        break;
//    default:
//        std::cout << "Default " << std::endl;
//        break;
//    }
//    if (foEvent == oracle::occi::Connection::FO_ERROR)
//        return oracle::occi::FO_RETRY;
//    if (foEvent == oracle::occi::Connection::FO_END) {
//        std::cout << "Failover complete" << std::endl;
//        return 0;
//    }
//    return 0;
//}


//// pthread_mutex_t mutex; // ÇÓÂ¾À©¸æ
//pthread_t tryTid; // ¥¹¥ì¥Ã¥É¼±ÊÌ»Ò
//pthread_t cancelTid; // ¥¹¥ì¥Ã¥É¼±ÊÌ»Ò
//bool success;

//// ³ÎÇ§¥¹¥ì¥Ã¥É
//void* tryThread(void* param) {
//
//    // pthread_mutex_lock(&mutex);
//    oracle::occi::Connection *cn = (oracle::occi::Connection*) param;
//    oracle::occi::Statement* st = cn->createStatement("SELECT 1 FROM DUAL;");
//    //    ResultSet rs = st->executeQuery();
//    //    st->closeResultSet(rs);
//    //    cn->terminateStatement(st);
//    success = true;
//
//    // pthread_cancel(cancelTid);
//    // pthread_mutex_unlock(&mutex);
//}

//// ¥­¥ã¥ó¥»¥ë¥¹¥ì¥Ã¥É
//void* cancelThread(void* param) {
//
//    //    int i = 0;
//    //    while (i < 10000)
//    //        i++;
//
//    // pthread_mutex_lock(&mutex);
//    sleep(3);
//    // OCIBreak();
//    // pthread_cancel(cancelTid);
//    // pthread_exit();
//    // pthread_mutex_unlock(&mutex);
//}

OcciConnection::OcciConnection(std::string tskid) :
	semi::db::DatabaseConnection(tskid) {
	// OcciConnection::OcciConnection(std::string tskid){
}

OcciConnection::~OcciConnection() {

	this->terminate();
}

void OcciConnection::init(std::string tskid) {

	this->DatabaseConnection::init(tskid);

	this->TaskID = tskid;

	this->env = 0;
	this->connection = 0;
	this->statement = 0;

	//    DatabaseURI uri;
	//    uri.load();
	//    user = uri.User;
	//    passwd = uri.Pass;
	//    db = "(DESCRIPTION =(ADDRESS_LIST =(ADDRESS = (PROTOCOL = TCP)(HOST = "
	//            + uri.URL + ")(PORT = 1521)))(CONNECT_DATA =(SID = " + uri.URN
	//            + ")(SERVER = DEDICATED)))";

	//    if (0 < user.length() && 0 < passwd.length() && 0 < db.length())
	//        return;


	if (0 == &this->conf || 0 == this->conf.url || 0 == std::string(
			this->conf.url).length()) {
		this->conf = conf::DbConfig::getConfig(tskid);

		this->logger.info(
				"OcciConnection::init DbConfig::getConfig " + tskid
						+ "    (DESCRIPTION =(ADDRESS_LIST =(ADDRESS = (PROTOCOL = TCP)(HOST = "
						+ std::string(this->conf.url) + ")(PORT = "
						+ std::string(this->conf.port)
						+ ")))(CONNECT_DATA =(SID = " + std::string(
						this->conf.urn) + ")(SERVER = DEDICATED)))");
	}

	this->db
			= "(DESCRIPTION =(ADDRESS_LIST =(ADDRESS = (PROTOCOL = TCP)(HOST = "
					+ std::string(this->conf.url) + ")(PORT = " + std::string(
					this->conf.port) + ")))(CONNECT_DATA =(SID = "
					+ std::string(this->conf.urn) + ")(SERVER = DEDICATED)))";

	//    std::cout << "  OcciConnection::init end " << this->db << std::endl;
	//    std::cout << user << std::endl;
	//    std::cout << passwd << std::endl;
	//    std::cout << db << std::endl;
	//    user = "TESTER1";
	//    passwd = "TESTER1";
	//    db
	//            = "(DESCRIPTION =(ADDRESS_LIST =(ADDRESS = (PROTOCOL = TCP)(HOST = 10.86.202.11)(PORT = 1521)))(CONNECT_DATA =(SID = enepdb)(SERVER = DEDICATED)))";

}

void OcciConnection::connect() {

	this->valid = false;
	this->env = 0;
	this->connection = 0;

	if (user.length() == 0 || passwd.length() == 0 || db.length() == 0)
		return;

	//    std::cout << "  OcciConnection::connect start createEnvironment"
	//            << std::endl;

	env = oracle::occi::Environment::createEnvironment(
			oracle::occi::Environment::DEFAULT);

	std::cout << "  OcciConnection::connect start createConnection"
			<< std::endl;

	connection = env->createConnection(user, passwd, db);

	// connection->setTAFNotify(fo_callback, NULL);

	if (logger.isDebugEnabled())
		logger.debug("env->createConnection(user, passwd, db);");

	if (0 != connection)
		this->valid = true;

	// pthread_mutex_init(&mutex, NULL);
}

semi::db::OcciStatement* OcciConnection::createStatement(std::string sqlStmt) {

	//    std::string sql = sqlStmt;
	//    std::stringstream ss;
	//    ss << "SQL:";
	//    ss << sqlStmt;
	//    std::cout << ss.str() <<std::endl:;

	semi::db::OcciConnection::RecentSQL = sqlStmt;

	if (this->isValid())
		connect();

	if (this->isValid())
		throw std::logic_error(
				"TNS is empty, It is not able to create statement. (common.OcciConnection::createStatement)");

	// ³ÎÇ§¥¹¥ì¥Ã¥É
	// success = false;
	// pthread_create(&cancelTid, NULL, cancelThread, connection);
	// pthread_create(&tryTid, NULL, tryThread, connection);
	//    // pthread_join(cancelTid, NULL); // ½ªÎ»ÂÔ

	//    OCIEnv *envhp = env->getOCIEnvironment();
	//    OCIServer *srvhp = connection->getOCIServer();
	//    OCISvcCtx *ctxhp = connection->getOCIServiceContext();
	//    OCISession *seshp = connection->getOCISession();
	//    OCIStmt *stmhp;
	//    OCIError *errhp;
	//    sword ret;
	//
	//    OCIHandleAlloc((dvoid*) envhp, (dvoid**) &errhp, OCI_HTYPE_ERROR,
	//            (size_t) 0, (dvoid**) 0);
	//
	//    //    ret = OCISessionBegin(ctxhp, errhp, seshp, OCI_CRED_RDBMS,
	//    //            (ub4) OCI_DEFAULT);
	//    //    std::cout << "OCISessionBegin " << ret << std::endl;
	//
	//    OCIHandleAlloc((dvoid*) envhp, (dvoid**) &stmhp, OCI_HTYPE_STMT,
	//            (size_t) 0, (dvoid**) 0);
	//
	//    //    OCIHandleAlloc((dvoid*) envhp, (dvoid**) &ctxhp, OCI_HTYPE_SVCCTX,
	//    //            (size_t) 0, (dvoid**) 0);
	//
	//    text* sql = (text*) "SELECT 1 FROM DUAL";
	//    ret = OCIStmtPrepare(stmhp, errhp, sql, (ub4) strlen((char*) sql),
	//            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
	//    std::cout << "OCIStmtPrepare " << ret << std::endl;
	//    if (ret == OCI_SUCCESS)
	//        //        //    OCISvcCtx *svchp, OCIStmt *stmtp, OCIError *errhp,
	//        //        //                                 ub4 iters, ub4 rowoff, CONST OCISnapshot *snap_in,
	//        //        //                                 OCISnapshot *snap_out, ub4 mode);
	//        //        ret = OCIStmtExecute(ctxhp, stmhp, errhp, (ub4) 1, (ub4) 0,
	//        //                (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL,
	//        //                (ub4) OCI_DEFAULT);
	//        //        std::cout << "OCIStmtExecute " << ret << std::endl;
	//        ret = OCITransCommit(ctxhp, errhp, (ub4) OCI_DEFAULT);
	//    std::cout << "OCITransCommit " << ret << std::endl;

	//    pthread_cancel( cancelTid);

	oracle::occi::Statement* stmt = connection->createStatement(sqlStmt);
	statement = new semi::db::OcciStatement(stmt);

	return statement;
}

bool OcciConnection::isValid() {
	bool ret = this->DatabaseConnection::isValid();
	if (ret)
		ret = (0 != this->connection);
}

void OcciConnection::terminate() {

	//    std::cout << "  OcciConnection::terminate start" << std::endl;

	if (0 == env || !this->valid)
		return;

	try {
		if (0 != connection) {
			if (statement != 0)
				terminateStatement(statement);

			env->terminateConnection(connection);
		}
	} catch (oracle::occi::SQLException sx) {

		logger.warn("env->terminateConnection Failed!");
	}

	oracle::occi::Environment::terminateEnvironment(env);

	env = 0;
	this->valid = false;

	if (logger.isDebugEnabled())
		logger.debug("env->terminateConnection(connection);");

	// ¥¹¥ì¥Ã¥É½ªÎ»
	// pthread_mutex_destroy(&mutex);
}

void OcciConnection::terminateStatement(semi::db::OcciStatement* stmt) {

	if (stmt->resultset != 0)
		stmt->closeResultSet(stmt->resultset);
	statement = 0;

	if (0 != connection)
		connection->terminateStatement(stmt->statement);
}

void OcciConnection::commit() {

	if (0 == connection)
		return;

	connection->commit();

	if (logger.isDebugEnabled())
		logger.debug("connection->commit();");
}

void OcciConnection::rollback() {

	try {
		if (0 == connection)
			return;

		connection->rollback();

		//    if (isInfoEnabled())
		logger.info("connection->rollback();");
	}
	// SQLÎã³°
	catch (oracle::occi::SQLException sx) {
		logger.warn("connection->rollback Failed!");
	}
}

//char* OcciConnection::getChar(ResultSet *rset, int index) {
//  std::string str = rset->getString(index);
//  return std::string2Char(str);
//}
//
//char* OcciConnection::std::string2Char(std::string &st) {
//  char *pt;
//  std::string *pst;
//
//  pst = new std::string(st);
//  pt = (char*) pst->c_str();
//
//  return pt;
//}


std::string OcciConnection::loggingSqlException(oracle::occi::SQLException sx) {

	std::stringstream ss;
	ss << "Exception thrown for SQL. Error number:";
	int code = sx.getErrorCode();
	ss << code;
	std::string* tmp = new std::string(sx.getMessage());
	std::string msg = std::string(tmp->c_str(), tmp->length() - 1);
	msg = ss.str() + " NOTE:" + msg;

	this->logger.error(msg);

	return msg;
}

OcciConnection OcciConnection::getConnection(std::string tskid) {

	std::cout << "  OcciConnection::getConnection start " << tskid
			<< " current:" << OcciConnection::instance.TaskID << std::endl;

	if (OcciConnection::instance.TaskID != tskid
			|| !OcciConnection::instance.isValid()) {
		OcciConnection::instance.terminate();
		OcciConnection::instance.init(tskid);
	}

	try {

		bool bl = OcciConnection::instance.isValid();
		if (!bl)
			OcciConnection::instance.connect();

		bl = OcciConnection::instance.isValid();
		if (bl) {
			OcciConnection::instance.logger.info(
					"occi db instance is valid at least.");
			bl = OcciConnection::instance.keepAlive(); // check network

			if (bl) {
				OcciConnection::instance.logger.info(
						"occi db instance is valid almost.");
				return OcciConnection::instance; // success
			}
		}
		// instance is null.
		if (!bl) {
			OcciConnection::instance.logger.info("occi db instance is invalid.");
			//            ret = this->avoidProcess(params);
			OcciConnection::instance.terminate();
			OcciConnection::instance = OcciConnection(tskid);
			return OcciConnection::instance;
		}
	}
	// sql exception
	catch (oracle::occi::SQLException sx) {

		//        // for retry
		//        if (!this->onRetry && this->Retry == retryCount)
		//            this->retryCount = 0;

		std::string msg = OcciConnection::instance.loggingSqlException(sx);
		//        ret.success = false;
		//        ret.message = &msg;

		OcciConnection::instance.rollback();
		OcciConnection::instance.terminate();

		throw sx;
		//        ret = this->avoidProcess(params);
	}
}

}
}
