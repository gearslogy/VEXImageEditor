//
// Created by liuyangping on 2021/2/8.
//

#ifndef VEXIMAGEEDITOR_IMAGEITEM_H
#define VEXIMAGEEDITOR_IMAGEITEM_H

#include <MOT/MOT_Director.h>

#include <CVEX/CVEX_Context.h>
#include <memory>
#include <vector>
#include <iostream>
#include <sstream>


struct ImageItem {
    

    ImageItem() = default;

    ImageItem(int w, int h) :
            width(w),
            height(h){
    }

    void bindItem(ImageItem *item) {
        if (this != item) // do not set self to this input
            bindImageItems.emplace_back(item);
    }

    // STEP 0
    void setupInputArguments();
    // STEP 1
    // before compile the VEX compiler should make sure the right number of arguments
    void compileVEXFile(const char* file);

    // STEP2
    // setup data
    void setupArgumentsData();

    // STEP3
    // Running VEX Code
    void runVex();


    bool isCompiled = false;
    CVEX_Context CVEXPtr;


    std::vector<UT_Vector2> uvCoordsBuffer; // buffer setup initialized
    CVEX_Value *uvCoordData;                // system internal data, name : "uv" , type: CVEX_TYPE_VECTOR2

    std::vector<UT_Vector4> outputImageBuffer;
    CVEX_Value *outputImageData;         // may be float,vector, vector4, we only maintain one output, even VEX support >=1 output arguments



    // bind other node
    std::vector<ImageItem *> bindImageItems;


    // width & height of this buffer
    int width = 500;
    int height = 500;

    // write out buffer to ppm , p3 
    void bufferToPPM(const char *file);

    // name for this item
    std::string name = "";
};


#endif //VEXIMAGEEDITOR_IMAGEITEM_H
