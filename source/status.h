#ifndef STATUS_H
#define STATUS_H

enum Status{
    Play = 0,
    Check = 1,
    DoubleCheck = 2,
    Promotion = 3,
    Mate = 4,
    Draw = 5,
    StaleMate = 6
};

#endif // STATUS_H
