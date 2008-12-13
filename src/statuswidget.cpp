//
// C++ Implementation: statuswidget
//
// Description: 
//
//
// Author:  <>, (C) 2008
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
	
	btnFavorite->setIcon(KIcon("rating"));
	btnReply->setIcon(KIcon("edit-undo"));
	btnRemove->setIcon(KIcon("edit-delete"));
	
	connect(btnReply, SIGNAL(clicked(bool)), this, SLOT(requestReplay()));
}


StatusWidget::~StatusWidget()
{
}

void StatusWidget::remove()
{
}

void StatusWidget::setFavorite(bool isFavorite)
{
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
	QString sign = "<b><a href='http://twitter.com/"+mCurrentStatus.user.screenName+"'>"+mCurrentStatus.user.screenName+"</a> - </b> ";
	sign += "<a href='http://twitter.com/" + mCurrentStatus.user.screenName + "/statuses/" + QString::number(mCurrentStatus.statusId) +
			 "'>" + formatDateTime(mCurrentStatus.creationDateTime) + "</a> - ";
	sign += mCurrentStatus.source;
	lblSign->setText(sign);
	lblStatus->setText(mCurrentStatus.content);
	//TODO: set Image
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

void StatusWidget::requestReplay()
{
	kDebug();
	emit sigReply(mCurrentStatus.user.screenName, mCurrentStatus.statusId);
}
