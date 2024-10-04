#include <iostream>
#include "isa-top.hpp"

struct Arguments{
    int listeningInterface;
    bool sortingByBytes;
}Arguments_t;

int handleArguments(int argc, char * argv[]){
    for(int i = 1; i < argc; ++i){
        // interface
        if(1){

        }
        // sorting
        else if(2){

        }
        else
            return 1; //error
    }


    return 0;
}


int main(int argc, char * argv[]){
    return 0;
}