//
// C++ Interface: statuswidget
//
// Description: 
//
//
// Author:  Mehrdad Momeny <mehrdad.momeny@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QFrame>
#include <ui_status_base.h>
#include "datacontainers.h"
/**
Status Widget

	@author Mehrdad Momeny <mehrdad.momeny@gmail.com>
*/
class StatusWidget : public QFrame, public Ui_StatusBase
{
Q_OBJECT
public:
    StatusWidget(QWidget *parent=0);

    ~StatusWidget();
	
	static QString formatDateTime(const QDateTime &time);
	
	Status currentStatus() const;
	void setCurrentStatus(const Status newStatus);
	void setUserImage(const QString &imgPath);
	
signals:
	void sigReply(QString &userName, uint statusId);
	
protected slots:
	void remove();
	void setFavorite(bool isFavorite);
	void requestReplay();
	
private:
	void updateUi();
	
	Status mCurrentStatus;

};

#endif
