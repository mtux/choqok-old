//
// C++ Implementation: statustextedit
//
// Description: 
//
//
// Author:  <>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "statustextedit.h"
#include <QKeyEvent>
StatusTextEdit::StatusTextEdit(QWidget *parent)
 : KTextEdit(parent)
{
	this->setAcceptRichText(false);
}


StatusTextEdit::~StatusTextEdit()
{
}

void StatusTextEdit::keyPressEvent(QKeyEvent * e)
{
	if ((e->key() == Qt::Key_Return) || (e->key() == Qt::Key_Enter)) {
		emit returnPressed();
		e->accept();
	}
}


