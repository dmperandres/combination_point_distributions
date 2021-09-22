//LIC

#include "graphics_view.h"

//HEA

_graphics_view::_graphics_view(QWidget *Parent):QGraphicsView(Parent)
{
  Factor=1.0;
}


//HEA

void _graphics_view::scaling_factor(float Factor1)
{
  Factor=Factor1;
  resetTransform();
  scale(Factor,Factor);
  horizontalScrollBar()->setValue(-1000);
  verticalScrollBar()->setValue(-1000);
}

//HEA

void _graphics_view::wheelEvent(QWheelEvent *Event)
{
  // See QWheelEvent documentation
  int Num_degrees = Event->delta()/8;
  int Num_steps = Num_degrees / 15;

  if (Num_steps<0) Factor*=1.1f;
  else Factor/=1.1f;

  resetTransform();
  scale(Factor,Factor);
  horizontalScrollBar()->setValue(-1000);
  verticalScrollBar()->setValue(-1000);
}
