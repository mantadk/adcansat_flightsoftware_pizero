#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <fstream>

namespace fs = std::filesystem;

void capture_images() {
    // Create images directory if it doesn't exist
    std::string imagesDir = "images";
    if (!fs::exists(imagesDir)) {
        fs::create_directory(imagesDir);
    }

    // Start libcamera-still in background mode
    std::string startCommand = "libcamera-still --timeout 0 --autostart --keypress &";
    std::system(startCommand.c_str());
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Ensure it starts properly
    
    while (true) {
        // Generate a timestamped filename
        std::time_t now = std::time(nullptr);
        std::string filename = "images/capture_" + std::to_string(now) + ".jpg";
        
        // Send a capture command to the running libcamera-still process
        std::ofstream keypressFile("/dev/input/event0");
        if (keypressFile) {
            keypressFile << " ";
            keypressFile.close();
            std::cout << "Image saved to: " << filename << std::endl;
        } else {
            std::cerr << "Failed to trigger image capture." << std::endl;
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
