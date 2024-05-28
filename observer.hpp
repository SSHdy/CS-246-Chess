#pragma once
#define CS246CHESS_OBSERVER_HPP

#include "pieces.hpp"
#include "containers.hpp"
#include "code_utils.inc"

class BoardObserver {
public:
    BoardObserver() = default;
    virtual ~BoardObserver() = default;

    virtual void get_notified(BoardCoor, OptPiece) = 0;
};


class BoardSubject {
public:
    BoardSubject() = default;
    virtual ~BoardSubject() = 0;

    virtual void attach(BoardObserver* p_o) { this->_p_observer = p_o; }

protected:
    BoardObserver& get_observer() { return *this->_p_observer; }

private:
    BoardObserver* _p_observer = nullptr;
};


inline BoardSubject::~BoardSubject() = default;
