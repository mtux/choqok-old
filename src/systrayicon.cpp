//
// C++ Implementation: systrayicon
//
// Description: 
//
//
// Author:  Mehrdad Momeny <mehrdad.momeny@gmail.com>, (C) 2008
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
	setToolTip(i18n("choqoK Twitter - Hit me to update your status"));
	
	mainWin = new MainWindow;
	
	setIcon(mainWin->windowIcon());
	
	if(Settings::showMainWinOnStart()){
		mainWin->show();
	}
	
	isQuickActivated = false;
	setupActions();
	quickWidget = new QuickTwit;
	
	connect(this, SIGNAL(activated( QSystemTrayIcon::ActivationReason )),
			 this, SLOT(sysTrayActivated(QSystemTrayIcon::ActivationReason)));
	connect(quickWidget, SIGNAL(sigStatusUpdated()), mainWin, SLOT(updateHomeTimeLine()));
}


SysTrayIcon::~SysTrayIcon()
{
}

void SysTrayIcon::setupActions()
{
// 	KAction *newTwit = new KAction(this);
// 	newTwit->setIcon(KIcon("document-new"));
// 	newTwit->setText(i18n("Quick Twit"));
// 	newTwit->setShortcut(Qt::ControlModifier | Qt::Key_N);
// 	newTwit->setGlobalShortcutAllowed(true, KAction::NoAutoloading);
// 	newTwit->setGlobalShortcut(KShortcut(Qt::ControlModifier | Qt::MetaModifier | Qt::Key_T));
// 	connect(newTwit, SIGNAL(triggered( bool )), this, SLOT(postQuickTwit()));
// 	mainWin->actionCollection()->addAction(QLatin1String("choqok_new_twit"), newTwit);
// 	
	QAction *newTwit = mainWin->actionCollection()->action("choqok_new_twit");
	connect(newTwit, SIGNAL(triggered( bool )), this, SLOT(postQuickTwit()));
	this->contextMenu()->addAction(newTwit);
	
	contextMenu()->addAction(mainWin->actionCollection()->action("update_timeline"));
	contextMenu()->addSeparator();
	
	KAction *showMain = new KAction(this);
	if(mainWin->isVisible())
		showMain->setText(i18n("Minimize"));
	else
		showMain->setText(i18n("Restore"));
	connect(showMain, SIGNAL(triggered( bool )), this, SLOT(toggleMainWindowVisibility()));
	actionCollection()->addAction("toggle-mainwin", showMain);
	contextMenu()->addAction(showMain);
}

void SysTrayIcon::quitApp()
{
	qApp->quit();
}

void SysTrayIcon::postQuickTwit()
{
	if(quickWidget->isVisible()){
		quickWidget->hide();
	}
	else{
		quickWidget->show();
	}
}

void SysTrayIcon::toggleMainWindowVisibility()
{
	if(mainWin->isVisible()){
		mainWin->hide();
		actionCollection()->action("toggle-mainwin")->setText("Restore");
	} else {
		mainWin->show();
		actionCollection()->action("toggle-mainwin")->setText("Minimize");
	}
}

void SysTrayIcon::sysTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
	if(reason == QSystemTrayIcon::Trigger){
		switch(Settings::systrayTriggerType()){
			case 0:
				toggleMainWindowVisibility();
				break;
			case 1:
				postQuickTwit();
				break;
			case 2:
				mainWin->updateTimeLines();
		};
	}
}

#include "systrayicon.moc"
