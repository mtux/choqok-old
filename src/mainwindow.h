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
class Backend;
class StatusTextEdit;
/**
 * This class serves as the main window for Kwitter.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author Mehrdad Momeny <mehrdad.momeny@gmail.com>
 * @version 0.1
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

protected slots:
    void optionsPreferences();
	void settingsChanged();
	
	void updateTimeLines();
	void homeTimeLinesRecived(QList<Status> &statusList);
	void replayTimeLineRecived(QList<Status> &statusList);

	void notify(const QString &title, const QString &message);
	
	void checkNewStatusCharactersCount();
	
	void postStatus();
	
	void error(QString &errMsg);

private:
    void setupActions();
	void setDefaultDirection();

private:
	Ui::MainWindow_base ui;
    Ui::prefs_base ui_prefs_base;
	Ui::accounts_base ui_accounts_base;
	QWidget *mainWidget;
	Backend *twitter;
	StatusTextEdit *txtNewStatus;
//     KToggleAction *m_toolbarAction;
//     KToggleAction *m_statusbarAction;
};

#endif 
