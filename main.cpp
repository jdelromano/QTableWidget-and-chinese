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

#include <QtWidgets>

#include "standarddelegate.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTableWidget tableWidget(3, 1);
    tableWidget.setItemDelegate(new StandardDelegate());
    tableWidget.setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::AnyKeyPressed);
    tableWidget.setSelectionBehavior(QAbstractItemView::SelectItems);

    tableWidget.setItem(0, 0, new QTableWidgetItem("Use this example with pinyin"));
    tableWidget.setItem(1, 0, new QTableWidgetItem("here type 'chang'"));
    tableWidget.setItem(2, 0, new QTableWidgetItem("here type 'cukun'"));

    tableWidget.resizeColumnsToContents();
    tableWidget.resize(300, 200);
    tableWidget.show();

    return app.exec();
}
