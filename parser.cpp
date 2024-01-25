
#include "depend/VTKParser.h"
#include <iostream>


int main(int argc, char **argv) {
    VTKparser p("/home/paul/Masters/VTKParser/loop_test.vtk");
    p.parse();
    p.print_stats();
    return 0;
}