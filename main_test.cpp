/****************************************************************************
** Upsample a video frame rate by interpolating in between frames
****************************************************************************/
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "interpolate.h"
#include "vid.h"
#include "inputparser.h"
#include <QDebug>

#define NUM_FRAMES ( 1 )
#define START_FRAME ( 15 )
#define END_FRAME ( 80 )

int main(int argc, char **argv)
{
   InputParser input( argc, argv );
   cv::Mat from;
   cv::Mat to;
   cv::Mat interp_frame;
   int start_frame = START_FRAME;
   int end_frame = END_FRAME;
   int inter_frames = NUM_FRAMES;
   const std::string &input_video = input.getCmdOption( "-v" );
   const std::string &start_frame_arg = input.getCmdOption( "-s" );
   const std::string &end_frame_arg = input.getCmdOption( "-e" );
   const std::string &inter_frames_arg = input.getCmdOption( "-i" );
   if ( input_video.empty( ) )
   {
      qDebug( ) << "Please include input video";
      return 1;
   }
   qDebug( ) << "input video" << input_video.data( );
   if ( !start_frame_arg.empty( ) )
   {
      start_frame = std::stoi( start_frame_arg, nullptr );
      qDebug( ) << "start" << start_frame;
   }
   if ( !end_frame_arg.empty( ) )
   {
      end_frame = std::stoi( end_frame_arg, nullptr );
      qDebug( ) << "end" << end_frame;
   }
   if ( !inter_frames_arg.empty( ) )
   {
      inter_frames = std::stoi( inter_frames_arg, nullptr );
      qDebug( ) << "inter_frames" << inter_frames;
   }

   Vid video_obj = Vid( QString::fromStdString( input_video ) );
   video_obj.setCurrentFrameIndex( start_frame );

   cv::VideoWriter out_vid;
   out_vid.open( "C:/cygwin/home/501219/OCV/out.avi", CV_FOURCC('M','J','P','G'), video_obj.getFPS( ) * ( inter_frames + 1 ), video_obj.getCSize( ), true );
   char output_name[ 50 ];

   for ( int i = start_frame; i < end_frame; i++ )
   {
      if ( video_obj.getCurrentFrame( from ) && video_obj.getNextFrame( to ) )
      {
         sprintf( output_name, "C:/cygwin/home/501219/OCV/images/frame%d.jpg", i );
         imwrite( output_name, from );
         out_vid << from;
         Interpolater interp( from, to, inter_frames );
         for ( int j=0; j < inter_frames; j++ )
         {
            qDebug( ) << "frame " << i << "." << j;
            interp.getNextFrame( interp_frame );
            cv::cvtColor( interp_frame, interp_frame, CV_RGB2BGR );
            out_vid << interp_frame;
            sprintf( output_name, "C:/cygwin/home/501219/OCV/images/frame%d.%d.jpg", i, j );
            imwrite( output_name, interp_frame );
         }
      }
   }
   out_vid << to;

   return 0;
}

#if 0
#include "opencv2/optflow.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"


#include <cstdio>
#include <iostream>

using namespace cv;
using namespace cv::optflow;
using namespace std;

#define APP_NAME "simpleflow_demo : "

static void help()
{
  // print a welcome message, and the OpenCV version
  printf("This is a demo of SimpleFlow optical flow algorithm,\n"
         "Using OpenCV version %s\n\n", CV_VERSION);

  printf("Usage: simpleflow_demo frame1 frame2 output_flow"
         "\nApplication will write estimated flow "
         "\nbetween 'frame1' and 'frame2' in binary format"
         "\ninto file 'output_flow'"
         "\nThen one can use code from http://vision.middlebury.edu/flow/data/"
         "\nto convert flow in binary file to image\n");
}

// binary file format for flow data specified here:
// http://vision.middlebury.edu/flow/data/
static void writeOpticalFlowToFile(const Mat& flow, FILE* file) {
  int cols = flow.cols;
  int rows = flow.rows;

  fprintf(file, "PIEH");

  if (fwrite(&cols, sizeof(int), 1, file) != 1 ||
      fwrite(&rows, sizeof(int), 1, file) != 1) {
    printf(APP_NAME "writeOpticalFlowToFile : problem writing header\n");
    exit(1);
  }

  for (int i= 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      Vec2f flow_at_point = flow.at<Vec2f>(i, j);

      if (fwrite(&(flow_at_point[0]), sizeof(float), 1, file) != 1 ||
          fwrite(&(flow_at_point[1]), sizeof(float), 1, file) != 1) {
        printf(APP_NAME "writeOpticalFlowToFile : problem writing data\n");
        exit(1);
      }
    }
  }
}

static void run(int argc, char** argv) {
  if (argc < 3) {
    printf(APP_NAME "Wrong number of command line arguments for mode `run`: %d (expected %d)\n",
           argc, 3);
    exit(1);
  }

  Mat frame1 = imread(argv[0],1);
  Mat frame2 = imread(argv[1],1);

  if (frame1.empty()) {
    printf(APP_NAME "Image #1 : %s cannot be read\n", argv[0]);
    exit(1);
  }

  if (frame2.empty()) {
    printf(APP_NAME "Image #2 : %s cannot be read\n", argv[1]);
    exit(1);
  }

  if (frame1.rows != frame2.rows && frame1.cols != frame2.cols) {
    printf(APP_NAME "Images should be of equal sizes\n");
    exit(1);
  }

  if (frame1.type() != 16 || frame2.type() != 16) {
    printf(APP_NAME "Images should be of equal type CV_8UC3\n");
    exit(1);
  }

  printf(APP_NAME "Read two images of size [rows = %d, cols = %d]\n",
         frame1.rows, frame1.cols);

  Mat flow;

  float start = (float)getTickCount();
  calcOpticalFlowSF(frame1, frame2,
                    flow,
                    3, 2, 4, 4.1, 25.5, 18, 55.0, 25.5, 0.35, 18, 55.0, 25.5, 10);
  printf(APP_NAME "calcOpticalFlowSF : %lf sec\n", (getTickCount() - start) / getTickFrequency());

  FILE* file = fopen(argv[2], "wb");
  if (file == NULL) {
    printf(APP_NAME "Unable to open file '%s' for writing\n", argv[2]);
    exit(1);
  }
  printf(APP_NAME "Writing to file\n");
  writeOpticalFlowToFile(flow, file);
  fclose(file);
}

static bool readOpticalFlowFromFile(FILE* file, Mat& flow) {
  char header[5];
  if (fread(header, 1, 4, file) < 4 && (string)header != "PIEH") {
    return false;
  }

  int cols, rows;
  if (fread(&cols, sizeof(int), 1, file) != 1||
      fread(&rows, sizeof(int), 1, file) != 1) {
    return false;
  }

  flow = Mat::zeros(rows, cols, CV_32FC2);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      Vec2f flow_at_point;
      if (fread(&(flow_at_point[0]), sizeof(float), 1, file) != 1 ||
          fread(&(flow_at_point[1]), sizeof(float), 1, file) != 1) {
        return false;
      }
      flow.at<Vec2f>(i, j) = flow_at_point;
    }
  }

  return true;
}

static bool isFlowCorrect(float u) {
  return !cvIsNaN(u) && (fabs(u) < 1e9);
}

static float calc_rmse(Mat flow1, Mat flow2) {
  float sum = 0;
  int counter = 0;
  const int rows = flow1.rows;
  const int cols = flow1.cols;

  for (int y = 0; y < rows; ++y) {
    for (int x = 0; x < cols; ++x) {
      Vec2f flow1_at_point = flow1.at<Vec2f>(y, x);
      Vec2f flow2_at_point = flow2.at<Vec2f>(y, x);

      float u1 = flow1_at_point[0];
      float v1 = flow1_at_point[1];
      float u2 = flow2_at_point[0];
      float v2 = flow2_at_point[1];

      if (isFlowCorrect(u1) && isFlowCorrect(u2) && isFlowCorrect(v1) && isFlowCorrect(v2)) {
        sum += (u1-u2)*(u1-u2) + (v1-v2)*(v1-v2);
        counter++;
      }
    }
  }
  return (float)sqrt(sum / (1e-9 + counter));
}

static void eval(int argc, char** argv) {
  if (argc < 2) {
    printf(APP_NAME "Wrong number of command line arguments for mode `eval` : %d (expected %d)\n",
           argc, 2);
    exit(1);
  }

  Mat flow1, flow2;

  FILE* flow_file_1 = fopen(argv[0], "rb");
  if (flow_file_1 == NULL) {
    printf(APP_NAME "Cannot open file with first flow : %s\n", argv[0]);
    exit(1);
  }
  if (!readOpticalFlowFromFile(flow_file_1, flow1)) {
    printf(APP_NAME "Cannot read flow data from file %s\n", argv[0]);
    exit(1);
  }
  fclose(flow_file_1);

  FILE* flow_file_2 = fopen(argv[1], "rb");
  if (flow_file_2 == NULL) {
    printf(APP_NAME "Cannot open file with first flow : %s\n", argv[1]);
    exit(1);
  }
  if (!readOpticalFlowFromFile(flow_file_2, flow2)) {
    printf(APP_NAME "Cannot read flow data from file %s\n", argv[1]);
    exit(1);
  }
  fclose(flow_file_2);

  float rmse = calc_rmse(flow1, flow2);
  printf("%lf\n", rmse);
}

int main(int argc, char** argv) {
  if (argc < 2) {
    printf(APP_NAME "Mode is not specified\n");
    help();
    exit(1);
  }
  string mode = (string)argv[1];
  int new_argc = argc - 2;
  char** new_argv = &argv[2];

  if ("run" == mode) {
    run(new_argc, new_argv);
  } else if ("eval" == mode) {
    eval(new_argc, new_argv);
  } else if ("help" == mode)
    help();
  else {
    printf(APP_NAME "Unknown mode : %s\n", argv[1]);
    help();
  }

  return 0;
}
#endif
