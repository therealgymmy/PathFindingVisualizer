#ifndef COROUTINE_H
#define COROUTINE_H

#include <windows.h>

//
// CreateFiber( STACK_SIZE, START_ADDRESS, OPTIONAL_PARAM )
//   STACK_SIZE == 0 <- would give us the default stack size for executable
//   START_ADDRESS   <- the starting function
//   OPTIONAL_PARAM  <- optional parameter
//

class Coroutine {

typedef LPVOID FiberPtr;
typedef LPVOID FiberDataPtr;
typedef VOID __stdcall ( *FiberFunction )( FiberDataPtr );

// Static
public:
    // initialize current thread to schedule for fiber
    static void _InitCurThread( FiberDataPtr pData );

    // default main loop for an object
    template< typename T >
    static void __stdcall _Default( void* ) {

        T *object = static_cast< T* >( GetFiberData() );

        object->crt();

    }

    static void _Resume( FiberPtr pFiber );
    static void _Yield( FiberPtr pFiber = nullptr );

private:
    // primary fiber in the thread
    static FiberPtr pPrimaryFiber;

// Member
public:
    // normal ctor, for function entry point
    Coroutine( FiberFunction func      = nullptr,
               FiberDataPtr  pData     = nullptr,
               size_t        stackSize = 0 );

    // template ctor, for object entry point
    template< typename T >
    Coroutine( T *pObject, size_t stackSize = 0 )
    : pFiber ( CreateFiber( stackSize, &_Default< T >, pObject ) ) {}

    virtual ~Coroutine();

    Coroutine( const Coroutine &rhs ) = delete;

    Coroutine &operator=( const Coroutine &rhs ) = delete;

    void start();

protected:
    void resume();
    void yield();

private:
    FiberPtr pFiber;

};

#endif//COROUTINE_H
