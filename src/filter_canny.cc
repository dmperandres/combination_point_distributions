//LIC

#include "filter_canny.h"
#include "glwidget.h"

using namespace _f_canny_ns;

//HEA

_filter_canny::_filter_canny()
{
  Canny_size=CANNY_SIZE_DEFAULT;
  Threshold1=CANNY_THRESHOLD1_DEFAULT;
  Threshold2=CANNY_THRESHOLD2_DEFAULT;

  Num_channels_output_image_0=1;

  Type_filter=_f_filter_ns::FILTER_CANNY;

  Type_input_images[0]=_f_filter_ns::TYPE_UI_8;
  Type_output_0=_f_filter_ns::TYPE_UI_8;

  Scaling_factor=1;

  Change_output_image_size=false;
  Use_dots=false;
}

//HEA

void _filter_canny::reset_data()
{
  Canny_size=CANNY_SIZE_DEFAULT;
  Threshold1=CANNY_THRESHOLD1_DEFAULT;
  Threshold2=CANNY_THRESHOLD2_DEFAULT;
}

//HEA

void _filter_canny::update_filter()
{
  // compute the mean
  cv::Canny(*Aux_input_images[0],*Output_image_0,double(Threshold1),double(Threshold2),Canny_size);
  *Output_image_0=255-*Output_image_0;

//  cv::Mat Color_image;

//  cvtColor(*Output_image_0,Color_image,cv::COLOR_GRAY2RGB,3);

//  cv::flip(Color_image,Color_image,0);

//  QImage image(Color_image.data,Color_image.cols,Color_image.rows,static_cast<int>(Color_image.step),QImage::Format_RGB888);
////  image.rgbSwapped();

//  image.save("images/results/canny_frog1.png");



//  cv::Mat Aux=Output_image_0->clone();

//  for (int i=0;i<Aux.rows-1;i++){
//    for (int j=0;j<Aux.cols-1;j++){
//      if (Aux.at<unsigned char>(i,j)==0){
//        // x
//        if (Aux.at<unsigned char>(i,j+1)==0){
//          // xx
//          if (Aux.at<unsigned char>(i+1,j+1)==0){
//            // xx
//            // ?x
//            if (Aux.at<unsigned char>(i+1,j)==255){
//              // xx
//              // 0x
//              Aux.at<unsigned char>(i,j+1)=255;;
//            }
//            else{
//              // xx
//              // xx
//              Aux.at<unsigned char>(i,j)=255;;
//              Aux.at<unsigned char>(i+1,j+1)=255;;
//            }
//          }
//          else{
//            // xx
//            // ?0
//            if (Aux.at<unsigned char>(i+1,j)==0){
//              // xx
//              // x0
//              Aux.at<unsigned char>(i,j)=255;;
//            }
//          }
//        }
//        else{
//          // x0
//          if (Aux.at<unsigned char>(i+1,j)==0){
//            // x0
//            // x?
//            if (Aux.at<unsigned char>(i+1,j+1)==0){
//              // x0
//              // xx
//              Aux.at<unsigned char>(i+1,j)=255;;
//            }
//          }
//        }
//      }
//      else{
//        // 0
//        if (Aux.at<unsigned char>(i,j+1)==0){
//          // 0x
//          if (Aux.at<unsigned char>(i+1,j+1)==0){
//            // 0x
//            // ?x
//            if (Aux.at<unsigned char>(i+1,j)==0){
//              // 0x
//              // xx
//              Aux.at<unsigned char>(i+1,j+1)=255;;
//            }
//          }
//        }
//      }
//    }
//  }



//  cvtColor(Aux,Color_image,cv::COLOR_GRAY2RGB,3);
//  cv::flip(Color_image,Color_image,0);
//  QImage image1(Color_image.data,Color_image.cols,Color_image.rows,static_cast<int>(Color_image.step),QImage::Format_RGB888);

//  image1.save("images/results/canny_frog1_good.png");


//  // filter the corners
//  Aux=Output_image_0->clone();

//  for (int i=0;i<Aux.rows-1;i++){
//    for (int j=0;j<Aux.cols-1;j++){
//      if (Aux.at<unsigned char>(i,j)==0){
//        // x
//        if (Aux.at<unsigned char>(i,j+1)==0){
//          // xx
//          if (Aux.at<unsigned char>(i+1,j+1)==0){
//            // xx
//            // ?x
//            if (Aux.at<unsigned char>(i+1,j)==255){
//              // xx
//              // 0x
//              Aux.at<unsigned char>(i,j+1)=128;
//            }
//            else{
//              // xx
//              // xx
//              Aux.at<unsigned char>(i,j)=128;
//              Aux.at<unsigned char>(i+1,j+1)=128;
//            }
//          }
//          else{
//            // xx
//            // ?0
//            if (Aux.at<unsigned char>(i+1,j)==0){
//              // xx
//              // x0
//              Aux.at<unsigned char>(i,j)=128;
//            }
//          }
//        }
//        else{
//          // x0
//          if (Aux.at<unsigned char>(i+1,j)==0){
//            // x0
//            // x?
//            if (Aux.at<unsigned char>(i+1,j+1)==0){
//              // x0
//              // xx
//              Aux.at<unsigned char>(i+1,j)=128;
//            }
//          }
//        }
//      }
//      else{
//        // 0
//        if (Aux.at<unsigned char>(i,j+1)==0){
//          // 0x
//          if (Aux.at<unsigned char>(i+1,j+1)==0){
//            // 0x
//            // ?x
//            if (Aux.at<unsigned char>(i+1,j)==0){
//              // 0x
//              // xx
//              Aux.at<unsigned char>(i+1,j+1)=128;
//            }
//          }
//        }
//      }
//    }
//  }

//  Color_image.setTo(cv::Vec3b(255,255,255));

//  for (int i=0;i<Aux.rows-1;i++){
//    for (int j=0;j<Aux.cols-1;j++){
//      if (Aux.at<unsigned char>(i,j)==0){
//        Color_image.at<cv::Vec3b>(i,j)=cv::Vec3b(0,0,0);
//      }
//      else{
//        if (Aux.at<unsigned char>(i,j)==128){
//          Color_image.at<cv::Vec3b>(i,j)=cv::Vec3b(255,0,0);
//        }
//      }
//    }
//  }

//  cv::flip(Color_image,Color_image,0);
//  QImage image2(Color_image.data,Color_image.cols,Color_image.rows,static_cast<int>(Color_image.step),QImage::Format_RGB888);

//  image2.save("images/results/canny_frog1_red.png");

//  *Output_image_0=Aux.clone();


}

//HEA

void _filter_canny::update()
{
  if (Input_images[0]==nullptr){
    QMessageBox MsgBox;
    MsgBox.setText("Error: Canny filter: no Input_images[0]");
    MsgBox.exec();
    qApp->quit();
    exit(-1);
  }

  for (int i=0;i<_f_filter_ns::MAX_CONNECTIONS;i++) Aux_input_images[i]=nullptr;

  if (Changed_input==true){
    Output_image_0->release();
    if (Num_channels_output_image_0==-1){ // input->output
      if (Input_images[0]->channels()==3){
        Output_image_0->create(Input_images[0]->rows,Input_images[0]->cols,CV_8UC3);
      }
      else{
        Output_image_0->create(Input_images[0]->rows,Input_images[0]->cols,CV_8UC1);
      }
    }
    else{ // adjust
      if (Num_channels_output_image_0==3){
        Output_image_0->create(Input_images[0]->rows,Input_images[0]->cols,CV_8UC3);
      }
      else{
        Output_image_0->create(Input_images[0]->rows,Input_images[0]->cols,CV_8UC1);
      }
    }
    Changed_input=false;
  }

  // Check the number of output channels
  for (int i=0;i<_f_filter_ns::MAX_CONNECTIONS;i++){
    if (Input_images[i]!=nullptr){
      if (Input_images[i]->channels()==3 && Num_channels_output_image_0==1){// Different number of channels between input and output
        // conversion
        Aux_input_images[i]=new cv::Mat;
        Aux_input_images[i]->create(Input_images[0]->rows,Input_images[0]->cols,CV_8UC1);
        cvtColor(*Input_images[i],*Aux_input_images[i],cv::COLOR_BGR2GRAY,1);
      }
      else {
        Aux_input_images[i]=Input_images[i];
      }
    }
  }

  update_filter();

  for (int i=0;i<_f_filter_ns::MAX_CONNECTIONS;i++){
    if (Aux_input_images[i]!=nullptr && Aux_input_images[i]!=Input_images[i]) delete Aux_input_images[i];
  }
}

//HEA

void _filter_canny::canny_size(int Value)
{
  Canny_size=Value;
}

//HEA

void _filter_canny::threshold1(int Value)
{
  Threshold1=Value;
}

//HEA

void _filter_canny::threshold2(int Value)
{
  Threshold2=Value;
}

//HEA

void _filter_canny::read_parameters(std::map<std::string,std::string> &Parameters)
{
  if (Parameters["_INI_"]=="EDITOR"){// default parameters
    canny_size(CANNY_SIZE_DEFAULT);
    threshold1(CANNY_THRESHOLD1_DEFAULT);
    threshold2(CANNY_THRESHOLD2_DEFAULT);
  }
  else{// Parameters from file or from initialised filter
    try{
      if (Parameters["kernel_size"]=="default") canny_size(CANNY_SIZE_DEFAULT);
      else canny_size(atoi(Parameters["kernel_size"].c_str()));

      if (Parameters["threshold1"]=="default") threshold1(CANNY_THRESHOLD1_DEFAULT);
      else threshold1(atoi(Parameters["threshold1"].c_str()));

      if (Parameters["threshold2"]=="default") threshold2(CANNY_THRESHOLD2_DEFAULT);
      else threshold2(atoi(Parameters["threshold2"].c_str()));
    }
    catch (const std::out_of_range) {
      QMessageBox MsgBox;
      MsgBox.setText("Error in the data of CANNY filter");
      MsgBox.exec();
      qApp->quit();
      exit(-1);
    }
  }
}

//HEA

void _filter_canny::write_parameters(std::map<std::string,std::string> &Parameters)
{
  char Aux[100];

  sprintf(Aux,"%d",canny_size());
  Parameters["kernel_size"]=string(Aux);
  sprintf(Aux,"%d",threshold1());
  Parameters["threshold1"]=string(Aux);
  sprintf(Aux,"%d",threshold2());
  Parameters["threshold2"]=string(Aux);
}

//HEA

_filter_canny_ui::_filter_canny_ui(_gl_widget *GL_widget1, string Name1)
{
  Name=Name1;
  Qtw_filter_canny=new _qtw_filter_canny(GL_widget1,this,Name);
}

//HEA

_filter_canny_ui::_filter_canny_ui(_gl_widget *GL_widget1, std::map<string, string> &Parameters, string Name1)
{
  read_parameters(Parameters);
  Name=Name1;
  Qtw_filter_canny=new _qtw_filter_canny(GL_widget1,this,Name);
}

//HEA

_filter_canny_ui::~_filter_canny_ui()
{
  delete Qtw_filter_canny;
}

//HEA

void _filter_canny_ui::reset_data()
{
  _filter_canny::reset_data();
  Qtw_filter_canny->parameter1(parameter1());
  Qtw_filter_canny->parameter2(parameter2());
  Qtw_filter_canny->parameter3(parameter3());
  hide();
}

//HEA

void _filter_canny_ui::show()
{
  Qtw_filter_canny->show();
}

//HEA

void _filter_canny_ui::hide()
{
  Qtw_filter_canny->hide();
}

//HEA

void *_filter_canny_ui::get_link()
{
  return Qtw_filter_canny->get_link();
}

//HEA

_qtw_filter_canny::_qtw_filter_canny(_gl_widget *GL_widget1,_filter_canny_ui *Filter1,string Box_name)
{
  GL_widget=GL_widget1;
  Filter=Filter1;

  Group_box_main=new QGroupBox(tr(Box_name.c_str()));
  Group_box_main->setAlignment(Qt::AlignCenter);

  QVBoxLayout *Vertical_box_main=new QVBoxLayout;

  // Parameter1
  QGroupBox *Group_box_parameter1=new QGroupBox(tr(String_group_box_parameter1.c_str()));
  Group_box_parameter1->setAlignment(Qt::AlignCenter);

  QGridLayout *Grid_parameter1 = new QGridLayout;

  QLabel *Label_parameter1_min= new QLabel(tr(String_label_parameter1_min.c_str()));
  QLabel *Label_parameter1_max= new QLabel(tr(String_label_parameter1_max.c_str()));

  Line_edit_parameter1=new QLineEdit();
  Line_edit_parameter1->setAlignment(Qt::AlignRight);
  Line_edit_parameter1->setReadOnly(true);
  Line_edit_parameter1->setEnabled(false);
  Line_edit_parameter1->setToolTip(tr(String_label_parameter1_tooltip.c_str()));

  Slider_parameter1 = new QSlider(Qt::Horizontal);
  Slider_parameter1->setRange(Parameter1_min_value,Parameter1_max_value);
  Slider_parameter1->setSingleStep(Parameter1_single_step);
  Slider_parameter1->setPageStep(Parameter1_page_step);
  Slider_parameter1->setTickInterval(Parameter1_tick_interval);
  Slider_parameter1->setTickPosition(QSlider::TicksRight);
  Slider_parameter1->setTracking(Parameter1_set_tracking);
  Slider_parameter1->setToolTip(tr(String_parameter1_tooltip.c_str()));

  parameter1(Filter->parameter1());

  Grid_parameter1->addWidget(Line_edit_parameter1,0,1,Qt::AlignCenter);
  Grid_parameter1->addWidget(Label_parameter1_min,1,0,Qt::AlignRight);
  Grid_parameter1->addWidget(Slider_parameter1,1,1);
  Grid_parameter1->addWidget(Label_parameter1_max,1,2,Qt::AlignLeft);

  Group_box_parameter1->setLayout(Grid_parameter1);

  connect(Slider_parameter1, SIGNAL(valueChanged(int)),this,SLOT(parameter1_slot(int)));

  // Parameter 2
  Group_box_parameter2=new QGroupBox(tr(String_group_box_parameter2.c_str()));
  Group_box_parameter2->setAlignment(Qt::AlignCenter);

  QGridLayout *Grid_parameter2 = new QGridLayout;

  QLabel *Label_parameter2_min= new QLabel(tr(String_label_parameter2_min.c_str()));
  QLabel *Label_parameter2_max= new QLabel(tr(String_label_parameter2_max.c_str()));

  Line_edit_parameter2=new QLineEdit();
  Line_edit_parameter2->setAlignment(Qt::AlignRight);
  Line_edit_parameter2->setReadOnly(true);
  Line_edit_parameter2->setEnabled(false);
  Line_edit_parameter2->setToolTip(tr(String_label_parameter2_tooltip.c_str()));

  Slider_parameter2 = new QSlider(Qt::Horizontal);
  Slider_parameter2->setRange(Parameter2_min_value,Parameter2_max_value);
  Slider_parameter2->setSingleStep(Parameter2_single_step);
  Slider_parameter2->setPageStep(Parameter2_page_step);
  Slider_parameter2->setTickInterval(Parameter2_tick_interval);
  Slider_parameter2->setTickPosition(QSlider::TicksRight);
  Slider_parameter2->setTracking(Parameter2_set_tracking);
  Slider_parameter2->setToolTip(tr(String_parameter2_slider_tooltip.c_str()));

  parameter2(Filter->parameter2());

  Grid_parameter2->addWidget(Line_edit_parameter2,0,1,Qt::AlignCenter);
  Grid_parameter2->addWidget(Label_parameter2_min,1,0,Qt::AlignRight);
  Grid_parameter2->addWidget(Slider_parameter2,1,1);
  Grid_parameter2->addWidget(Label_parameter2_max,1,2,Qt::AlignLeft);

  Group_box_parameter2->setLayout(Grid_parameter2);

  connect(Slider_parameter2, SIGNAL(valueChanged(int)),this,SLOT(parameter2_slot(int)));

  // parameter 3
  Group_box_parameter3=new QGroupBox(tr(String_group_box_parameter3.c_str()));
  Group_box_parameter3->setAlignment(Qt::AlignCenter);

  QGridLayout *Grid_parameter3 = new QGridLayout;

  QLabel *Label_parameter3_min= new QLabel(tr(String_label_parameter3_min.c_str()));
  QLabel *Label_parameter3_max= new QLabel(tr(String_label_parameter3_max.c_str()));

  Line_edit_parameter3=new QLineEdit();
  Line_edit_parameter3->setAlignment(Qt::AlignRight);
  Line_edit_parameter3->setReadOnly(true);
  Line_edit_parameter3->setEnabled(false);
  Line_edit_parameter3->setToolTip(tr(String_label_parameter3_tooltip.c_str()));

  Slider_parameter3 = new QSlider(Qt::Horizontal);
  Slider_parameter3->setRange(Parameter3_min_value,Parameter3_max_value);
  Slider_parameter3->setSingleStep(Parameter3_single_step);
  Slider_parameter3->setPageStep(Parameter3_page_step);
  Slider_parameter3->setTickInterval(Parameter3_tick_interval);
  Slider_parameter3->setTickPosition(QSlider::TicksRight);
  Slider_parameter3->setTracking(Parameter3_set_tracking);
  Slider_parameter3->setToolTip(tr(String_parameter3_slider_tooltip.c_str()));

  parameter3(Filter->parameter3());

  Grid_parameter3->addWidget(Line_edit_parameter3,0,1,Qt::AlignCenter);
  Grid_parameter3->addWidget(Label_parameter3_min,1,0,Qt::AlignRight);
  Grid_parameter3->addWidget(Slider_parameter3,1,1);
  Grid_parameter3->addWidget(Label_parameter3_max,1,2,Qt::AlignLeft);

  Group_box_parameter3->setLayout(Grid_parameter3);

  connect(Slider_parameter3, SIGNAL(valueChanged(int)),this,SLOT(parameter3_slot(int)));

  //
  Vertical_box_main->addWidget(Group_box_parameter1);
  Vertical_box_main->addWidget(Group_box_parameter2);
  Vertical_box_main->addWidget(Group_box_parameter3);
  Vertical_box_main->addStretch();

  Group_box_main->setLayout(Vertical_box_main);
}

//HEA

void _qtw_filter_canny::parameter1(int Value)
{
  QString Str;

  Str=Str.number(Value);
  Line_edit_parameter1->setText(Str);
  Slider_parameter1->blockSignals(true);
  Slider_parameter1->setValue((Value/2)-1);
  Slider_parameter1->blockSignals(false);
}

//HEA

void _qtw_filter_canny::parameter2(int Value)
{
  QString Str;

  Str=Str.number(Value);
  Line_edit_parameter2->setText(Str);
  Slider_parameter2->blockSignals(true);
  Slider_parameter2->setValue(Value);
  Slider_parameter2->blockSignals(false);
}

//HEA

void _qtw_filter_canny::parameter3(int Value)
{
  QString Str;

  Str=Str.number(Value);
  Line_edit_parameter3->setText(Str);
  Slider_parameter3->blockSignals(true);
  Slider_parameter3->setValue(Value);
  Slider_parameter3->blockSignals(false);
}

//HEA

void _qtw_filter_canny::parameter1_slot(int Value)
{
  QString Str;
  char Aux[100];

  sprintf(Aux,"%2d",Value*2+1);
  Str=Aux;
  Line_edit_parameter1->setText(Str);
  Filter->parameter1(Value*2+1);
  GL_widget->update_effect(Filter->name());
}

//HEA

void _qtw_filter_canny::parameter2_slot(int Value)
{
  QString Str;
  char Aux[100];

  sprintf(Aux,"%2d",Value);
  Str=Aux;
  Line_edit_parameter2->setText(Str);
  Filter->parameter2(Value);
  GL_widget->update_effect(Filter->name());
}

//HEA

void _qtw_filter_canny::parameter3_slot(int Value)
{
  QString Str;
  char Aux[100];

  sprintf(Aux,"%2d",Value);
  Str=Aux;
  Line_edit_parameter3->setText(Str);
  Filter->parameter3(Value);
  GL_widget->update_effect(Filter->name());
}
