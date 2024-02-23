#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <random>
#include <chrono>

using namespace std;

constexpr int N = 8; // Number of guests
mutex mtx;
condition_variable cv;
bool cupcakes[N] = {false}; // Status of each guest
bool all_entered = false; // Flag to track if all guests have entered

void guest(int id) {
    unique_lock<mutex> lck(mtx);
    cv.wait(lck); // Wait until the Minotaur starts the game

    // Entering the labyrinth
    if (!cupcakes[id]) {
        cupcakes[id] = true;
        cout << "Guest " << id << " has entered the labyrinth.\n";
    }

    // Exiting the labyrinth
    if (!cupcakes[(id + 1) % N]) {
        cupcakes[(id + 1) % N] = true;
        cout << "Guest " << (id + 1) % N << " has signaled and eaten the cupcake.\n";
    }

    // Check if all guests have entered
    bool all_signaled = true;
    for (int i = 0; i < N; ++i) {
        if (!cupcakes[i] && i != id) {
            all_signaled = false;
            break;
        }
    }
    if (all_signaled && !all_entered) {
        all_entered = true;
        cout << "All guests have entered the labyrinth.\n";
    }
}

int main() {
    vector<int> guest_order(N);
    for (int i = 0; i < N; ++i) {
        guest_order[i] = i;
    }
    shuffle(guest_order.begin(), guest_order.end(), default_random_engine(chrono::system_clock::now().time_since_epoch().count()));

    thread guests[N];
    
    for (int i = 0; i < N; ++i) {
        guests[i] = thread(guest, guest_order[i]);
    }
    this_thread::sleep_for(chrono::seconds(1)); // Wait for guests to initialize
    cv.notify_all(); // Start the game
    // Join all guest threads
    for (int i = 0; i < N; ++i) {
        guests[i].join();
    }
    return 0;
}
