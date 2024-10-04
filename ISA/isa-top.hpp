#include <iostream>
#include <vector>
#include <ncurses.h>


class IsaTop{
    public:
        IsaTop(int listeningInterface,bool sortByBytes);
    private:
        // listenig Interface
        int listeningInterface;
        // TRUE if sort by bytes/s, FALSE if sort by packets/s
        bool sortByBytes;

};