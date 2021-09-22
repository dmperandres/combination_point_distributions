//LIC

#include "filter_load_image.h"
#include "glwidget.h"

using namespace _f_load_image_ns;

//HEA

_filter_load_image::_filter_load_image()
{
//  load_image=load_image_DEFAULT;

  Num_channels_output_image_0=1;

  Type_filter=_f_filter_ns::FILTER_LOAD_IMAGE_;

  Type_input_images[0]=_f_filter_ns::TYPE_UI_8;
  Type_output_0=_f_filter_ns::TYPE_UI_8;

  Scaling_factor=1;

  Change_output_image_size=false;
  Use_dots=false;
}

//HEA

void _filter_load_image::reset_data()
{
//  load_image=load_image_DEFAULT;
}

//HEA

void _filter_load_image::update_filter()
{
  if (File_name==""){
    Output_image_0->setTo(cv::Vec3b(255,255,255));
  }
  else{
    if (Load_image){
      _image_IO Image_io;

      cv::Mat Image;

      Image_io.read_image(File_name,Image);
      // flip the original image to adapt it to OpenGL
      cv::flip(Image,Image,0);
      cv::resize(Image,*Output_image_0,cv::Size(Output_image_0->cols,Output_image_0->rows),0,0,cv::INTER_CUBIC);

      Load_image=false;
    }
  }
}

//HEA

void _filter_load_image::update()
{
  if (Input_images[0]==nullptr){
    QMessageBox MsgBox;
    MsgBox.setText("Error: load_image filter: no Input_images[0]");
    MsgBox.exec();
    qApp->quit();
    exit(-1);
  }

  for (int i=0;i<_f_filter_ns::MAX_CONNECTIONS;i++) Aux_input_images[i]=nullptr;

  if (Changed_input==true){
    Output_image_0->release();
    Output_image_0->create(Input_images[0]->rows,Input_images[0]->cols,CV_8UC3);
    Changed_input=false;
    Load_image=true;
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

void _filter_load_image::read_parameters(std::map<std::string,std::string> &Parameters)
{
  if (Parameters["_INI_"]=="EDITOR"){// default parameters
    file_name("");
  }
  else{// Parameters from file or from initialised filter
    try{
      if (Parameters["load_image"]=="default") file_name("");
      else{
         file_name(Parameters["load_image"]);
      }
    }
    catch (const std::out_of_range) {
      QMessageBox MsgBox;
      MsgBox.setText("Error in the data of load_image filter");
      MsgBox.exec();
      qApp->quit();
      exit(-1);
    }
  }
}

//HEA

void _filter_load_image::write_parameters(std::map<std::string,std::string> &Parameters)
{
  char Aux[100];

  Parameters["load_image"]=file_name();
}

//HEA

_filter_load_image_ui::_filter_load_image_ui(_gl_widget *GL_widget1, string Name1)
{
  Name=Name1;
  Qtw_filter_load_image=new _qtw_filter_load_image(GL_widget1,this,Name);
}

//HEA

_filter_load_image_ui::_filter_load_image_ui(_gl_widget *GL_widget1, std::map<string, string> &Parameters, string Name1)
{
  read_parameters(Parameters);
  Name=Name1;
  Qtw_filter_load_image=new _qtw_filter_load_image(GL_widget1,this,Name);
}

//HEA

_filter_load_image_ui::~_filter_load_image_ui()
{
  delete Qtw_filter_load_image;
}

//HEA

void _filter_load_image_ui::reset_data()
{
  _filter_load_image::reset_data();
//  Qtw_filter_load_image->parameter1(parameter1());
//  Qtw_filter_load_image->parameter2(parameter2());
//  Qtw_filter_load_image->parameter3(parameter3());
  hide();
}

//HEA

void _filter_load_image_ui::show()
{
  Qtw_filter_load_image->show();
}

//HEA

void _filter_load_image_ui::hide()
{
  Qtw_filter_load_image->hide();
}

//HEA

void *_filter_load_image_ui::get_link()
{
  return Qtw_filter_load_image->get_link();
}

//HEA

_qtw_filter_load_image::_qtw_filter_load_image(_gl_widget *GL_widget1,_filter_load_image_ui *Filter1,string Box_name)
{
  GL_widget=GL_widget1;
  Filter=Filter1;

  Group_box_main=new QGroupBox(tr(Box_name.c_str()));
  Group_box_main->setAlignment(Qt::AlignCenter);

  QVBoxLayout *Vertical_box_main=new QVBoxLayout;

  // Button to LOAD IMAGE
  QPushButton *Button_parameter1= new QPushButton(tr("Load image"));

  connect(Button_parameter1, SIGNAL(clicked()),this,SLOT(parameter1_slot()));

  //
  Vertical_box_main->addWidget(Button_parameter1);
  Vertical_box_main->addStretch();

  Group_box_main->setLayout(Vertical_box_main);
}

//HEA

void _qtw_filter_load_image::parameter1_slot()
{
  QString File_name1 = QFileDialog::getOpenFileName(this, tr("Load the image"),"./images",tr("Images (*.png *.jpg)"));

  if (!File_name1.isEmpty()){
    Filter->parameter1(File_name1.toStdString());
    GL_widget->update_effect(Filter->name());
  }
}
