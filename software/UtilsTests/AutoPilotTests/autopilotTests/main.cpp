#include <QApplicationStateChangeEvent>
#include <iostream>
#include <mibAutopilot.h>

#include "Tester.h"
using namespace std;


int main(int argc, char *argv[])
{


    QPointF p1 = QPointF(19.8332, 50.0871);

    QPointF p2 = QPointF(50.0662, 19.9173 );


    QPointF r = p1- p2;
    std::cout<< sqrt(pow(r.x(),2) + pow(r.y(),2)) << "\n";
    return 0 ;
}

