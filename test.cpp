#include "log.cpp"
#include <thread>
#include <string>

/** Log like a crazy man to test concurrent access. */
void crazyLogger(const std::string &id)
{
    for(int i=0; i < 100; i++) {
        LOGI(id," ", i, " A normal log line mixing types ", 2 ," two ", &i);
        // Simulate a concurrent access.
        logInfo, logInfo.begin, id," ", i, " An other log line";
        // We sleep here to be sure that the other thread is trying to log.
        std::this_thread::sleep_for(
            std::chrono::milliseconds(1)
        );
        logInfo, " but splited this time", logInfo.end;
        // We sleep here to be sure that the other thread is waken up as we
		// unlocked the logInfo.
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

/** Test purpose main. */
int main ()
{
    using namespace std;

    LOGI(3, " hi hi");
    int i;
    LOGI(&i , " no seg fault please.");
    logInfo, logInfo.begin,  "line in ";
    logInfo, 3;
    logInfo, " instuctions", logInfo.end;

    std::thread t1(crazyLogger, "Me ");
    std::thread t2(crazyLogger, "You");
    t1.join();
    t2.join();
}
