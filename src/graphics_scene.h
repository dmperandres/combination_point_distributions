//LIC

#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include <QGraphicsScene>
#include <QMenu>
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsView>
#include <QScrollBar>
#include <QMimeData>
#include <iostream>

class _window;

//HEA
// This class is for foing the drag and drop od the tree items

class _graphics_scene : public QGraphicsScene
{
Q_OBJECT

public:
  _graphics_scene(_window *Window1){Window=Window1;}

protected:
  void dragEnterEvent(QGraphicsSceneDragDropEvent *Event);
  void dragMoveEvent(QGraphicsSceneDragDropEvent *Event);
  void dropEvent(QGraphicsSceneDragDropEvent *Event);

private:
  _window *Window;
};
#endif
