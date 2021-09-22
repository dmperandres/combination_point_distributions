//LIC

#include "filter_distance_field_float.h"
#include "glwidget.h"

using namespace _f_distance_field_float_ns;

_filter_distance_field_float::_filter_distance_field_float()
{
  Num_channels_output_image_0=1;

  Type_filter=_f_filter_ns::FILTER_DISTANCE_FIELD_FLOAT;

  Type_input_images[0]=_f_filter_ns::TYPE_UI_8;
  Type_output_0=_f_filter_ns::TYPE_FLOAT_32;

  Scaling_factor=1;
  Change_output_image_size=false;
  Use_dots=false;
}

//HEA

void _filter_distance_field_float::reset_data()
{
}

//HEA

void _filter_distance_field_float::jump_flooding(cv::Mat *Input_image,cv::Mat *Output_image)
{
  vector<_f_distance_field_float_ns::_point> Image1;
  vector<_f_distance_field_float_ns::_point> Image2;
  vector<_f_distance_field_float_ns::_point> *Read=nullptr,*Write=nullptr;
  _f_distance_field_float_ns::_point Point;
  _f_distance_field_float_ns::_point Point_aux;
  int Step;
  bool Read_image1;
  int Pos;
  int Pos1;
  float Distance;
  float Distance_aux;
//  float Max_distance=-1e8;

  Image1.resize(Input_image->total());
  Image2.resize(Input_image->total());

  // asigning the seeds (distance=0,x0,y0)
  for (int i=0;i<Input_image->rows;i++){
    for (int j=0;j<Input_image->cols;j++){
      if (Input_image->at<unsigned char>(i,j)==0){
        // this is a seed
        Point.Distance=0;
        Point.x=j;
        Point.y=i;
        Image1[i*Input_image->cols+j]=Point;
      }
    }

  }

  if (Input_image->cols>=Input_image->rows) Step=Input_image->cols/2;
  else Step=Input_image->rows/2;

  Read_image1=true;

  while (Step>=1){
    // select the reading buffer
    if (Read_image1==true){
      Read=&Image1;
      Write=&Image2;
    }
    else{
      Write=&Image1;
      Read=&Image2;
    }

//    Max_distance=-1e8;
    // iterate all the pixels
    for (int i=0;i<Input_image->rows;i++){
      for (int j=0;j<Input_image->cols;j++){
        Pos=i*Input_image->cols+j;

        // Closest seed (if any)
        Point=(*Read)[static_cast<unsigned long>(Pos)];
        // Go ahead and write our current closest seed, if any. If we don't do this
        // we might lose this information if we don't update our seed this round.
        (*Write)[static_cast<unsigned long>(Pos)]=Point;

        // this is a seed, skip it
        if (Point.x == j && Point.y ==i) continue;

        if(Point.x == -1 || Point.y== -1 ){
          // No closest seed has been found yet
          Distance = -1;
        }
        else{
         // Current closest seed's distance
         Distance = sqrtf((Point.x-j)*(Point.x-j) + (Point.y-i)*(Point.y-i));
        }

        // To find each point's closest seed, we look at its 8 neighbors thusly:
        //   (x-step,y-step) (x,y-step) (x+step,y-step)
        //   (x-step,y     ) (x,y     ) (x+step,y     )
        //   (x-step,y+step) (x,y+step) (x+step,y+step)

        int Pos_y;
        int Pos_x;
        for( int k = -1; k <= 1; k++ ){
          for( int l = -1; l <= 1; l++ ){
            if (k!=0 || l!=0){
              // not to check with itself

              // Calculate neighbor's row and column
              Pos_y = i + k * Step;
              Pos_x = j + l * Step;

              // If the neighbor is outside the bounds of the buffer, skip it
              if( Pos_x < 0 || Pos_x >= Input_image->cols || Pos_y < 0 || Pos_y >= Input_image->rows ) continue;

              // Calculate neighbor's absolute index
              Pos1 =(Pos_y*Input_image->cols)+Pos_x;

              // Retrieve the neighbor
              Point_aux=(*Read)[static_cast<unsigned long>(Pos1)];

              // If the neighbor doesn't have a closest seed yet, skip it
              if(Point_aux.x==-1 || Point_aux.y==-1) continue;

              // Calculate the distance from us to the neighbor's closest seed
              Distance_aux=sqrtf((Point_aux.x-j)*(Point_aux.x-j) + (Point_aux.y-i)*(Point_aux.y-i));

              // If dist is -1, it means we have no closest seed, so we might as well take this one
              // Otherwise, only adopt this new seed if it's closer than our current closest seed
              if (Distance==-1 || Distance_aux<Distance) {
                Point_aux.Distance=Distance_aux;
                (*Write)[static_cast<unsigned long>(Pos)]=Point_aux;
                Distance=Distance_aux;
              }
            }
          }
        }

        // Update the Max_distance
//        if ((*Write)[i].Distance>Max_distance) Max_distance=(*Write)[i].Distance;

      }
    }


    // Halve the step.
    Step /= 2;

    // Swap the buffers for the next round
    Read_image1=!Read_image1;
  }

  for (unsigned int i=0;i<Input_image->total();i++){
    Output_image->at<float>(int(i))=(*Write)[i].Distance;
  }
}

//HEA

void _filter_distance_field_float::update_filter()
{
  jump_flooding(Aux_input_images[0],Output_image_0);
}

//HEA

void _filter_distance_field_float::update()
{
  if (Input_images[0]==nullptr){
    QMessageBox MsgBox;
    MsgBox.setText("Error: distance_field_float filter: no Input_images[0]");
    MsgBox.exec();
    qApp->quit();
    exit(-1);
  }

  for (int i=0;i<_f_filter_ns::MAX_CONNECTIONS;i++) Aux_input_images[i]=nullptr;

  if (Changed_input==true){
    Output_image_0->release();
    if (Num_channels_output_image_0==-1){ // input->output
      if (Input_images[0]->channels()==3){
        Output_image_0->create(Input_images[0]->rows,Input_images[0]->cols,CV_32FC3);
      }
      else{
        Output_image_0->create(Input_images[0]->rows,Input_images[0]->cols,CV_32FC1);
      }
    }
    else{ // adjust
      if (Num_channels_output_image_0==3){
        Output_image_0->create(Input_images[0]->rows,Input_images[0]->cols,CV_32FC3);
      }
      else{
        Output_image_0->create(Input_images[0]->rows,Input_images[0]->cols,CV_32FC1);
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

//void _filter_distance_field_float::read_parameters(std::map<std::string,std::string> &Parameters)
//{
//  Q_UNUSED(Parameters)
//}

//HEA

//void _filter_distance_field_float::write_parameters(std::map<std::string,std::string> &Parameters)
//{
//  Q_UNUSED(Parameters)
//}

//HEA

_filter_distance_field_float_ui::_filter_distance_field_float_ui(_gl_widget *GL_widget1,string Name1)
{
  Name=Name1;
  Qtw_filter_distance_field_float=new _qtw_filter_distance_field_float(GL_widget1,this,Name1);
}

//HEA

_filter_distance_field_float_ui::_filter_distance_field_float_ui(_gl_widget *GL_widget1, std::map<std::string,std::string> &Parameters, string Name1)
{
  read_parameters(Parameters);
  Name=Name1;
  Qtw_filter_distance_field_float=new _qtw_filter_distance_field_float(GL_widget1,this,Name1);
}

//HEA

_filter_distance_field_float_ui::~_filter_distance_field_float_ui()
{
  delete Qtw_filter_distance_field_float;
}

//HEA

void _filter_distance_field_float_ui::reset_data()
{
  _filter_distance_field_float::reset_data();
  hide();
}

//HEA

void _filter_distance_field_float_ui::show()
{
  Qtw_filter_distance_field_float->show();
}

//HEA

void _filter_distance_field_float_ui::hide()
{
  Qtw_filter_distance_field_float->hide();
}

//HEA

void *_filter_distance_field_float_ui::get_link()
{
  return Qtw_filter_distance_field_float->get_link();
}

//HEA

_qtw_filter_distance_field_float::_qtw_filter_distance_field_float(_gl_widget *GL_widget1,_filter_distance_field_float_ui *Filter1,string Box_name)
{
  QString Str;

  GL_widget=GL_widget1;
  Filter=Filter1;

  Group_box_main=new QGroupBox(tr(Box_name.c_str()));
  Group_box_main->hide();
}
