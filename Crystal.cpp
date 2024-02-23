#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

constexpr int N = 8; // Number of guests
mutex mtx;
bool showroom_available = true; // Flag to indicate showroom availability

void guest(int id) {
    while (true) {
        unique_lock<mutex> lck(mtx);
        if (showroom_available) {
            showroom_available = false; // Mark showroom as busy
            lck.unlock(); // Release the lock before entering the critical section
            
            cout << "Guest " << id << " is entering the showroom.\n";
            
            // Simulate guest viewing the showroom for a random duration
            this_thread::sleep_for(chrono::seconds(1 + rand() % 5));
            
            lck.lock(); // Re-acquire the lock to modify the shared variable
            showroom_available = true; // Mark showroom as available
            
            cout << "Guest " << id << " has exited the showroom.\n";
            break; // Exit the loop after exiting the showroom
        }
    }
}

int main() {
    srand(time(nullptr));
    thread guests[N];
    
    for (int i = 0; i < N; ++i) {
        guests[i] = thread(guest, i);
    }

    for (int i = 0; i < N; ++i) {
        guests[i].join();
    }
    return 0;
}
