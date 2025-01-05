#include <queue>
#include <random>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

class Monitor {
private:
    std::mutex mtx;                      // Мьютекс для синхронизации
    std::condition_variable cv;          // Условная переменная
    std::queue<int> messageQueue;        // Очередь для хранения сообщений

public:
    void sendMessage(int message) {
        std::unique_lock<std::mutex> lock(mtx);
        messageQueue.push(message);      // Добавляем сообщение в очередь
        std::cout << "Message sent: " << message << "\n";
        cv.notify_one();                 // Уведомляем поток-потребитель
    }

    void receiveMessage() {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [this]() { return !messageQueue.empty(); }); // Ожидание, пока очередь не станет непустой

        int message = messageQueue.front(); // Извлекаем сообщение из очереди
        messageQueue.pop();                 // Удаляем сообщение из очереди

        std::cout << "Message received: " << message << "\n";
    }
};

int main() {
    Monitor monitor;

    std::thread producer([&monitor]() {
        for (int i = 1; i <= 5; ++i) {
            monitor.sendMessage(i); // Отправляем сообщение
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Имитация задержки
        }
    });

    std::thread consumer([&monitor]() {
        for (int i = 1; i <= 5; ++i) {
            monitor.receiveMessage(); // Получаем сообщение
        }
    });

    producer.join();
    consumer.join();

    std::cin.get();
    return 0;
}
