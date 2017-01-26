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

#ifndef STARDELEGATE_H
#define STARDELEGATE_H

#include <QStyledItemDelegate>

class StandardDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    StandardDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;

protected:
    bool editorEvent ( QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index ) Q_DECL_OVERRIDE;
    bool eventFilter(QObject * object, QEvent * ev) Q_DECL_OVERRIDE;

    int m_lastKeyPressed;
    QString m_lastKeyPressedText;

};

#endif
