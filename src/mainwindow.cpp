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
#include <QTimer>
#include <QKeyEvent>

#include "constants.h"
#include "backend.h"
#include "statustextedit.h"
#include "mediamanagement.h"

MainWindow::MainWindow()
	: KXmlGuiWindow()
{
	kDebug();
    // accept dnd
//     setAcceptDrops(true);
	this->setAttribute(Qt::WA_DeleteOnClose, false);
	Settings::setLatestStatusId(0);
	Settings::self()->readConfig();
	twitter = new Backend;
	connect(twitter, SIGNAL(homeTimeLineRecived(QList< Status >&)), this, SLOT(homeTimeLinesRecived(QList< Status >&)));
	connect(twitter, SIGNAL(replyTimeLineRecived(QList< Status >&)), this, SLOT(replyTimeLineRecived(QList< Status >&)));
	connect(twitter, SIGNAL(sigPostNewStatusDone(bool)), this, SLOT(postingNewStatusDone(bool)));
    // tell the KXmlGuiWindow that this is indeed the main widget
	mainWidget = new QWidget;
    ui.setupUi(mainWidget);
	
	txtNewStatus = new StatusTextEdit(mainWidget);
	txtNewStatus->setObjectName("txtNewStatus");
	txtNewStatus->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	txtNewStatus->setMaximumHeight(70);
	txtNewStatus->setFocus(Qt::OtherFocusReason);
	txtNewStatus->setTabChangesFocus ( true );
	ui.inputLayout->addWidget(txtNewStatus);
	
	setCentralWidget(mainWidget);
    // then, setup our actions
    setupActions();
	statusBar()->show();
	setupGUI();
	
	setDefaultDirection();
	
	if(Settings::loadTimeLineOnStart())
		updateTimeLines();
	timelineTimer = new QTimer(this);
	timelineTimer->setInterval(Settings::updateInterval()*60000);
	timelineTimer->start();
	
	mediaMan = new MediaManagement(this);
	
	connect(timelineTimer, SIGNAL(timeout()), this, SLOT(updateTimeLines()));
	connect(txtNewStatus, SIGNAL(charsLeft(int)), this, SLOT(checkNewStatusCharactersCount(int)));
	connect(txtNewStatus, SIGNAL(returnPressed(QString&)), this, SLOT(postStatus(QString&)));
	connect(twitter, SIGNAL(sigError(QString&)), this, SLOT(error(QString&)));
	connect(this, SIGNAL(sigSetUserImage(StatusWidget*)), this, SLOT(setUserImage(StatusWidget*)));
}

MainWindow::~MainWindow()
{
	kDebug();
	//TODO Save Status list
	if(Settings::isSaveStatus()){
		saveStatuses(Settings::saveStatusCount());
	} else {
		Settings::setLatestStatusId(0);
	}
	Settings::self()->writeConfig();
}

void MainWindow::setupActions()
{
	KStandardAction::quit(qApp, SLOT(quit()), actionCollection());
	connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(quitApp()));
	KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());

	KAction *actUpdate = new KAction(KIcon("view-refresh"), i18n("Update timeline"), this);
	actionCollection()->addAction(QLatin1String("update_timeline"), actUpdate);
	actUpdate->setShortcut(Qt::Key_F5);
	actUpdate->setGlobalShortcutAllowed(true);
	actUpdate->setGlobalShortcut(KShortcut(Qt::ControlModifier | Qt::MetaModifier | Qt::Key_F5));
	connect(actUpdate, SIGNAL(triggered( bool )), this, SLOT(updateTimeLines()));
	
	KAction *newTwit = new KAction(this);
	newTwit->setIcon(KIcon("document-new"));
	newTwit->setText(i18n("Quick Twit"));
	actionCollection()->addAction(QLatin1String("choqok_new_twit"), newTwit);
	newTwit->setShortcut(Qt::ControlModifier | Qt::Key_T);
	newTwit->setGlobalShortcutAllowed(true);
	newTwit->setGlobalShortcut(KShortcut(Qt::ControlModifier | Qt::MetaModifier | Qt::Key_T));
}

void MainWindow::optionsPreferences()
{
	kDebug();
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

void MainWindow::checkNewStatusCharactersCount(int numOfChars)
{
	if(numOfChars < 30){
		ui.lblCounter->setStyleSheet("QLabel {color: red}");
	} else {
		ui.lblCounter->setStyleSheet("QLabel {color: green}");
	}
	ui.lblCounter->setText(i18n("%1", numOfChars));
}

void MainWindow::settingsChanged()
{
	kDebug();
// 	twitter->requestCurrentUser();
	setDefaultDirection();
	timelineTimer->setInterval(Settings::updateInterval()*60000);
}

void MainWindow::notify(const QString &message)
{
	statusBar()->showMessage( message, TIMEOUT);
}

void MainWindow::systemNotify(const QString title, const QString message, QString iconUrl)
{
	switch(Settings::notifyType()){
			case 0:
				break;
			case 1:
				break;
			case 2://Libnotify!
				QString libnotifyCmd = QString("notify-send -t ") + QString::number(Settings::notifyInterval()*1000) + QString(" -u low -i "+ iconUrl +" \"") + title + QString("\" \"") + message + QString("\"");
				QProcess::execute(libnotifyCmd);
				break;
		}
}

void MainWindow::updateTimeLines()
{
	kDebug();
	twitter->requestTimeLine(Backend::HomeTimeLine);
	twitter->requestTimeLine(Backend::ReplyTimeLine);
	if(Settings::latestStatusId()==0)
		isStartMode = true;
	else
		isStartMode = false;
	statusBar()->showMessage(i18n("Loading timelines..."));
}

void MainWindow::homeTimeLinesRecived(QList< Status > & statusList)
{
	kDebug();
	statusBar()->showMessage(i18n("Latest friends timeline recived!"), TIMEOUT);
	if(statusList.count()==0){
		kDebug()<<"Status list is empty";
		statusBar()->showMessage(i18n("No new twits recived, The list is up to date."), TIMEOUT);
		return;
	}
	QList<Status>::const_iterator it = statusList.constBegin();
	QList<Status>::const_iterator endIt = statusList.constEnd();
	for(;it!=endIt; ++it){
		if(!isStartMode){
			MainWindow::systemNotify(it->user.screenName, it->content,
									 mediaMan->getImageLocalPathIfExist(it->user.profileImageUrl));
		}
		StatusWidget *wt = new StatusWidget(this);
		wt->setCurrentStatus(*it);
		emit sigSetUserImage(wt);
		connect(wt, SIGNAL(sigReply(QString&, uint)), this, SLOT(prepareReply(QString&, uint)));
		listHomeStatus.append(wt);
		ui.frameHome->layout()->addWidget(wt);
	}
	uint latestId = statusList.first().statusId;
	if(latestId > Settings::latestStatusId()){
		kDebug()<<"Latest sets to: "<<latestId;
		Settings::setLatestStatusId(latestId);
	}
}

void MainWindow::replyTimeLineRecived(QList< Status > & statusList)
{
	kDebug();
	statusBar()->showMessage(i18n("Latest replies timeline recived!"), TIMEOUT);
	if(statusList.count()==0){
		kDebug()<<"Status list is empty";
		statusBar()->showMessage(i18n("No new twits received, The list is up to date."), TIMEOUT);
		return;
	}
	
	QList<Status>::const_iterator it = statusList.constBegin();
	QList<Status>::const_iterator endIt = statusList.constEnd();
	
	for(;it!=endIt; ++it){
		if(!isStartMode){
			MainWindow::systemNotify(it->user.screenName, it->content,
									 mediaMan->getImageLocalPathIfExist(it->user.profileImageUrl));
		}
		StatusWidget *wt = new StatusWidget(this);
		wt->setCurrentStatus(*it);
		emit sigSetUserImage(wt);
		connect(wt, SIGNAL(sigReply(QString&, uint)), this, SLOT(prepareReply(QString&, uint)));
		listReplyStatus.append(wt);
		ui.frameReply->layout()->addWidget(wt);
	}
	uint latestId = statusList.first().statusId;
	if(latestId > Settings::latestStatusId()){
		kDebug()<<"Latest sets to: "<<latestId;
		Settings::setLatestStatusId(latestId);
	}
}

void MainWindow::setDefaultDirection()
{
// 	this->setLayoutDirection((Qt::LayoutDirection)Settings::direction());
	ui.tabs->widget(0)->setLayoutDirection((Qt::LayoutDirection)Settings::direction());
	ui.tabs->widget(1)->setLayoutDirection((Qt::LayoutDirection)Settings::direction());
// 	txtNewStatus->document()->firstBlock()->
// 	inputLayout->setLayoutDirection((Qt::LayoutDirection)Settings::direction());
	txtNewStatus->setDefaultDirection((Qt::LayoutDirection)Settings::direction());
}

void MainWindow::error(QString & errMsg)
{
	MainWindow::systemNotify(i18n("Transaction faild"), errMsg, APPNAME);
}

void MainWindow::postStatus(QString & status)
{
	kDebug();
	//TODO will check for urls!
	if(status.size()>MAX_STATUS_SIZE){
		QString err = i18n("Status text size is more than server limit size.");
		error(err);
		return;
	}
	statusBar()->showMessage(i18n("Posting New status..."));
	txtNewStatus->setEnabled(false);
	twitter->postNewStatus(status);
}

void MainWindow::postingNewStatusDone(bool isError)
{
	if(!isError){
		txtNewStatus->setText(QString());
		MainWindow::systemNotify("Success!", "New status posted successfully", APPNAME);
	}
	txtNewStatus->setEnabled(true);
	txtNewStatus->setDefaultDirection((Qt::LayoutDirection)Settings::direction());
}

bool MainWindow::saveStatuses(int count)
{
	if(count >= listHomeStatus.count()){
		//Save all:
		
	} else {
		
	}
	return false;///When implement this function remove this!
}

void MainWindow::setUserImage(StatusWidget * widget)
{
	QString imgPath = mediaMan->getImageLocalPathDownloadIfNotExist(widget->currentStatus().user.screenName,
			 widget->currentStatus().user.profileImageUrl, this);
	widget->setUserImage(imgPath);
}

void MainWindow::prepareReply(QString &userName, uint statusId)
{
	kDebug();
	QString current = txtNewStatus->toPlainText();
	txtNewStatus->setText("@"+userName + " " + current);
	replyToStatusId = statusId;
	txtNewStatus->setDefaultDirection((Qt::LayoutDirection)Settings::direction());
}

void MainWindow::keyPressEvent(QKeyEvent * e)
{
	if(e->key() == Qt::Key_Escape){
		if(txtNewStatus->isEnabled()){
// 			this->close();
		} else {
			twitter->abortPostNewStatus();
		}
	}
}

void MainWindow::quitApp()
{
	deleteLater();
}

void MainWindow::abortPostNewStatus()
{
	kDebug();
	twitter->abortPostNewStatus();
}


#include "mainwindow.moc"
