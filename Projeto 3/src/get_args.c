#include <string.h>

#include "get_args.h"

char* getArg(int argc, char* argv[], char* arg){

    for(int i = 1; i < argc; i++){

        if(argv[i][0] == PREFIX)
            if(!strcmp(argv[i] + 1, arg))
                if(i + 1 <= argc && argv[i + 1][0] != PREFIX)
                    return argv[i + 1];
    }

    return "";
}