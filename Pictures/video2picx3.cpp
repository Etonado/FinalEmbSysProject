#include <iostream>
#include <string>
#include <filesystem>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

const int target_width = 32;
const int target_height = 18;

void save_image(const Mat &input, const String &filename)
{
    // Check if the input image is empty
    if (input.empty())
    {
        std::cout << "Error: The input image is empty" << std::endl;
        return;
    }

    // Save the input image to a file
    imwrite(filename, input);
}

int main()
{
    // Open the video file
    VideoCapture capture("/home/benjamin/Documents/Kandidat/embedded/Final_ass/FinalEmbSysProject/Pictures/embedded_dataset/straight_arm.mp4");

    // Check if the video was successfully opened
    if (!capture.isOpened())
    {
        std::cout << "Error: Could not open the video file" << std::endl;
        return -1;
    }

    // Extract and compress frames from the video
    Mat frame, gray, mirrored, blurred, mir_blur;
    int frame_index = 1;

    while (true)
    {
        // Read the next frame from the video
        capture >> frame;

        // Stop if we reached the end of the video
        if (frame.empty())
            break;

        // Convert the frame to grayscale
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        // Resize the grayscale image to the target size
        Mat resized;
        cv::resize(gray, resized, Size(target_width, target_height));

        // Flip the resized image horizontally (i.e., create a mirror image)
        flip(resized, mirrored, 1);

        // Blur the resized image using a Gaussian filter
        GaussianBlur(resized, blurred, Size(5, 5), 0);

        GaussianBlur(mirrored, mir_blur, Size(5, 5), 0);
        // Save the original, mirrored, and blurred versions of the image

        save_image(resized, "./pics/straight_arm/" + std::to_string(frame_index) + ".jpg");
        frame_index++;

        save_image(mirrored, "./pics/straight_arm/" + std::to_string(frame_index) + ".jpg");
        frame_index++;

        save_image(blurred, "./pics/straight_arm/" + std::to_string(frame_index) + ".jpg");
        frame_index++;

        save_image(mir_blur, "./pics/straight_arm/" + std::to_string(frame_index) + ".jpg");
        frame_index++;
        /*
                save_image(resized, dir+ss.str()+typ);
                frame_index++;
                ss << frame_index;
                save_image(mirrored, dir+ss.str()+typ);
                frame_index++;
                ss << frame_index;
                save_image(blurred, dir+ss.str()+typ);
                frame_index++;
                ss << frame_index;
                save_image(mir_blur, dir+ss.str()+typ);
        */

        // Increment the frame index
    }

    return 0;
}
