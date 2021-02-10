#include <iostream>
#include "ImageItem.h"


#include <QApplication>
#include <QLabel>
#include "MainWindow.h"
#include <QSplitter>
#include <MOT/MOT_Director.h>

int main(int argc , char **argv){
  /*
    ImageItem item(100, 100);
    item.name = "Layer1";

    std::string code = R"(
    cvex test(vector2 uv = 0 ; export vector4 out=0)
    {
        //printf("enter the vex code %f, %f\n",uv.x,uv.y );
        out = set(uv.x, uv.y, 0, 1);
    })";

    std::string vexFile(item.name);
    vexFile += ".vfl";
    std::ofstream out(vexFile);
    out << code;
    out.close();

    item.setupInputArguments();
    item.compileVEXFile(vexFile.c_str());
    item.setupArgumentsData();
    item.runVex();
    item.bufferToPPM("test.ppm");
    */
    QApplication a(argc, argv);
    MainWidget w;
    w.setWindowTitle("VEX Canvas");


    std::ifstream  styleIN;
    styleIN.open("houdinistylesheet");
    std::stringstream stylestream;
    stylestream << styleIN.rdbuf(); 
    QString str(stylestream.str().c_str());

    


    w.setStyleSheet(str);
    w.show();
    return a.exec();


  
}