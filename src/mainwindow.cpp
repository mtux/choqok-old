/*
 * MainWindow.cpp
 *
 * Copyright (C) 2008 Mehrdad Momeny <mehrdad.momeny@gmail.com>
 */
#include "mainwindow.h"
#include "settings.h"
#include "statuswidget.h"

#include <kconfigdialog.h>
#include <kstatusbar.h>

#include <kaction.h>
#include <kactioncollection.h>
#include <kstandardaction.h>

#include <KDE/KLocale>

#include <QProcess>

#include "backend.h"

MainWindow::MainWindow()
	: KXmlGuiWindow()
{
    // accept dnd
//     setAcceptDrops(true);
	twitter = new Backend;
	connect(twitter, SIGNAL(homeTimeLineRecived(QList< Status >&)), this, SLOT(homeTimeLinesRecived(QList< Status >&)));
	connect(twitter, SIGNAL(replayTimeLineRecived(QList< Status >&)), this, SLOT(replayTimeLineRecived(QList< Status >&)));
    // tell the KXmlGuiWindow that this is indeed the main widget
	mainWidget = new QWidget;
    ui.setupUi(mainWidget);
	setCentralWidget(mainWidget);
    // then, setup our actions
    setupActions();
	statusBar()->show();
	setupGUI();
	
	
	setDefaultDirection();
	ui.txtNewStatus->setTabChangesFocus ( true );
	connect(ui.txtNewStatus, SIGNAL(textChanged()), this, SLOT(checkNewStatusCharactersCount()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupActions()
{
	KStandardAction::quit(qApp, SLOT(closeAllWindows()), actionCollection());

	KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());

	KAction *actUpdate = new KAction(KIcon("view-refresh"), i18n("Update timeline"), this);
	actionCollection()->addAction(QLatin1String("update_timeline"), actUpdate);
	actUpdate->setShortcut(Qt::Key_F5);
	connect(actUpdate, SIGNAL(triggered( bool )), this, SLOT(updateTimeLines()));
// 	
}

void MainWindow::optionsPreferences()
{
    // The preference dialog is derived from prefs_base.ui
    //
    // compare the names of the widgets in the .ui file
    // to the names of the variables in the .kcfg file
    //avoid to have 2 dialogs shown
    if ( KConfigDialog::showDialog( "settings" ) )  {
        return;
    }
    KConfigDialog *dialog = new KConfigDialog(this, "settings", Settings::self());
	
    QWidget *generalSettingsDlg = new QWidget;
    ui_prefs_base.setupUi(generalSettingsDlg);
    dialog->addPage(generalSettingsDlg, i18n("General"), "package_setting");
	
	QWidget *accountsSettingsDlg = new QWidget;
	ui_accounts_base.setupUi(accountsSettingsDlg);
	dialog->addPage(accountsSettingsDlg, i18n("Account"), "accounts_setting");
	
    connect(dialog, SIGNAL(settingsChanged(QString)), this, SLOT(settingsChanged()));
    dialog->setAttribute( Qt::WA_DeleteOnClose );
    dialog->show();
}

void MainWindow::checkNewStatusCharactersCount()
{
	int count = ui.txtNewStatus->toPlainText().count();
	int remainChar = 140 - count;
	if(remainChar < 30){
		ui.lblCounter->setStyleSheet("QLabel {color: red}");
	} else {
		ui.lblCounter->setStyleSheet("QLabel {color: green}");
	}
	ui.lblCounter->setText(i18n("%1", remainChar));
}

void MainWindow::settingsChanged()
{
	twitter->requestCurrentUser();
	setDefaultDirection();
}

void MainWindow::notify(const QString &title, const QString &message)
{
	switch(Settings::notifyType()){
		case 0:
			break;
		case 1:
			break;
		case 2://Libnotify!
			QString libnotifyCmd = QString("notify-send -t ") + QString::number((int)Settings::notifyInterval()*1000) + QString(" -u low -i  \"") + title + QString("\" \"") + message + QString("\"");
			QProcess::execute(libnotifyCmd);
			break;
	}
}

void MainWindow::updateTimeLines()
{
	kDebug();
	twitter->requestTimeLine(Backend::HomeTimeLine);
// 	twitter->requestTimeLine(Backend::ReplayTimeLine);
}

void MainWindow::homeTimeLinesRecived(QList< Status > & statusList)
{
	kDebug();
	if(statusList.count()==0){
		kDebug()<<"Status list is empty";
		statusBar()->showMessage(i18n("No new twits recived, The list is up to date."), Settings::notifyInterval());
	}
	QList<Status>::const_iterator it = statusList.constBegin();
	QList<Status>::const_iterator endIt = statusList.constEnd();
	for(;it!=endIt; ++it){
		if( it->statusId > Settings::latestStatusId()){
// 			kDebug()<<"StatusId: "<<it->statusId<<"UserId: "<<it->user.userId;
			QString notifyMessage;
			StatusWidget *wt = new StatusWidget(this);
			wt->setCurrentStatus(*it);
// 			ui.frameHome->layout()->addWidget(wt);
			ui.tabs->widget(0)->layout()->addWidget(wt);
		}
	}
}

void MainWindow::replayTimeLineRecived(QList< Status > & statusList)
{
}

void MainWindow::setDefaultDirection()
{
// 	this->setLayoutDirection((Qt::LayoutDirection)Settings::direction());
	ui.tabs->widget(0)->setLayoutDirection((Qt::LayoutDirection)Settings::direction());
	ui.tabs->widget(1)->setLayoutDirection((Qt::LayoutDirection)Settings::direction());
// 	txtNewStatus->document()->firstBlock()->
// 	inputLayout->setLayoutDirection((Qt::LayoutDirection)Settings::direction());
}

#include "mainwindow.moc"
