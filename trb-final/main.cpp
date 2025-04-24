#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdint>
#include "httplib.h"
#include <vector> 
#include <stack>

using namespace std;

struct rgb{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;

    rgb(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(g){}
    rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : red(r), green(g), blue(b), alpha(a){}
    rgb() : red(0), green(0), blue(0){}
};

struct pixel{
    rgb px;

    pixel() : px(){}
    pixel(rgb RGB) : px(RGB){}

    void addValue(int addValue){
        int resultValue;

        resultValue = px.red + addValue;
        px.red = validPixelValue(resultValue);

        resultValue = px.green + addValue;
        px.green = validPixelValue(resultValue);
        
        resultValue = px.blue + addValue;
        px.blue = validPixelValue(resultValue);
        
    }

    void subtValue(int addValue){
        int resultValue;

        resultValue = px.red - addValue;
        px.red = validPixelValue(resultValue);

        resultValue = px.green - addValue;
        px.green = validPixelValue(resultValue);
        
        resultValue = px.blue - addValue;
        px.blue = validPixelValue(resultValue);
        
    }

    void multValue(int value){
        int resultValue;

        resultValue = px.red * value;
        px.red = validPixelValue(resultValue);

        resultValue = px.green * value;
        px.green = validPixelValue(resultValue);
        
        resultValue = px.blue * value;
        px.blue = validPixelValue(resultValue);
        
    }

    void divValue(int value){
        int resultValue;

        resultValue = px.red / value;
        px.red = validPixelValue(resultValue);

        resultValue = px.green / value;
        px.green = validPixelValue(resultValue);
        
        resultValue = px.blue / value;
        px.blue = validPixelValue(resultValue);
        
    }

    void addPixel(const pixel& pxA, const pixel& pxB){
        int resultValue;

        resultValue = pxA.px.red + pxB.px.red;
        px.red = validPixelValue(resultValue);

        resultValue = pxA.px.green + pxB.px.green;
        px.green = validPixelValue(resultValue);

        resultValue = pxA.px.blue + pxB.px.blue;
        px.blue = validPixelValue(resultValue);

        px.alpha = 255;
    }

    void subtPixel(const pixel& pxA, const pixel& pxB){
        int resultValue;

        resultValue = pxA.px.red - pxB.px.red;
        px.red = validPixelValue(resultValue);

        resultValue = pxA.px.green - pxB.px.green;
        px.green = validPixelValue(resultValue);

        resultValue = pxA.px.blue - pxB.px.blue;
        px.blue = validPixelValue(resultValue);

        px.alpha = 255;
    }

    void logicAND(const pixel& pxA, const pixel& pxB){
        if((pxA.px.red == 255) && (pxB.px.red == 255)){
            px.red = 255;
        }else{
            px.red = 0;
        }  
    }

    void logicOR(const pixel& pxA, const pixel& pxB){
        if((pxA.px.red == 255) || (pxB.px.red == 255)){
            px.red = 255;
        }else{
            px.red = 0;
        }
    }

    void logicXOR(const pixel& pxA, const pixel& pxB){
        if(pxA.px.red != pxB.px.red){
            px.red = 255;
        }else{
            px.red = 0;
        }
    }

    void logicNOT(const pixel& pxl){
        if(pxl.px.red == 255){
            px.red = 0;
        }else {
            px.red = 255;
        }
    }

    void blending(const pixel& pxA, const pixel& pxB, float value){
        int resultValue;
        resultValue= value * pxA.px.red + (1-value) * pxB.px.red;
        validPixelValue(resultValue);
        px.red = resultValue;

        resultValue = value * pxA.px.green + (1-value) * pxB.px.green;
        validPixelValue(resultValue);
        px.green = resultValue;

        resultValue = value * pxA.px.blue + (1-value) * pxB.px.blue;
        validPixelValue(resultValue);
        px.blue = resultValue;

        px.alpha = 255;
    }

    void linearAverage(const pixel& pxA, const pixel& pxB){
        addPixel(pxA,pxB);
        px.red /= 2;
        px.green /= 2;
        px.blue /= 2;
        px.alpha = 255;
    }

    void setPixel(const pixel& pxl){
        px.red = pxl.px.red;
        px.green = pxl.px.green;
        px.blue = pxl.px.blue;
        px.alpha = pxl.px.alpha;
    }

    void equalizeHistogram(int newPixelValue){
        
        uint8_t pixelValue = validPixelValue(newPixelValue);

        px.red = pixelValue;
    }


    void rgbTo8Bit(){
        px.red = (px.red + px.green + px.blue)/3;
    }

    void rgbTo1Bit(int threshold){
        px.red >= threshold ? px.red = 255 : px.red = 0; 
    }

    void convolutionMean(const pixel& pxTarget, const pixel& pxLeft, const pixel& pxRight, const pixel& pxUp, const pixel& pxDown, const pixel& pxNW, const pixel& pxNE, const pixel& pxSW, const pixel& pxSE){
        
        int value = (pxTarget.px.red + pxLeft.px.red + pxRight.px.red + pxUp.px.red + pxDown.px.red + pxNW.px.red + pxNE.px.red + pxSW.px.red + pxSE.px.red)/9;
        uint8_t newPixelValue = validPixelValue(value);
        px.red = newPixelValue;

        value = (pxTarget.px.green + pxLeft.px.green + pxRight.px.green + pxUp.px.green + pxDown.px.green + pxNW.px.green + pxNE.px.green + pxSW.px.green + pxSE.px.green)/9;
        newPixelValue = validPixelValue(value);
        px.green = newPixelValue;

        value = (pxTarget.px.blue + pxLeft.px.blue + pxRight.px.blue + pxUp.px.blue + pxDown.px.blue + pxNW.px.blue + pxNE.px.blue + pxSW.px.blue + pxSE.px.blue)/9;
        newPixelValue = validPixelValue(value);
        px.blue = newPixelValue;

        px.alpha = 255;    
    }

    void convolutionMax(const pixel& pxTarget, const pixel& pxLeft, const pixel& pxRight, const pixel& pxUp, const pixel& pxDown, const pixel& pxNW, const pixel& pxNE, const pixel& pxSW, const pixel& pxSE){
        uint8_t value;

        value = pxTarget.px.red > pxLeft.px.red ? pxTarget.px.red : pxLeft.px.red;

        if (value < pxRight.px.red){
            value = pxRight.px.red;
        }

        if (value < pxUp.px.red){
            value = pxUp.px.red;
        }

        if (value < pxDown.px.red){
            value = pxDown.px.red;
        }

        if (value < pxNW.px.red){
            value = pxNW.px.red;
        }

        if (value < pxNE.px.red){
            value = pxNE.px.red;
        }

        if (value < pxSW.px.red){
            value = pxSW.px.red;
        }

        if (value < pxSE.px.red){
            value = pxSE.px.red;
        }

        px.red = value;

        value = pxTarget.px.green > pxLeft.px.green ? pxTarget.px.green : pxLeft.px.green;

        if (value < pxRight.px.green){
            value = pxRight.px.green;
        }

        if (value < pxUp.px.green){
            value = pxUp.px.green;
        }

        if (value < pxDown.px.green){
            value = pxDown.px.green;
        }

        if (value < pxNW.px.green){
            value = pxNW.px.green;
        }

        if (value < pxNE.px.green){
            value = pxNE.px.green;
        }

        if (value < pxSW.px.green){
            value = pxSW.px.green;
        }

        if (value < pxSE.px.green){
            value = pxSE.px.green;
        }

        px.green = value;

        value = pxTarget.px.blue > pxLeft.px.blue ? pxTarget.px.blue : pxLeft.px.blue;

        if (value < pxRight.px.blue){
            value = pxRight.px.blue;
        }

        if (value < pxUp.px.blue){
            value = pxUp.px.blue;
        }

        if (value < pxDown.px.blue){
            value = pxDown.px.blue;
        }

        if (value < pxNW.px.blue){
            value = pxNW.px.blue;
        }

        if (value < pxNE.px.blue){
            value = pxNE.px.blue;
        }

        if (value < pxSW.px.blue){
            value = pxSW.px.blue;
        }

        if (value < pxSE.px.blue){
            value = pxSE.px.blue;
        }

        px.blue = value;

        px.alpha = 255;

    }

    void convolutionMin(const pixel& pxTarget, const pixel& pxLeft, const pixel& pxRight, const pixel& pxUp, const pixel& pxDown, const pixel& pxNW, const pixel& pxNE, const pixel& pxSW, const pixel& pxSE){
        uint8_t value;

        value = pxTarget.px.red < pxLeft.px.red ? pxTarget.px.red : pxLeft.px.red;

        if (value > pxRight.px.red){
            value = pxRight.px.red;
        }

        if (value > pxUp.px.red){
            value = pxUp.px.red;
        }

        if (value > pxDown.px.red){
            value = pxDown.px.red;
        }

        if (value > pxNW.px.red){
            value = pxNW.px.red;
        }

        if (value > pxNE.px.red){
            value = pxNE.px.red;
        }

        if (value > pxSW.px.red){
            value = pxSW.px.red;
        }

        if (value > pxSE.px.red){
            value = pxSE.px.red;
        }

        px.red = value;

        value = pxTarget.px.green < pxLeft.px.green ? pxTarget.px.green : pxLeft.px.green;

        if (value > pxRight.px.green){
            value = pxRight.px.green;
        }

        if (value > pxUp.px.green){
            value = pxUp.px.green;
        }

        if (value > pxDown.px.green){
            value = pxDown.px.green;
        }

        if (value > pxNW.px.green){
            value = pxNW.px.green;
        }

        if (value > pxNE.px.green){
            value = pxNE.px.green;
        }

        if (value > pxSW.px.green){
            value = pxSW.px.green;
        }

        if (value > pxSE.px.green){
            value = pxSE.px.green;
        }

        px.green = value;

        value = pxTarget.px.blue < pxLeft.px.blue ? pxTarget.px.blue : pxLeft.px.blue;

        if (value > pxRight.px.blue){
            value = pxRight.px.blue;
        }

        if (value > pxUp.px.blue){
            value = pxUp.px.blue;
        }

        if (value > pxDown.px.blue){
            value = pxDown.px.blue;
        }

        if (value > pxNW.px.blue){
            value = pxNW.px.blue;
        }

        if (value > pxNE.px.blue){
            value = pxNE.px.blue;
        }

        if (value > pxSW.px.blue){
            value = pxSW.px.blue;
        }

        if (value > pxSE.px.blue){
            value = pxSE.px.blue;
        }

        px.blue = value;

        px.alpha = 255;
    }


    uint8_t validPixelValue(int value){
        if(value>255){
            return 255;
        }
        if(value < 0){
            return 0;
        }
        return value;
    }
};

pixel transformStringtoRGBA(string& stringValue){
    string STRvalue;
    uint8_t valueRGB[4] = {0,0,0,255};
    stringstream ss(stringValue);
    char ch;
    int cont =0;

    while(ss >> ch){

        if(isdigit(ch)){
            STRvalue += ch;
        }else{
            int integer = stoi(STRvalue);
            valueRGB[cont] = static_cast<uint8_t>(integer);
            STRvalue = "";
            cont++;
        }
    }
    
    rgb RGB = {valueRGB[0], valueRGB[1], valueRGB[2], valueRGB[3]};

    pixel px(RGB);

    return px;
}

vector<vector<pixel>> parse_json_pixels(string& body){
    vector<vector<pixel>> pixels;
    vector<pixel> row;
    string stringValue;
    stack<char> stack;
    
    //Remover '[ ]' de abertura e de fechamento
    body.erase(0,1);
    body.erase(body.size() -1,1);

    stringstream ss(body);
    char ch;

    size_t pos = 0;

    while(ss >> ch) {   /// PILHA [, 
        //Entrou na coluna
        if (ch == '['){
            stack.push(ch);

            while(!stack.empty()){
                ss >> ch;
                if(ch == '['){
                    stack.push(ch);
                }else if (ch == ']'){
                    pixels.push_back(row);
                    row.clear();
                    stringValue = "";
                    stack.pop();
                }else if (isdigit(ch)){
                    while(ch != ',' && ch != ']'){
                        stringValue += ch;
                        ss >> ch;
                    }
                    if(ch==','){
                        stringValue += ch;
                    }else{
                        pixel px = transformStringtoRGBA(stringValue);
                        row.push_back(px);
                        stringValue = "";
                        if(!stack.empty()){                        
                            stack.pop();
                        }
                    }
                    
                }
            }
        }
    }

    return pixels;
}

vector<string> separeJsonImages(string& body){
    stringstream ss(body);
    char ch;
    vector<string> imgJSON;
    string stringValue;
    int i=0;

    while(ss >> ch){
        if(ch == 'S'){
            imgJSON.push_back(stringValue);
            i++;
            stringValue = "";
        }else{
            stringValue += ch;
        }
    }
    imgJSON.push_back(stringValue);

    return imgJSON;
}

string pixelToString(const pixel& p, int cont, int height, bool is8bit) {
    stringstream rgbString;
    if (is8bit){
        if (cont < height) {
            rgbString << "\n[" << (int)p.px.red << "],\n";
        }
        else{
            rgbString << "\n[" << (int)p.px.red << "]\n";
        }
    }else{
        if (cont < height) {
            rgbString << "\n[" <<(int)p.px.red << "," << (int)p.px.green << "," << (int)p.px.blue << "," << (int)p.px.alpha << "],\n";
        }else{
            rgbString << "\n[" <<(int)p.px.red << "," << (int)p.px.green << "," << (int)p.px.blue << "," << (int)p.px.alpha << "]\n";
        }
    }
    return rgbString.str();
}


string imgToString(const vector<vector<pixel>>& image, bool is8bit) {
    ostringstream result;
    string imgSTR;
    int height = image.size();
    int width = image.empty() ? 0 : image[0].size();
    int cont = 0;
    result << "[" << "\n";

    for (const auto& row : image) {
	result << "[";
        for (const auto& px : row) {
            cont = (cont==width) ? 1 : ++cont;
            result << pixelToString(px, cont, width, is8bit); // Adiciona a string do pixel
        }
	result << "],\n";// Nova linha após cada linha de pixels
    }

    result << "]" << "\n";

    imgSTR = result.str();

    imgSTR.erase(imgSTR.size() -4, 2);

    return imgSTR;
}

string histogramValuesToString(const vector<int> values){
    ostringstream result;

    result << "[";
    for(int i=0; i<=255; i++){
        if(i != 255){
            result << values[i] << ",";
        }else{
            result << values[i] << "]";
        }
    }
    return result.str();
}


vector<vector<pixel>> arithmeticOperation(vector<vector<pixel>>& imgMatrix, int value, char op){

    for(size_t i = 0; i < imgMatrix.size(); i++){
        for(size_t j = 0; j < imgMatrix[i].size(); j++){
            switch (op) {
            case '+':
                imgMatrix[i][j].addValue(value);
                break;
            case '-':
                imgMatrix[i][j].subtValue(value);
                break;
            case '*':
                imgMatrix[i][j].multValue(value);
                break;
            case '/':
                imgMatrix[i][j].divValue(value);
                break;
            }
            
        }
    }

    return imgMatrix;
}

vector<vector<pixel>> convertTo8Bit(vector<vector<pixel>>& imgMatrix){

    for(size_t i = 0; i < imgMatrix.size(); i++){
        for(size_t j = 0; j < imgMatrix[i].size(); j++){
            imgMatrix[i][j].rgbTo8Bit();
        }
    }

    return imgMatrix;
}

vector<vector<pixel>> convertTo1Bit(vector<vector<pixel>>& imgMatrix, int threshold){
    for(size_t i = 0; i < imgMatrix.size(); i++){
        for(size_t j = 0; j < imgMatrix[i].size(); j++){
            imgMatrix[i][j].rgbTo8Bit();
            imgMatrix[i][j].rgbTo1Bit(threshold);
        }
    }

    return imgMatrix;
}


vector<vector<pixel>> invertImage(vector<vector<pixel>>& imgMatrix, char op){
    for(size_t i = 0; i < imgMatrix.size(); i++){
        for(size_t j = 0; j < imgMatrix[i].size()/2; j++){
            if (op == 'h'){
                swap(imgMatrix[i][j], imgMatrix[i][(imgMatrix[i].size()- 1 - j)]);
            }else if(op == 'v'){
                swap(imgMatrix[j][i], imgMatrix[imgMatrix.size()- 1 - j][i]);
            }
        }
    }

    return imgMatrix;
}

vector<vector<pixel>> addImages(vector<vector<pixel>>& imgA, vector<vector<pixel>>& imgB){
    vector<vector<pixel>> imgResult(imgA.size(), vector<pixel>(imgA[0].size()));
    
    for(size_t i = 0; i < imgA.size(); i++){
        for(size_t j = 0; j < imgA[i].size(); j++){
           imgResult[i][j].addPixel(imgA[i][j].px, imgB[i][j].px);
        }

    }

    return imgResult;
}

vector<vector<pixel>> subtImages(vector<vector<pixel>>& imgA, vector<vector<pixel>>& imgB){
    vector<vector<pixel>> imgResult(imgA.size(), vector<pixel>(imgA[0].size()));
    
    for(size_t i = 0; i < imgA.size(); i++){
        for(size_t j = 0; j < imgA[i].size(); j++){
           imgResult[i][j].subtPixel(imgA[i][j].px, imgB[i][j].px);
        }

    }

    return imgResult;
}

vector<vector<pixel>> diferenceWith2Images(vector<vector<pixel>>& imgA, vector<vector<pixel>>& imgB){
    vector<vector<pixel>> imgResult(imgA.size(), vector<pixel>(imgA[0].size()));
    
    for(size_t i = 0; i < imgA.size(); i++){
        for(size_t j = 0; j < imgA[i].size(); j++){
            pixel pxC, pxD;
            pxC.subtPixel(imgA[i][j].px, imgB[i][j].px);
            pxD.subtPixel(imgB[i][j].px, imgA[i][j].px);
            imgResult[i][j].addPixel(pxC.px, pxD.px);
        }

    }

    return imgResult;

}

vector<vector<pixel>> logicWith2Image(vector<vector<pixel>>& imgA, vector<vector<pixel>>& imgB, char op){
    vector<vector<pixel>> imgResult(imgA.size(), vector<pixel>(imgA[0].size()));
    
    for(size_t i = 0; i < imgA.size(); i++){
        for(size_t j = 0; j < imgA[i].size(); j++){
            switch (op){
                case 'a':
                    imgResult[i][j].logicAND(imgA[i][j].px, imgB[i][j]);
                    break;
                case 'o':
                    imgResult[i][j].logicOR(imgA[i][j].px, imgB[i][j]);
                    break;
                case 'x':
                    imgResult[i][j].logicXOR(imgA[i][j].px, imgB[i][j]);
                    break;
            }
            
        }

    }

    return imgResult;

}



vector<vector<pixel>> logicNOT(vector<vector<pixel>>& img){
    vector<vector<pixel>> imgResult(img.size(), vector<pixel>(img[0].size()));
    
    for(size_t i = 0; i < img.size(); i++){
        for(size_t j = 0; j < img[i].size(); j++){
            imgResult[i][j].logicNOT(img[i][j].px);         
        }

    }

    return imgResult;

}

vector<vector<pixel>> blending(vector<vector<pixel>>& imgA, vector<vector<pixel>>& imgB, float value){
    vector<vector<pixel>> imgResult(imgA.size(), vector<pixel>(imgA[0].size()));
    
    for(size_t i = 0; i < imgA.size(); i++){
        for(size_t j = 0; j < imgA[i].size(); j++){
           imgResult[i][j].blending(imgA[i][j].px, imgB[i][j].px, value);
        }

    }

    return imgResult;
}

vector<vector<pixel>> linearAverage(vector<vector<pixel>>& imgA, vector<vector<pixel>>& imgB){
    vector<vector<pixel>> imgResult(imgA.size(), vector<pixel>(imgA[0].size()));
    
    for(size_t i = 0; i < imgA.size(); i++){
        for(size_t j = 0; j < imgA[i].size(); j++){
           imgResult[i][j].linearAverage(imgA[i][j].px, imgB[i][j].px);
        }

    }

    return imgResult;
}

vector<int>getHistogram(vector<vector<pixel>>& img){
    vector<int> histogram(256,0);
    for(size_t i = 0; i < img.size(); i++){
        for(size_t j = 0; j < img[i].size(); j++){
            img[i][j].rgbTo8Bit();
            histogram[(int) img[i][j].px.red]++;
        }
    }

    return histogram;
}

vector<vector<pixel>> equalizeHistogram(vector<vector<pixel>>& img){
    int width = img[0].size();
    int height = img.size();
    vector<int> histogram(256, 0);
    vector<vector<pixel>> imgResult(height, vector<pixel>(width));
    
    for(size_t i = 0; i < img.size(); i++){
        for(size_t j = 0; j < img[i].size(); j++){
            img[i][j].rgbTo8Bit();
            histogram[(int) img[i][j].px.red]++;
        }
    }

    vector<int> cfd(256, 0);
    cfd[0] = histogram[0];
    vector<int> newPixelValue(256, 0);

    for (int i=0; i<=255; i++){
        if(i!=0){
            cfd[i] = histogram[i]+cfd[i-1];
        }
        float tempValue = (static_cast<float>(cfd[i]-cfd[0])/static_cast<float>((width*height)-cfd[0]));
        newPixelValue[i] = tempValue*255;
    }


    for(size_t i = 0; i < img.size(); i++){
        for(size_t j = 0; j < img[i].size(); j++){
            imgResult[i][j].equalizeHistogram(newPixelValue[(int) img[i][j].px.red]);
        }
    }

    return imgResult;
}

vector<vector<pixel>> convolutionMean(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding){

    for(size_t i = padding; i < imgResult.size(); i++){
        for(size_t j = padding; j < imgResult[0].size(); j++){
            imgResult[i-padding][j-padding].convolutionMean(img[i][j], img[i][j-1], img[i][j+1], img[i-1][j], img[i+1][j], img[i-1][j-1], img[i-1][j+1], img[i+1][j-1], img[i+1][j+1]);
        }
    }

    return imgResult;
}

vector<vector<pixel>> convolutionMax(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding){
    for(size_t i = padding; i < imgResult.size(); i++){
        for(size_t j = padding; j < imgResult[0].size(); j++){
            imgResult[i-padding][j-padding].convolutionMax(img[i][j], img[i][j-1], img[i][j+1], img[i-1][j], img[i+1][j], img[i-1][j-1], img[i-1][j+1], img[i+1][j-1], img[i+1][j+1]);
        }
    }

    return imgResult;
}


vector<vector<pixel>> convolutionMin(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding){
    for(size_t i = padding; i < imgResult.size(); i++){
        for(size_t j = padding; j < imgResult[0].size(); j++){
            imgResult[i-padding][j-padding].convolutionMin(img[i][j], img[i][j-1], img[i][j+1], img[i-1][j], img[i+1][j], img[i-1][j-1], img[i-1][j+1], img[i+1][j-1], img[i+1][j+1]);
        }
    }

    return imgResult;
}

vector<vector<pixel>> edgeAdjust(vector<vector<pixel>>& img, int kernel){
    size_t originalRows = img.size();
    size_t originalColums = img[0].size();
    int padding = kernel / 2;
    vector<vector<pixel>> imgResult(originalRows + kernel, vector<pixel>(originalColums + kernel));
    pixel pxl;
    pxl.px.alpha = 255;

    for(size_t i = 0; i < imgResult.size(); i++){
        for(size_t j = 0; j < imgResult[0].size(); j++){
            if (i < padding) { // Borda superior
                if (j < padding) { // Canto superior esquerdo
                    pxl = img[0][0];
                } else if (j >= originalColums + padding) { // Canto superior direito
                    pxl = img[0][originalColums - 1];
                } else { // Borda superior (meio)
                    pxl = img[0][j - padding];
                }
            } else if (i >= originalRows + padding) { // Borda inferior
                if (j < padding) { // Canto inferior esquerdo
                    pxl = img[originalRows - 1][0];
                } else if (j >= originalColums + padding) { // Canto inferior direito
                    pxl = img[originalRows - 1][originalColums - 1];
                } else { // Borda inferior (meio)
                    pxl = img[originalRows - 1][j - padding];
                }
            } else if (j < padding) { // Borda esquerda (sem ser os cantos)
                pxl = img[i - padding][0];
            } else if (j >= originalColums + padding) { // Borda direita (sem ser os cantos)
                pxl = img[i - padding][originalColums - 1];
            } else { // Região central (imagem original)
                pxl = img[i - padding][j - padding];
            }
            imgResult[i][j].setPixel(pxl);
        }
    }

    return imgResult;
}


bool validOperationWith2Images (vector<vector<pixel>>& imgA, vector<vector<pixel>>& imgB){

    if(imgA.empty() || imgB.empty()){
        return false;
    }
    else if((imgA.size() != imgB.size()) || (imgA[0].size() !=  imgB[0].size())){
        return false;
    }

    return true;
}

int main() {
   httplib::Server svr;

   svr.set_default_headers({
   {"Access-Control-Allow-Origin", "*"} // Permite todas as origens
   });

  svr.Post("/process/add", [](const httplib::Request& req, httplib::Response& res){
    if(!req.has_param("value")){
        res.set_content(req.body, "application/json");
        return;
    }
    int value = stoi(req.get_param_value("value"));

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    img = arithmeticOperation(img, value, '+');

    string responseIMG = imgToString(img, false);

    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/process/subt", [](const httplib::Request& req, httplib::Response& res){
    if(!req.has_param("value")){
        res.set_content(req.body, "application/json");
        return;
    }
    int value = stoi(req.get_param_value("value"));

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    img = arithmeticOperation(img, value, '-');

    string responseIMG = imgToString(img, false);

    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/process/mult", [](const httplib::Request& req, httplib::Response& res){
    if(!req.has_param("value")){
        res.set_content(req.body, "application/json");
        return;
    }
    int value = stoi(req.get_param_value("value"));

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    img = arithmeticOperation(img, value, '*');

    string responseIMG = imgToString(img, false);

    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/process/div", [](const httplib::Request& req, httplib::Response& res){
    if(!req.has_param("value")){
        res.set_content(req.body, "application/json");
        return;
    }
    int value = stoi(req.get_param_value("value"));

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    img = arithmeticOperation(img, value, '/');

    string responseIMG = imgToString(img, false);

    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/convert/8bit", [](const httplib::Request& req, httplib::Response& res){

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    img = convertTo8Bit(img);

    string responseIMG = imgToString(img, true);

    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/convert/1bit", [](const httplib::Request& req, httplib::Response& res){
    if(!req.has_param("value")){
        res.set_content(req.body, "application/json");
        return;
    }

    int value = stoi(req.get_param_value("value"));

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    img = convertTo1Bit(img, value);

    string responseIMG = imgToString(img, true);


    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/invert/horizontal", [](const httplib::Request& req, httplib::Response& res){

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    img = invertImage(img, 'h');

    string responseIMG = imgToString(img, false);

    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/invert/vertical", [](const httplib::Request& req, httplib::Response& res){

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    img = invertImage(img, 'v');

    string responseIMG = imgToString(img, false);

    res.set_content(responseIMG, "application/json");
  });



  svr.Post("/add", [](const httplib::Request& req, httplib::Response& res){

    string body = req.body;
    vector<string> jsonIMG = separeJsonImages(body);
    vector<vector<pixel>> imgA = parse_json_pixels(jsonIMG[0]);
    vector<vector<pixel>> imgB = parse_json_pixels(jsonIMG[1]);
    vector<vector<pixel>> result = addImages(imgA, imgB);

    string responseIMG = imgToString(result, false);

    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/subt", [](const httplib::Request& req, httplib::Response& res){

    string body = req.body;
    vector<string> jsonIMG = separeJsonImages(body);
    vector<vector<pixel>> imgA = parse_json_pixels(jsonIMG[0]);
    vector<vector<pixel>> imgB = parse_json_pixels(jsonIMG[1]);
    
    if(validOperationWith2Images(imgA, imgB)){
        vector<vector<pixel>> result = subtImages(imgA, imgB);

        string responseIMG = imgToString(result, false);
    
        res.set_content(responseIMG, "application/json");
    }
    else{
        res.set_content("Images are different sizes", "application/json");
    }
  });

  svr.Post("/diference", [](const httplib::Request& req, httplib::Response& res){

    string body = req.body;
    vector<string> jsonIMG = separeJsonImages(body);
    vector<vector<pixel>> imgA = parse_json_pixels(jsonIMG[0]);
    vector<vector<pixel>> imgB = parse_json_pixels(jsonIMG[1]);

    if(validOperationWith2Images(imgA, imgB)){
        vector<vector<pixel>> result = diferenceWith2Images(imgA, imgB);

        string responseIMG = imgToString(result, false);
    
        res.set_content(responseIMG, "application/json");    
    }else {
        res.set_content("Images are different sizes", "application/json");
    }
  });

  svr.Post("/logic/and", [](const httplib::Request& req, httplib::Response& res){

    string body = req.body;
    vector<string> jsonIMG = separeJsonImages(body);
    vector<vector<pixel>> imgA = parse_json_pixels(jsonIMG[0]);
    vector<vector<pixel>> imgB = parse_json_pixels(jsonIMG[1]);

    if(validOperationWith2Images(imgA, imgB)){

        // Convertendo para escala de cinza
        imgA = convertTo8Bit(imgA);
        imgB = convertTo8Bit(imgB);

        // Binarizando
        imgA = convertTo1Bit(imgA, 128);
        imgB = convertTo1Bit(imgB, 128);

        vector<vector<pixel>> result = logicWith2Image(imgA, imgB, 'a');

        string responseIMG = imgToString(result, true);
    
        res.set_content(responseIMG, "application/json");    
    }else {
        res.set_content("Images are different sizes", "application/json");
    }
  });

  svr.Post("/logic/or", [](const httplib::Request& req, httplib::Response& res){

    string body = req.body;
    vector<string> jsonIMG = separeJsonImages(body);
    vector<vector<pixel>> imgA = parse_json_pixels(jsonIMG[0]);
    vector<vector<pixel>> imgB = parse_json_pixels(jsonIMG[1]);

    if(validOperationWith2Images(imgA, imgB)){

        // Convertendo para escala de cinza
        imgA = convertTo8Bit(imgA);
        imgB = convertTo8Bit(imgB);

        // Binarizando
        imgA = convertTo1Bit(imgA, 128);
        imgB = convertTo1Bit(imgB, 128);

        vector<vector<pixel>> result = logicWith2Image(imgA, imgB, 'o');

        string responseIMG = imgToString(result, true);
    
        res.set_content(responseIMG, "application/json");    
    }else {
        res.set_content("Images are different sizes", "application/json");
    }
  });


  svr.Post("/logic/xor", [](const httplib::Request& req, httplib::Response& res){

    string body = req.body;
    vector<string> jsonIMG = separeJsonImages(body);
    vector<vector<pixel>> imgA = parse_json_pixels(jsonIMG[0]);
    vector<vector<pixel>> imgB = parse_json_pixels(jsonIMG[1]);

    if(validOperationWith2Images(imgA, imgB)){

        // Convertendo para escala de cinza
        imgA = convertTo8Bit(imgA);
        imgB = convertTo8Bit(imgB);

        // Binarizando
        imgA = convertTo1Bit(imgA, 128);
        imgB = convertTo1Bit(imgB, 128);

        vector<vector<pixel>> result = logicWith2Image(imgA, imgB, 'x');

        string responseIMG = imgToString(result, true);
    
        res.set_content(responseIMG, "application/json");    
    }else {
        res.set_content("Images are different sizes", "application/json");
    }
  });

  svr.Post("/logic/not", [](const httplib::Request& req, httplib::Response& res){

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);

    // Convertendo para escala de cinza
    img = convertTo8Bit(img);

    // Binarizando
    img = convertTo1Bit(img, 128);

    vector<vector<pixel>> result = logicNOT(img);

    string responseIMG = imgToString(result, true);

    res.set_content(responseIMG, "application/json");    
  });

  svr.Post("/linear/blending", [](const httplib::Request& req, httplib::Response& res){
    if(!req.has_param("value")){
        res.set_content(req.body, "application/json");
        return;
    }

    string body = req.body;

    float value = (stoi(req.get_param_value("value")))/100.00;

    vector<string> jsonIMG = separeJsonImages(body);

    vector<vector<pixel>> imgA = parse_json_pixels(jsonIMG[0]);
    vector<vector<pixel>> imgB = parse_json_pixels(jsonIMG[1]);

    vector<vector<pixel>> result = blending(imgA, imgB, value);

    string responseIMG = imgToString(result, false);

    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/linear/average", [](const httplib::Request& req, httplib::Response& res){

    string body = req.body;

    vector<string> jsonIMG = separeJsonImages(body);

    vector<vector<pixel>> imgA = parse_json_pixels(jsonIMG[0]);
    vector<vector<pixel>> imgB = parse_json_pixels(jsonIMG[1]);

    vector<vector<pixel>> result = linearAverage(imgA, imgB);

    string responseIMG = imgToString(result, false);

    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/histogram", [](const httplib::Request& req, httplib::Response& res){

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);

    vector<int> result = getHistogram(img);

    string response = histogramValuesToString(result);

    res.set_content(response, "application/json");
  });

  svr.Post("/histogram/equalize", [](const httplib::Request& req, httplib::Response& res){

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);

    vector<vector<pixel>> result = equalizeHistogram(img);
    vector<int> histogram = getHistogram(result);

    string responseIMG = imgToString(result, true);
    string responseHistogram = histogramValuesToString(histogram);


    ostringstream json;
    json   << "{\n \"image\": " << responseIMG 
           << ",\n \"histogram\": " << responseHistogram << "\n}";

    res.set_content(json.str(), "application/json");
  });

  svr.Post("/convolutional/mean", [](const httplib::Request& req, httplib::Response& res){

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);

    vector<vector<pixel>> result = edgeAdjust(img, 3);
    img = convolutionMean(result, img, 3/2);

    string responseIMG = imgToString(img, false);


    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/convolutional/max", [](const httplib::Request& req, httplib::Response& res){

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);

    vector<vector<pixel>> result = edgeAdjust(img, 3);
    img = convolutionMax(result, img, 3/2);

    string responseIMG = imgToString(img, false);


    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/convolutional/min", [](const httplib::Request& req, httplib::Response& res){

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);

    vector<vector<pixel>> result = edgeAdjust(img, 3);
    img = convolutionMin(result, img, 3/2);

    string responseIMG = imgToString(img, false);


    res.set_content(responseIMG, "application/json");
  });

   svr.listen("localhost", 8080);
   return 0;
}