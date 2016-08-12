#ifndef MYSTRUCT_H
#define MYSTRUCT_H
//ID  should be in blob_t
struct blob_t
{
    int x;
    int y;
    int size;
    int ID;
    blob_t()
    {
        ID = -1;
        x = 0;
        y = 0;
        size = 0;
    }
};

struct note_t
{
    int tone;
    int bend;
    note_t()
    {
        tone = -1;
    }
};



#endif
