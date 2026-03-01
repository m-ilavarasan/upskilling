#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

class Account {
public:
    Account(int balance) : balance(balance) {}

    void deposit(int amount) {
        balance += amount;
    }

    void withdraw(int amount) {
        balance -= amount;
    }

    int getBalance() const {
        return balance;
    }

    std::mutex mtx;

private:
    int balance;
};

void transfer(Account& from, Account& to, int amount) {
    std::cout << "Thread " << std::this_thread::get_id()
              << " trying to lock FROM\n";

  //  std::lock_guard<std::mutex> lock1(from.mtx);
    std::scoped_lock<std::mutex, std::mutex> lock(from.mtx, to.mtx);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "Thread " << std::this_thread::get_id()
              << " trying to lock TO\n";

 //   std::lock_guard<std::mutex> lock2(to.mtx);

    from.withdraw(amount);
    to.deposit(amount);

    std::cout << "Transfer done by "
              << std::this_thread::get_id() << "\n";
}

int main() {
    Account acc1(1000);
    Account acc2(1000);

    std::thread t1(transfer, std::ref(acc1), std::ref(acc2), 100);
    std::thread t2(transfer, std::ref(acc2), std::ref(acc1), 50);

    t1.join();
    t2.join();

    std::cout << "Final Balance acc1: " << acc1.getBalance() << "\n";
    std::cout << "Final Balance acc2: " << acc2.getBalance() << "\n";
}
