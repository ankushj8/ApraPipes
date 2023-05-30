#include <boost/test/unit_test.hpp>

#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/core/cuda.hpp>
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/core/cuda_stream_accessor.hpp"
#include <opencv2/cudawarping.hpp>
using namespace std;
using namespace cv;

BOOST_AUTO_TEST_SUITE(opencvresize_tests_)

tuple<float, float> square_(float real, float comp){
    float comp_ = 2*comp * real;
    float real_ = real * real - comp * comp;
    return make_tuple(real_, comp_);
}

tuple<float, float> add_(float no_1_real, float no_1_comp, float no_2_real, float no_2_comp){
    return make_tuple(no_1_real + no_2_real, no_1_comp + no_2_comp);
}

float distance(float real, float comp){
    return comp * comp + real * real;
}

void testqwerty(){
    VideoWriter writer;
    writer.open("./data/outcpp.mp4", cv::VideoWriter::fourcc('X', '2', '6', '4'), 2, Size(1000, 1000), false);
    if (!writer.isOpened()) {
        std::cout << "could not open" << std::endl;
    }

    tuple <float, float> iter;
    float dist;
    std::vector<cv::Mat> frames;
    for (int i = 0; i < 500; i++) {
        frames.push_back(cv::Mat(1000, 1000, CV_8UC1 ));
    }
    for (int j = 0; j < 1000; j++) {
        for (int k = 0; k < 1000; k++) {
            for (int i = 0; i < 500; i++) {
                frames[i].at<unsigned char>(j, k) = 255;
           }
        }
    }

    for (int j = 0; j < 1000; j++) {
        for (int k = 0; k < 1000; k++) {
            float real = 0;
            float comp = 0;
            for (int i = 0; i < 500; i++) {
                tie(real, comp) = square_(real, comp);
                tie(real, comp) = add_(real, comp, (float) (j-500) / 250, (float) (k-500) / 250);
                dist = distance(real, comp);
                if (dist > (float) 4) {
                    for (int l = i; l < 500; l++) {
                        frames[l].at<unsigned char>(j, k) = 0;
                    }
                    break;
                }
            }
        }
    }
    cv::imwrite("./data/mono_1920x9601.jpg", frames[499]);

    for (int l = 0; l < 500; l++) {
        writer.write(frames[l]);
    }
    writer.release();
}

BOOST_AUTO_TEST_CASE(qwerty){
    testqwerty();
}

BOOST_AUTO_TEST_SUITE_END()