//LIC

#include "image_IO.h"

using namespace std;

//HEA

void _image_IO::read_image(string Name,cv::Mat &Image_in)
{
  #ifdef WINDOWS
  string Aux_string;
  for (int i=0;i<Name.size();i++){
    if (Name[i]=='/'){
      Aux_string.push_back('\\');
    }
    else Aux_string.push_back(Name[i]);
  }
  Name=Aux_string;
  #endif

  Image_in=cv::imread(Name,cv::IMREAD_UNCHANGED);
  if( !Image_in.data ){ // check if the image has been loaded properly
    cout << "Error reading the image: " << Name << endl;
    exit(-1);
  }

  if (Image_in.channels()==4){   
     cv::cvtColor(Image_in,Image_in,cv::COLOR_BGRA2RGB);
  }

  if (Image_in.depth()!=CV_8U){
    cout << "Error in the depth of the image" << endl;
    exit(-1);
  }
}

//HEA

void _image_IO::write_image(string Name,cv::Mat &Image_out)
{
	imwrite(Name, Image_out);
}
