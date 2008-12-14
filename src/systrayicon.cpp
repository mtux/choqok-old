//
// C++ Implementation: systrayicon
//
// Description: 
//
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "systrayicon.h"
#include "settings.h"
#include <kaction.h>
#include <kactioncollection.h>
#include <QMenu>

SysTrayIcon::SysTrayIcon(QWidget* parent): KSystemTrayIcon(parent)
{
	setIcon(QIcon("/usr/share/icons/default.kde4/64x64/apps/kde.png"));
	mainWin = new MainWindow;
	if(Settings::showMainWinOnStart()){
		mainWin->show();
	}
	setupActions();
	
// 	connect(&quickWidget, SIGNAL(sigPostStatus(QString&)), mainWin, SLOT(postStatus(QString&)));
// 	connect(&quickWidget, SIGNAL(rejected()), mainWin, SLOT(abortPostNewStatus()));
}


SysTrayIcon::~SysTrayIcon()
{
}

void SysTrayIcon::setupActions()
{
	KAction *newTwit = new KAction(this);
	newTwit->setIcon(KIcon("document-new"));
	newTwit->setText(i18n("Quick Twit"));
	newTwit->setGlobalShortcutAllowed(true);
	newTwit->setGlobalShortcut(KShortcut(Qt::ControlModifier+Qt::MetaModifier+Qt::Key_T));
	connect(newTwit, SIGNAL(triggered( bool )), &quickWidget, SLOT(show()));
	this->actionCollection()->addAction("choqok-new-quick-twit", newTwit);
	this->contextMenu()->addAction(newTwit);
}

void SysTrayIcon::quitApp()
{
	qApp->quit();
}

void SysTrayIcon::postQuickTwit()
{
	quickWidget.show();
}

#include "systrayicon.moc"
