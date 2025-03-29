#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

void capture_images() {
    // Create images directory if it doesn't exist
    std::string imagesDir = "images";
    if (!fs::exists(imagesDir)) {
        fs::create_directory(imagesDir);
    }
    
    while (true) {
        // Generate a timestamped filename
        std::time_t now = std::time(nullptr);
        std::string filename = "images/capture_" + std::to_string(now) + ".jpg";
        
        // Capture image using libcamera-still
        std::string command = "libcamera-still -o " + filename;
        int result = std::system(command.c_str());
        
        if (result == 0) {
            std::cout << "Image saved to: " << filename << std::endl;
        } else {
            std::cerr << "Failed to capture image." << std::endl;
        }
        
        // Wait for 1 second before capturing the next image
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    std::thread captureThread(capture_images);
    captureThread.join();
    return 0;
}
