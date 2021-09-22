//LIC

#ifndef FILTER_LOAD_IMAGE
#define FILTER_LOAD_IMAGE

#include <QDialog>
#include <QGroupBox>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include "line_edit.h"
#include <string>
#include "filter.h"

#include "image_IO.h"

#define DEFINED_LOAD_IMAGE

namespace _f_load_image_ns
{
  // parameter 1
  const std::string String_group_box_parameter1("Mode");
  const std::string String_checkbox_parameter1("Load image");

//  const std::string String_label_parameter1_min("3");
//  const std::string String_label_parameter1_max("7");
//  const std::string String_label_parameter1_tooltip("Display the value of Canny size");
//  const int Parameter1_min_value=1;
//  const int Parameter1_max_value=3;
//  const int Parameter1_single_step=1;
//  const int Parameter1_page_step=1;
//  const int Parameter1_tick_interval=1;
//  const bool Parameter1_set_tracking=false;
//  const std::string String_parameter1_tooltip("Controls the appearance of borders: more to the left, less to the right");

//  // parameter 2
//  const std::string String_group_box_parameter2("Threshold 1");
//  const std::string String_label_parameter2_min("0 ");
//  const std::string String_label_parameter2_max("255");
//  const std::string String_label_parameter2_tooltip("Display the value of threshold 1");
//  const int Parameter2_min_value=0;
//  const int Parameter2_max_value=255;
//  const int Parameter2_single_step=1;
//  const int Parameter2_page_step=10;
//  const int Parameter2_tick_interval=15;
//  const bool Parameter2_set_tracking=false;
//  const std::string String_parameter2_slider_tooltip("Controls the appearance of borders: more to the left, less to the right");

//  //parameter3
//  const std::string String_group_box_parameter3("Threshold 2");
//  const std::string String_label_parameter3_min("0 ");
//  const std::string String_label_parameter3_max("255");
//  const std::string String_label_parameter3_tooltip("Display the value of threshold 2");
//  const int Parameter3_min_value=0;
//  const int Parameter3_max_value=255;
//  const int Parameter3_single_step=1;
//  const int Parameter3_page_step=10;
//  const int Parameter3_tick_interval=15;
//  const bool Parameter3_set_tracking=false;
//  const std::string String_parameter3_slider_tooltip("Controls the appearance of borders: more to the left, less to the right");

  // Default values
//  const bool load_image_DEFAULT=true;// take care of the previous warning
//  const int CANNY_THRESHOLD1_DEFAULT=100;
//  const int CANNY_THRESHOLD2_DEFAULT=200;

//  enum class _state:unsigned char{STATE_NO_IMAGE,STATE_LOAD_IMAGE,STATE_IMAGE_LOADED};

  const std::string BOX_TEXT="Load image parameters";
}

class _gl_widget;
class _qtw_filter_load_image;

//HEA

class _filter_load_image : public _filter
{
public:
  _filter_load_image();
  void reset_data();
  bool change_output_image_size(){return Change_output_image_size;}
  bool use_dots(){return Use_dots;}

  void update_filter();
  void update();
  
  void file_name(std::string File_name1){File_name=File_name1;Load_image=true;}
  std::string file_name(){return File_name;}

  void read_parameters(std::map<std::string,std::string> &Parameters);
  void write_parameters(std::map<std::string,std::string> &Parameters);

protected:
  std::string File_name;
  bool Load_image=true;
};

//HEA

class _filter_load_image_ui : public _filter_load_image
{
public:
  _filter_load_image_ui(_gl_widget *GL_widget1, std::string Name1=_f_load_image_ns::BOX_TEXT);
  _filter_load_image_ui(_gl_widget *GL_widget1,std::map<std::string,std::string> &Parameters,std::string Name1=_f_load_image_ns::BOX_TEXT);
  ~_filter_load_image_ui();
  void reset_data();

  void show();
  void hide();
  void *get_link();

  void parameter1(std::string File_name){_filter_load_image::file_name(File_name);}
  std::string parameter1(){return _filter_load_image::file_name();}

private:
  _qtw_filter_load_image *Qtw_filter_load_image=nullptr;
};

//HEA

class _qtw_filter_load_image: public QWidget
{
  Q_OBJECT
public:
  _qtw_filter_load_image(_gl_widget *GL_widget1,_filter_load_image_ui *Filter1,std::string Box_name=_f_load_image_ns::BOX_TEXT);

  void show(){Group_box_main->show();}
  void hide(){Group_box_main->hide();}
  QGroupBox *get_link(){return Group_box_main;}

private slots:
  void parameter1_slot();

private:
  QGroupBox *Group_box_main=nullptr;

  _filter_load_image_ui *Filter=nullptr;
  _gl_widget *GL_widget=nullptr;
};
#endif
