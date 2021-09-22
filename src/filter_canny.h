//LIC

#ifndef FILTER_CANNY_H
#define FILTER_CANNY_H

#include <QDialog>
#include <QGroupBox>
#include <QSlider>
#include <QLabel>
#include "line_edit.h"
#include <string>
#include "filter.h"

#define DEFINED_FILTER_CANNY

namespace _f_canny_ns
{
  // parameter 1
  const std::string String_group_box_parameter1("Canny size");
  const std::string String_label_parameter1_min("3");
  const std::string String_label_parameter1_max("7");
  const std::string String_label_parameter1_tooltip("Display the value of Canny size");
  const int Parameter1_min_value=1;
  const int Parameter1_max_value=3;
  const int Parameter1_single_step=1;
  const int Parameter1_page_step=1;
  const int Parameter1_tick_interval=1;
  const bool Parameter1_set_tracking=false;
  const std::string String_parameter1_tooltip("Controls the appearance of borders: more to the left, less to the right");

  // parameter 2
  const std::string String_group_box_parameter2("Threshold 1");
  const std::string String_label_parameter2_min("0 ");
  const std::string String_label_parameter2_max("255");
  const std::string String_label_parameter2_tooltip("Display the value of threshold 1");
  const int Parameter2_min_value=0;
  const int Parameter2_max_value=255;
  const int Parameter2_single_step=1;
  const int Parameter2_page_step=10;
  const int Parameter2_tick_interval=15;
  const bool Parameter2_set_tracking=false;
  const std::string String_parameter2_slider_tooltip("Controls the appearance of borders: more to the left, less to the right");

  //parameter3
  const std::string String_group_box_parameter3("Threshold 2");
  const std::string String_label_parameter3_min("0 ");
  const std::string String_label_parameter3_max("255");
  const std::string String_label_parameter3_tooltip("Display the value of threshold 2");
  const int Parameter3_min_value=0;
  const int Parameter3_max_value=255;
  const int Parameter3_single_step=1;
  const int Parameter3_page_step=10;
  const int Parameter3_tick_interval=15;
  const bool Parameter3_set_tracking=false;
  const std::string String_parameter3_slider_tooltip("Controls the appearance of borders: more to the left, less to the right");

  // Default values
  const int CANNY_SIZE_DEFAULT=3;// take care of the previous warning
  const int CANNY_THRESHOLD1_DEFAULT=100;
  const int CANNY_THRESHOLD2_DEFAULT=200;

  const std::string BOX_TEXT="Canny parameters";
}

class _gl_widget;
class _qtw_filter_canny;

//HEA

class _filter_canny : public _filter
{
public:
  _filter_canny();
  void reset_data();
  bool change_output_image_size(){return Change_output_image_size;}
  bool use_dots(){return Use_dots;}

  void update_filter();
  void update();
  
  void canny_size(int Size);
  int canny_size(){return Canny_size;}

  void threshold1(int Threshold);
  int threshold1(){return Threshold1;}

  void threshold2(int Threshold);
  int threshold2(){return Threshold2;}

  void read_parameters(std::map<std::string,std::string> &Parameters);
  void write_parameters(std::map<std::string,std::string> &Parameters);

protected:
  int Canny_size,Threshold1,Threshold2;
};

//HEA

class _filter_canny_ui : public _filter_canny
{
public:
  _filter_canny_ui(_gl_widget *GL_widget1, std::string Name1="Canny parameters");
  _filter_canny_ui(_gl_widget *GL_widget1,std::map<std::string,std::string> &Parameters,std::string Name1="Canny parameters");
  ~_filter_canny_ui();
  void reset_data();

  void show();
  void hide();
  void *get_link();  
  void get_parameters(std::map<std::string,std::string> &Parameters);

  void parameter1(int Value){_filter_canny::canny_size(Value);}
  int parameter1(){return _filter_canny::canny_size();}

  void parameter2(int Value){_filter_canny::threshold1(Value);}
  int parameter2(){return _filter_canny::threshold1();}

  void parameter3(int Value){_filter_canny::threshold2(Value);}
  int parameter3(){return _filter_canny::threshold2();}

private:
  _qtw_filter_canny *Qtw_filter_canny=nullptr;
};

//HEA

class _qtw_filter_canny: public QWidget
{
  Q_OBJECT
public:
  _qtw_filter_canny(_gl_widget *GL_widget1,_filter_canny_ui *Filter1,std::string Box_name="Canny parameters");

  void show(){Group_box_main->show();}
  void hide(){Group_box_main->hide();}
  QGroupBox *get_link(){return Group_box_main;}

  void parameter1(int Value);
  void parameter2(int Value);
  void parameter3(int Value);

protected slots:
  void parameter1_slot(int Value);
  void parameter2_slot(int Value);
  void parameter3_slot(int Value);

private:
  QGroupBox *Group_box_main=nullptr;
  QGroupBox *Group_box_parameter1=nullptr;
  QGroupBox *Group_box_parameter2=nullptr;
  QGroupBox *Group_box_parameter3=nullptr;

  // canny size
  QSlider *Slider_parameter1=nullptr;
  QLineEdit *Line_edit_parameter1=nullptr;

  // threshold1
  QSlider *Slider_parameter2=nullptr;
  QLineEdit *Line_edit_parameter2=nullptr;

  // threshold2
  QSlider *Slider_parameter3=nullptr;
  QLineEdit *Line_edit_parameter3=nullptr;

  _filter_canny_ui *Filter=nullptr;
  _gl_widget *GL_widget=nullptr;
};
#endif
