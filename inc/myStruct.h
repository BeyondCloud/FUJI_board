#ifndef MYSTRUCT_H
#define MYSTRUCT_H
struct blob_t
{
    int x;
    int y;
    int size;
    int ID;
    blob_t()
    {
        x = 0;
        y = 0;
        size = 0;
        ID = 0;
    }

};
struct note_t
{
    int tone;
    int bend;
};

#endif
