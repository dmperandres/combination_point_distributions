//LIC

#include "filter_dots_edges.h"
#include "glwidget.h"

using namespace _f_dots_edges_ns;

//HEA

_filter_dots_edges::_filter_dots_edges()
{
  Num_channels_output_image_0=1;

  Type_filter=_f_filter_ns::FILTER_DOTS_EDGES;

  Type_input_images[0]=_f_filter_ns::TYPE_UI_8;
  Type_output_0=_f_filter_ns::TYPE_UI_8;

  Scaling_factor=1;
  Change_output_image_size=false;
  Use_dots=false;

  Distance=DOTS_EDGES_DISTANCE_DEFAULT;
}

//HEA

void _filter_dots_edges::reset_data()
{
}

//HEA

int _filter_dots_edges::search_next_pixel(int Row,int Col,cv::Mat *Aux,int &Row1,int &Col1,float &Distance1)
{
  // 107
  // 2 6
  // 345

  if (Row>0 && Row<Aux->rows-1 && Col>0 && Col<Aux->cols-1){
    if (Aux->at<unsigned char>(Row+1,Col)==0){
      Row1=Row+1;
      Col1=Col;
      Distance1=1;
      return 0;
    }
    if (Aux->at<unsigned char>(Row+1,Col-1)==0){
      Row1=Row+1;
      Col1=Col-1;
      Distance1=1.4142f;
      return 1;
    }
    if (Aux->at<unsigned char>(Row,Col-1)==0){
      Row1=Row;
      Col1=Col-1;
      Distance1=1;
      return 2;
    }
    if (Aux->at<unsigned char>(Row-1,Col-1)==0){
      Row1=Row-1;
      Col1=Col-1;
      Distance1=1.4142f;
      return 3;
    }
    if (Aux->at<unsigned char>(Row-1,Col)==0){
      Row1=Row-1;
      Col1=Col;
      Distance1=1;
      return 4;
    }
    if (Aux->at<unsigned char>(Row-1,Col+1)==0){
      Row1=Row-1;
      Col1=Col+1;
      Distance1=1.4142f;
      return 5;
    }
    if (Aux->at<unsigned char>(Row,Col+1)==0){
      Row1=Row;
      Col1=Col+1;
      Distance1=1;
      return 6;
    }
    if (Aux->at<unsigned char>(Row+1,Col+1)==0){
      Row1=Row+1;
      Col1=Col+1;
      Distance1=1.4142f;
      return 7;
    }
    return -1;
  }
  else return -1;
}

//HEA

void _filter_dots_edges::border(cv::Mat *Input0,cv::Mat *Output0)
{
  list<_vertex2i> List_pos_black_pixels;
  vector<vector<list<_vertex2i>::iterator>> Matrix;
  list<_vertex2i>::iterator It;
  cv::Mat Image_aux;
  _vertex2i Seed;
  int Pos;
  int Row1,Col1;
  float Distance0,Distance1;

  Matrix.resize(Input0->rows);
  for (int Row=0;Row<Input0->rows;Row++) Matrix[Row].resize(Input0->cols);

  Output0->setTo(255);

  Image_aux=Input0->clone();

  for (int Row=1;Row<Input0->rows-1;Row++){
    for (int Col=1;Col<Input0->cols-1;Col++){
      if (Image_aux.at<unsigned char>(Row,Col)==0){
        // *x*
        // xx*
        // ***
        if (Image_aux.at<unsigned char>(Row+1,Col)==0 && Image_aux.at<unsigned char>(Row,Col-1)==0){
          Image_aux.at<unsigned char>(Row,Col)=255;
          continue;
        }
        // *x*
        // *xx
        // ***
        if (Image_aux.at<unsigned char>(Row+1,Col)==0 && Image_aux.at<unsigned char>(Row,Col+1)==0){
          Image_aux.at<unsigned char>(Row,Col)=255;
          continue;
        }
        // ***
        // xx*
        // *x*
        if (Image_aux.at<unsigned char>(Row,Col-1)==0 && Image_aux.at<unsigned char>(Row-1,Col)==0){
          Image_aux.at<unsigned char>(Row,Col)=255;
          continue;
        }
        // ***
        // *xx
        // *x*
        if (Image_aux.at<unsigned char>(Row,Col+1)==0 && Image_aux.at<unsigned char>(Row-1,Col)==0){
          Image_aux.at<unsigned char>(Row,Col)=255;
          continue;
        }
      }
    }
  }


  for (int Row=0;Row<Input0->rows;Row++){
    for (int Col=0;Col<Input0->cols;Col++){
      Matrix[Row][Col]=List_pos_black_pixels.end();
    }
  }

  // first the black pixels are saved in a vector
  for (int Row=0;Row<Input0->rows;Row++){
    for (int Col=0;Col<Input0->cols;Col++){
      if (Image_aux.at<unsigned char>(Row,Col)==0){
        List_pos_black_pixels.push_back(_vertex2i(Row,Col));
        Matrix[Row][Col]=--List_pos_black_pixels.end();
      }
    }
  }

  random_device RD;
  mt19937 MT(RD());
  uniform_real_distribution<float> Random(-1,std::nextafter(1.0,2.0));

  while (List_pos_black_pixels.size()>0){
    Distance0=0;
    // now we get the seed
    Seed=List_pos_black_pixels.front();
    // remove the element
    List_pos_black_pixels.pop_front();
    // remove from the mat
    Matrix[Seed.row][Seed.col]=List_pos_black_pixels.end();
    // remove from aux
    Image_aux.at<unsigned char>(Seed.row,Seed.col)=255;

    // put pixel in output
    Output0->at<unsigned char>(Seed.row,Seed.col)=0;

    // now search a black pixel from top position CCW
    // 107
    // 2 6
    // 345

    float Distance_aux;
//    float Distance_limit=Distance;
    Pos=search_next_pixel(Seed.row,Seed.col,&Image_aux,Row1,Col1,Distance1);
    while (Pos!=-1){
      Distance0+=Distance1;//+Random(MT);
      Distance_aux=Distance*(1-Displacement_randomness*Random(MT));
      if (Distance0>=Distance_aux){
        Output0->at<unsigned char>(Row1,Col1)=0;
        Distance0-=Distance_aux;
      }
      // remove from list
//      cout << "Row1=" << Row1 << " Col1=" << Col1 << endl;
      It=Matrix[Row1][Col1];
      List_pos_black_pixels.erase(It);
      // remove from the mat
      Matrix[Row1][Col1]=List_pos_black_pixels.end();
      // remove from aux
      Image_aux.at<unsigned char>(Row1,Col1)=255;

      Pos=search_next_pixel(Row1,Col1,&Image_aux,Row1,Col1,Distance1);
    }
  }
}

//HEA

void _filter_dots_edges::update_filter()
{
  border(Aux_input_images[0],Output_image_0);
}

//HEA

void _filter_dots_edges::update()
{
  if (Input_images[0]==nullptr){
    QMessageBox MsgBox;
    MsgBox.setText("Error: dot_edges filter: no Input_images[0]");
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

void _filter_dots_edges::read_parameters(std::map<std::string,std::string> &Parameters)
{
  if (Parameters["_INI_"]=="EDITOR"){// default parameters
    distance(DOTS_EDGES_DISTANCE_DEFAULT);
    displacement_randomness(DOTS_EDGES_DISPLACEMENT_RANDONESS_DEFAULT);
  }
  else{// Parameters from file or from initialised filter
    try{
      if (Parameters["distance"]=="default") distance(DOTS_EDGES_DISTANCE_DEFAULT);
      else distance(atof(Parameters["distance"].c_str()));

      if (Parameters["displacement_randomness"]=="default") displacement_randomness(DOTS_EDGES_DISPLACEMENT_RANDONESS_DEFAULT);
      else displacement_randomness(atof(Parameters["displacement_randomness"].c_str()));
    }
    catch (const std::out_of_range) {
      QMessageBox MsgBox;
      MsgBox.setText("Error in the data of DOTS_EDGES filter");
      MsgBox.exec();
      qApp->quit();
      exit(-1);
    }
  }
}

//HEA

void _filter_dots_edges::write_parameters(std::map<std::string,std::string> &Parameters)
{
  char Aux[100];

  sprintf(Aux,"%5.2f",distance());
  Parameters["distance"]=string(Aux);
  sprintf(Aux,"%5.2f",displacement_randomness());
  Parameters["displacement_randomness"]=string(Aux);
}

//HEA

_filter_dots_edges_ui::_filter_dots_edges_ui(_gl_widget *GL_widget1,string Name1)
{
  Name=Name1;
  Qtw_filter_dots_edges=new _qtw_filter_dots_edges(GL_widget1,this,Name1);
}

//HEA

_filter_dots_edges_ui::_filter_dots_edges_ui(_gl_widget *GL_widget1, std::map<string, string> &Parameters, string Name1)
{
  read_parameters(Parameters);
  Name=Name1;
  Qtw_filter_dots_edges=new _qtw_filter_dots_edges(GL_widget1,this,Name1);
}

//HEA

_filter_dots_edges_ui::~_filter_dots_edges_ui()
{
  delete Qtw_filter_dots_edges;
}

//HEA

void _filter_dots_edges_ui::reset_data()
{
  _filter_dots_edges::reset_data();
  hide();
}

//HEA

void _filter_dots_edges_ui::show()
{
  Qtw_filter_dots_edges->show();
}

//HEA

void _filter_dots_edges_ui::hide()
{
  Qtw_filter_dots_edges->hide();
}

///HEA

void *_filter_dots_edges_ui::get_link()
{
  return Qtw_filter_dots_edges->get_link();
}

//HEA

_qtw_filter_dots_edges::_qtw_filter_dots_edges(_gl_widget *GL_widget1,_filter_dots_edges_ui *Filter1,string Box_name)
{
  GL_widget=GL_widget1;
  Filter=Filter1;

  Group_box_main=new QGroupBox(tr(Box_name.c_str()));
  Group_box_main->setAlignment(Qt::AlignCenter);

  QVBoxLayout *Vertical_box_main=new QVBoxLayout;

  // Parameter1
  QGroupBox *Group_box_parameter1=new QGroupBox(tr(String_group_box_parameter1.c_str()));
  Group_box_parameter1->setAlignment(Qt::AlignCenter);

  QVBoxLayout *Verticalbox_parameter1=new QVBoxLayout;

  Doublespinbox_parameter1=new QDoubleSpinBox;
  Doublespinbox_parameter1->setMinimum(DOTS_EDGES_MIN_DISTANCE_DEFAULT);
  Doublespinbox_parameter1->setMaximum(DOTS_EDGES_MAX_DISTANCE_DEFAULT);
  Doublespinbox_parameter1->setSingleStep(1.0);
  Doublespinbox_parameter1->setDecimals(2);

  parameter1(Filter->parameter1());

  Verticalbox_parameter1->addWidget(Doublespinbox_parameter1);

  Group_box_parameter1->setLayout(Verticalbox_parameter1);

  connect(Doublespinbox_parameter1, SIGNAL(valueChanged(double)),this,SLOT(parameter1_slot(double)));

  // Parameter 2 Displacement factor
  QGroupBox *Group_box_parameter2=new QGroupBox(tr(String_group_box_parameter2.c_str()));
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
  Slider_parameter2->setToolTip(tr(String_parameter2_tooltip.c_str()));

  parameter2(Filter->parameter2());

  Grid_parameter2->addWidget(Line_edit_parameter2,0,1,Qt::AlignCenter);
  Grid_parameter2->addWidget(Label_parameter2_min,1,0,Qt::AlignRight);
  Grid_parameter2->addWidget(Slider_parameter2,1,1);
  Grid_parameter2->addWidget(Label_parameter2_max,1,2,Qt::AlignLeft);

  Group_box_parameter2->setLayout(Grid_parameter2);

  connect(Slider_parameter2, SIGNAL(valueChanged(int)),this,SLOT(parameter2_slot(int)));
  //
  Vertical_box_main->addWidget(Group_box_parameter1);
  Vertical_box_main->addWidget(Group_box_parameter2);
  Vertical_box_main->addStretch();

  Group_box_main->setLayout(Vertical_box_main);
}

//HEA

void _qtw_filter_dots_edges::parameter1(float Value)
{
  Doublespinbox_parameter1->blockSignals(true);
  Doublespinbox_parameter1->setValue(Value);
  Doublespinbox_parameter1->blockSignals(false);
}

//HEA

void _qtw_filter_dots_edges::parameter2(float Value)
{
  QString Str;
  char Aux[100];

  sprintf(Aux,"%2.2f",Value);
  Str=Aux;
  Line_edit_parameter2->setText(Str);
  Value=int(Value*Parameter2_factor);
  Slider_parameter2->blockSignals(true);
  Slider_parameter2->setValue(Value);
  Slider_parameter2->blockSignals(false);
}

//HEA

void _qtw_filter_dots_edges::parameter1_slot(double Value)
{
  Filter->parameter1(float(Value));
  GL_widget->update_effect(Filter->name());
}

//HEA

void _qtw_filter_dots_edges::parameter2_slot(int Value)
{
  QString Str;
  float Value1=float(Value)/Parameter2_factor;

  Str=Str.number(float(Value1),'f',2);
  Line_edit_parameter2->setText(Str);
  Filter->parameter2(Value1);
  Filter->update();
  GL_widget->update_effect(Filter->name());
}
