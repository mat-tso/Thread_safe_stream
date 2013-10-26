# The problem

The c++ Standard Template Library offers numerous great tools including streams.
All basic objects can be inserted in an stream using the << operator
or extracted with >>.

Here is an example of a program that prints "hello world":

    cout << "Hello " << "world "

The problem is that if working in an multi thread environement,
2 thread could output in parallel. The following could happend:
`"Hello OUTPUT_FROM_AN_OTHER_THEAD world"` instead of
`"Hello world OUTPUT_FROM_AN_OTHER_THEAD"`.

# The solution

To prevent this the safeStream class implements a thread safe ostream.
Use it as such:

    SafeStream scout(cout);
    // Then use scout to access to cout.
    ...
    ... span threads
    ...
    // Open a session for your thread
    scout << scout.begin;
    // You are now garanty that any other thread will now wait for your session end
    scout << "Hello " << "world ";
    // Do not forget to close the session.
    scout << scout.end;

# A log class

Deriving the SafeStream class, the Log class offers a thread safe,
stream orriented, way of logging. Create a logger by giving a stream and
a line prefix to the Log constructor. Then define a macro as such:

    Log logger(cerr, "Log");
    #define LOG(...) logger, logger.begin , ##__VA_ARGS__ , logger.end

As the Log::operator, is just just forwarding to the SafeStream::operator<<,
you just defined a pseudo type and number parameter varatic function!
See the @ref LOGI macro in log.hpp for an example.

Note: Of course with c++11, at it's varatic templates (challenge breaker),
defining such log(...) is easy peasy (tail recursion feels so good ;).

    template<class... T> log(T...);
    template<class H, class... T> log(H head, T... tail) {
        mutex.lockIfNotAlready();
        stream << head;
        log(tail...);
    }
    template<> log<>() {mutex.unlock();}

# How to compile

Use the cmake build system.

    # Create a build folder to avoid messing with the src files.
    mkdir build && cd build
    # Generate makefiles and build your executables.
    cmake .. && make
    # Check that all tests are passed.
    make test

# TODO list
- Add a RAII class to encapsulate the need for begin and end (exception robusness).
This would make the safe stream even safer at the cost of an object creation for each lock.
- Allow stream manipulator (endl,...) to be used direcly.
