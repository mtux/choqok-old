//
// C++ Implementation: backend
//
// Description: 
//
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "backend.h"

#include <QHttp>
#include <QHttpRequestHeader>
#include <KDE/KLocale>
#include <QDomDocument>

#include "settings.h"

Backend::Backend(QObject* parent): QObject(parent)
{
	urls[HomeTimeLine] = "http://twitter.com/statuses/friends_timeline.xml";
	urls[ReplayTimeLine] = "http://twitter.com/statuses/replies.xml";
	urls[UserTimeLine] = "http://twitter.com/statuses/user_timeline.xml";
	login();
	monthes["Jan"] = 1;
	monthes["Feb"] = 2;
	monthes["Mar"] = 3;
	monthes["Apr"] = 4;
	monthes["May"] = 5;
	monthes["Jun"] = 6;
	monthes["Jul"] = 7;
	monthes["Aug"] = 8;
	monthes["Sep"] = 9;
	monthes["Oct"] = 10;
	monthes["Nov"] = 11;
	monthes["Dec"] = 12;
}


Backend::~Backend()
{
	logout();
}

void Backend::postNewStatus(QString & statusMessage)
{
}

void Backend::login()
{
	
}

void Backend::logout()
{
}

void Backend::requestTimeLine(TimeLineType type, int page)
{
	kDebug();
	if(type == All){
		QString errMsg = i18n("Internal coding error, requesting all timelines not tolerated!");
		kDebug()<<"Internal coding error, requesting all timelines not tolerated!";
		emit sigError(errMsg);
		return;
	}
	QUrl url(urls[type]);
	QHttp *timelineHttp = new QHttp(this);
	timelineHttp->setHost(url.host(), url.port(80));
	timelineHttp->setUser(Settings::username(), Settings::password());
	kDebug()<<"Latest status Id: "<<Settings::latestStatusId();
	switch(type){
		case HomeTimeLine:
			connect(timelineHttp, SIGNAL(done( bool )), this, SLOT(homeTimeLineDone(bool)));
			homeBuffer.open(QIODevice::WriteOnly);
			timelineHttp->get(url.path() + 
					(Settings::latestStatusId() ? "?since_id=" + QString::number(Settings::latestStatusId()) : ""), &homeBuffer);
			break;
		case ReplayTimeLine:
			connect(timelineHttp, SIGNAL(done( bool )), this, SLOT(replayTimeLineDone(boo)));
			replayBuffer.open(QIODevice::WriteOnly);
			timelineHttp->get(url.path() + 
					(Settings::latestStatusId() ? "?since_id=" + QString::number(Settings::latestStatusId()) : ""), &replayBuffer);
			break;
		default:
			kDebug()<<"Unknown TimeLine Type";
			break;
	};
}

void Backend::homeTimeLineDone(bool isError)
{
	kDebug()<<isError;
	if(isError){
		QString errMsg = getErrorString(qobject_cast<QHttp *>(sender()));
		kDebug()<<errMsg;
		emit sigError(errMsg);
		return;
	}
	
// 	homeBuffer.close();
	
	QDomDocument document;
	QList<Status> statusList;
	
	document.setContent(homeBuffer.data());
// 	kDebug()<<homeBuffer.data().data();
	homeBuffer.close();
	QDomElement root = document.documentElement();
	
	if (root.tagName() != "statuses") {
		return;
	}
	QDomNode node = root.firstChild();
	QString timeStr;
	while (!node.isNull()) {
		if (node.toElement().tagName() != "status") {
			return;
		}
		QDomNode node2 = node.firstChild();
		Status status;
		while (!node2.isNull()) {
			if(node2.toElement().tagName() == "created_at")
				timeStr = node2.toElement().text();
			else if(node2.toElement().tagName() == "text")
				status.content = node2.toElement().text();
			else if(node2.toElement().tagName() == "id")
				status.statusId = node2.toElement().text().toInt();
			else if(node2.toElement().tagName() == "in_reply_to_status_id")
				status.replayToStatusId = node2.toElement().text().toULongLong();
			else if(node2.toElement().tagName() == "in_reply_to_user_id")
				status.replayToUserId = node2.toElement().text().toULongLong();
			else if(node2.toElement().tagName() == "in_reply_to_screen_name")
				status.replayToUserScreenName = node2.toElement().text();
			else if(node2.toElement().tagName() == "source")
				status.source = node2.toElement().text();
			else if(node2.toElement().tagName() == "truncated")
				status.isTruncated = QVariant(node2.toElement().text().toInt()).toBool();
			else if(node2.toElement().tagName() == "favorited")
				status.isFavorited = QVariant(node2.toElement().text().toInt()).toBool();
			else if(node2.toElement().tagName() == "user"){
				QDomNode node3 = node2.firstChild();
				while (!node3.isNull()) {
					if (node3.toElement().tagName() == "screen_name") {
						status.user.screenName = node3.toElement().text();
					} else if (node3.toElement().tagName() == "profile_image_url") {
						status.user.profileImageUrl = node3.toElement().text();
					} else if (node3.toElement().tagName() == "id") {
						status.user.userId = node3.toElement().text().toUInt();
					}
					node3 = node3.nextSibling();
				}
			}
			node2 = node2.nextSibling();
		}
		node = node.nextSibling();
		QDateTime time = dateFromString(timeStr);
		status.creationDateTime = QDateTime(time.date(), time.time(), Qt::UTC);
		statusList.append(status);
	}
	kDebug()<<statusList.count()<<" new Status recived.";
	emit homeTimeLineRecived(statusList);
}

void Backend::replayTimeLineDone(bool isError)
{
	kDebug()<<isError;
	if(isError){
		QString errMsg = getErrorString(qobject_cast<QHttp *>(sender()));
		kDebug()<<errMsg;
		emit sigError(errMsg);
		return;
	}
}

QString Backend::getErrorString(QHttp * sender)
{
	QString errType;
	switch(sender->error()){
		case QHttp::NoError:
			errType = i18n("No error occured! ");
			break;
		case QHttp::HostNotFound:
			errType = i18n("Host not found, ");
			break;
		case QHttp::ConnectionRefused:
			errType = i18n("Connection refused by server, please try again later. ");
			break;
		case QHttp::UnexpectedClose:
			errType = i18n("Connection terminated unexpected, please try again later. ");
			break;
		case QHttp::Aborted:
			errType = i18n("Aborted, ");
			break;
		case QHttp::InvalidResponseHeader:
			errType = i18n("Invalid response header, ");
			break;
		case QHttp::WrongContentLength:
			errType = i18n("Wrong Content length, ");
			break;
		case QHttp::UnknownError:
		default:
			errType = i18n("Unknown error occured, ");
			break;
	}
	errType += sender->errorString();
	
	return errType;
}

void Backend::requestCurrentUser()
{
	QUrl url(urls[UserTimeLine]);
	QHttp *timelineHttp = new QHttp(this);
	timelineHttp->setHost(url.host(), url.port(80));
	timelineHttp->setUser(Settings::username(), Settings::password());
	connect(timelineHttp, SIGNAL(done( bool )), this, SLOT(currentUserDone(bool)));
	userIdBuffer.open(QIODevice::WriteOnly);
	timelineHttp->get(url.path() + "?count=1", &userIdBuffer);

}

void Backend::currentUserDone(bool isError)
{
	kDebug()<<isError;
	if(isError){
		QString errMsg = getErrorString(qobject_cast<QHttp *>(sender()));
		kDebug()<<errMsg;
		emit sigError(errMsg);
		return;
	}
	
// 	homeBuffer.close();
	
	QDomDocument document;
	
	document.setContent(userIdBuffer.data());
	
	userIdBuffer.close();
	QDomElement root = document.documentElement();
	
	if (root.tagName() != "statuses") {
		return;
	}
	QDomNode node = root.firstChild();
	
	User user;
	
	while (!node.isNull()) {
		if (node.toElement().tagName() != "status") {
			return;
		}
		QDomNode node2 = node.firstChild();
		while (!node2.isNull()) {
			if(node2.toElement().tagName() == "user"){
				QDomNode node3 = node2.firstChild();
				while (!node3.isNull()) {
					if (node3.toElement().tagName() == "screen_name") {
						user.screenName = node3.toElement().text();
					} else if (node3.toElement().tagName() == "profile_image_url") {
						user.profileImageUrl = node3.toElement().text();
					} else if (node3.toElement().tagName() == "id") {
						user.userId = node3.toElement().text().toUInt();
					}
					node3 = node3.nextSibling();
				}
			}
			node2 = node2.nextSibling();
		}
		node = node.nextSibling();
	}
	Settings::setCurrentUserId(user.userId);
	Settings::setCurrentUserScreenName(user.screenName);
	emit currentUserInfo(user);
}

QDateTime Backend::dateFromString(const QString &date) {
	char s[10];
	int year, day, hours, minutes, seconds;
	sscanf(qPrintable(date), "%*s %s %d %d:%d:%d %*s %d", s, &day, &hours, &minutes, &seconds, &year);
	int month = monthes[s];
	return QDateTime(QDate(year, month, day), QTime(hours, minutes, seconds));
}
