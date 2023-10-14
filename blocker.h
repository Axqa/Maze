#ifndef BLOCKER_H
#define BLOCKER_H
#include <QObject>

template<class T> class Blocker {
    T *blocked;
    bool previous;
public:
    Blocker(T *blocked)
        : blocked(blocked),
          previous(blocked->blockSignals(true)) {}
    ~Blocker() { blocked->blockSignals(previous); }
    T *operator->() { return blocked; }

};

template<class T> inline Blocker<T> whileBlocking(T *blocked) {
    return Blocker<T>(blocked);
}
#endif // BLOCKER_H
