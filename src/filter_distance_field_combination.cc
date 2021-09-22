#include "filter_distance_field_combination.h"
#include "glwidget.h"

using namespace _f_distance_field_combination_ns;

_filter_distance_field_combination::_filter_distance_field_combination()
{
  Mixer=DISTANCE_FIELD_COMBINATION_MIXER_DEFAULT;
  Value_function_a=DISTANCE_FIELD_VALUE_FUNCTON_A_DEFAULT;
  Value_function_b=DISTANCE_FIELD_VALUE_FUNCTON_B_DEFAULT;

  Num_channels_output_image_0=1;

  Type_filter=_f_filter_ns::FILTER_DISTANCE_FIELD_COMBINATION;

  Type_input_images[0]=_f_filter_ns::TYPE_UI_8;
  Type_output_0=_f_filter_ns::TYPE_UI_8;

  Scaling_factor=1;
  Change_output_image_size=false;
  Use_dots=false;

  Max_distance=-1;
}

//HEA

void _filter_distance_field_combination::reset_data()
{
  Mixer=DISTANCE_FIELD_COMBINATION_MIXER_DEFAULT;
  Value_function_a=DISTANCE_FIELD_VALUE_FUNCTON_A_DEFAULT;
  Value_function_b=DISTANCE_FIELD_VALUE_FUNCTON_B_DEFAULT;
}

//HEA

//void _filter_distance_field_combination::combination(cv::Mat *Input_borders,cv::Mat *Input_areas,cv::Mat *Input_distance_field,cv::Mat *Output0)
//{
//  // copy the Image lines to output
//  Input_borders->copyTo(*Output0);

//  random_device RD;
//  mt19937 MT(RD());
//  uniform_real_distribution<float> Random(0,Mixer*5);

//  if (Input_distance_field->type()==CV_8UC1){
//    // now add the dots of image areas dependin on the distances field
//    for (unsigned int i=0;i<Input_areas->total();i++){
//      if (Input_areas->at<unsigned char>(i)==0){
//        if (Input_distance_field->at<unsigned char>(i)==0) continue;
//        else{
//          if (Input_distance_field->at<unsigned char>(i)>=Random(MT)){
//            Output0->at<unsigned char>(i)=0;
//          }
//        }
//      }
//    }
//  }
//  else{
//    for (unsigned int i=0;i<Input_areas->total();i++){
//      if (Input_areas->at<unsigned char>(i)==0){
//        if (Input_distance_field->at<float>(i)==0) continue;
//        else{
////          if (Input_distance_field->at<float>(i)>=Mixer){
//          if (Input_distance_field->at<float>(i)>=Random(MT)){
//            Output0->at<unsigned char>(i)=1;
//          }
//        }
//      }
//    }
//  }
//}

void _filter_distance_field_combination::combination(cv::Mat *Input_borders,cv::Mat *Input_areas,cv::Mat *Input_distance_field,cv::Mat *Output0)
{
  float Value;
  float Distance;
  float Weight;

  Num_black_pixels=0;

  Output0->setTo(255);

  random_device RD;
  mt19937 MT(RD());
  uniform_real_distribution<float> Random(1-Mixer,std::nextafter(1.0f,2.0f));
//  uniform_real_distribution<float> Random(0,std::nextafter(1.0f,2.0f));

  if (Input_distance_field->type()==CV_8UC1){
    // now add the dots of image areas dependin on the distances field
    for (int i=0;i<int(Input_areas->total());i++){
      if (Input_areas->at<unsigned char>(i)==0){
        if (Input_distance_field->at<unsigned char>(i)==0) continue;
        else{
          if (Input_distance_field->at<unsigned char>(i)>=Random(MT)){
            Output0->at<unsigned char>(i)=0;
            Num_black_pixels++;
          }
        }
      }
    }
  }
  else{ // float distance field
    for (int i=0;i<int(Input_areas->total());i++){
      if (Input_borders->at<unsigned char>(i)!=255 || Input_areas->at<unsigned char>(i)!=255){
//        Weight=Mixer*2-1;

        if (Value_function_a!=Value_function_b){
          if (Mixer<=Value_function_a) Weight=-1;
          else{
            if (Mixer>=Value_function_b) Weight=1;
            else{
              float m=2/(Value_function_b-Value_function_a);
              float b=1-m*Value_function_b;
              Weight=m*Mixer+b;
            }
          }
        }
        else{
          if (Mixer<=Value_function_a) Weight=-1;
          else Weight=1;
        }

        Value=Random(MT);
        Distance=Input_distance_field->at<float>(i)+Weight;
        if (Distance<=Value){
          if (Input_borders->at<unsigned char>(i)==0){
            Output0->at<unsigned char>(i)=0;
            Num_black_pixels++;
          }
        }
        else{
          if (Input_areas->at<unsigned char>(i)==0){
            Output0->at<unsigned char>(i)=1;
            Num_black_pixels++;
          }
        }
      }
    }
  }

  info1(Num_black_pixels);
}

// Example that only uses alpha
// the positions of the point is not important

void _filter_distance_field_combination::combination1(cv::Mat *Input_borders,cv::Mat *Input_areas,cv::Mat *Input_distance_field,cv::Mat *Output0)
{
  float Value;
//  float Distance;
//  float Weight;

  Output0->setTo(255);

  random_device RD;
  mt19937 MT(RD());
  uniform_real_distribution<float> Random(0,std::nextafter(1.0f,2.0f));

  if (Input_distance_field->type()==CV_8UC1){
    // now add the dots of image areas dependin on the distances field
    for (int i=0;i<int(Input_areas->total());i++){
      if (Input_areas->at<unsigned char>(i)==0){
        if (Input_distance_field->at<unsigned char>(i)==0) continue;
        else{
          if (Input_distance_field->at<unsigned char>(i)>=Random(MT)){
            Output0->at<unsigned char>(i)=0;
          }
        }
      }
    }
  }
  else{ // float distance field
    for (int i=0;i<int(Input_areas->total());i++){
      if (Input_borders->at<unsigned char>(i)!=255 || Input_areas->at<unsigned char>(i)!=255){
        Value=Random(MT);
        if (Mixer<=Value){
          if (Input_borders->at<unsigned char>(i)==0){
            Output0->at<unsigned char>(i)=0;
          }
        }
        else{
          if (Input_areas->at<unsigned char>(i)==0){
            Output0->at<unsigned char>(i)=1;
          }
        }
      }
    }
  }
}

// using the distance of the points
// the bias [-1,1]] makes that the interval [0,1] can be displaced
// bias=-1 -> interval[-1,0] as the random is alway >=0 and the condition for borders is distance<= random -> only the borders are drawn
// bias=0 -> interval[0,1] (normal case) as the random is alway >=0 and the condition for borders is distance<= random -> only the borders are drawn
// bias=1 -> interval[1,2] as the random is alway >=0 and the condition for borders is distance<= random -> only the area is drawn
//
// the interval is displaced

void _filter_distance_field_combination::combination2(cv::Mat *Input_borders,cv::Mat *Input_areas,cv::Mat *Input_distance_field,cv::Mat *Output0)
{
  float Value;
  float Distance;
  float Weight;

  Output0->setTo(255);

  random_device RD;
  mt19937 MT(RD());
  uniform_real_distribution<float> Random(0,std::nextafter(1.0f,2.0f));

  Weight=2*Mixer-1;


  if (Input_distance_field->type()==CV_8UC1){
    // now add the dots of image areas dependin on the distances field
    for (int i=0;i<int(Input_areas->total());i++){
      if (Input_areas->at<unsigned char>(i)==0){
        if (Input_distance_field->at<unsigned char>(i)==0) continue;
        else{
          if (Input_distance_field->at<unsigned char>(i)>=Random(MT)){
            Output0->at<unsigned char>(i)=0;
          }
        }
      }
    }
  }
  else{ // float distance field
    for (int i=0;i<int(Input_areas->total());i++){
      if (Input_borders->at<unsigned char>(i)!=255 || Input_areas->at<unsigned char>(i)!=255){

        Value=Random(MT);
        Distance=Input_distance_field->at<float>(i)+Weight;
        if (Distance<=Value){
          if (Input_borders->at<unsigned char>(i)==0){
            Output0->at<unsigned char>(i)=0;
          }
        }
        else{
          if (Input_areas->at<unsigned char>(i)==0){
            Output0->at<unsigned char>(i)=1;
          }
        }
      }
    }
  }
}


void _filter_distance_field_combination::combination3(cv::Mat *Input_borders,cv::Mat *Input_areas,cv::Mat *Input_distance_field,cv::Mat *Output0)
{
  float Value;
  float Distance;
  float Weight;

  Num_black_pixels=0;

  Output0->setTo(255);

  random_device RD;
  mt19937 MT(RD());
  uniform_real_distribution<float> Random(0,std::nextafter(1.0f,2.0f));



  Weight=2*Mixer-1;


  if (Input_distance_field->type()==CV_8UC1){
    // now add the dots of image areas dependin on the distances field
    for (int i=0;i<int(Input_areas->total());i++){
      if (Input_areas->at<unsigned char>(i)==0){
        if (Input_distance_field->at<unsigned char>(i)==0) continue;
        else{
          if (Input_distance_field->at<unsigned char>(i)>=Random(MT)){
            Output0->at<unsigned char>(i)=0;
            Num_black_pixels++;
          }
        }
      }
    }
  }
  else{ // float distance field
    for (int i=0;i<int(Input_areas->total());i++){
      if (Input_borders->at<unsigned char>(i)!=255 || Input_areas->at<unsigned char>(i)!=255){

        Value=Random(MT);
        //
        Distance=Input_distance_field->at<float>(i);

        if (Distance<=Value_function_a) Distance=0;
        else{
          if (Distance<=Value_function_b){
            Distance=(Distance-Value_function_a)/(Value_function_b-Value_function_a);
          }
          else{
            Distance=1;
          }
        }

        Distance=Distance+Weight;
        if (Distance<=Value){
          if (Input_borders->at<unsigned char>(i)==0){
            Output0->at<unsigned char>(i)=0;
            Num_black_pixels++;
          }
        }
        else{
          if (Input_areas->at<unsigned char>(i)==0){
            Output0->at<unsigned char>(i)=1;
            Num_black_pixels++;
          }
        }
      }
    }
  }

  info1(Num_black_pixels);
}


void _filter_distance_field_combination::combination4(cv::Mat *Input_borders,cv::Mat *Input_areas,cv::Mat *Input_distance_field,cv::Mat *Output0)
{
  float Value;
  float Distance;
  float Weight;

  Output0->setTo(255);

  random_device RD;
  mt19937 MT(RD());
  uniform_real_distribution<float> Random(0,std::nextafter(1.0f,2.0f));



  Weight=2*Mixer-1;


  if (Input_distance_field->type()==CV_8UC1){
    // now add the dots of image areas dependin on the distances field
    for (int i=0;i<int(Input_areas->total());i++){
      if (Input_areas->at<unsigned char>(i)==0){
        if (Input_distance_field->at<unsigned char>(i)==0) continue;
        else{
          if (Input_distance_field->at<unsigned char>(i)>=Random(MT)){
            Output0->at<unsigned char>(i)=0;
          }
        }
      }
    }
  }
  else{ // float distance field
    for (int i=0;i<int(Input_areas->total());i++){
      if (Input_borders->at<unsigned char>(i)!=255 || Input_areas->at<unsigned char>(i)!=255){

        Value=Random(MT);
        //
        Distance=Input_distance_field->at<float>(i);

        if (Value_function_a<=Value_function_b){
          if (Distance<=Value_function_a) Distance=0;
          else{
            if (Distance<=Value_function_b){
              Distance=(Distance-Value_function_a)/(Value_function_b-Value_function_a);
            }
            else{
              Distance=1;
            }
          }
        }
        else{
          if (Distance<=Value_function_b) Distance=1;
          else{
            if (Distance<=Value_function_a){
              Distance=(Distance-Value_function_b)/(Value_function_a-Value_function_b)+1;
            }
            else{
              Distance=0;
            }
          }
        }


        Distance=Distance+Weight;
        if (Distance<=Value){
          if (Input_borders->at<unsigned char>(i)==0){
            Output0->at<unsigned char>(i)=0;
          }
        }
        else{
          if (Input_areas->at<unsigned char>(i)==0){
            Output0->at<unsigned char>(i)=1;
          }
        }
      }
    }
  }
}


// 201201
// prueba para mantener los puntos de los bordes

void _filter_distance_field_combination::combination5(cv::Mat *Input_borders,cv::Mat *Input_areas,cv::Mat *Input_distance_field,cv::Mat *Output0)
{
  float Value;
  float Distance;
//  float Weight;

  Num_black_pixels=0;

  Output0->setTo(255);

  random_device RD;
  mt19937 MT(RD());
  uniform_real_distribution<float> Random(0,std::nextafter(1.0f,2.0f));

//  Weight=2*Mixer-1;

  if (Input_distance_field->type()==CV_8UC1){
    // now add the dots of image areas dependin on the distances field
    for (int i=0;i<int(Input_areas->total());i++){
      if (Input_areas->at<unsigned char>(i)==0){
        if (Input_distance_field->at<unsigned char>(i)==0) continue;
        else{
          if (Input_distance_field->at<unsigned char>(i)>=Random(MT)){
            Output0->at<unsigned char>(i)=0;
            Num_black_pixels++;
          }
        }
      }
    }
  }
  else{ // float distance field
    for (int i=0;i<int(Input_areas->total());i++){
      if (Input_borders->at<unsigned char>(i)!=255){
        Output0->at<unsigned char>(i)=0;
        Num_black_pixels++;
      }
      else{
        if (Input_areas->at<unsigned char>(i)!=255){
          Value=Random(MT);
          //
          Distance=Input_distance_field->at<float>(i);

          if (Distance<=Value){
            Output0->at<unsigned char>(i)=1;
            Num_black_pixels++;
          }
        }
      }
    }
  }

  info1(Num_black_pixels);
}



//HEA

void _filter_distance_field_combination::update_distance_field(cv::Mat *Input_borders,cv::Mat *Input_areas,cv::Mat *Input_distance_field)
{
  Q_UNUSED(Input_borders)
  // check the max_distance and min_distance of the areas

  float Max_areas=-10000;
  for (int i=0;i<int(Input_areas->total());i++){
    if (Input_areas->at<unsigned char>(i)!=255){
      if (Input_distance_field->at<float>(i)>Max_areas) Max_areas=Input_distance_field->at<float>(i);
    }
  }

  Max_distance=Max_areas;

  for (int i=0;i<int(Input_distance_field->total());i++){
    Input_distance_field->at<float>(i)=Input_distance_field->at<float>(i)/Max_distance;
  }
}

//HEA

void _filter_distance_field_combination::update_filter()
{
  // last one was 4 for white borders
  combination3(Aux_input_images[0],Aux_input_images[1],Aux_input_images[2],Output_image_0);
//  combination5(Aux_input_images[0],Aux_input_images[1],Aux_input_images[2],Output_image_0);
}

//HEA

void _filter_distance_field_combination::update()
{
  if (Input_images[0]==nullptr){
    QMessageBox MsgBox;
    MsgBox.setText("Error: distance_field_combination filter: no Input_images[0]");
    MsgBox.exec();
    qApp->quit();
    exit(-1);
  }

  for (int i=0;i<_f_filter_ns::MAX_CONNECTIONS;i++) Aux_input_images[i]=nullptr;

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

    update_distance_field(Aux_input_images[0],Aux_input_images[1],Aux_input_images[2]);

    Changed_input=false;
  }

  update_filter();

  for (int i=0;i<_f_filter_ns::MAX_CONNECTIONS;i++){
    if (Aux_input_images[i]!=nullptr && Aux_input_images[i]!=Input_images[i]) delete Aux_input_images[i];
  }
}

//HEA

void _filter_distance_field_combination::read_parameters(std::map<std::string,std::string> &Parameters)
{
  if (Parameters["_INI_"]=="EDITOR"){// default parameters
    mixer(DISTANCE_FIELD_COMBINATION_MIXER_DEFAULT);
    value_function_a(DISTANCE_FIELD_VALUE_FUNCTON_A_DEFAULT);
    value_function_b(DISTANCE_FIELD_VALUE_FUNCTON_B_DEFAULT);
  }
  else{// Parameters from file or from initialised filter
    try{
      if (Parameters["mixer"]=="default") mixer(DISTANCE_FIELD_COMBINATION_MIXER_DEFAULT);
      else mixer(float(atof(Parameters["mixer"].c_str())));

      if (Parameters["value_function_a"]=="default") value_function_a(DISTANCE_FIELD_VALUE_FUNCTON_A_DEFAULT);
      else value_function_a(float(atof(Parameters["value_function_a"].c_str())));

      if (Parameters["value_function_b"]=="default") value_function_b(DISTANCE_FIELD_VALUE_FUNCTON_B_DEFAULT);
      else value_function_b(float(atof(Parameters["value_function_b"].c_str())));

    }
    catch (const std::out_of_range) {
      QMessageBox MsgBox;
      MsgBox.setText("Error in the data of DISTANCE FIEL COMBINATION filter");
      MsgBox.exec();
      qApp->quit();
      exit(-1);
    }
  }
}

//HEA

void _filter_distance_field_combination::write_parameters(std::map<std::string,std::string> &Parameters)
{
  char Aux[100];

  sprintf(Aux,"%f",double(mixer()));
  Parameters["mixer"]=string(Aux);

  sprintf(Aux,"%f",double(value_function_a()));
  Parameters["value_function_a"]=string(Aux);

  sprintf(Aux,"%f",double(value_function_b()));
  Parameters["value_function_b"]=string(Aux);
}

//HEA

_filter_distance_field_combination_ui::_filter_distance_field_combination_ui(_gl_widget *GL_widget1, string Name1)
{
  Name=Name1;
  Qtw_filter_distance_field_combination=new _qtw_filter_distance_field_combination(GL_widget1,this,Name);
}

//HEA

_filter_distance_field_combination_ui::_filter_distance_field_combination_ui(_gl_widget *GL_widget1, std::map<string, string> &Parameters, string Name1)
{
  read_parameters(Parameters);
  Name=Name1;
  Qtw_filter_distance_field_combination=new _qtw_filter_distance_field_combination(GL_widget1,this,Name);
}

//HEA

_filter_distance_field_combination_ui::~_filter_distance_field_combination_ui()
{
  delete Qtw_filter_distance_field_combination;
}

//HEA

void _filter_distance_field_combination_ui::reset_data()
{
  _filter_distance_field_combination::reset_data();
  Qtw_filter_distance_field_combination->parameter1(parameter1());
  hide();
}

//HEA

void _filter_distance_field_combination_ui::show()
{
  Qtw_filter_distance_field_combination->show();
}

//HEA

void _filter_distance_field_combination_ui::hide()
{
  Qtw_filter_distance_field_combination->hide();
}

//HEA

void *_filter_distance_field_combination_ui::get_link()
{
  return Qtw_filter_distance_field_combination->get_link();
}


//HEA

void _filter_distance_field_combination_ui::info1(unsigned int Value1)
{
  Qtw_filter_distance_field_combination->info(Value1);
}


//HEA

_qtw_filter_distance_field_combination::_qtw_filter_distance_field_combination(_gl_widget *GL_widget1,_filter_distance_field_combination_ui *Filter1,string Box_name)
{
  GL_widget=GL_widget1;
  Filter=Filter1;

  Group_box_main=new QGroupBox(tr(Box_name.c_str()));
  Group_box_main->setAlignment(Qt::AlignCenter);

  QVBoxLayout *Vertical_box_main=new QVBoxLayout;

  // Parameter 1
  Group_box_parameter1=new QGroupBox(tr(String_group_box_parameter1.c_str()));
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
  Slider_parameter1->setToolTip(tr(String_parameter1_slider_tooltip.c_str()));

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

  // Parameter 3
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

  // info
  QGroupBox *Group_box_info1=new QGroupBox(tr(String_group_box_info1.c_str()));
  Group_box_info1->setAlignment(Qt::AlignCenter);

  QVBoxLayout *Verticalbox_info1 = new QVBoxLayout;

  Line_edit_info1=new QLineEdit();
  Line_edit_info1->setAlignment(Qt::AlignRight);
  Line_edit_info1->setReadOnly(true);
  Line_edit_info1->setEnabled(false);
  // warnig to the adjust
  QString Str=Str.number(0);
  Line_edit_info1->setText(Str);

  Verticalbox_info1->addWidget(Line_edit_info1);

  Group_box_info1->setLayout(Verticalbox_info1);


  //
  Vertical_box_main->addWidget(Group_box_parameter1);
  Vertical_box_main->addWidget(Group_box_parameter2);
  Vertical_box_main->addWidget(Group_box_parameter3);
  Vertical_box_main->addWidget(Group_box_info1);
  Vertical_box_main->addStretch();

  Group_box_main->setLayout(Vertical_box_main);
}

//HEA

void _qtw_filter_distance_field_combination::parameter1(float Value)
{
  QString Str;
  char Aux[100];

  sprintf(Aux,"%4.3f",double(Value));
  Value=Value*_f_distance_field_combination_ns::Parameter1_factor;

  Str=Aux;
  Line_edit_parameter1->setText(Str);
  Slider_parameter1->blockSignals(true);
  Slider_parameter1->setValue(int(Value));
  Slider_parameter1->blockSignals(false);
}

//HEA

void _qtw_filter_distance_field_combination::parameter2(float Value)
{
  QString Str;
  char Aux[100];

  sprintf(Aux,"%4.3f",double(Value));
  Value=Value*_f_distance_field_combination_ns::Parameter2_factor;

  Str=Aux;
  Line_edit_parameter2->setText(Str);
  Slider_parameter2->blockSignals(true);
  Slider_parameter2->setValue(int(Value));
  Slider_parameter2->blockSignals(false);
}

//HEA

void _qtw_filter_distance_field_combination::parameter3(float Value)
{
  QString Str;
  char Aux[100];

  sprintf(Aux,"%4.3f",double(Value));
  Value=Value*_f_distance_field_combination_ns::Parameter3_factor;

  Str=Aux;
  Line_edit_parameter3->setText(Str);
  Slider_parameter3->blockSignals(true);
  Slider_parameter3->setValue(int(Value));
  Slider_parameter3->blockSignals(false);
}

//HEA

void _qtw_filter_distance_field_combination::info(unsigned int Value)
{
  QString Str;

  Str=Str.number(Value);
  Line_edit_info1->setText(Str);
}

//HEA

void _qtw_filter_distance_field_combination::parameter1_slot(int Size)
{
   QString Str;
  char Aux[100];
  float Value=float(Size)/_f_distance_field_combination_ns::Parameter1_factor;

  sprintf(Aux,"%4.3f",double(Value));
  Str=Aux;
  Line_edit_parameter1->setText(Str);
  Filter->parameter1(Value);
  GL_widget->update_effect(Filter->name());
}

//HEA

void _qtw_filter_distance_field_combination::parameter2_slot(int Size)
{
   QString Str;
  char Aux[100];
  float Value=float(Size)/_f_distance_field_combination_ns::Parameter2_factor;

  sprintf(Aux,"%4.3f",double(Value));
  Str=Aux;
  Line_edit_parameter2->setText(Str);
  Filter->parameter2(Value);
  GL_widget->update_effect(Filter->name());
}

//HEA

void _qtw_filter_distance_field_combination::parameter3_slot(int Size)
{
   QString Str;
  char Aux[100];
  float Value=float(Size)/_f_distance_field_combination_ns::Parameter3_factor;

  sprintf(Aux,"%4.3f",double(Value));
  Str=Aux;
  Line_edit_parameter3->setText(Str);
  Filter->parameter3(Value);
  GL_widget->update_effect(Filter->name());
}
