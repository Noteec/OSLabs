#include <queue>
#include <random>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

class Monitor {
private:
    std::mutex mtx;                    
    std::condition_variable cv;        
    std::queue<int> messageQueue;       

public:
    void sendMessage(int message) {
        std::unique_lock<std::mutex> lock(mtx);
        messageQueue.push(message);      
        std::cout << "Message sent: " << message << "\n";
        cv.notify_one();                
    }

    void receiveMessage() {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [this]() { return !messageQueue.empty(); }); 

        int message = messageQueue.front(); 
        messageQueue.pop();                

        std::cout << "Message received: " << message << "\n";
    }
};

int main() {
    Monitor monitor;

    std::thread producer([&monitor]() {
        for (int i = 1; i <= 5; ++i) {
            monitor.sendMessage(i); 
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
        }
    });

    std::thread consumer([&monitor]() {
        for (int i = 1; i <= 5; ++i) {
            monitor.receiveMessage();
        }
    });

    producer.join();
    consumer.join();

    std::cin.get();
    return 0;
}
