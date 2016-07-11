/*!
    \file spin_lock.h
    \brief Spin-lock synchronization primitive definition
    \author Ivan Shynkarenka
    \date 22.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_SPIN_LOCK_H
#define CPPCOMMON_THREADS_SPIN_LOCK_H

#include "threads/locker.h"
#include "time/timestamp.h"

#include <atomic>
#include <chrono>

namespace CppCommon {

//! Spin-lock synchronization primitive
/*!
    The purpose of a spin lock is to prevent multiple threads from concurrently accessing a shared data structure.
    In contrast to a mutex, threads will busy-wait and waste CPU cycles instead of yielding the CPU to another thread.
    Do not use spinlocks unless you are certain that you understand the consequences!

    Thread-safe.

    https://en.wikipedia.org/wiki/Spinlock
*/
class SpinLock
{
public:
    SpinLock() noexcept : _lock(false) {}
    SpinLock(const SpinLock&) = delete;
    SpinLock(SpinLock&&) = default;
    ~SpinLock() = default;

    SpinLock& operator=(const SpinLock&) = delete;
    SpinLock& operator=(SpinLock&&) = default;

    //! Is already locked?
    /*!
        Will not block.

        \return 'true' if the spin-lock is already locked, 'false' if the spin-lock is released
    */
    bool IsLocked() noexcept;

    //! Try to acquire spin-lock without block
    /*!
        Will not block.

        \return 'true' if the spin-lock was successfully acquired, 'false' if the spin-lock is busy
    */
    bool TryLock() noexcept;

    //! Try to acquire spin-lock for the given spin count
    /*!
        Will block for the given spin count in the worst case.

        \param spin - Spin count
        \return 'true' if the spin-lock was successfully acquired, 'false' if the spin-lock is busy
    */
    bool TryLockSpin(int64_t spin) noexcept;

    //! Try to acquire spin-lock for the given nanoseconds
    /*!
        Will block for the given nanoseconds in the worst case.

        \param nanoseconds - Nanoseconds to wait for the spin-lock
        \return 'true' if the spin-lock was successfully acquired, 'false' if the spin-lock is busy
    */
    bool TryLockFor(int64_t nanoseconds) noexcept;
    //! Try to acquire spin-lock for the given time duration
    /*!
        Will block for the given time duration in the worst case.

        \param duration - Time duration to wait for the spin-lock
        \return 'true' if the spin-lock was successfully acquired, 'false' if the spin-lock is busy
    */
    template <class Rep, class Period>
    bool TryLockFor(const std::chrono::duration<Rep, Period>& duration) noexcept
    { return TryLockFor(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()); }
    //! Try to acquire spin-lock until the given timestamp
    /*!
        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the spin-lock
        \return 'true' if the spin-lock was successfully acquired, 'false' if the spin-lock is busy
    */
    template <class Clock, class Duration>
    bool TryLockUntil(const std::chrono::time_point<Clock, Duration>& timestamp) noexcept
    { return TryLockFor(timestamp - std::chrono::high_resolution_clock::now()); }

    //! Acquire spin-lock with block
    /*!
        Will block.
    */
    void Lock() noexcept;

    //! Release spin-lock
    /*!
        Will not block.
    */
    void Unlock() noexcept;

private:
    std::atomic<bool> _lock;
};

/*! \example threads_spin_lock.cpp Spin-lock synchronization primitive example */

} // namespace CppCommon

#include "spin_lock.inl"

#endif // CPPCOMMON_THREADS_SPIN_LOCK_H
