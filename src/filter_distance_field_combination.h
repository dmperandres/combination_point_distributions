//LIC

#ifndef FILTER_DISTANCE_FIELD_COMBINATION_H
#define FILTER_DISTANCE_FIELD_COMBINATION_H

#include <QDialog>
#include <QGroupBox>
#include <QSlider>
#include <QLabel>
#include <QDoubleSpinBox>
#include "line_edit.h"
#include <string>
#include "filter.h"

#define DEFINED_FILTER_DISTANCE_FIELD_COMBINATION

namespace _f_distance_field_combination_ns
{
  // parameter 1
  const std::string String_group_box_parameter1("Mixing");
  const std::string String_label_parameter1_min("A");
  const std::string String_label_parameter1_max("B");
  const std::string String_label_parameter1_tooltip("Display the value of bright");
  const int Parameter1_min_value=0;
  const int Parameter1_max_value=250;
  const int Parameter1_single_step=1;
  const int Parameter1_page_step=10;
  const int Parameter1_tick_interval=15;
  const bool Parameter1_set_tracking=false;
  const std::string String_parameter1_slider_tooltip("Controls the appearance of bright");
//  const float Parameter1_middle_point=0.5f;
//  const float Parameter1_middle_value=0.8f;
  const float Parameter1_factor=250;

  // parameter 2
  const std::string String_group_box_parameter2("Limit A");
  const std::string String_label_parameter2_min("0");
  const std::string String_label_parameter2_max("1");
  const std::string String_label_parameter2_tooltip("Display the value of bright");
  const int Parameter2_min_value=0;
  const int Parameter2_max_value=250;
  const int Parameter2_single_step=1;
  const int Parameter2_page_step=10;
  const int Parameter2_tick_interval=15;
  const bool Parameter2_set_tracking=false;
  const std::string String_parameter2_slider_tooltip("Controls the appearance of bright");
//  const float Parameter2_middle_point=0.5f;
//  const float Parameter2_middle_value=0.8f;
  const float Parameter2_factor=250;

  // parameter 3
  const std::string String_group_box_parameter3("Limit B");
  const std::string String_label_parameter3_min("0");
  const std::string String_label_parameter3_max("1");
  const std::string String_label_parameter3_tooltip("Display the value of bright");
  const int Parameter3_min_value=0;
  const int Parameter3_max_value=250;
  const int Parameter3_single_step=1;
  const int Parameter3_page_step=10;
  const int Parameter3_tick_interval=15;
  const bool Parameter3_set_tracking=false;
  const std::string String_parameter3_slider_tooltip("Controls the appearance of bright");
//  const float Parameter3_middle_point=0.5f;
//  const float Parameter3_middle_value=0.8f;
  const float Parameter3_factor=250;

  // info 1
  const std::string String_group_box_info1("Number of dots");

  // Default values
  const float DISTANCE_FIELD_COMBINATION_MIXER_DEFAULT=0;
  const float DISTANCE_FIELD_COMBINATION_MIXER_MINIMUM_DEFAULT=0;
  const float DISTANCE_FIELD_COMBINATION_MIXER_MAXIMUM_DEFAULT=1;
  const float DISTANCE_FIELD_VALUE_FUNCTON_A_DEFAULT=0;
  const float DISTANCE_FIELD_VALUE_FUNCTON_B_DEFAULT=1;

  const std::string BOX_TEXT="Stippling Combination parameters";
}

class _gl_widget;
class _qtw_filter_distance_field_combination;

//HEA

class _filter_distance_field_combination : public _filter
{
public:
  _filter_distance_field_combination();
  void reset_data();
  bool change_output_image_size(){return Change_output_image_size;}
  bool use_dots(){return Use_dots;}

  void combination1(cv::Mat *Input_borders, cv::Mat *Input_areas, cv::Mat *Input_distance_field, cv::Mat *Output0);
  void combination2(cv::Mat *Input_borders, cv::Mat *Input_areas, cv::Mat *Input_distance_field, cv::Mat *Output0);
  void combination3(cv::Mat *Input_borders, cv::Mat *Input_areas, cv::Mat *Input_distance_field, cv::Mat *Output0);
  void combination4(cv::Mat *Input_borders, cv::Mat *Input_areas, cv::Mat *Input_distance_field, cv::Mat *Output0);
  void combination5(cv::Mat *Input_borders, cv::Mat *Input_areas, cv::Mat *Input_distance_field, cv::Mat *Output0);

  void combination(cv::Mat *Input_borders, cv::Mat *Input_areas, cv::Mat *Input_distance_field, cv::Mat *Output0);
  void update_distance_field(cv::Mat *Input_borders,cv::Mat *Input_areas,cv::Mat *Input_distance_field);
  void update_filter();
  void update();

  void mixer(float Mixer1){Mixer=Mixer1;}
  float mixer(){return Mixer;}

  void value_function_a(float Value_function_a1){Value_function_a=Value_function_a1;}
  float value_function_a(){return  Value_function_a;}

  void value_function_b(float Value_function_b1){Value_function_b=Value_function_b1;}
  float value_function_b(){return  Value_function_b;}

   virtual void info1(unsigned int Value){Q_UNUSED(Value)}

  void read_parameters(std::map<std::string,std::string> &Parameters);
  void write_parameters(std::map<std::string,std::string> &Parameters);

protected:
  int Type_image;
  float Mixer;
  float Max_distance;
  float Value_function_a;
  float Value_function_b;
  unsigned int Num_black_pixels;
};

//HEA

class _filter_distance_field_combination_ui : public _filter_distance_field_combination
{
public:
  _filter_distance_field_combination_ui(_gl_widget *GL_widget1, std::string Name1="Distance field float parameters");
  _filter_distance_field_combination_ui(_gl_widget *GL_widget1,std::map<std::string,std::string> &Parameters,std::string Name1="Distance field float parameters");
  ~_filter_distance_field_combination_ui();
  void reset_data();

  void show();
  void hide();
  void *get_link();
  void get_parameters(std::map<std::string,std::string> &Parameters);

  void parameter1(float Value){_filter_distance_field_combination::mixer(Value);}
  float parameter1(){return _filter_distance_field_combination::mixer();}

  void parameter2(float Value){_filter_distance_field_combination::value_function_a(Value);}
  float parameter2(){return _filter_distance_field_combination::value_function_a();}

  void parameter3(float Value){_filter_distance_field_combination::value_function_b(Value);}
  float parameter3(){return _filter_distance_field_combination::value_function_b();}

  void info1(unsigned int Value1);

private:
  _qtw_filter_distance_field_combination *Qtw_filter_distance_field_combination=nullptr;
};

//HEA

class _qtw_filter_distance_field_combination: public QWidget
{
  Q_OBJECT
public:

  _qtw_filter_distance_field_combination(_gl_widget *GL_widget1,_filter_distance_field_combination_ui *Filter1,std::string Box_name="Distance field float parameters");

  void show(){Group_box_main->show();}
  void hide(){Group_box_main->hide();}
  QGroupBox *get_link(){return Group_box_main;}

  void parameter1(float Value);
  void parameter2(float Value);
  void parameter3(float Value);

  void info(unsigned int Value);

protected slots:
  void parameter1_slot(int Value);
  void parameter2_slot(int Value);
  void parameter3_slot(int Value);

private:
  QGroupBox *Group_box_main=nullptr;
  QGroupBox *Group_box_parameter1=nullptr;
  QGroupBox *Group_box_parameter2=nullptr;
  QGroupBox *Group_box_parameter3=nullptr;
  // info
  QGroupBox *Group_box_info1=nullptr;

  // Mixer
  QSlider *Slider_parameter1=nullptr;
  QLineEdit *Line_edit_parameter1=nullptr;

  // Value function a
  QSlider *Slider_parameter2=nullptr;
  QLineEdit *Line_edit_parameter2=nullptr;

  // Value function b
  QSlider *Slider_parameter3=nullptr;
  QLineEdit *Line_edit_parameter3=nullptr;

  // number of dots
   QLineEdit *Line_edit_info1=nullptr;

  _filter_distance_field_combination_ui *Filter=nullptr;
  _gl_widget *GL_widget=nullptr;
};

#endif
