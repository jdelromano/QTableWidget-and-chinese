# QTableWidget-and-chinese
A possible solution for typing with pinyin in a QTableWidget

Here I tried a workaround using the patch provided by Sebastian Sauer at https://bugreports.qt.io/browse/QTBUG-54848

## What's the bug
With a Qt table, at the moment, typing in a HAN based language such as Chinese or Japanese is not really comfortable.
The first typed charachter is not sent to the IME system (i.e. pinyin) but directly to the editor created by the QAbstractItemDelegate.

When typing with pinyin, this is the event flow that is triggered

 1. 'QStyledItemDelegate::editorEvent'   
 Here we catch the event that triggered the editing    
 1. 'QStyledItemDelegate::createEditor'
 creates the editor (QLineEdit), it may trigger a workload.
 1. 'QStyledItemDelegate::setEditorData'
 the editor is creator and we have the possibilities to recall the data from the model, it may trigger a workload.
 1. 'QStyledItemDelegate::eventFilter'
 After all the above, the keyPressEvent is finally dispatched to the new editor, at this point we call the workaround.
 
 