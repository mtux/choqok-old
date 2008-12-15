//
// C++ Implementation: statuswidget
//
// Description: 
//
//
// Author:  Mehrdad Momeny <mehrdad.momeny@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "statuswidget.h"
#include "settings.h"

StatusWidget::StatusWidget(QWidget *parent)
 : QFrame(parent)
{
	setupUi(this);
	
	timer.start(UPDATEINTERVAL);
	
	btnFavorite->setIcon(KIcon("rating"));
	btnReply->setIcon(KIcon("edit-undo"));
	btnRemove->setIcon(KIcon("edit-delete"));
	
	connect(btnReply, SIGNAL(clicked(bool)), this, SLOT(requestReply()));
	connect(&timer, SIGNAL(timeout()), this, SLOT(updateSign()));
	connect(btnFavorite, SIGNAL(clicked(bool)), this, SLOT(setFavorite(bool)));
	connect(btnRemove, SIGNAL(clicked(bool)), this, SLOT(requestDestroy()));
}


StatusWidget::~StatusWidget()
{
}

void StatusWidget::remove()
{
}

void StatusWidget::setFavorite(bool isFavorite)
{
	emit sigFavorite(mCurrentStatus.statusId, isFavorite);
}

Status StatusWidget::currentStatus() const
{
	return mCurrentStatus;
}

void StatusWidget::setCurrentStatus(const Status newStatus)
{
	mCurrentStatus = newStatus;
	updateUi();
}

void StatusWidget::updateUi()
{
// 	kDebug()<<"ScreenName: "<<mCurrentStatus.user.screenName<<"Current: "<<mCurrentStatus.user.userId<<" Settings: "<<Settings::currentUserId();
	if(mCurrentStatus.user.screenName == Settings::username()){
		btnReply->setVisible(false);
	} else {
		btnRemove->setVisible(false);
	}
	lblSign->setText(generateSign());
	lblStatus->setText(mCurrentStatus.content);
}

QString StatusWidget::formatDateTime(const QDateTime &time) {
	int seconds = time.secsTo(QDateTime::currentDateTime());
	if (seconds <= 15) return "Just now";
	if (seconds <= 45) return "about " + QString::number(seconds) + " second" + (seconds == 1 ? "" : "s") + " ago";
	int minutes = (seconds - 45 + 59) / 60;
	if (minutes <= 45) return "about " + QString::number(minutes) + " minute" + (minutes == 1 ? "" : "s") + " ago";
	int hours = (seconds - 45 * 60 + 3599) / 3600;
	if (hours <= 18) return "about " + QString::number(hours) + " hour" + (hours == 1 ? "" : "s") + " ago";
	int days = (seconds - 18 * 3600 + 24 * 3600 - 1) / (24 * 3600);
	return "about " + QString::number(days) + " day" + (days == 1 ? "" : "s") + " ago";
}



void StatusWidget::setUserImage(const QString & imgPath)
{
	lblImage->setPixmap(QPixmap(imgPath));
}

void StatusWidget::requestReply()
{
	kDebug();
	emit sigReply(mCurrentStatus.user.screenName, mCurrentStatus.statusId);
}

QString StatusWidget::generateSign()
{
	QString sign = "<b><a href='http://twitter.com/"+mCurrentStatus.user.screenName+"'>"+mCurrentStatus.user.screenName+"</a> - </b> ";
	sign += "<a href='http://twitter.com/" + mCurrentStatus.user.screenName + "/statuses/" + QString::number(mCurrentStatus.statusId) +
			"'>" + formatDateTime(mCurrentStatus.creationDateTime) + "</a> - ";
	sign += mCurrentStatus.source;
	return sign;
}

void StatusWidget::updateSign()
{
	lblSign->setText(generateSign());
}

void StatusWidget::requestDestroy()
{
	emit sigDestroy(mCurrentStatus.statusId);
}

#include "statuswidget.moc"
