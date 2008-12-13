//
// C++ Interface: datacontainers
//
// Description: 
//
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef DATACONTAINERS_H
#define DATACONTAINERS_H
#include <QDateTime>

struct User{
public:
	uint userId;
	QString name;
	QString screenName;
	QString location;
	QString description;
	QString profileImageUrl;
	QString homePageUrl;
	bool isProtected;
	uint followersCount;
};

struct Status{
public:
	QDateTime creationDateTime;
	uint statusId;
	QString content;
	QString source;
	bool isTruncated;
	uint replayToStatusId;
	uint replayToUserId;
	bool isFavorited;
	QString replayToUserScreenName;
	User user;
};
#endif