//LIC

#ifndef FILTER_STIPPLING_EBG_H
#define FILTER_STIPPLING_EBG_H

#include <QDialog>
#include <QGroupBox>
#include <QSlider>
#include <QLabel>
#include <QComboBox>
#include <memory>
#include <string>

#include "line_edit.h"
#include "filter.h"
#include "random.h"
#include "data_dots_ebg_ext.h"

#define DEFINED_FILTER_EBG

namespace _f_stippling_ebg_ns
{
  const std::vector<std::string> Vec_nom_output_size={"Optimal for input","A4","A5","A6","A7","A8","A9","A10"};

  // parameter 1: output size
  const std::string String_group_box_parameter1("Output size");

  // parameter 2: pixel density
  const std::string String_group_box_parameter2("Pixel density");

  // parameter 3 displacement randomenes
  const std::string String_group_box_parameter3("Displacement ramdomness");
  const std::string String_label_parameter3_min("0 ");
  const std::string String_label_parameter3_max("1");
  const std::string String_label_parameter3_tooltip("Display the value of displacement ramdomness");
  const int Parameter3_min_value=0;
  const int Parameter3_max_value=100;
  const int Parameter3_single_step=1;
  const int Parameter3_page_step=5;
  const int Parameter3_tick_interval=10;
  const bool Parameter3_set_tracking=false;
  const std::string String_parameter3_tooltip("Controls the ramdom displacement of dots");
  // warning: the capture value must be adjusted to a odd value
  const int Parameter3_factor=100;

  // parameter 4: output mode
  const std::string String_group_box_parameter4("Output mode");

  // parameter 5: Black level
  const std::string String_group_box_parameter5("Black level");
  const std::string String_label_parameter5_min("0  ");
  const std::string String_label_parameter5_max("255");
  const std::string String_label_parameter5_tooltip("Display the value of the black level");
  const int parameter5_min_value=0;
  const int parameter5_max_value=255;
  const int parameter5_single_step=1;
  const int parameter5_page_step=5;
  const int parameter5_tick_interval=10;
  const bool parameter5_set_tracking=false;
  const std::string String_parameter5_tooltip("Controls the black level used as a threshold");

  // info 1
  const std::string String_group_box_info1("Number of dots");

  // Default values
  const int BLACK=0.;
  const int WHITE=255.;

  const int VEC_PIXEL_DENSITY[3]={300,600,1200};
  const int VEC_PIXEL_DENSITY_FACTOR[3]={1,2,4};

  const unsigned short MIN_DOT_SIZE=4;
  const unsigned short MAX_DOT_SIZE=8;

  const int AVERAGE_DOT_SIZE=(MAX_DOT_SIZE+MIN_DOT_SIZE)/2;

  const unsigned short NUM_ROWS_DOT_MATRIX=4;
  const unsigned short NUM_COLS_DOT_MATRIX=10;

  const float STIPPLING_EBG_SCALING_FACTOR_DEFAULT=3.0;
  const float STIPPLING_EBG_DISPLACEMENT_DEFAULT=0.25;
  const int STIPPLING_EBG_NUM_ROWS_DOT_MATRIX_DEFAULT=NUM_ROWS_DOT_MATRIX;
  const int STIPPLING_EBG_NUM_COLS_DOT_MATRIX_DEFAULT=NUM_COLS_DOT_MATRIX;
  const int STIPPLING_EBG_MIN_DOT_SIZE_DEFAULT=MIN_DOT_SIZE;
  const int STIPPLING_EBG_MAX_DOT_SIZE_DEFAULT=MAX_DOT_SIZE;
  const _dots_ebg_ext_ns::_output_size STIPPLING_EBG_OUTPUT_SIZE_DEFAULT=_dots_ebg_ext_ns::_output_size::_OUTPUT_SIZE_OPTIMAL;
  const int STIPPLING_EBG_PIXEL_DENSITY_DEFAULT=int(_dots_ebg_ext_ns::_pixel_density::PIXEL_DENSITY_300PPI);
  const _dots_ebg_ext_ns::_output_mode STIPPLING_EBG_OUTPUT_MODE_DEFAULT=_dots_ebg_ext_ns::OUTPUT_MODE_GRAYSCALE;
  const int STIPPLING_EBG_BLACK_LEVEL_DEFAULT=200;
}


class _gl_widget;
class _qtw_filter_stippling_ebg;

//HEA

class _filter_stippling_ebg : public _filter
{
public:
  _filter_stippling_ebg();
  void reset_data();

  void update_size();
  void update_filter();
  void update();

  bool change_output_image_size(){return Change_output_image_size;}
  bool use_dots(){return Use_dots;}

//  void output_image_size(int &Width1,int &Height1);

  void output_size(_dots_ebg_ext_ns::_output_size Output_size1){Output_size=Output_size1;}
  _dots_ebg_ext_ns::_output_size output_size(){return Output_size;}

  void pixel_density(int Pixel_density1);
  int pixel_density(){return Pixel_density;}

  void displacement_randomness(float Displacement_randomness1);
  float displacement_randomness(){return Displacement_randomness;}

  void output_mode(_dots_ebg_ext_ns::_output_mode Output_mode1){Output_mode=Output_mode1;}
  _dots_ebg_ext_ns::_output_mode output_mode(){return Output_mode;}

  void black_level(int Black_level1){Black_level=Black_level1;}
  int black_level(){return Black_level;}

  void set_dots_images(std::vector<std::vector<std::shared_ptr<cv::Mat>>> *Dots1){Dots=Dots1;}
//  void set_dots_texture_packet(int Dots_texture_packet1){Dots_texture_packet=Dots_texture_packet1;}

  void put_dot(cv::Mat *Output_image1, float Row1, float Col1, unsigned int Selected_dot_size1, unsigned int Index_row1, unsigned int Index_col1);
  void stippling(cv::Mat *Input_image0, cv::Mat *Output_image0);

  void save_seeds();
  void load_seeds();

  virtual void info1(unsigned int Value){Q_UNUSED(Value)}

  void read_parameters(std::map<std::string,std::string> &Parameters);
  void write_parameters(std::map<std::string, std::string> &Parameters);

protected:
  void adjust_image_sizes(int &Width1, int &Height1);

  float Displacement_randomness=_f_stippling_ebg_ns::STIPPLING_EBG_DISPLACEMENT_DEFAULT;
  int Num_rows_dot_matrix,Num_cols_dot_matrix;
  int Dot_size_min=_f_stippling_ebg_ns::MIN_DOT_SIZE;
  int Dot_size_max=_f_stippling_ebg_ns::MAX_DOT_SIZE;
  int Dots_texture_packet;

  _dots_ebg_ext_ns::_output_size Output_size;
//  _dots_ebg_ext_ns::_pixel_density Pixel_density;

  int Pixel_density;
  int Pixel_density_factor;
//  int Output_mode;
  _dots_ebg_ext_ns::_output_mode Output_mode=_dots_ebg_ext_ns::OUTPUT_MODE_GRAYSCALE;
  int Black_level;

  std::vector<std::vector<std::shared_ptr<cv::Mat>>> *Dots;

  _random_uniform_int Index_row,Index_col;
  _random_uniform_int Dot_size;
  _random_uniform_double Displacement;

  bool Seeds_initialized;

  unsigned int Counter_of_dots;

  float Scale_factor=3;

  _gl_widget *GL_widget=nullptr;
};

//HEA

class _filter_stippling_ebg_ui : public _filter_stippling_ebg
{
public:
  _filter_stippling_ebg_ui(_gl_widget *GL_widget1,std::string Name1="Stippling EBS parameters");
  _filter_stippling_ebg_ui(_gl_widget *GL_widget1,std::map<std::string,std::string> &Parameters,std::string Box_name="Stippling EBS parameters");
  ~_filter_stippling_ebg_ui();
  void reset_data();

  void show();
  void hide();
  void *get_link();

  void parameter1(int Value){output_size(_dots_ebg_ext_ns::_output_size(Value));}
  int parameter1(){return int(output_size());}

  void parameter2(int Value){pixel_density(Value);}
  int parameter2(){return pixel_density();}

  void parameter3(float Value){_filter_stippling_ebg::displacement_randomness(Value);}
  float parameter3(){return _filter_stippling_ebg::displacement_randomness();}

  void parameter4(int Value){output_mode(_dots_ebg_ext_ns::_output_mode(Value));}
  int parameter4(){return int(output_mode());}

  void parameter5(int Value){black_level(Value);}
  int parameter5(){return black_level();}

  void info1(unsigned int Value1);

private:
  _qtw_filter_stippling_ebg *Qtw_filter_stippling_ebg=nullptr;
};

//HEA

class _qtw_filter_stippling_ebg: public QWidget
{
  Q_OBJECT
public:

  _qtw_filter_stippling_ebg(_gl_widget *GL_widget1,_filter_stippling_ebg_ui *Filter1,std::string Box_name="Stippling EBS parameters");

  void show(){Group_box_main->show();}
  void hide(){Group_box_main->hide();}
  QGroupBox *get_link(){return Group_box_main;}

  void parameter1(int Value);
  void parameter2(int Value);
  void parameter3(float Value);
  void parameter4(int Value);
  void parameter5(int Value);

  void info1(unsigned int Value);

protected slots:
  void parameter1_slot(int Value);
  void parameter2_slot(int Value);
  void parameter3_slot(int Value);
  void parameter4_slot(int Value);
  void parameter5_slot(int Value);

private:
  QGroupBox *Group_box_main=nullptr;
  QGroupBox *Group_box_parameter1=nullptr;
  QGroupBox *Group_box_parameter2=nullptr;
  QGroupBox *Group_box_parameter3=nullptr;
  QGroupBox *Group_box_parameter4=nullptr;
  QGroupBox *Group_box_parameter5=nullptr;

  QGroupBox *Group_box_info1=nullptr;

  // output size
  QComboBox *Combo_box_parameter1=nullptr;

  // pixel density
  QComboBox *Combo_box_parameter2=nullptr;

  // Displacement ramdomness
  QSlider *Slider_parameter3=nullptr;
  QLineEdit *Line_edit_parameter3=nullptr;

  // output mode
  QComboBox *Combo_box_parameter4=nullptr;

  // black level
  QLineEdit *Line_edit_parameter5=nullptr;
  QSlider *Slider_parameter5=nullptr;

  // number of dots
   QLineEdit *Line_edit_info1=nullptr;

  _filter_stippling_ebg_ui *Filter=nullptr;
  _gl_widget *GL_widget=nullptr;
};
#endif
