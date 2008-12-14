//
// C++ Implementation: quicktwit
//
// Description: 
//
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "quicktwit.h"
#include <QKeyEvent>
#include "statustextedit.h"
#include "backend.h"
#include "mainwindow.h"
#include "constants.h"
#include "settings.h"

QuickTwit::QuickTwit(QWidget* parent): KDialog(parent)
{
	txtStatus = new StatusTextEdit(this);
	txtStatus->setDefaultDirection((Qt::LayoutDirection)Settings::direction());
	this->setMainWidget(txtStatus);
	this->resize(250, 100);
	txtStatus->setFocus(Qt::OtherFocusReason);
	
	this->setCaption("What are you doing?");
	setButtonText(KDialog::Ok, QString::number(MAX_STATUS_SIZE));
	
	twitter = new Backend(this);
	connect(txtStatus, SIGNAL(returnPressed(QString&)), this, SLOT(slotPostNewStatus(QString&)));
	connect(txtStatus, SIGNAL(charsLeft(int)), this, SLOT(checkNewStatusCharactersCount(int)));
}


QuickTwit::~QuickTwit()
{
}

void QuickTwit::showNearMouse()
{
	QPoint cursorPos = QCursor::pos();
	
// 	move();
}

void QuickTwit::checkNewStatusCharactersCount(int numOfChars)
{
	setButtonText(KDialog::Ok, QString::number(numOfChars));
}

void QuickTwit::slotPostNewStatusDone(bool isError)
{
	if(!isError){
		
		MainWindow::systemNotify("Success!", "New status posted successfully", APPNAME);
	}
	this->close();
}

void QuickTwit::slotPostNewStatus(QString & newStatus)
{
	twitter->postNewStatus(newStatus);
	this->hide();
}

#include "quicktwit.moc"