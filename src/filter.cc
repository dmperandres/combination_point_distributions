//LIC

#include "filter.h"

//HEA

void _filter::set_images(cv::Mat *Input_image0, cv::Mat *Output_image0, cv::Mat *Input_image1, cv::Mat *Input_image2)
{
  assert(Input_image0!=nullptr);
  assert(Output_image0!=nullptr);
  Input_images[0]=Input_image0;
  Input_images[1]=Input_image1;
  Input_images[2]=Input_image2;
  Output_image_0=Output_image0;
  Ready=true;
}

//HEA

void _filter::copy_input_to_output()
{
  // Check the number of input channels
  if (Input_images[0]->channels()!=Output_image_0->channels()){// Different number of input channels
    if (Input_images[0]->channels()==3){ // color to gray
      cvtColor(*Input_images[0],*Output_image_0,cv::COLOR_BGR2GRAY,1);
    }
    else{ // gray to color
      cvtColor(*Input_images[0],*Output_image_0,cv::COLOR_GRAY2RGB,3);
    }
  }
  else{// the same number of channels
    *Output_image_0=Input_images[0]->clone();
  }

  cv::line(*Output_image_0, cv::Point(0,0),cv::Point(Output_image_0->cols,Output_image_0->rows),0,5);
  cv::line(*Output_image_0, cv::Point(0,Output_image_0->rows),cv::Point(Output_image_0->cols,0),0,5);
}
