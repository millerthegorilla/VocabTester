#if !defined (SYNCHRO_H)
#define SYNCHRO_H
//------------------------------
// (c) Reliable Software 2000-03
//------------------------------
#include <Win/SysHandle.h>

namespace Win
{

	class CritSection
	{
	public:
		CritSection () 
			: _count (0)
		{
			::InitializeCriticalSection (&_critSection);
		}
		~CritSection ()
		{
			::DeleteCriticalSection (&_critSection);
		}

		unsigned Acquire () 
		{ 
			::EnterCriticalSection (&_critSection);
			return ++_count;
		}
		void Release () 
		{ 
			Assert (_count != 0);
			--_count;
			::LeaveCriticalSection (&_critSection);
		}

		CRITICAL_SECTION _critSection;
		unsigned			_count;
	};

	class Lock 
	{
	public:
		Lock (CritSection & critSect) 
			: _critSect (critSect) 
		{
			_critSect.Acquire ();
		}
		~Lock ()
		{
			_critSect.Release ();
		}
	private:
		CritSection & _critSect;
	};
	
	class Unlock
	{
	public:
		// release section only if it's been already entered
		Unlock (CritSection & critSect)
			: _critSect (critSect)
		{
			_count = _critSect.Acquire ();
			if (_count > 1)
				_critSect.Release ();
			_critSect.Release ();
		}
		~Unlock ()
		{
			if (_count > 1)
				_critSect.Acquire ();
		}
	private:
		CritSection & _critSect;
		unsigned _count;
	};

	class LockPtr
	{
	public:
		LockPtr (CritSection * critSect) 
			: _critSect (critSect) 
		{
			if (_critSect)
				_critSect->Acquire ();
		}
		~LockPtr ()
		{
			if (_critSect)
				_critSect->Release ();
		}
	private:
		CritSection * _critSect;
	};
	
	class UnlockPtr
	{
	public:
		// release section only if it's been already entered
		UnlockPtr (CritSection * critSect)
			: _critSect (critSect)
		{
			if (_critSect)
			{
				_count = _critSect->Acquire ();
				if (_count > 1)
					_critSect->Release ();
				_critSect->Release ();
			}
		}
		~UnlockPtr ()
		{
			if (_critSect != 0 && _count > 1)
				_critSect->Acquire ();
		}
	private:
		CritSection	*	_critSect;
		unsigned		_count;
	};

	class Mutex: public Sys::AutoHandle
	{
	public:
		Mutex (char const * name = 0, bool ownInitially = false)
			: Sys::AutoHandle (::CreateMutex (0, ownInitially? TRUE: FALSE, name))
		{
			Win::ClearError ();
		}
		void Release ()
		{
			ReleaseMutex (H ());
		}
	};

	class MutexLock
	{
	public:
		MutexLock (Mutex & mutex, unsigned timeout = INFINITE)
			: _mutex (mutex)
		{
			_status = ::WaitForSingleObject (_mutex.ToNative (), timeout);
		}
		~MutexLock ()
		{
			_mutex.Release ();
		}
		bool WasAbandoned () const { return _status == WAIT_ABANDONED; }
		bool WasTimeout () const { return _status == WAIT_TIMEOUT; }
	private:
		Mutex & _mutex;
		DWORD	_status;
	};

	class Event: public Sys::AutoHandle
	{
	public:
		// start in non-signaled state (red light)
		// auto reset after every Wait
		Event ()
			: Sys::AutoHandle (::CreateEvent (0, false, false, 0))
		{
			if (IsNull ())
				throw Win::Exception ("Cannot create semaphore");
		}
		// put into signaled state: release waiting thread
		void Release ()
		{
			::SetEvent (H ());
		}
		// return true if signaled, false if timeout
		bool Wait (int timeout = INFINITE)
		{
			// Wait until event is in signaled (green) state
			return ::WaitForSingleObject(H (), timeout) == WAIT_OBJECT_0;
		}

	protected:
		Event (HANDLE h)
			: Sys::AutoHandle (h) 
		{}
	};

	// These can be shared between processes
	class NewEvent: public Event
	{
	public:
		NewEvent (std::string const & name)
			: Event (::CreateEvent (0, false, false, name.c_str ())),
			  _name (name)
		{
			if (IsNull ())
				throw Win::Exception ("Cannot create event", name.c_str ());
		}
		// Generate unique name
		NewEvent ();
		std::string const & GetName () const { return _name; }
	private:
		std::string _name;
	};

	class ExistingEvent: public Event
	{
	public:
		ExistingEvent (std::string const & name)
			: Event (::OpenEvent (EVENT_ALL_ACCESS | EVENT_MODIFY_STATE, 
								  FALSE, 
								  name.c_str ()))
		{
			if (IsNull ())
				throw Win::Exception ("Cannot open event", name.c_str ());
		}
	};

	class Semaphore: public Sys::AutoHandle
	{
	public:
		Semaphore (long maxCount, long initCount = 0)
			: Sys::AutoHandle (::CreateSemaphore (0, initCount, maxCount, 0))
		{
			if (IsNull ())
				throw Win::Exception ("Cannot create semaphore");
		}
		void Increment (int count = 1)
		{
			// increment semaphore count (and release waiting thread)
			if (::ReleaseSemaphore (H (), count, 0) == FALSE)
				throw Win::Exception ("Releasing semaphore failed");
		}
		void Wait ()
		{
			// Wait until count greated than zero and decrement count
			::WaitForSingleObject(H (), INFINITE);
		}
	};

	class TrafficLight : public Sys::AutoHandle
	{
	public:
		// Start in non-signaled state (red light)
		// Manual reset
		TrafficLight (char const *name = 0, bool existingLight = false)
			: Sys::AutoHandle(existingLight ? ::OpenEvent(SYNCHRONIZE | EVENT_MODIFY_STATE, false, name) :
										  ::CreateEvent (0, true, false, name))
		{
			if (IsNull ())
				throw Win::Exception ("Cannot create event");
		}

		// put into signaled state
		void GreenLight () 
		{ 
			::SetEvent (H ()); 
		}

		// put into non-signaled state
		void RedLight () 
		{ 
			::ResetEvent (H ()); 
		}

		void Wait (int timeout = INFINITE)
		{
			// Wait until event is in signaled (green) state
			::WaitForSingleObject(H (), timeout);
		}

	};

#if 0	//	CreateWaitableTimer not available on Win95
	class WaitableTimer : public Sys::AutoHandle
	{
	public:
		WaitableTimer ();
		WaitableTimer (std::string const & name);

		void Set (unsigned int timeout);	// Timeout in miliseconds
		bool Wait ();	// Returns true if timer signaled otherwise wait failed

		std::string const & GetName () const { return _name; }

	private:
		std::string	_name;
	};
#endif

	class AtomicCounter
	{
	public:
		AtomicCounter () 
			: _counter(0) 
		{}

		long Inc ()
		{
			// Return the sign (or zero) of the New value
			return ::InterlockedIncrement (&_counter);
		}

		long Dec ()
		{
			// Return the sign (or zero) of the New value
			return ::InterlockedDecrement (&_counter);
		}

		bool IsNonZero () const
		{
			return _counter != 0;
		}

		bool IsNonZeroReset ()
		{
			return ::InterlockedExchange (&_counter, 0) != 0;
		}

		bool IsNonZeroSet (long newVal)
		{
			return ::InterlockedExchange (&_counter, newVal) != 0;
		}
  
	private:
		volatile long _counter;
	};
}

#endif
