#include <QApplicationStateChangeEvent>
#include <iostream>
#include <mibAutopilot.h>

#include "Tester.h"
using namespace std;


int main(int argc, char *argv[])
{


    QPointF p1 = QPointF(19.9162,50.063);
    QPointF p2 = QPointF(50.063,19.9162);


    QPointF r = p1- p2;
    std::cout<< sqrt(pow(r.x(),2) + pow(r.y(),2)) << "\n";
    return 0 ;
}

