//
// C++ Interface: statustextedit
//
// Description: 
//
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef STATUSTEXTEDIT_H
#define STATUSTEXTEDIT_H

#include <ktextedit.h>

/**
	@author 
*/
class StatusTextEdit : public KTextEdit
{
	Q_OBJECT
public:
    StatusTextEdit(QWidget *parent=0);

    ~StatusTextEdit();
	
protected:
	void keyPressEvent(QKeyEvent *e);
	
signals:
	void returnPressed();
};

#endif
