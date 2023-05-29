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

tuple<float, float> multiply_(float no_1_comp, float no_1_real, float no_2_comp, float no_2_real){
    float comp = no_1_comp * no_2_real + no_1_real * no_2_comp;
    float real = no_1_real * no_2_real - no_1_comp * no_2_comp;
    return make_tuple(comp, real);
}

tuple<float, float> add_(float no_1_comp, float no_1_real, float no_2_comp, float no_2_real){
    return make_tuple(no_1_comp + no_2_comp, no_1_real + no_2_real);
}

float distance(float comp, float real){
    return comp * comp + real * real;
}

void testqwerty(){
    auto img = cv::imread("./data/mono_1920x960.jpg", 0);
    if(!img.data)
    {
        std::cout << "image read failed" << std::endl;
        return;
    }
    std::cout << "hola" << std::endl;
    std::cout << img.rows << "<>" << img.cols << std::endl;
    cv::imwrite("./data/mono_1920x9601.jpg", img);

    Mat frame(100, 100, CV_8UC1);
    for (int j = 0; j < 100; j++) {
        for (int k = 0; k < 100; k++) {
            frame.at<unsigned char>(j, k) = 255;
        }
    }
    cv::imwrite("./data/mono_1920x9604.jpg", frame);
    VideoWriter writer;
    
    writer.open("./data/outcpp.mp4", cv::VideoWriter::fourcc('X', '2', '6', '4'), 5, Size(100, 100), false);
    if (!writer.isOpened()) {
        std::cout << "could not open" << std::endl;
    }
    tuple <float, float> iter;
    vector<vector<vector<float> > > values(100, vector<vector<float> > (100, vector<float>(2)));
    for (int j = 0; j < 100; j++) {
        for (int k = 0; k < 100; k++) {
            values[j][k][0] = 0;
            values[j][k][1] = 0;
        }
    }
    
    
    for (int i = 0; i < 500; i++) {
        for (int j = 0; j < 100; j++) {
            for (int k = 0; k < 100; k++) {
                // cout << values[j][k][0] << " " << values[j][k][1]<< std::endl;
                iter = multiply_(values[j][k][0], values[j][k][1], values[j][k][0], values[j][k][1]);
                // cout << get<1>(iter) << " " << get<1>(iter)<< std::endl;
                iter = add_(get<0>(iter), get<1>(iter), float((j-50) / 25),float ((k-50) / 25));
                // cout << get<1>(iter) << " " << get<1>(iter)<< std::endl;
                float dist;
                dist = distance(get<0>(iter), get<1>(iter));
                // cout << dist<< std::endl;
                if (dist > 4.0) {
                    frame.at<unsigned char>(j, k) = 0;
                    std::cout << "boundary" << std::endl;
                }
                values[j][k][0] = (float) get<0>(iter);
                values[j][k][1] = (float) get<1>(iter);
            if (k == 0 && j ==0){
                cout << i << " " << j << " " <<  k << " " << values[j][k][0] << " " <<  values[j][k][1] << " " << dist << endl;
            }
            }
        }
        //Mat frame_o_cv=Mat(100,100, CV_8SC1,(int*)frame_cpp.data());
        
        std::cout << "frame" << i << "finished" << std::endl;
        writer.write(frame);
    }
    writer.release();

    std::cout << "hola" << std::endl;
}


// void testopencvcpuresize_(){
//     Mat frame(100, 100, CV_8SC1);  // Creating a red frame
//     vector<vector<vector<vector<float> > > > values(
//         150, vector<vector<vector<float> > >(100, vector<vector<float> >(100, vector<float>(2))));
//     for (int j = 0; j < 100; j++) {
//         for (int k = 0; k < 100; k++) {
//             //frame[j][k] = 1;
//             frame.at<Vec3b>(j, k) = 1;
//         }
//     }

//     tuple <float, float> result;
//     result = multiply_(8, 5, 2, 9);
//     cout << "Product: " << get<0>(result) << " " << get<1>(result) << endl;
//     result = add_(8, 5, 2, 9);
//     cout << "Sum: " << get<0>(result) << " " << get<1>(result) << endl;
//     float dist = distance(8, 5);
//     cout << "Distance: " << dist << endl;
//     tuple <float, float> iter;
//     std::cout << "writing to a file1" << std::endl;
//     VideoWriter writer;
//     std::cout << "writing to a file2" << std::endl;

//     // std::cout << getBuildInformation() << std::endl;
//     // std::cout << "writing to a file10" << std::endl;

    
//     auto img = cv::imread("C:\\Users\\developer\\Desktop\\ApraPipes\\_build\\Release\\sample.jpg", 0);

//     // myImage = imread("C:\\Users\\developer\\Desktop\\ApraPipes\\_build\\Release\\sample.jpg");
//     if (!img.data) {
//         std::cout << "image read failed" << std::endl;
//     }
//     else{
//         std::cout << "hola" << std::endl;
//         std::cout << img.rows << "<>" << img.cols << std::endl;
//         std::cout << "hola" << std::endl;
//     }
//     std::cout << "writing to a file11" << std::endl;

//     // imwrite("C:\\Users\\developer\\Desktop\\ApraPipes\\_build\\Release\\sample2.jpg.jpg", myImage);
//     // myImage.release();
//     std::cout << "writing to a file12" << std::endl;

//     writer.open("outcpp.mp4", VideoWriter::fourcc('M', 'J', 'P', 'G'), 150, Size(100, 100));
//     if (!writer.isOpened())
//     {
//         cout << "Could not open the output video for write: " << endl;
//     }
//     std::cout << "writing to a file3" << std::endl;
//     for (int i = 0; i < 150; i++) {
        
//         for (int j = 0; j < 100; j++) {
//             for (int k = 0; k < 100; k++) {
//                 iter = multiply_(values[j][k][0], values[j][k][1], values[j][k][0], values[j][k][1]);
//                 iter = add_(get<0>(iter), get<1>(iter), j / 50, k / 50);
//                 float dist;
//                 dist = distance(get<0>(iter), get<1>(iter));
//                 if (dist > 4) {
//                     //frame_cpp[j][k] = 0;
//                     frame.at<Vec3b>(j, k) = 0;
//                 }
//                 values[j][k][0] = get<0>(iter);
//                 values[j][k][1] = get<1>(iter);
//             }
//         }
//         //Mat frame_o_cv=Mat(100,100, CV_8SC1,(int*)frame_cpp.data());
        
//         std::cout << "writing to a file" << std::endl;
//         writer.write(frame);
//         // imwrite("test.frame", frame);
//     }
//     writer.release();
//     // cv::imwrite("./data/testOutput/testopencvcpuresize_.mp4", video);
    
//     //writer.write(video);
//     //cv::VideoWriter video("outcpp.mp4", cv::VideoWriter::fourcc('M','J','P','G'),150, cv::Size(100, 100));
// }


// BOOST_AUTO_TEST_CASE(cpu){
//     testopencvcpuresize_();
// }


BOOST_AUTO_TEST_CASE(qwerty){
    testqwerty();
}


BOOST_AUTO_TEST_CASE(fileWrite){
    std::cout << "writing to a file" << std::endl;
}


BOOST_AUTO_TEST_SUITE_END()