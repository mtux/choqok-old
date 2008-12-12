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
 : QWidget(parent)
{
	setupUi(this);
	
	btnFavorite->setIcon(KIcon("rating"));
	btnReplay->setIcon(KIcon("edit-undo"));
	btnRemove->setIcon(KIcon("edit-delete"));
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
	if(mCurrentStatus.user.userId == Settings::currentUserId()){
		btnReplay->setVisible(false);
	} else {
		btnRemove->setVisible(false);
	}
	QString tw = "<b><a href='http://twitter.com/"+mCurrentStatus.user.screenName+"'>"+mCurrentStatus.user.screenName+"</a> :</b> ";
	tw+= mCurrentStatus.content;
	lblStatus->setText(tw);
	//TODO: set Image
}


