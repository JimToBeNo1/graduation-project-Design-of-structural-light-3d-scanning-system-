#include<iostream>  //cin cout
#include<stdio.h> //printf
#include "mw.h"
#include <QDialog>
#include <QApplication>




int main(int argc, char *argv[])
{
    //UI界面
    QApplication a(argc, argv);
    MW w;
    w.show();





    return a.exec();
}
