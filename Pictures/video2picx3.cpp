#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

const int target_width = 80;
const int target_height = 45;

void save_image(const Mat& input, const String& filename)
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
    VideoCapture capture("video.mp4");

    // Check if the video was successfully opened
    if (!capture.isOpened())
    {
        std::cout << "Error: Could not open the video file" << std::endl;
        return -1;
    }

    // Extract and compress frames from the video
    Mat frame, gray, mirrored, blurred;
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

        // Save the original, mirrored, and blurred versions of the image
        std::stringstream ss;
        ss << frame_index;

        save_image(resized, ss.str() + "_original.jpg");
        save_image(mirrored, ss.str() + "_mirrored.jpg");
        save_image(blurred, ss.str() + "_blurred.jpg");

        // Increment the frame index
        frame_index++;
    }

    return 0;
}

