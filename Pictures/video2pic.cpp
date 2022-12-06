#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

const int target_width = 80;
const int target_height = 45;

void compress_image(const Mat& input, const String& filename)
{
    // Check if the input image is empty
    if (input.empty())
    {
        std::cout << "Error: The input image is empty" << std::endl;
        return;
    }

    // Convert the input image to grayscale
    Mat gray;
    cvtColor(input, gray, COLOR_BGR2GRAY);

    // Resize the grayscale image to the target size
    Mat output;
    cv::resize(gray, output, Size(target_width, target_height));

    // Save the output image
    imwrite(filename, output);
}

int main()
{
    // Open the video file
    VideoCapture capture("video.mp4");

    // Check if the video was successfully opened
    if (!capture.isOpened())
    {
        std::cout << "Error: Could not open the video file" << std::endl;
        return -1;
    }

    // Extract and compress frames from the video
    Mat frame;
    int frame_index = 1;
    while (true)
    {
        // Read the next frame from the video
        capture >> frame;

        // Stop if we reached the end of the video
        if (frame.empty())
            break;

        // Construct the output filename
        std::stringstream ss;
        ss << frame_index << ".jpg";

        // Compress the frame and save it to a file
        compress_image(frame, ss.str());

        // Increment the frame index
        frame_index++;
    }

    return 0;
}

