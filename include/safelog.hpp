#pragma once

#include "safestream.hpp"

/** Thread safe log class.
  *
  * The insertion operator is "," rather than <<.
  * A safe stream that adds "{qualifier}: " at each session start,
  * and append a new line on session stop.
  *
  * This class is pretty much useless in c++11 as implementing a log(...)
  * function is straight forward using varatic template.
  */
class Log : public SafeStream
{
public:
    /** Log class constructor.
      *
      * @param[in] logStream is the stream to protect from concurent access.
      * @param[in] qualifier + ": " will prefix each log lines.
      */
    Log(std::ostream & logStream, std::string qualifier) :
        SafeStream(logStream), _qualifier(qualifier) {}

    /** A wrapper to SafeStream::operator<<.
      *
      * Used in variadic macro to give the ilusion of a varadic function.
      */
    template<class T>
    Log & operator, (const T ti) {
        operator<< (ti);
        return *this;
    }

    /** Append the log qualifier on session start. */
    virtual void openSession() {
        *this << _qualifier << ": ";
    }
    /** Append a new line and flush on session end. */
    virtual void closeSession() {
        typedef std::ostream& ostreamManipulator(std::ostream&);
        SafeStream::operator<< <ostreamManipulator> (std::endl);
    }
private:
    /** The log qualifier, prefix each log line. */
    const std::string _qualifier;
};

