//
// Created by liuyangping on 2021/2/8.
//

#include "ImageItem.h"
#include <fstream>
#include <iostream>

void ImageItem::setupInputArguments()
{
    // first add input the uv
    CVEXPtr.addInput("uv", CVEX_TYPE_VECTOR2, true);

    // then add input the cd0,cd1,cd2....
    int startIndex = 0;
    std::for_each(bindImageItems.begin(), bindImageItems.end(), [&startIndex, this](ImageItem* item) {
        std::stringstream os;
        os << "cd" << startIndex;
        std::string result;
        os >> result;

        this->CVEXPtr.addInput(result.c_str(), CVEX_TYPE_VECTOR4, true);
        startIndex += 1;
        });
}

void ImageItem::compileVEXFile(const char* file)
{
    const char* argv[4096];
    argv[0] = file;
    if (!CVEXPtr.load(1, argv)) {
        std::cout << "Error loading vex:" << argv[0] << std::endl;
        std::cout << CVEXPtr.getVexWarnings() << std::endl;
        std::cout << CVEXPtr.getVexErrors() << std::endl;
        isCompiled = false;
    }
    else isCompiled = true;

}



void ImageItem::setupArgumentsData()
{
    // gen buffer of the uvcoord
    std::cout << "setup the uv coord buffer end\n";
    uvCoordsBuffer.clear();
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int index = col + row * width;
            float u = (static_cast<float>(col)) / static_cast<float>(width - 1);
            float v = (static_cast<float>(row)) / static_cast<float>(height - 1);
            //std::cout << "set up index uv :" << index << " u:" << u << "v:" << v << std::endl;
            uvCoordsBuffer.emplace_back(UT_Vector2(u, v));
        }
    }

    // Check to see whether VEX function has a "vector2 uv" parameter
    uvCoordData = CVEXPtr.findInput("uv", CVEX_TYPE_VECTOR2);
    if (uvCoordData) {
        uvCoordData->setTypedData(uvCoordsBuffer.data(), width * height);
    }
    else {
        std::cout << "can not find the uv arguments , setup the uv data error\n";
    }


    // then other input

    // then add input the cd0,cd1,cd2....
    int startIndex = 0;
    std::for_each(bindImageItems.begin(), bindImageItems.end(), [&startIndex, this](ImageItem* item) {
        std::stringstream os;
        os << "cd" << startIndex;
        std::string result;
        os >> result;
        CVEX_Value* valuePtr = CVEXPtr.findInput(result.c_str(), CVEX_TYPE_VECTOR4);
        if (valuePtr) {
            valuePtr->setTypedData(item->outputImageBuffer.data(), width * height); // copy value for this item!
        }
        else {
            std::cout << "can not set other input argument data for:" << result << std::endl;
        }


        startIndex += 1;
        });




    // Check to see whether VEX function has a "export type out" parameter
    outputImageData = CVEXPtr.findOutput("out", CVEX_TYPE_VECTOR4);
    if (outputImageData) {
        outputImageBuffer.clear();
        outputImageBuffer.resize(width * height);
        outputImageData->setTypedData(outputImageBuffer.data(), width * height);
    }
}

void ImageItem::runVex()
{
    // Run the CVEX program
    CVEXPtr.run(width * height, false);
}

void ImageItem::bufferToPPM(const char *file)
{
    std::ofstream out(file);
    if (out.fail()) {
        std::cout << "can not open file " << file << " to write"<< std::endl;
        return;
    }

    out << "P3\n";
    out << width << " "<< height << "\n";
    out << 255 << "\n";

    for(auto &cd: outputImageBuffer){
        // convert cd to 0-255
        int r = cd.x() * 255; r = std::min(255, r);
        int g = cd.y() * 255; g = std::min(255, g);
        int b = cd.z() * 255; b = std::min(255, b);
        out << r << " " << g << " " << b << "\n";
    }

    out.close();
}
