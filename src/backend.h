//
// C++ Interface: backend
//
// Description: 
//
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BACKEND_H
#define BACKEND_H

#include <QtCore/QObject>

#include "datacontainers.h"
#include "QBuffer"
#include "QHttp"
// class QNetworkAccessManager;

/**
	@author 
*/
class Backend : public QObject
{
	Q_OBJECT
public:
	enum TimeLineType{All=0, HomeTimeLine, ReplayTimeLine, UserTimeLine};
    Backend(QObject* parent=0);

    ~Backend();
	
	void login();
	void logout();
	
public slots:
// 	void updateTimeLines(TimeLineType type=All, int page=0);
	void postNewStatus(QString &statusMessage);
	void requestTimeLine(TimeLineType type, int page=0);
	void requestCurrentUser();
	
signals:
	void result(bool isDone);
	void sigError(QString &errorMessage);
	void homeTimeLineRecived(QList<Status> &statusList);
	void replayTimeLineRecived(QList<Status> &statusList);
	void currentUserInfo(User);
// 	void directMessagesRecived(QList<Status> &statusList);
	
protected slots:
	void homeTimeLineDone(bool isError);
	void replayTimeLineDone(bool isError);
	void currentUserDone(bool isError);
	
private:
	QString getErrorString(QHttp *sender);
	QString urls[4];
	QBuffer homeBuffer;
	QBuffer replayBuffer;
	QBuffer userIdBuffer;
	QHttp statusHttp;
// 	QHttp timelineHttp;
};

#endif
