#include <iostream>
#include <string>
#include "src/multitreading.h"

int main(int argc, char *argv[]) {
    std::string  owner_id{};
    int num{};
    for ( int i = 0; i < argc; ++i) {
        if ( std::string(argv[i]) == "-id" ) {
            owner_id = argv[i + 1];
        } else if ( std::string(argv[i]) == "-n" ) {
            num = std::stoi(argv[i + 1]);
        }
    }
    if ( owner_id.empty() || num <= 0 || num > 40 ) {
        std::cout << "Usage: vk-api-parser [-id wall id without '-'] [-n num of running threads, max 40]\n";
        return 0;
    }
    owner_id = '-' + owner_id;
    std::cout << owner_id << "   " << num  <<'\n';
    thread_creator t(owner_id, num);
    //thread_creator t("-91050183", 30);
    t.start(10000);
   return 0;
}
