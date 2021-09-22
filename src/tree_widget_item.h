//LIC

#ifndef TREE_WIDGET_ITEM_H
#define TREE_WIDGET_ITEM_H

#include <QTreeWidgetItem>
#include <QMouseEvent>
#include <iostream>

//HEA

class _tree_widget_item : public QTreeWidgetItem
{
public:
  _tree_widget_item(QTreeWidget *Parent, int Type1 = Type):QTreeWidgetItem(Parent,Type1){}
  _tree_widget_item(_tree_widget_item *Tree_widget_item1, int Type1 = Type):QTreeWidgetItem(Tree_widget_item1,Type1){}
};

#endif
