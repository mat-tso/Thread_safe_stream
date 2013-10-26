#pragma once

#include <cassert>
#include <mutex>
#include <iostream>

/** Thread safe output stream.
  *
  * Insert SafeStream::begin to start a session => lock the stream
  * Insert SafeStream::end to stop a session => unlock the stream.
  * Insert anything else to insert in the protected stream (requires a session).
  *
  * @todo It would be better if the open/close session was part of a trait
  * class instead of virtual but this would imply partial specialization for
  * the insertion operator, which is a pain in the ass in c++O3.
  */
class SafeStream
{
public:
    /** @param[in] stream is the stream to protect from concurrent access. */
    SafeStream(std::ostream & stream) : lock_taken(false), _output(stream) {}
    virtual ~SafeStream() {};

    /** Manipulator: type used to begin a session. */
    struct Begin {};
    /** A static instance of Begin to avoid instantiating one for each session start */
    static const Begin begin;

    /** Manipulator: type used to end a session. */
    struct End {};
    /** A static instance of End to avoid instantiating one for each session stop. */
    static const End end;

    /** The insertion operator.
      *
      * @tparam T If Begin or End, start or end a session.
      *           If any other type, append to the stream using operator <<.
      * @param[in] ti The object to be output or a manipulator.
      */
    template<class T>
    SafeStream & operator<< (const T & ti);

private:
    /** Open session */
    virtual void openSession() {}
    /** Close session */
    virtual void closeSession() {}

    /** Start a session. */
    void start() {
        lock();
        openSession();
    }
    /** End the current session. */
    void stop() {
        closeSession();
        unlock();
    }
    /** Lock the stream */
    void lock() {
        _mutex.lock();
        lock_taken = true;
    }
    /** Unlock the stream */
    void unlock() {
        assert(lock_taken);
        lock_taken = false;
        _mutex.unlock();
    }

    /** The mutex state, for debug purpose only. */
    bool lock_taken;

    /** The stream mutex*/
    std::mutex _mutex;
    /** The concurrency access protected stream. */
    std::ostream & _output;
};

const SafeStream::Begin SafeStream::begin;
const SafeStream::End SafeStream::end;

/** Add a token to stream. */
template<class T>
SafeStream & SafeStream::operator<< (const T & ti) {
    assert(lock_taken);
    _output << ti;
    return *this;
}

/** Start a session.
  *
  * Will block if is a session is already started.
  */
template<>
SafeStream & SafeStream::operator<< <SafeStream::Begin> (const SafeStream::Begin &) {
    start();
    return *this;
}

/** Stop the current session. */
template<>
SafeStream & SafeStream::operator<< <SafeStream::End> (const SafeStream::End &) {
    stop();
    return *this;
}
