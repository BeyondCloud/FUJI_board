#ifndef MYSTRUCT_H
#define MYSTRUCT_H
struct blob_t
{
    int x;
    int y;
    int size;
    blob_t()
    {
        size = 0;
    }
};
struct note_t
{
    int tone;
    int bend;
};

#endif
