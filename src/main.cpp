



#include "main_thread.h"
#include<iostream>


int main(int argc, char* argv[]){
    //std::cout << "count_of_threads: "  << std::stoi(argv[1]) << std::endl;
    //std::cout << "count_of posts: " << std::stoi(argv[2]) << std::endl;
    vk_parse::main_thread p("91050183", "output.db", 30);
    p.start(10000);
    return 0;
}