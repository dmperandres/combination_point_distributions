//LIC

#ifndef FILTER_H
#define FILTER_H

#include <opencv.hpp>
#include <string>
#include <memory>
#include <string>

namespace _f_filter_ns
{

  const int MAX_CONNECTIONS=3;

  typedef enum {TYPE_UI_8=CV_8UC1,TYPE_FLOAT_32=CV_32FC1} _type;

  typedef struct {
    std::string Type;
    std::string Name;
    int Num_parameters;
    int Num_input_ports;
    int Num_channels;
  } _filter_parameters;

  typedef struct {
    std::string Name;
    std::string Description;
  } _filter_text;

  // 1 include the name of the filter as an identifier
  typedef enum {
  FILTER_BILATERAL=10,
  FILTER_BINARY,
  FILTER_BORDER_MASK,
  FILTER_CANNY,
  FILTER_CANNY_AUTO,
  FILTER_CCVT,
  FILTER_COLORIZE_PIXELS_,// error without the last _
  FILTER_COMBINATION,
  FILTER_CONTRAST_BRIGHT,
  FILTER_DILATION,
  FILTER_DISTANCE_FIELD,  
  FILTER_DISTANCE_FIELD_COMBINATION,
  FILTER_DISTANCE_FIELD_FLOAT,
  FILTER_DISTANCE_FIELD_LINES,
  FILTER_DOG,
  FILTER_DOT_EBG,
//  FILTER_DOT_EBG_MASK,
  FILTER_DOT_SVG,
  FILTER_DOT_SVG_MASK,
  FILTER_DOTS_EDGES,
  FILTER_EROTION,
  FILTER_GAUSSIAN,
  FILTER_HALFTONING_ACSP,
  FILTER_HALFTONING_CAH,
  FILTER_HALFTONING_OST,
  FILTER_HALFTONING_SFC,
  FILTER_INVERSION,
  FILTER_KANG,
  FILTER_LAPLACIAN,
  FILTER_LOAD_IMAGE_,// error without the last _
  FILTER_MEAN_SHIFT,
  FILTER_MEASURE_SSIM_PSNR,
  FILTER_RETINEX,
  FILTER_RETINEX_A,
  FILTER_RETINEX_B,
  FILTER_RETINEX_B_GPU,
  FILTER_RWT,
  FILTER_SOBEL,
  FILTER_STIPPLING_EBG,
  FILTER_STIPPLING_EBG_MASK,
  FILTER_STIPPLING_SAS,
  FILTER_STIPPLING_SBE,
  FILTER_THRESHOLD,
  FILTER_WCVD
  } _type_filter;

  // 2 include the name of the filter as a string
  static std::map<std::string,_type_filter> Type_filter_name={
    {"BILATERAL",FILTER_BILATERAL},
    {"BINARY",FILTER_BINARY},
    {"BORDER_MASK",FILTER_BORDER_MASK},
    {"CANNY",FILTER_CANNY},
    {"CANNY_AUTO",FILTER_CANNY_AUTO},
    {"CCVT",FILTER_CCVT},
    {"COLORIZE_PIXELS",FILTER_COLORIZE_PIXELS_},
    {"COMBINATION",FILTER_COMBINATION},
    {"CONTRAST",FILTER_CONTRAST_BRIGHT},
    {"DILATION",FILTER_DILATION},
    {"DISTANCE_FIELD",FILTER_DISTANCE_FIELD},
    {"DF_COMBINATION",FILTER_DISTANCE_FIELD_COMBINATION},
    {"DISTANCE_FIELD_FLOAT",FILTER_DISTANCE_FIELD_FLOAT},
    {"DISTANCE_FIELD_LINES",FILTER_DISTANCE_FIELD_LINES},
    {"DOG",FILTER_DOG},
    {"DOT_EBG",FILTER_DOT_EBG},
    {"DOT_SVG",FILTER_DOT_SVG},
    {"DOT_SVG_MASK",FILTER_DOT_SVG_MASK},
    {"DOTS_EDGES",FILTER_DOTS_EDGES},
    {"EROTION",FILTER_EROTION},
    {"GAUSSIAN",FILTER_GAUSSIAN},
    {"HALFTONING_ACSP",FILTER_HALFTONING_ACSP},
    {"HALFTONING_CAH",FILTER_HALFTONING_CAH},
    {"HALFTONING_OST",FILTER_HALFTONING_OST},
    {"HALFTONING_SFC",FILTER_HALFTONING_SFC},
    {"INVERSION",FILTER_INVERSION},
    {"KANG",FILTER_KANG},
    {"LAPLACIAN",FILTER_LAPLACIAN},
    {"LOAD_IMAGE",FILTER_LOAD_IMAGE_},
    {"MEAN_SHIFT",FILTER_MEAN_SHIFT},
    {"RETINEX",FILTER_RETINEX},
    {"RETINEX_A",FILTER_RETINEX_A},
    {"RETINEX_B",FILTER_RETINEX_B},
    {"RETINEX_B_GPU",FILTER_RETINEX_B_GPU},
    {"RWT",FILTER_RWT},
    {"SOBEL",FILTER_SOBEL},
    {"SSIM_PSNR",FILTER_MEASURE_SSIM_PSNR},
    {"STIPPLING_EBG",FILTER_STIPPLING_EBG},
    {"STIPPLING_EBG_MASK",FILTER_STIPPLING_EBG_MASK},
    {"STIPPLING_SAS",FILTER_STIPPLING_SAS},
    {"STIPPLING_SBE",FILTER_STIPPLING_SBE},
    {"THRESHOLD",FILTER_THRESHOLD},
    {"WCVD",FILTER_WCVD},    
  };

  // 3 information for the node editor
  // Type, Name, Num_parameters, Num_input_ports, num_channels (for 3 channels draw in orange)
  static std::map<_type_filter,_filter_parameters> Filter_name_parameters={
    {FILTER_BILATERAL,{"BILATERAL","bilateral",2,1,1}},
    {FILTER_BINARY,{"BINARY","binary",2,1,1}},
    {FILTER_BORDER_MASK,{"BORDER_MASK","border_mask",4,1,1}},
    {FILTER_CANNY,{"CANNY","canny",3,1,1}},
    {FILTER_CANNY_AUTO,{"CANNY_AUTO","canny_auto",0,1,1}},
    {FILTER_CCVT,{"CCVT","ccvt",3,1,1}},    
    {FILTER_COLORIZE_PIXELS_,{"COLORIZE_PIXELS","color_pixels",1,1,1}},
    {FILTER_COMBINATION,{"COMBINATION","combination",1,2,1}},
    {FILTER_CONTRAST_BRIGHT,{"CONTRAST","contrast",2,1,1}},
    {FILTER_DILATION,{"DILATION","dilation",2,1,1}},
    {FILTER_DISTANCE_FIELD,{"DISTANCE_FIELD","distance_field",1,1,1}},
    {FILTER_DISTANCE_FIELD_COMBINATION,{"DF_COMBINATION","df_combination",3,3,1}},
    {FILTER_DISTANCE_FIELD_FLOAT,{"DISTANCE_FIELD_FLOAT","distance_field_float",0,1,1}},
//    {FILTER_DISTANCE_FIELD_FLOAT,{"DISTANCE_FIELD_FLOAT","distance_field_float",0,2,1}},
    {FILTER_DISTANCE_FIELD_LINES,{"DISTANCE_FIELD_LINES","distance_field_lines",0,1,1}},
    {FILTER_DOG,{"DOG","dog",3,1,1}},
    {FILTER_DOT_EBG,{"DOT_EBG","dot_ebg",1,2,1}},
//    {FILTER_DOT_EBG_MASK,{"DOT_EBG_MASK","dot_ebg_mask",1,3,1}},
    {FILTER_DOT_SVG,{"DOT_SVG","dot_svg",1,2,1}},
    {FILTER_DOT_SVG_MASK,{"DOT_SVG_MASK","dot_svg_mask",1,3,1}},
    {FILTER_DOTS_EDGES,{"DOTS_EDGES","dots_edges",1,1,1}},
    {FILTER_EROTION,{"EROTION","erotion",2,1,1}},
    {FILTER_GAUSSIAN,{"GAUSSIAN","gaussian",1,1,1}},
    {FILTER_HALFTONING_ACSP,{"HALFTONING_ACSP","halftoning_acsp",3,1,1}},
    {FILTER_HALFTONING_CAH,{"HALFTONING_CAH","halftoning_cah",2,1,1}},
    {FILTER_HALFTONING_OST,{"HALFTONING_OST","halftoning_ost",0,1,1}},
    {FILTER_HALFTONING_SFC,{"HALFTONING_SFC","halftoning_sfc",1,1,1}},
    {FILTER_INVERSION,{"INVERSION","inversion",2,1,1}},
    {FILTER_KANG,{"KANG","kang",0,1,1}},
    {FILTER_LAPLACIAN,{"LAPLACIAN","laplacian",3,1,1}},
    {FILTER_LOAD_IMAGE_,{"LOAD_IMAGE","load_image",0,1,1}},
    {FILTER_MEAN_SHIFT,{"MEAN_SHIFT","mean_shift",3,1,3}},
    {FILTER_MEASURE_SSIM_PSNR,{"SSIM_PSNR","ssim_psnr",0,2,1}},
    {FILTER_RETINEX,{"RETINEX","retinex",1,1,3}},
    {FILTER_RETINEX_A,{"RETINEX_A","retinex_a",3,1,3}},
    {FILTER_RETINEX_B,{"RETINEX_B","retinex_b",4,1,3}},
    {FILTER_RETINEX_B_GPU,{"RETINEX_B_GPU","retinex_b_gpu",4,1,3}},
    {FILTER_RWT,{"RWT","rwt",4,1,1}},
    {FILTER_SOBEL,{"SOBEL","sobel",3,1,1}},
    {FILTER_STIPPLING_EBG,{"STIPPLING_EBG","stippling_ebg",1,1,1}},
    {FILTER_STIPPLING_EBG_MASK,{"STIPPLING_EBG_MASK","stippling_ebg_mask",1,2,1}},
    {FILTER_STIPPLING_SAS,{"STIPPLING_SAS","stippling_sas",5,1,1}},
    {FILTER_STIPPLING_SBE,{"STIPPLING_SBE","stippling_sbe",3,1,1}},
    {FILTER_THRESHOLD,{"THRESHOLD","threshold",2,1,1}},
    {FILTER_WCVD,{"WCVD","wcvd",2,1,1}},
  };

  // 4 help information
  static std::map<_type_filter,_filter_text> Filter_name_text={
  {FILTER_BILATERAL,{"<b>Bilateral</b>","<p>This filter applies a bilateral filter</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_BINARY,{"<b>Binary</b>","<p>This filter applies a bilateral filter</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_BORDER_MASK,{"<b>Border mask</b>","<p>This filter applies a border mask filter</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_CANNY,{"<b>Canny</b>","<p>This filter applies Canny edge detector</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_CANNY_AUTO,{"<b>Canny auto</b>","<p>This filter applies Canny edge detector</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_CCVT,{"<b>Capacity-Constrained Voronoid Tessellation</b>","<p>This filter applies Capacity-Constrained Voronoid Tesseallation method</p><p><b>Input:</b> Grayscale image <font color='#ff0000' font size=4>(square)</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_COLORIZE_PIXELS_,{"<b>Colorize pixels</b>","<p>This filter colorizes the pixels depending on their values</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Color image</p>"}},
  {FILTER_COMBINATION,{"<b>Combination</b>","<p>This filter combines two inputs to produce one output applying an operation</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_CONTRAST_BRIGHT,{"<b>Contrast-Bright</b>","<p>This filter applies a change in contrast and/or bright</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_DILATION,{"<b>Dilation</b>","<p>This filter applies a dilation</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_DISTANCE_FIELD,{"<b>Distance Field</b>","<p>This filter computes a distance field</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},    
  {FILTER_DISTANCE_FIELD_COMBINATION,{"<b>Distance Field Combination</b>","<p>This filter applies Distance Field Combination method</p><p><b>Input 0:</b> borders image</p><p><b>Input 1:</b> areas image</p><p><b>Input 2:</b> float distace field</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_DISTANCE_FIELD_FLOAT,{"<b>Distance Field float</b>","<p>This filter computes a distance field but producing a buffer of floats</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> floats image</p>"}},
  {FILTER_DISTANCE_FIELD_LINES,{"<b>Distance Field Lines</b>","<p>This filter computes a distance field and then show the lineas that fixt to it with a user given width</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_DOG,{"<b>Difference of Gaussians</b>","<p>This filter applies a Difference of Gaussians selecting only the edges</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_DOT_EBG,{"<b>Dot EBG</b>","<p>This filter draw the dots with scanned real dots</p><p><b>Input 0:</b> Grayscale image (placement)</p><b>Input 1:</b> Grayscale image (dot size modulation)</p><p><b>Output:</b> Grayscale image</p><p>Use: black pixels of the input image 0 are converted to dots. The input image 1 can be used to modulate the size of the dots: the darker the bigger</p>"}},
//  {FILTER_DOT_EBG_MASK,{"<b>Dot EBG mask</b>","<p>This filter draw the dots with scanned real dots</p><p><b>Input 0:</b> Grayscale image (placement)</p><b>Input 1:</b> Grayscale image (dot size modulation)</p><p><b>Output:</b> Grayscale image</p><p>Use: black pixels of the input image 0 are converted to dots. The input image 1 can be used to modulate the size of the dots: the darker the bigger</p>"}},
  {FILTER_DOT_SVG,{"<b>Dot SVG</b>","<p>This filter draw the dots with vectorial figures</p><p><b>Input 0:</b> Grayscale image (placement)</p><b>Input 1:</b> Grayscale image (dot size modulation)</p><p><b>Output:</b> Grayscale image</p><p>Use: black pixels of the input image 0 are converted to dots. The input image 1 can be used to modulate the size of the dots: the darker the bigger</p>"}},
  {FILTER_DOT_SVG_MASK,{"<b>Dot SVG Mask</b>","<p>This filter draw the dots with vectorial figures</p><p><b>Input 0:</b> Grayscale image (placement)</p><b>Input 1:</b> Grayscale image (dot size modulation)</p><p><b>Output:</b> Grayscale image</p><p>Use: black pixels of the input image 0 are converted to dots. The input image 1 can be used to modulate the size of the dots: the darker the bigger</p>"}},
  {FILTER_DOTS_EDGES,{"<b>Dots edges</b>","<p>This filter creates the eddges filled with dots</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_EROTION,{"<b>Erotion</b>","<p>This filter applies a erotion</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_GAUSSIAN,{"<b>Gaussian</b>","<p>This filter applies a Gaussian blur</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_HALFTONING_ACSP,{"<b>Adaptive Clustering Selective Precipitation</b>","<p>This filter applies Adaptive Clustering Selective Precipitation halftoning</p><p><b>Input:</b> Grayscale image <font color='#ff0000' font size=4>(square, power of 2)</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_HALFTONING_CAH,{"<b>Contrast-Aware Halftoning</b>","<p>This filter applies the Contrast-Aware halftoning</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_HALFTONING_OST,{"<b>Ostromoukhov</b>","<p>This filter applies Ostromoukhov's halftoning</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_HALFTONING_SFC,{"Space Filling Curve","<p>This filter applies Space Filling Curve halftoning</p><p><b>Input:</b> Grayscale image <font color='#ff0000' font size=4>(square, power of 2)</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_INVERSION,{"<b>Inversion</b>","<p>This filter applies an Inversion</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_KANG,{"<b>Kang</b>","<p>This filter applies Kang's lines detector</p><p><b>Input:</b> <font color='#ff0000' font size=4>Color image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_LAPLACIAN,{"<b>Laplacian</b>","<p>This filter applies Laplacian edge detector</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_LOAD_IMAGE_,{"<b>Load image</b>","<p>This filter loads an image</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_MEAN_SHIFT,{"<b>Mean-shift</b>","<p>This filter applies a Mean-shift</p><p><b>Input:</b> <font color='#ff0000' font size=4>Color image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_MEASURE_SSIM_PSNR,{"<b>Measure SSIM and PSNR</b>","<p>This filter measures the SSIM and PSNR of Input 1 with relation to Input 0. The output is equal to Input 0</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_RETINEX,{"<b>Retinex</b>","<p>This filter applies a local contrast with Retinex</p><p><b>Input:</b> <font color='#ff0000' font size=4>Color image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_RETINEX_A,{"<b>Retinex A</b>","<p>This filter applies a local contrast with Retinex A</p><p><b>Input:</b> <font color='#ff0000' font size=4>Color image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_RETINEX_B,{"<b>Retinex B</b>","<p>This filter applies a local contrast with Retinex B</p><p><b>Input:</b> <font color='#ff0000' font size=4>Color image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_RETINEX_B_GPU,{"<b>Retinex B GPU</b>","<p>This filter applies a local contrast with Retinex B in GPU</p><p><b>Input:</b> <font color='#ff0000' font size=4>Color image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_RWT,{"<b>Recursive Wang Tiling</b>","<p>This filter applies Recursive Wang Tiling method</p><p><b>Input:</b> Grayscale image <font color='#ff0000' font size=4>(square)</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_SOBEL,{"<b>Sobel</b>","<p>This filter applies Sobel edge detector</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_STIPPLING_EBG,{"<b>Stippling Exaple-Based Grayscale</b>","<p>This filter applies the Example-Based Grayscale stippling method</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_STIPPLING_EBG_MASK,{"<b>Stippling Exaple-Based Grayscale with Mask</b>","<p>This filter applies the Example-Based Grayscale stippling with mask method </p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_STIPPLING_SAS,{"<b>Structure-Aware Stippling</b>","<p>This filter applies Structure-Aware stipling</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_STIPPLING_SBE,{"<b>Stippling by Example</b>","<p>This filter applies Stippling by Example method</p><p><b>Input:</b> Grayscale image <font color='#ff0000' font size=4>(best for 4096x4096 size)</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_THRESHOLD,{"<b>Threshold</b>","<p>This filter applies a threshold</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  {FILTER_WCVD,{"<b>Weigthed Centroidal Voronoi Diagram</b>","<p>This filter applies the Weigthed Centroidal Voronoi Diagram method</p><p><b>Input:</b> Grayscale image</p><p><b>Output:</b> Grayscale image</p>"}},
  };
}


class _gl_widget;

class _filter
{
public:
  std::string name(){return Name;}
  void set_images(cv::Mat *Input_image0, cv::Mat *Output_image0, cv::Mat *Input_image1=nullptr, cv::Mat *Input_image2=nullptr);
  _f_filter_ns::_type_filter type_filter(){return Type_filter;}

  bool change_output_image_size(){return Change_output_image_size;}
  void set_change_output_image_size(bool Change_output_image_size1){Change_output_image_size=Change_output_image_size1;}

  void set_scaling_factor(float Scaling_factor1){Scaling_factor=Scaling_factor1;}
  float scaling_factor(){return Scaling_factor;}

  void changed_input(bool Changed_input1){Changed_input=Changed_input1;}
  bool changed_input(){return Changed_input;}

  _f_filter_ns::_type get_type_input(int Image1){return Type_input_images[Image1];}
  _f_filter_ns::_type get_type_output_0(){return Type_output_0;}

  void num_channels_input_image(int Image1,int Num_channels1){Num_channels_input_images[Image1]=Num_channels1;}
  void num_channels_output_image0(int Num_channels1){Num_channels_output_image_0=Num_channels1;}

  int num_channels_input_image(int Image1){return Num_channels_input_images[Image1];}
  int num_channels_output_image_0(){return Num_channels_output_image_0;}

  virtual void update()=0;
  virtual void show()=0;
  virtual void hide()=0;
  virtual void *get_link()=0;
  virtual void reset_data()=0;
  virtual void read_parameters(std::map<std::string,std::string> &Parameters)=0;
  virtual void write_parameters(std::map<std::string,std::string> &Parameters)=0;
  virtual bool use_dots()=0;

  void copy_input_to_output();

protected:
  cv::Mat *Input_images[_f_filter_ns::MAX_CONNECTIONS];
  cv::Mat *Output_image_0=nullptr;

  cv::Mat *Aux_input_images[_f_filter_ns::MAX_CONNECTIONS];

  int Num_channels_input_images[_f_filter_ns::MAX_CONNECTIONS];
  int Num_channels_output_image_0=1;

  _f_filter_ns::_type Type_input_images[_f_filter_ns::MAX_CONNECTIONS];
  _f_filter_ns::_type Type_output_0=_f_filter_ns::TYPE_UI_8;
  
  bool Ready=false;
  std::string Name;
  _f_filter_ns::_type_filter Type_filter;
  bool Change_output_image_size=false;
  float Scaling_factor=1.0;
  bool Use_dots=false;
  bool Changed_input=true;
};

#endif
