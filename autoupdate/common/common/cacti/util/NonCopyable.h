#ifndef INCLUDED_CACTI_UTIL_NONCOPYABLE_H
#define INCLUDED_CACTI_UTIL_NONCOPYABLE_H

#if defined(HAS_PRAGMA_ONCE)
#pragma PRAGMA_ONCE_DECLARE
#endif

// Private copy constructor and copy assignment ensure classes derived from
// class NonCopyable cannot be copied.
class NonCopyable
{
protected:
	NonCopyable() {}
	~NonCopyable() {}
private:  // emphasize the following members are private
	NonCopyable( const NonCopyable& );
	NonCopyable& operator=( const NonCopyable&);
};

#endif // INCLUDED_NONCOPYABLE_H
