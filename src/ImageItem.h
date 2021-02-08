//
// Created by liuyangping on 2021/2/8.
//

#ifndef VEXIMAGEEDITOR_IMAGEITEM_H
#define VEXIMAGEEDITOR_IMAGEITEM_H

#include <CVEX/CVEX_Context.h>
#include <memory>
#include <vector>
#include <iostream>


struct ImageItem {
    ImageItem () = default;
    ImageItem(int w, int h, CVEX_Type outputDataType = CVEX_TYPE_VECTOR3) :
            width(w),
            height(h),
            outputType(outputDataType) {
    }

    void bindItem(ImageItem *item) {
        bindImageItems.emplace_back(item);
    }

    void setupInputArguments(){

    }




    // before compile the VEX compiler should make sure the right number of arguments
    void compileVEXFile(const char *file) {
        char *argv[2] = {nullptr, nullptr};
        strcpy(argv[0], file);
        if (!CVEXPtr->load(0, argv)) {
            std::cout << "Error loading vex:" << argv[0] << std::endl;
        } else isCompiled = true;

    }


    bool isCompiled = false;
    std::unique_ptr<CVEX_Context> CVEXPtr = std::make_unique<CVEX_Context>();


    std::vector<CVEX_Value *> uvCoord;            // system internal data, name : "uv" , type: CVEX_TYPE_VECTOR2
    std::vector<CVEX_Value *> inputListImageData; // support multiple input arguments
    CVEX_Value *outputImageData;        //may be float,vector, vector4, we only maintain one output, even VEX support >=1 output arguments

    // bind other node

    std::vector<ImageItem *> bindImageItems;


    // width & height of this buffer
    int width = 500;
    int height = 500;

    // Image Type of output
    CVEX_Type outputType;
};


#endif //VEXIMAGEEDITOR_IMAGEITEM_H
