/*
 * mainwindow.h
 *
 * Copyright (C) 2008 Mehrdad Momeny <mehrdad.momeny@gmail.com>
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <kxmlguiwindow.h>

#include "ui_prefs_base.h"
#include "ui_mainwindow_base.h"
#include "ui_accounts_base.h"
#include "datacontainers.h"


#define TIMEOUT 5000
class Backend;
class StatusTextEdit;
class StatusWidget;
class QTimer;
class MediaManagement;
/**
 * This class serves as the main window for choqoK.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author Mehrdad Momeny <mehrdad.momeny@gmail.com>
 */
class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    MainWindow();

    /**
     * Default Destructor
     */
	virtual ~MainWindow();
	
	static void systemNotify(const QString title, const QString message, QString iconUrl);

protected slots:
    void optionsPreferences();
	void settingsChanged();
	
	void updateTimeLines();
	void homeTimeLinesRecived(QList<Status> &statusList);
	void replyTimeLineRecived(QList<Status> &statusList);
	void postingNewStatusDone(bool isError);
	void prepareReply(QString &userName, uint statusId);

	void notify(const QString &message);
	
	void checkNewStatusCharactersCount(int numOfChars);
	
// 	void postStatus();
	void postStatus(QString &status);
	
	void error(QString &errMsg);
	
	void setUserImage(StatusWidget *widget);
	
	void quitApp();
	
	void abortPostNewStatus();
	
signals:
	void sigSetUserImage(StatusWidget *widget);
	
protected:
	void keyPressEvent(QKeyEvent * e);

private:
    void setupActions();
	void setDefaultDirection();
// 	void setTxtNewStatusDirection();
// 	QString prepareNewStatus(QString newStatus=QString());
	/**
	 *    Will store @count count of current statuses
	 * @param count 
	 * @return True on success, and false on failer
	 */
	bool saveStatuses(int count);

private:
	QTimer *timelineTimer;
	Ui::MainWindow_base ui;
    Ui::prefs_base ui_prefs_base;
	Ui::accounts_base ui_accounts_base;
	QWidget *mainWidget;
	Backend *twitter;
	StatusTextEdit *txtNewStatus;
	QList<StatusWidget*> listHomeStatus;
	QList<StatusWidget*> listReplyStatus;
	MediaManagement *mediaMan;
	uint replyToStatusId;
	
	bool isStartMode;//used for Notify, if true: notify will not send for any or all new twits, if false will send.
	
//     KToggleAction *m_toolbarAction;
//     KToggleAction *m_statusbarAction;
};

#endif 
