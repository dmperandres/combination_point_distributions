//LIC

#include "filter_stippling_ebg.h"
#include "glwidget.h"

using namespace _f_stippling_ebg_ns;

//HEA

_filter_stippling_ebg::_filter_stippling_ebg()
{
  Output_size=STIPPLING_EBG_OUTPUT_SIZE_DEFAULT;
  Scaling_factor=STIPPLING_EBG_SCALING_FACTOR_DEFAULT;
  Displacement_randomness=STIPPLING_EBG_DISPLACEMENT_DEFAULT;
  Num_rows_dot_matrix=STIPPLING_EBG_NUM_ROWS_DOT_MATRIX_DEFAULT;
  Num_cols_dot_matrix=STIPPLING_EBG_NUM_COLS_DOT_MATRIX_DEFAULT;
  Dot_size_min=STIPPLING_EBG_MIN_DOT_SIZE_DEFAULT;
  Dot_size_max=STIPPLING_EBG_MAX_DOT_SIZE_DEFAULT;
  Pixel_density=STIPPLING_EBG_PIXEL_DENSITY_DEFAULT;
  Pixel_density_factor=VEC_PIXEL_DENSITY_FACTOR[Pixel_density];
  Black_level=STIPPLING_EBG_BLACK_LEVEL_DEFAULT;

  Num_channels_output_image_0=1;

  Type_filter=_f_filter_ns::FILTER_STIPPLING_EBG;

  Type_input_images[0]=_f_filter_ns::TYPE_UI_8;
  Type_output_0=_f_filter_ns::TYPE_UI_8;

  Change_output_image_size=true;

  Seeds_initialized=false;
  Dots_texture_packet=0;
  Use_dots=true;
}

//HEA

void _filter_stippling_ebg::reset_data()
{
  Output_size=STIPPLING_EBG_OUTPUT_SIZE_DEFAULT;
  Scaling_factor=STIPPLING_EBG_SCALING_FACTOR_DEFAULT;
  Displacement_randomness=STIPPLING_EBG_DISPLACEMENT_DEFAULT;
  Num_rows_dot_matrix=STIPPLING_EBG_NUM_ROWS_DOT_MATRIX_DEFAULT;
  Num_cols_dot_matrix=STIPPLING_EBG_NUM_COLS_DOT_MATRIX_DEFAULT;
  Dot_size_min=STIPPLING_EBG_MIN_DOT_SIZE_DEFAULT;
  Dot_size_max=STIPPLING_EBG_MAX_DOT_SIZE_DEFAULT;
  Pixel_density=STIPPLING_EBG_PIXEL_DENSITY_DEFAULT;
  Pixel_density_factor=VEC_PIXEL_DENSITY_FACTOR[Pixel_density];
  Black_level=STIPPLING_EBG_BLACK_LEVEL_DEFAULT;
}

//HEA

void _filter_stippling_ebg::update_size()
{
  int Width1;
  int Height1;
  float Ratio1;
  int Width2;
  int Height2;

  GL_widget->get_input_size(Width1,Height1);
  Ratio1=float(Width1)/float(Height1);

  switch (Output_size){
    case _dots_ebg_ext_ns::_output_size::_OUTPUT_SIZE_OPTIMAL:
      Width2=0;
    break;
    case _dots_ebg_ext_ns::_output_size::_OUTPUT_SIZE_A4:
      Width2=3506;
    break;
    case _dots_ebg_ext_ns::_output_size::_OUTPUT_SIZE_A5:
      Width2=2480;
    break;
    case _dots_ebg_ext_ns::_output_size::_OUTPUT_SIZE_A6:
      Width2=1753;
    break;
    case _dots_ebg_ext_ns::_output_size::_OUTPUT_SIZE_A7:
      Width2=1240;
    break;
    case _dots_ebg_ext_ns::_output_size::_OUTPUT_SIZE_A8:
      Width2=876;
    break;
    case _dots_ebg_ext_ns::_output_size::_OUTPUT_SIZE_A9:
      Width2=620;
    break;
    case _dots_ebg_ext_ns::_output_size::_OUTPUT_SIZE_A10:
      Width2=438;
    break;
  }

  if (Width2!=0){
    // compute the width depending of the pixel density
//    Width2=Width2*Pixel_density_factor;
    // compute the width that corresponds to the Scale_factor
    Width2=int(float(Width2)/Scale_factor);

    Height2=int(float(Width2)/Ratio1);

    adjust_image_sizes(Width2,Height2);

//    cout << "new size width=" << Width2 << " height=" << Height2 << endl;

    GL_widget->change_input_size(Width2,Height2);

    GL_widget->update_all_filters_changed_input();
    GL_widget->update_all_filters();
    GL_widget->refresh_image();
  }
  else{
    GL_widget->change_input_size(Width1,Height1);
  }
}


//HEA

void _filter_stippling_ebg::pixel_density(int Pixel_density1)
{
  Pixel_density=Pixel_density1;
  Pixel_density_factor=VEC_PIXEL_DENSITY_FACTOR[Pixel_density];
}

//HEA

void _filter_stippling_ebg::put_dot(cv::Mat *Output_image0,float Row1,float Col1,unsigned int Selected_dot_size1,unsigned int Index_row1,unsigned int Index_col1)
{
  int Row,Col;
  int Row_out,Col_out;
  unsigned char Value;
  unsigned int Row_aux=Selected_dot_size1*Index_row1*Pixel_density_factor;
  unsigned int Col_aux=Selected_dot_size1*Index_col1*Pixel_density_factor;

  // All the pixels of the dot are modified
  for (Row=0;Row<int(Selected_dot_size1*Pixel_density_factor);Row++){
    for (Col=0;Col<int(Selected_dot_size1*Pixel_density_factor);Col++){
      // get the value of the image of dots of size Selected_dot_size1

      Value=(*Dots)[Pixel_density][Selected_dot_size1]->at<unsigned char>(Row+Row_aux,Col+Col_aux);
      // output position
      Row_out=Row1+Row;
      Col_out=Col1+Col;

      if (Row_out<0 || Row_out>=Output_image0->rows || Col_out<0 || Col_out>=Output_image0->cols) continue;
      // the row and col are correct, the color is changed
      Output_image0->at<unsigned char>(Row_out,Col_out)=(unsigned char)((float)Output_image0->at<unsigned char>(Row_out,Col_out)*(float)Value/255.0);
    }
  }
}

//HEA

void _filter_stippling_ebg::stippling(cv::Mat *Input_image0,cv::Mat *Output_image0)
{
  int Row,Col,Selected_dot_size;
  float Pos_row,Pos_col;
  int Selected_row,Selected_col;
  _vertex2f Position;
  unsigned int Counter_of_dots=0;

  if (Seeds_initialized==false){
    Index_row.seed();
    Dot_size.seed();
    Index_col.seed();
    Displacement.seed();
    save_seeds();
    Seeds_initialized=true;
  }
  else load_seeds();

  Index_row.init(0,Num_rows_dot_matrix-1);
  Index_col.init(0,Num_cols_dot_matrix-1);
  Dot_size.init(Dot_size_min,Dot_size_max-1);
  Displacement.init(-Displacement_randomness,Displacement_randomness);

  for (Row=0;Row<Input_image0->rows;Row++){
    for (Col=0;Col<Input_image0->cols;Col++){
      if (Input_image0->at<unsigned char>(Row,Col)!=WHITE){
        // compute a random dot size
        Selected_dot_size=Dot_size.value();

        Counter_of_dots++;
        //
        Selected_row=int(Index_row.value());
        Selected_col=int(Index_col.value());

        Pos_row=float(Row)*Scaling_factor*float(Pixel_density_factor)+(Displacement.value()*float(AVERAGE_DOT_SIZE)*float(Pixel_density_factor))-float(Selected_dot_size)/2.0f;
        Pos_col=float(Col)*Scaling_factor*float(Pixel_density_factor)+(Displacement.value()*float(AVERAGE_DOT_SIZE)*float(Pixel_density_factor))-float(Selected_dot_size)/2.0f;
        put_dot(Output_image0,Pos_row,Pos_col,Selected_dot_size,Selected_row,Selected_col);
      }
    }
  }

  cout << "EBG Num black dots=" << Counter_of_dots << endl;

  info1(Counter_of_dots);

  if (Output_mode==_dots_ebg_ext_ns::OUTPUT_MODE_MONO_COLOR){
    for (Row=0;Row<Output_image_0->rows;Row++){
      for (Col=0;Col<Output_image_0->cols;Col++){
        if (Output_image_0->at<unsigned char>(Row,Col)<=Black_level) Output_image_0->at<unsigned char>(Row,Col)=BLACK;
        else Output_image_0->at<unsigned char>(Row,Col)=WHITE;
      }
    }
  }
}

//HEA

void _filter_stippling_ebg::update_filter()
{
  Output_image_0->setTo(255);
  stippling(Aux_input_images[0],Output_image_0);
}

//HEA

void _filter_stippling_ebg::update()
{
  if (Input_images[0]==nullptr){
    QMessageBox MsgBox;
    MsgBox.setText("Error: stippling_ebg filter: no Input_images[0]");
    MsgBox.exec();
    qApp->quit();
    exit(-1);
  }

  for (int i=0;i<_f_filter_ns::MAX_CONNECTIONS;i++) Aux_input_images[i]=nullptr;

  if (Changed_input==true){
    int Width1=int(roundf(float(Input_images[0]->cols)*Scaling_factor*Pixel_density_factor));
    int Height1=int(roundf(float(Input_images[0]->rows)*Scaling_factor*Pixel_density_factor));

    adjust_image_sizes(Width1,Height1);

    Output_image_0->release();
    if (Num_channels_output_image_0==-1){ // input->output
      if (Input_images[0]->channels()==3){
        Output_image_0->create(Height1,Width1,CV_8UC3);
      }
      else{
        Output_image_0->create(Height1,Width1,CV_8UC1);
      }
    }
    else{ // adjust
      if (Num_channels_output_image_0==3){
        Output_image_0->create(Height1,Width1,CV_8UC3);
      }
      else{
        Output_image_0->create(Height1,Width1,CV_8UC1);
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

void _filter_stippling_ebg::displacement_randomness(float Displacement_randomness1)
{
  Displacement_randomness=Displacement_randomness1;
}

//HEA

void _filter_stippling_ebg::save_seeds()
{
  Index_row.save_seed();
  Index_col.save_seed();
  Displacement.save_seed();
  Dot_size.save_seed();
}

//HEA

void _filter_stippling_ebg::load_seeds()
{
  Index_row.load_seed();
  Index_col.load_seed();
  Displacement.load_seed();
  Dot_size.load_seed();
}

//HEA

void _filter_stippling_ebg::adjust_image_sizes(int &Width1, int &Height1)
{
  float Rest;

  if (Width1%4!=0){
    Rest=roundf(float(Width1)/4.0f);
    Width1=int(Rest)*4;

  }
  if (Height1%4!=0){
    Rest=roundf(float(Height1)/4.0f);
    Height1=int(Rest)*4;
  }
}

//HEA

void _filter_stippling_ebg::read_parameters(std::map<std::string,std::string> &Parameters)
{
  if (Parameters["_INI_"]=="EDITOR"){// default parameters
    output_size(STIPPLING_EBG_OUTPUT_SIZE_DEFAULT);
    pixel_density(STIPPLING_EBG_PIXEL_DENSITY_DEFAULT);
    displacement_randomness(STIPPLING_EBG_DISPLACEMENT_DEFAULT);
    output_mode(STIPPLING_EBG_OUTPUT_MODE_DEFAULT);
    black_level(STIPPLING_EBG_BLACK_LEVEL_DEFAULT);
  }
  else{// Parameters from file or from initialised filter
    try{

      if (Parameters["output_size"]=="default") output_size(STIPPLING_EBG_OUTPUT_SIZE_DEFAULT);
      else{
        if (Parameters["output_size"]=="A4") output_size(_dots_ebg_ext_ns::_output_size::_OUTPUT_SIZE_A4);
        if (Parameters["output_size"]=="A5") output_size(_dots_ebg_ext_ns::_output_size::_OUTPUT_SIZE_A5);
        if (Parameters["output_size"]=="A6") output_size(_dots_ebg_ext_ns::_output_size::_OUTPUT_SIZE_A6);
        if (Parameters["output_size"]=="A7") output_size(_dots_ebg_ext_ns::_output_size::_OUTPUT_SIZE_A7);
        if (Parameters["output_size"]=="A8") output_size(_dots_ebg_ext_ns::_output_size::_OUTPUT_SIZE_A8);
        if (Parameters["output_size"]=="A9") output_size(_dots_ebg_ext_ns::_output_size::_OUTPUT_SIZE_A9);
        if (Parameters["output_size"]=="A10") output_size(_dots_ebg_ext_ns::_output_size::_OUTPUT_SIZE_A10);
      }

      if (Parameters["pixel_density"]=="default") pixel_density(STIPPLING_EBG_PIXEL_DENSITY_DEFAULT);
      else{
        if (Parameters["pixel_density"]=="300ppi") pixel_density(int(_dots_ebg_ext_ns::_pixel_density::PIXEL_DENSITY_300PPI));
        if (Parameters["pixel_density"]=="600ppi") pixel_density(int(_dots_ebg_ext_ns::_pixel_density::PIXEL_DENSITY_600PPI));
        if (Parameters["pixel_density"]=="1200ppi") pixel_density(int(_dots_ebg_ext_ns::_pixel_density::PIXEL_DENSITY_1200PPI));
      }

      if (Parameters["displacement_ramdomness"]=="default") displacement_randomness(STIPPLING_EBG_DISPLACEMENT_DEFAULT);
      else displacement_randomness(atof(Parameters["displacement_ramdomness"].c_str()));

      if (Parameters["black_dots"]=="default") output_mode(STIPPLING_EBG_OUTPUT_MODE_DEFAULT);
      else{
        if (Parameters["black_dots"]=="true") output_mode(_dots_ebg_ext_ns::OUTPUT_MODE_MONO_COLOR);
        else black_level(int(_dots_ebg_ext_ns::OUTPUT_MODE_GRAYSCALE));
      }

      if (Parameters["black_threshold"]=="default") black_level(STIPPLING_EBG_BLACK_LEVEL_DEFAULT);
      else black_level(atoi(Parameters["black_threshold"].c_str()));
    }
    catch (const std::out_of_range) {
      QMessageBox MsgBox;
      MsgBox.setText("Error in the data of STIPPLING_EBG filter");
      MsgBox.exec();
      qApp->quit();
      exit(-1);
    }
  }
}

//HEA

void _filter_stippling_ebg::write_parameters(std::map<std::string,std::string> &Parameters)
{
  char Aux[100];

  switch (int(output_size())){
  case 0:Parameters["output_size"]=string("default");break;
  case 1:Parameters["output_size"]=string("A4");break;
  case 2:Parameters["output_size"]=string("A5");break;
  case 3:Parameters["output_size"]=string("A6");break;
  case 4:Parameters["output_size"]=string("A7");break;
  case 5:Parameters["output_size"]=string("A8");break;
  case 6:Parameters["output_size"]=string("A9");break;
  case 7:Parameters["output_size"]=string("A10");break;
  }

  switch (pixel_density()){
  case 0:Parameters["pixel_density"]=string("300ppi");break;
  case 1:Parameters["pixel_density"]=string("600ppi");break;
  case 2:Parameters["pixel_density"]=string("1200ppi");break;
  }

  sprintf(Aux,"%5.2f",displacement_randomness());
  Parameters["displacement_ramdomness"]=string(Aux);

  if (output_mode()==int(_dots_ebg_ext_ns::OUTPUT_MODE_MONO_COLOR)) Parameters["black_dots"]=string("true");
  else Parameters["black_dots"]=string("false");

  sprintf(Aux,"%3d",black_level());
  Parameters["black_threshold"]=string(Aux);
}

//HEA

_filter_stippling_ebg_ui::_filter_stippling_ebg_ui(_gl_widget *GL_widget1, string Name1)
{
  set_dots_images(&(GL_widget1->Dots_ebg));
  GL_widget=GL_widget1;
  Name=Name1;
  Qtw_filter_stippling_ebg=new _qtw_filter_stippling_ebg(GL_widget1,this,Name);
}

//HEA

_filter_stippling_ebg_ui::_filter_stippling_ebg_ui(_gl_widget *GL_widget1, std::map<string, string> &Parameters, string Box_name)
{
  set_dots_images(&(GL_widget1->Dots_ebg));
  read_parameters(Parameters);
  GL_widget=GL_widget1;
  Name=Box_name;
  Qtw_filter_stippling_ebg=new _qtw_filter_stippling_ebg(GL_widget1,this,Box_name);
}

//HEA

_filter_stippling_ebg_ui::~_filter_stippling_ebg_ui()
{
  delete Qtw_filter_stippling_ebg;
}

//HEA

void _filter_stippling_ebg_ui::reset_data()
{
  _filter_stippling_ebg::reset_data();
  Qtw_filter_stippling_ebg->parameter2(parameter2());
  hide();
}

//HEA

void _filter_stippling_ebg_ui::show()
{
  Qtw_filter_stippling_ebg->show();
}

//HEA

void _filter_stippling_ebg_ui::hide()
{
  Qtw_filter_stippling_ebg->hide();
}

//HEA

void *_filter_stippling_ebg_ui::get_link()
{
  return Qtw_filter_stippling_ebg->get_link();
}

//HEA

void _filter_stippling_ebg_ui::info1(unsigned int Value1)
{
  Qtw_filter_stippling_ebg->info1(Value1);
}

//HEA

_qtw_filter_stippling_ebg::_qtw_filter_stippling_ebg(_gl_widget *GL_widget1,_filter_stippling_ebg_ui *Filter1,string Box_name)
{
  GL_widget=GL_widget1;
  Filter=Filter1;

  Group_box_main=new QGroupBox(tr(Box_name.c_str()));
  Group_box_main->setAlignment(Qt::AlignCenter);

  QVBoxLayout *Vertical_box_main=new QVBoxLayout;

  // parameter 1: output size
  QGroupBox *Group_box_parameter1=new QGroupBox(tr(String_group_box_parameter1.c_str()));
  Group_box_parameter1->setAlignment(Qt::AlignCenter);

  QVBoxLayout *Verticalbox_parameter1=new QVBoxLayout;

  Combo_box_parameter1 = new QComboBox;
  for (unsigned int i=0; i<Vec_nom_output_size.size();i++){
    Combo_box_parameter1->addItem(QString::fromStdString(Vec_nom_output_size[i]));
  }

//  Combo_box_parameter1->setToolTip(tr("This value allows to change the output parameter2\n300ppi: mode to print in low parameter2 mono printers and for on-screen files\n600ppi: mode to print in medium parameter2 mono printers\n1200ppi: mode to print in high parameter1 mono printers"));

  parameter1(Filter->parameter1());

  Verticalbox_parameter1->addWidget(Combo_box_parameter1);

  Group_box_parameter1->setLayout(Verticalbox_parameter1);

  connect(Combo_box_parameter1, SIGNAL(currentIndexChanged(int)), this,SLOT(parameter1_slot(int)));

  // Parameter 2
  QGroupBox *Group_box_parameter2=new QGroupBox(tr(String_group_box_parameter2.c_str()));
  Group_box_parameter2->setAlignment(Qt::AlignCenter);

  QVBoxLayout *Vertical_box_parameter2=new QVBoxLayout;

  Combo_box_parameter2 = new QComboBox;
  Combo_box_parameter2->addItem(tr("300ppi"));
  Combo_box_parameter2->addItem(tr("600ppi"));
  Combo_box_parameter2->addItem(tr("1200ppi"));
//  Combo_box_parameter2->setToolTip(tr("This value allows to change the output pixel density\n300ppi: mode to print in low resolution mono printers and for on-screen files\n600ppi: mode to print in medium resolution mono printers\n1200ppi"));

  parameter2(Filter->parameter2());

  Vertical_box_parameter2->addWidget(Combo_box_parameter2);

  Group_box_parameter2->setLayout(Vertical_box_parameter2);

  connect(Combo_box_parameter2, SIGNAL(currentIndexChanged(int)), this,SLOT(parameter2_slot(int)));

  // Parameter 3
  QGroupBox *Group_box_parameter3=new QGroupBox(tr(String_group_box_parameter3.c_str()));
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
  Slider_parameter3->setToolTip(tr(String_parameter3_tooltip.c_str()));

  parameter3(Filter->parameter3());

  Grid_parameter3->addWidget(Line_edit_parameter3,0,1,Qt::AlignCenter);
  Grid_parameter3->addWidget(Label_parameter3_min,1,0,Qt::AlignRight);
  Grid_parameter3->addWidget(Slider_parameter3,1,1);
  Grid_parameter3->addWidget(Label_parameter3_max,1,3,Qt::AlignLeft);

  Group_box_parameter3->setLayout(Grid_parameter3);

  connect(Slider_parameter3, SIGNAL(valueChanged(int)),this,SLOT(parameter3_slot(int)));

  // parameter 4 output mode
  QGroupBox *Group_box_parameter4=new QGroupBox(tr(String_group_box_parameter4.c_str()));
  Group_box_parameter4->setAlignment(Qt::AlignCenter);

  QVBoxLayout *Vertical_box_parameter4=new QVBoxLayout;

  Combo_box_parameter4 = new QComboBox;
  Combo_box_parameter4->addItem(tr("Gray-scale"));
  Combo_box_parameter4->addItem(tr("Black&White"));
  Combo_box_parameter4->setToolTip(tr("This value allows to change the number of colors of the output\nGray-scale: Normal mode\nBlack-and-white: mode to print in high resolution mono printers. The user must select the black threshold"));

  parameter4(Filter->parameter4());

  Vertical_box_parameter4->addWidget(Combo_box_parameter4);

  Group_box_parameter4->setLayout(Vertical_box_parameter4);

  connect(Combo_box_parameter4, SIGNAL(currentIndexChanged(int)), this,SLOT(parameter4_slot(int)));

  // parameter 5 Black level
  Group_box_parameter5=new QGroupBox(tr(String_group_box_parameter5.c_str()));
  Group_box_parameter5->setAlignment(Qt::AlignCenter);

  QGridLayout *Grid_parameter5 = new QGridLayout;

  QLabel *Label_parameter5_min= new QLabel(tr(String_label_parameter5_min.c_str()));
  QLabel *Label_parameter5_max= new QLabel(tr(String_label_parameter5_max.c_str()));

  Line_edit_parameter5=new QLineEdit();
  Line_edit_parameter5->setAlignment(Qt::AlignRight);
  Line_edit_parameter5->setReadOnly(true);
  Line_edit_parameter5->setEnabled(false);
  Line_edit_parameter5->setToolTip(tr(String_label_parameter5_tooltip.c_str()));

  Slider_parameter5 = new QSlider(Qt::Horizontal);
  Slider_parameter5->setRange(parameter5_min_value,parameter5_max_value);
  Slider_parameter5->setSingleStep(parameter5_single_step);
  Slider_parameter5->setPageStep(parameter5_page_step);
  Slider_parameter5->setTickInterval(parameter5_tick_interval);
  Slider_parameter5->setTickPosition(QSlider::TicksRight);
  Slider_parameter5->setTracking(parameter5_set_tracking);
  Slider_parameter5->setToolTip(tr(String_parameter5_tooltip.c_str()));

  parameter5(Filter->parameter5());

  Grid_parameter5->addWidget(Line_edit_parameter5,0,1,Qt::AlignCenter);
  Grid_parameter5->addWidget(Label_parameter5_min,1,0,Qt::AlignRight);
  Grid_parameter5->addWidget(Slider_parameter5,1,1);
  Grid_parameter5->addWidget(Label_parameter5_max,1,2,Qt::AlignLeft);

  Group_box_parameter5->setLayout(Grid_parameter5);
  Group_box_parameter5->setEnabled(false);

  connect(Slider_parameter5, SIGNAL(valueChanged(int)),this,SLOT(parameter5_slot(int)));

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
  Vertical_box_main->addWidget(Group_box_parameter4);
  Vertical_box_main->addWidget(Group_box_parameter5);
  Vertical_box_main->addWidget(Group_box_info1);
  Vertical_box_main->addStretch();

  Group_box_main->setLayout(Vertical_box_main);
}

//HEA

void _qtw_filter_stippling_ebg::parameter1(int Value)
{
  Combo_box_parameter1->blockSignals(true);
  Combo_box_parameter1->setCurrentIndex(Value);
  Combo_box_parameter1->blockSignals(false);
}

//HEA

void _qtw_filter_stippling_ebg::parameter2(int Value)
{
  Combo_box_parameter2->blockSignals(true);
  Combo_box_parameter2->setCurrentIndex(Value);
  Combo_box_parameter2->blockSignals(false);
}

//HEA

void _qtw_filter_stippling_ebg::parameter3(float Value)
{
  QString Str;

  Str=Str.number(Value);
  Line_edit_parameter3->setText(Str);
  Value=Value*float(Parameter3_factor);
  Slider_parameter3->blockSignals(true);
  Slider_parameter3->setValue(int(Value));
  Slider_parameter3->blockSignals(false);
}

//HEA

void _qtw_filter_stippling_ebg::parameter4(int Value)
{
  Combo_box_parameter4->blockSignals(true);
  Combo_box_parameter4->setCurrentIndex(Value);
  Combo_box_parameter4->blockSignals(false);
}

//HEA

void _qtw_filter_stippling_ebg::parameter5(int Value)
{
  QString Str;
  char Aux[100];

  sprintf(Aux,"%d",Value);
  Str=Aux;
  Line_edit_parameter5->setText(Str);
  Slider_parameter5->blockSignals(true);
  Slider_parameter5->setValue(Value);
  Slider_parameter5->blockSignals(false);
}

//HEA

void _qtw_filter_stippling_ebg::info1(unsigned int Value)
{
  QString Str;

  Str=Str.number(Value);
  Line_edit_info1->setText(Str);
}

//HEA

void _qtw_filter_stippling_ebg::parameter1_slot(int Value)
{
  Filter->changed_input(true);
  Filter->parameter1(Value);
  Filter->update_size();
  GL_widget->update_effect(Filter->name());
}

//HEA

void _qtw_filter_stippling_ebg::parameter2_slot(int Value)
{
  Filter->changed_input(true);
  Filter->parameter2(Value);
  Filter->update_size();
  GL_widget->update_effect(Filter->name());
}

//HEA

void _qtw_filter_stippling_ebg::parameter3_slot(int Size)
{
  QString Str;
  char Aux[100];
  float Value=float(Size)/float(Parameter3_factor);

  sprintf(Aux,"%4.2f",Value);
  Str=Aux;
  Line_edit_parameter3->setText(Str);
  Filter->parameter3(Value);
  Filter->update_size();
  GL_widget->update_effect(Filter->name());
}

//HEA

void _qtw_filter_stippling_ebg::parameter4_slot(int Value)
{
  switch(_dots_ebg_ext_ns::_output_mode(Value)){
  case _dots_ebg_ext_ns::OUTPUT_MODE_GRAYSCALE:
    Group_box_parameter5->setEnabled(false);
    break;
  case _dots_ebg_ext_ns::OUTPUT_MODE_MONO_COLOR:
    Group_box_parameter5->setEnabled(true);
    break;
  default:break;
  }
  Filter->parameter4(Value);
  Filter->update_size();
  GL_widget->update_effect(Filter->name());
}

//HEA

void _qtw_filter_stippling_ebg::parameter5_slot(int Value)
{
QString Str;
char Aux[100];

  sprintf(Aux,"%d",Value);
  Str=Aux;
  Line_edit_parameter5->setText(Str);
  Filter->parameter5(Value);
  Filter->update_size();
  GL_widget->update_effect(Filter->name());
}
