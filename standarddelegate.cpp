// Copyright [2017] [Banana.ch SA - Lugano Switzerland]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include <QLineEdit>
#include <QString>
#include <QWidget>
#include <QKeyEvent>

#include "standarddelegate.h"
#include "inputspecials.hpp"


QWidget *StandardDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &/*option*/,
                                        const QModelIndex &index) const

{
    QLineEdit * lineEdit = new QLineEdit(parent);
    return lineEdit;

}

void StandardDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
    if (lineEdit)
    {
        QString textToSet = index.data(Qt::EditRole).toString();
        if (!m_lastKeyPressedText.isEmpty() && m_lastKeyPressedText.at(0).isLetter())
        {
            if (InputSpecials::isUsingImeKeyboard() )
            {
                editor->setProperty("DelegateToIme", true);
                textToSet.clear();
            }
        }
        lineEdit->setText(textToSet);
        return;
    }
}

bool StandardDelegate::eventFilter ( QObject * object, QEvent * ev )
{
    if(ev && ev->type() == QEvent::KeyPress)
       {

          QWidget *editor = qobject_cast<QWidget*>(object);
          if (editor)
          {
             QKeyEvent* keyEv = static_cast<QKeyEvent *>(ev);
             QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
             if(lineEdit && lineEdit->property("DelegateToIme").toBool())
             {
                editor->setProperty("DelegateToIme", false);
                ev->accept();
                InputSpecials::NotifyToIme(lineEdit, keyEv);
                return true;
             }
          }
    }
    return QStyledItemDelegate::eventFilter(object, ev);
}


bool StandardDelegate::editorEvent( QEvent * e, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index )
{
   m_lastKeyPressed = -1;
   m_lastKeyPressedText.clear();
   if(e->type() == QEvent::KeyPress)
   {
       QKeyEvent* keyEvent = static_cast<QKeyEvent *>(e);
       if(keyEvent)
       {
            m_lastKeyPressed = keyEvent->key();
            m_lastKeyPressedText = keyEvent->text();
       }
   }
   return QStyledItemDelegate::editorEvent(e, model, option, index);
}
