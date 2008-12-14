//
// C++ Interface: systrayicon
//
// Description: 
//
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SYSTRAYICON_H
#define SYSTRAYICON_H

#include <ksystemtrayicon.h>
#include "mainwindow.h"
/**
System tray icon!

	@author Mehrdad Momeny <mehrdad.momeny@gmail.com>
*/
class SysTrayIcon : public KSystemTrayIcon
{
	Q_OBJECT
public:
    SysTrayIcon(QWidget* parent=0);

    ~SysTrayIcon();
public slots:
	void quitApp();
private:
	void setupActions();
	MainWindow *mainWin;
};

#endif
