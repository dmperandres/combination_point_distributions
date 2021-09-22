//LIC

#ifndef FILTER_DOTS_EDGES_H
#define FILTER_DOTS_EDGES_H

#include <QDialog>
#include <QGroupBox>
#include <QSlider>
#include <QLabel>
#include <QDoubleSpinBox>
#include <string>

#include "line_edit.h"
#include "filter.h"

#define DEFINED_FILTER_DOTS_EDGES

namespace _f_dots_edges_ns
{
  // border umbral
  // parameter 1
  const std::string String_group_box_parameter1("Distance inter dots");

  // parameter 2: Displacement randomness
  const std::string String_group_box_parameter2("Displacement randomness");
  const std::string String_label_parameter2_min("0");
  const std::string String_label_parameter2_max("1");
  const std::string String_label_parameter2_tooltip("Display the value of displacement randomness");
  const int Parameter2_min_value=0;
  const int Parameter2_max_value=250;
  const int Parameter2_single_step=1;
  const int Parameter2_page_step=5;
  const int Parameter2_tick_interval=25;
  const bool Parameter2_set_tracking=false;
  const std::string String_parameter2_tooltip("Controls the random displacement of dots");
  const float Parameter2_factor=250;

  // Default values
  const float DOTS_EDGES_MIN_DISTANCE_DEFAULT=1.0;
  const float DOTS_EDGES_MAX_DISTANCE_DEFAULT=15.0;
  const float DOTS_EDGES_DISTANCE_DEFAULT=1.0;
  const float DOTS_EDGES_DISPLACEMENT_RANDONESS_DEFAULT=0.0;
}

class _gl_widget;
class _qtw_filter_dots_edges;

//HEA

class _filter_dots_edges : public _filter
{
public:
  _filter_dots_edges();
  void reset_data();
  bool change_output_image_size(){return Change_output_image_size;}
  bool use_dots(){return Use_dots;}

  int search_next_pixel(int Row, int Col, cv::Mat *Aux, int &Row1, int &Col1, float &Distance1);
  void border(cv::Mat *Input0,cv::Mat *Output0);
  void update_filter();
  void update();

  void distance(float Distance1){Distance=Distance1;}
  float distance(){return Distance;}

  void displacement_randomness(float Displacement_randomness1){Displacement_randomness=Displacement_randomness1;}
  float displacement_randomness(){return Displacement_randomness;}

  void read_parameters(std::map<std::string,std::string> &Parameters);
  void write_parameters(std::map<std::string, std::string> &Parameters);

protected:
  float Distance;
  float Displacement_randomness;
};

//HEA

class _filter_dots_edges_ui :public _filter_dots_edges
{
public:
  _filter_dots_edges_ui(_gl_widget *GL_widget1,std::string Name1="Dots edges parameters");
  _filter_dots_edges_ui(_gl_widget *GL_widget1, std::map<std::string,std::string> &Parameters, std::string Name1="Dots edges parameters");
  ~_filter_dots_edges_ui();
  void reset_data();

  void show();
  void hide();
  void *get_link();

  void parameter1(float Value){_filter_dots_edges::distance(Value);}
  float parameter1(){return _filter_dots_edges::distance();}

  void parameter2(float Value){_filter_dots_edges::displacement_randomness(Value);}
  float parameter2(){return _filter_dots_edges::displacement_randomness();}

private:
  _qtw_filter_dots_edges *Qtw_filter_dots_edges=nullptr;
};

//HEA

class _qtw_filter_dots_edges: public QWidget
{
  Q_OBJECT
public:

  _qtw_filter_dots_edges(_gl_widget *GL_widget1,_filter_dots_edges_ui *Filter1,std::string Box_name="Dots edges parameters");
  void show(){Group_box_main->show();}
  void hide(){Group_box_main->hide();}
  QGroupBox *get_link(){return Group_box_main;}

  void parameter1(float Value);
  void parameter2(float Value);

protected slots:
  void parameter1_slot(double Value);
  void parameter2_slot(int Value);

private:
  QGroupBox *Group_box_main=nullptr;
  QGroupBox *Group_box_parameter1=nullptr;
  QGroupBox *Group_box_parameter2=nullptr;

  // 1 Distance
  QDoubleSpinBox *Doublespinbox_parameter1=nullptr;

  // 2 displacemente factor
  QSlider *Slider_parameter2=nullptr;
  QLineEdit *Line_edit_parameter2=nullptr;

  _filter_dots_edges_ui *Filter=nullptr;
  _gl_widget *GL_widget=nullptr;
};
#endif
