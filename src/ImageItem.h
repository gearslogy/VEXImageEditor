//
// Created by liuyangping on 2021/2/8.
//

#ifndef VEXIMAGEEDITOR_IMAGEITEM_H
#define VEXIMAGEEDITOR_IMAGEITEM_H

#include <CVEX/CVEX_Context.h>
#include <memory>
#include <vector>
struct ImageItem {

    void bindItem(ImageItem *item){
        bindImageItems.emplace_back(item);
    }

    std::vector<ImageItem*> bindImageItems;
    bool isCompiled = false;
    std::unique_ptr<CVEX_Context> CVEXPtr= std::make_unique<CVEX_Context>() ;
    CVEX_Value *inputImageData;
    CVEX_Value *outputImageData;

};


#endif //VEXIMAGEEDITOR_IMAGEITEM_H
