#include "MainWindow.h"
#include <QString>
#include <QFont>
#include "ImageItem.h"
#include <QFile>
#include <QTextStream>
MainWidget::MainWidget(QWidget* p /*= nullptr*/) :QWidget(p)
{
    mainSpt = new QSplitter(this);

    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);


    view = new ImageView(this);
    scene = new ImageScene;
    view->setScene(scene);
    //scene->addPixmap(QPixmap("test.ppm"));

    editor = new CodeEditor(this);
    auto font = editor->font();
    font.setBold(true);
    font.setPixelSize(20);
    editor->setFont(font);
    prescript();
    connect(editor, &CodeEditor::textChanged, this, &MainWidget::compileAndRunning);



    mainSpt->addWidget(view);
    mainSpt->addWidget(editor);
    mainSpt->setStretchFactor(0, 1);
    mainSpt->setStretchFactor(1, 10);

    mainLayout->addWidget(mainSpt);

    resize(1280, 720);

    

}

void MainWidget::compileAndRunning()
{
    std::cout << "---------------- Compile --------------------------\n";
    
    // make argv

    const char* fileName = "test.vfl";

    QFile data(fileName);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);
        out << editor->toPlainText();
    }





    //outscript(fileName, editor->toPlainText());
    
        
    auto  width = 600;
    auto  height = 600;

    ImageItem  vexImageEngine(width, height); 
    
    vexImageEngine.CVEXPtr.clearAllFunctions();
    vexImageEngine.CVEXPtr.clear();
    vexImageEngine.CVEXPtr.clearVexErrors();
    vexImageEngine.setupInputArguments();
    vexImageEngine.compileVEXFile(fileName);
   
    if (vexImageEngine.isCompiled) {
        vexImageEngine.setupArgumentsData();
        vexImageEngine.runVex();

    }
    else {
        qDebug() << "Error compile\n";
        return;
    }

    QImage memMap(width, height, QImage::Format_ARGB32);
    memMap.fill(Qt::black);
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int index = col + row * width;
            UT_Vector4 rgba = vexImageEngine.outputImageBuffer[index];
            float r = rgba.x()*255;   r = std::min(255.0f, r);
            float g = rgba.y()*255;   g = std::min(255.0f, g);
            float b = rgba.z()*255;   b = std::min(255.0f, b);
            float a = rgba.w()*255;   a = std::min(255.0f, a);
            memMap.setPixelColor(row, col, QColor(r, g, b, a));
        }
    }

    this->scene->clear();
    QPixmap map(width,height);
    map = map.fromImage(memMap);
    this->scene->addPixmap(map);
   
    
}

void MainWidget::prescript()
{
    const char* code = R"(cvex test(vector2 uv = 0 ; export vector4 out=0)
{
    #define numpoints 3
    #define expvalue 4.0
    #define light_intensity  0.1

    //printf("enter the vex code %f, %f\n",uv.x,uv.y );
    vector2 pos = (uv - set(0.5,0.5) ) * 2.0;
    vector L = 0;

    for(int i = 0; i <numpoints ; i++){

        float per_angle = float(360) /  float(numpoints) ;
        float angle = per_angle * float(i)  ;
        float lg_x = sin(  radians(angle) )*0.5;
        float lg_y = cos(  radians(angle) )*0.6;

        vector2 newpos = set(lg_x,lg_y);
        float data = light_intensity*light_intensity / pow(length(pos + newpos),expvalue);

        L += set(data,data,data);
    }
    out = set(L.x,L.y,L.z,1);

  
})";

    const char* code2 = R"(cvex test(vector2 uv = 0 ; export vector4 out=0)
{
        
    matrix3 rot= ident();
    rotate(rot , radians(-90) , set(0,0,1) );
    vector newuv = set(uv.x, uv.y, 0);
    newuv *= rot;
    vector tex = texture("d:/yys.jpg", newuv.x,newuv.y); 
    // convert to grey
    out = luminance(tex);
    out.w = 1;
})";
    editor->append(QString(code2));
    editor->setBuffer(QString(code2));
}

void MainWidget::outscript(const QString& filePath, const QString& text)
{
    // this will error in slot....
    std::ofstream file;
    file.open((filePath.toStdString()));
    std::cout << "SLOT::MainWidget::compileAndRunning code :" << text.toStdString() << std::endl;
    file << text.toStdString();
    file.close();
}

