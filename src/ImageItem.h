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
    void setupInputArguments() {


        // first add input the uv
        CVEXPtr->addInput("uv", CVEX_TYPE_VECTOR2, false);

        // then add input the cd0,cd1,cd2....
        int startIndex = 0;
        std::for_each(bindImageItems.begin(), bindImageItems.end(), [&startIndex, this](ImageItem *item) {
            std::stringstream os;
            os << "cd" << startIndex;
            std::string result;
            os >> result;

            this->CVEXPtr->addInput(result.c_str(), CVEX_TYPE_VECTOR4 , true);
            startIndex += 1;
        });

    }

    // STEP 1
    // before compile the VEX compiler should make sure the right number of arguments
    void compileVEXFile(const char *file) {
        const char* argv[4096];
        argv[0] = file;
        if (!CVEXPtr->load(1, argv)) {
            std::cout << "Error loading vex:" << argv[0] << std::endl;
            std::cout << CVEXPtr->getVexWarnings() << std::endl;
            std::cout << CVEXPtr->getVexErrors() << std::endl;
        } else isCompiled = true;
       

    }

    // STEP2
    // setup data
    void setupArgumentsData() {
        // gen buffer of the uvcoord
        std::cout << "setup the uv coord buffer end\n";
        uvCoordsBuffer.clear();
        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                int index = col + row * width;
                float u = (static_cast<float>(col) + 1.0f) / static_cast<float>(width);
                float v = (static_cast<float>(row) + 1.0f) / static_cast<float>(height);
                uvCoordsBuffer.emplace_back(UT_Vector2(u, v));
            }
        }

        // uv buffer data out
        // first let's setup the uvcoord
        // Check to see whether VEX function has a "vector2 uv" parameter
        uvCoordData = CVEXPtr->findInput("uv", CVEX_TYPE_VECTOR2);
        if (uvCoordData) {
            // Give the uv real data
            uvCoordData->setTypedData(uvCoordsBuffer.data(), width * height);
        } else {
            std::cout << "can not find the uv arguments , setup the uv data error\n";
        }


        // then other input

        // then add input the cd0,cd1,cd2....
        int startIndex = 0;
        std::for_each(bindImageItems.begin(), bindImageItems.end(), [&startIndex, this](ImageItem *item) {
            std::stringstream os;
            os << "cd" << startIndex;
            std::string result;
            os >> result;
            CVEX_Value  *valuePtr = this->CVEXPtr->findInput(result.c_str(), CVEX_TYPE_VECTOR4);
            if(valuePtr){
                valuePtr->setTypedData(item->outputImageBuffer.data(), width * height); // copy value for this item!
            }else{
                std::cout << "can not set other input argument data for:" << result << std::endl;
            }


            startIndex += 1;
        });




        // Check to see whether VEX function has a "export type out" parameter
        outputImageData = CVEXPtr->findOutput("out", CVEX_TYPE_VECTOR4);
        if (outputImageData){
            outputImageBuffer.clear();
            outputImageBuffer.resize(width * height);
            outputImageData->setTypedData(outputImageBuffer.data(), width * height);
        }
    }

    // STEP3
    // Running VEX Code
    void runVex() {
        // Run the CVEX program
        CVEXPtr->run(width*height, false);
    }



    bool isCompiled = false;
    std::unique_ptr<CVEX_Context> CVEXPtr = std::make_unique<CVEX_Context>();


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
};


#endif //VEXIMAGEEDITOR_IMAGEITEM_H
