#include "coroutine.h"
#include <iostream>

using namespace std;

Coroutine::FiberPtr Coroutine::pPrimaryFiber = nullptr;

void Coroutine::_InitCurThread( FiberDataPtr pData ) {
    if ( !pPrimaryFiber ) {
        pPrimaryFiber = ConvertThreadToFiber( pData );
    }
    else {
        cerr << "This thread is already converted to primary fiber" << endl;
    }
}

void Coroutine::_Resume( FiberPtr pFiber ) {
    SwitchToFiber( pFiber );
}

void Coroutine::_Yield( FiberPtr pFiber ) {
    SwitchToFiber( pPrimaryFiber );
}

Coroutine::Coroutine( FiberFunction func, FiberDataPtr pData, size_t stackSize )
: pFiber( CreateFiber( stackSize, func, pData ) ) {}

Coroutine::~Coroutine() {
    DeleteFiber( pFiber );
}

void Coroutine::start() {
    resume();
}

void Coroutine::resume() {
    _Resume( pFiber );
}

void Coroutine::yield() {
    _Yield( pFiber );
}
