#include "isa-top.hpp"

IsaTop::IsaTop(int listeningInterface,bool sortByBytes){
    this->sortByBytes = sortByBytes;
    this->listeningInterface = listeningInterface;
}