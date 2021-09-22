//LIC

#ifndef FILTER_DISTANCE_FIELD_FLOAT_H
#define FILTER_DISTANCE_FIELD_FLOAT_H

#include <QDialog>
#include <QGroupBox>
#include <QSlider>
#include <QLabel>
#include <string>

#include "line_edit.h"
#include "filter.h"

#define DEFINED_FILTER_DISTANCE_FIELD_FLOAT

namespace _f_distance_field_float_ns
{
  class _point
  {
  public:
    float Distance=-1;
    int x=-1;
    int y=-1;
  };
}

class _gl_widget;
class _qtw_filter_distance_field_float;

//HEA

class _filter_distance_field_float : public _filter
{
public:
  _filter_distance_field_float();
  void reset_data();
  bool change_output_image_size(){return Change_output_image_size;}
  bool use_dots(){return Use_dots;}

  void jump_flooding(cv::Mat *Input_image, cv::Mat *Output_image);
  void update_filter();
  void update();

  void read_parameters(std::map<std::string,std::string> &Parameters){Q_UNUSED(Parameters)};
  void write_parameters(std::map<std::string,std::string> &Parameters){Q_UNUSED(Parameters)};
};

//HEA

class _filter_distance_field_float_ui : public _filter_distance_field_float
{
public:
  _filter_distance_field_float_ui(_gl_widget *GL_widget1,std::string Name1="Distance field float parameters");
  _filter_distance_field_float_ui(_gl_widget *GL_widget1,std::map<std::string,std::string> &Parameters, std::string Name1="Distance field float parameters");
  ~_filter_distance_field_float_ui();
  void reset_data();

  void show();
  void hide();
  void *get_link();

private:
  _qtw_filter_distance_field_float *Qtw_filter_distance_field_float=nullptr;
};

//HEA

class _qtw_filter_distance_field_float: public QWidget
{
  Q_OBJECT
public:

  _qtw_filter_distance_field_float(_gl_widget *GL_widget1,_filter_distance_field_float_ui *Filter1,std::string Box_name="Distance field float parameters");

  void show(){}
  void hide(){}
  QGroupBox *get_link(){return Group_box_main;}

private:
  QGroupBox *Group_box_main=nullptr;
  QGroupBox *Group_box_parameter1=nullptr;

  //
  QSlider *Slider_parameter1=nullptr;
  QLineEdit *Line_edit_parameter1=nullptr;

  _filter_distance_field_float_ui *Filter=nullptr;
  _gl_widget *GL_widget=nullptr;
};
#endif
