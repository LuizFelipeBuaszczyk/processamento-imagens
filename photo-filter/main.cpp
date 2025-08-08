#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdint>
#include "httplib.h"
#include <vector> 
#include <stack>
#include <algorithm>
#include <cmath>

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
        px.green = pixelValue;
        px.blue = pixelValue;
        px.alpha = 255;
    }


    void rgbTo8Bit(){
        px.red = (px.red + px.green + px.blue)/3;
    }

    void rgbTo1Bit(int threshold){
        px.red >= threshold ? px.red = 255 : px.red = 0; 
    }

    //void convolutionMean(const pixel& pxTarget, const pixel& pxLeft, const pixel& pxRight, const pixel& pxUp, const pixel& pxDown, const pixel& pxNW, const pixel& pxNE, const pixel& pxSW, const pixel& pxSE){
    void convolutionMean(vector<vector<pixel>>& img, int padding, int targetY, int targetX, int pixelAmount)  { 
        int valueRed, valueGreen, valueBlue;
        valueRed = valueGreen = valueBlue = 0;

        for(int i = targetY-padding; i <= targetY+padding; i++){
            for (int j = targetX-padding; j <= targetX+padding; j++){
                valueRed += img[i][j].px.red;
                valueGreen += img[i][j].px.green;
                valueBlue += img[i][j].px.blue;
            }
        }
        
        px.red = validPixelValue(valueRed/pixelAmount);

        px.green = validPixelValue(valueGreen/pixelAmount);

        px.blue = validPixelValue(valueBlue/pixelAmount);

        px.alpha = 255;    
    }

    void convolutionMax(vector<vector<pixel>>& img, int padding, int targetY, int targetX){
        uint8_t valueRed, valueGreen, valueBlue;
        valueRed = valueGreen = valueBlue = 0;

        for(int i = targetY-padding; i <= targetY+padding; i++){
            for (int j = targetX-padding; j <= targetX+padding; j++){
                
                if (valueRed < img[i][j].px.red){
                    valueRed = img[i][j].px.red;
                }

                if (valueGreen < img[i][j].px.green){
                    valueGreen = img[i][j].px.green;
                }

                if (valueBlue < img[i][j].px.blue){
                    valueBlue = img[i][j].px.blue;
                }
            }
        }

        px.red = valueRed;
        px.green = valueGreen;
        px.blue = valueBlue;
        px.alpha = 255;

    }

    void convolutionMin(vector<vector<pixel>>& img, int padding, int targetY, int targetX){
        uint8_t valueRed, valueGreen, valueBlue;
        valueRed = valueGreen = valueBlue = 255;


        for(int i = targetY-padding; i <= targetY+padding; i++){
            for (int j = targetX-padding; j <= targetX+padding; j++){
                
                if (valueRed > img[i][j].px.red){
                    valueRed = img[i][j].px.red;
                }

                if (valueGreen > img[i][j].px.green){
                    valueGreen = img[i][j].px.green;
                }

                if (valueBlue > img[i][j].px.blue){
                    valueBlue = img[i][j].px.blue;
                }
            }
        }

        px.red = valueRed;
        px.green = valueGreen;
        px.blue = valueBlue;
        px.alpha = 255;
    }

    void convolutionConservativeSmoothing(vector<vector<pixel>>& img, int padding, int targetY, int targetX){
        vector<uint8_t> valuesRed;
        vector<uint8_t> valuesGreen;
        vector<uint8_t> valuesBlue;
        pixel targetPixel = img[targetY][targetX];

        for(int i = targetY-padding; i <= targetY+padding; i++){
            for (int j = targetX-padding; j <= targetX+padding; j++){
                if (!((i == targetY) && (j == targetX))){
                    valuesRed.push_back(img[i][j].px.red);
                    valuesGreen.push_back(img[i][j].px.green);
                    valuesBlue.push_back(img[i][j].px.blue);
                }
            }
        }

        uint8_t maxRed, maxGreen, maxBlue;
        uint8_t minRed, minGreen, minBlue; 

        sort(valuesRed.begin(), valuesRed.end());
        minRed = valuesRed[0];
        maxRed = valuesRed[valuesRed.size() - 1];

        sort(valuesGreen.begin(), valuesGreen.end());
        minGreen = valuesGreen[0];
        maxGreen = valuesGreen[valuesGreen.size() - 1];

        sort(valuesBlue.begin(), valuesBlue.end());
        minBlue = valuesBlue[0];
        maxBlue = valuesBlue[valuesBlue.size() - 1];
       
        px.red = targetPixel.px.red;
        if (px.red > maxRed){
            px.red = maxRed;
        }
        else {
            if (px.red < minRed){
                px.red = minRed;
            }
        }

        // Green
        px.green = targetPixel.px.green;
        if (px.green > maxGreen){
            px.green = maxGreen;
        }
        else {
            if (px.green < minGreen){
                px.green = minGreen;
            }
        }

        // Blue
        px.blue = targetPixel.px.blue;
        if (px.blue > maxBlue){
            px.blue = maxBlue;
        }
        else {
            if (px.blue < minBlue){
                px.blue = minBlue;
            }
        }

        px.alpha = 255;
    }

    void convolutionOrder(vector<vector<pixel>>& img, int padding, int targetY, int targetX, int selectedValue){
        vector<uint8_t> valuesRed;
        vector<uint8_t> valuesGreen;
        vector<uint8_t> valuesBlue;


        for(int i = targetY-padding; i <= targetY+padding; i++){
            for (int j = targetX-padding; j <= targetX+padding; j++){
                valuesRed.push_back(img[i][j].px.red);
                valuesGreen.push_back(img[i][j].px.green);
                valuesBlue.push_back(img[i][j].px.blue);
            }
        }

        // Ordenação -- MUUUITO LENTA
        sort(valuesRed.begin(), valuesRed.end());
        sort(valuesGreen.begin(), valuesGreen.end());
        sort(valuesBlue.begin(), valuesBlue.end());
       
        px.red = valuesRed[selectedValue];
        px.green = valuesGreen[selectedValue];
        px.blue = valuesBlue[selectedValue];
        px.alpha = 255;
    }


    void convolutionMedian(vector<vector<pixel>>& img, int padding, int targetY, int targetX){
        vector<uint8_t> valuesRed;
        vector<uint8_t> valuesGreen;
        vector<uint8_t> valuesBlue;


        for(int i = targetY-padding; i <= targetY+padding; i++){
            for (int j = targetX-padding; j <= targetX+padding; j++){
                valuesRed.push_back(img[i][j].px.red);
                valuesGreen.push_back(img[i][j].px.green);
                valuesBlue.push_back(img[i][j].px.blue);
            }
        }

        sort(valuesRed.begin(), valuesRed.end());
        sort(valuesGreen.begin(), valuesGreen.end());
        sort(valuesBlue.begin(), valuesBlue.end());

        px.red = valuesRed[valuesRed.size()/2];
        px.green = valuesGreen[valuesGreen.size()/2];
        px.blue = valuesBlue[valuesBlue.size()/2];
        px.alpha = 255;
    }

    void gaussian(vector<vector<pixel>>& img, int padding, int targetY, int targetX, vector<vector<float>>& gaussianKernel){   

        float total = 0;
        float sumRed = 0;
        float sumGreen = 0;
        float sumBlue = 0;
        int totalRed ;
        int totalGreen;
        vector<uint8_t> totalBlue;


        for(int i = targetY-padding, y=0; i <= targetY+padding; i++, y++){
            for (int j = targetX-padding, x=0; j <= targetX+padding; j++, x++){

                sumRed      +=  (img[i][j].px.red   * gaussianKernel[y][x]);
                sumGreen    +=  (img[i][j].px.green * gaussianKernel[y][x]);
                sumBlue     +=  (img[i][j].px.blue  * gaussianKernel[y][x]);
            } 
        }


        int value = sumRed;
        px.red = validPixelValue(value);

        value = sumGreen;
        px.green = validPixelValue(value);

        value = sumBlue;
        px.blue = validPixelValue(value);

        px.alpha = 255;

        // Imprimir o kernel
    }

    void SetPixelRed(int value){
        uint8_t pxValue = validPixelValue(value);
        px.red = pxValue;
    }

    void prewitBorderDetection(vector<vector<pixel>>& img, int padding, int targetY, int targetX){
        float valueX, valueY; 
        int xDerivate[3][3], yDerivate[3][3];

        xDerivate[0][0] = -1; xDerivate[0][1] = 0; xDerivate[0][2] = 1;
        xDerivate[1][0] = -1; xDerivate[1][1] = 0; xDerivate[1][2] = 1;
        xDerivate[2][0] = -1; xDerivate[2][1] = 0; xDerivate[2][2] = 1;

        yDerivate[0][0] = -1; yDerivate[0][1] = -1; yDerivate[0][2] = -1;
        yDerivate[1][0] =  0; yDerivate[1][1] =  0; yDerivate[1][2] =  0;
        yDerivate[2][0] =  1; yDerivate[2][1] = 1; yDerivate[2][2] = 1;   

        valueX = valueY = 0;

        for(int i = targetY-padding, y=0; i <= targetY+padding; i++, y++){
            for (int j = targetX-padding, x=0; j <= targetX+padding; j++, x++){
                valueX += (img[i][j].px.red * yDerivate[y][x]);
                valueY += (img[i][j].px.red * xDerivate[y][x]);                
            } 
        }

        int value = pow(((valueX*valueX) + (valueY*valueY)), 0.5);

        px.red = validPixelValue(value);
    }

    void sobelBorderDetection(vector<vector<pixel>>& img, int padding, int targetY, int targetX){
        float valueX, valueY; 
        int xDerivate[3][3], yDerivate[3][3];

        xDerivate[0][0] = 1; xDerivate[0][1] = 0; xDerivate[0][2] = -1;
        xDerivate[1][0] = 2; xDerivate[1][1] = 0; xDerivate[1][2] = -2;
        xDerivate[2][0] = 1; xDerivate[2][1] = 0; xDerivate[2][2] = -1;

        yDerivate[0][0] = 1; yDerivate[0][1] = 2; yDerivate[0][2] = 1;
        yDerivate[1][0] =  0; yDerivate[1][1] =  0; yDerivate[1][2] =  0;
        yDerivate[2][0] =  -1; yDerivate[2][1] =  -2; yDerivate[2][2] =  -1;   

        valueX = valueY = 0;

        for(int i = targetY-padding, y=0; i <= targetY+padding; i++, y++){
            for (int j = targetX-padding, x=0; j <= targetX+padding; j++, x++){
                valueX += (img[i][j].px.red * yDerivate[y][x]);
                valueY += (img[i][j].px.red * xDerivate[y][x]);                
            } 
        }

        int value = pow(((valueX*valueX) + (valueY*valueY)), 0.5);

        px.red = validPixelValue(value);
    }


    void laplacianBorderDetection(vector<vector<pixel>>& img, int padding, int targetY, int targetX){
        int Derivate[3][3];
        int value = 0;

        Derivate[0][0] = -1; Derivate[0][1] = -1; Derivate[0][2] = -1;
        Derivate[1][0] = -1; Derivate[1][1] = 8; Derivate[1][2] = -1;
        Derivate[2][0] = -1; Derivate[2][1] = -1; Derivate[2][2] = -1;



        for(int i = targetY-padding, y=0; i <= targetY+padding; i++, y++){
            for (int j = targetX-padding, x=0; j <= targetX+padding; j++, x++){
                value += (img[i][j].px.red * Derivate[y][x]);
            } 
        }

        px.red = validPixelValue(value);
    }

    void morphoDilation(vector<vector<pixel>>& img, int padding, int targetY, int targetX, int type){
        px.red = 0;
        switch (type){
            case 1: // Square
                for(int i = targetY-padding; i <= targetY+padding; i++){
                    for (int j = targetX-padding; j <= targetX+padding; j++){
                        if (img[i][j].px.red == 255){
                            px.red = 255;
                            px.alpha = 255;
                            break;
                        }
                    } 
                }
                break;
            case 2: { // Diamond 
                bool pixelFound = false;

                for (int i = targetY - padding; i <= targetY + padding; i++) {
                    for (int j = targetX - padding; j <= targetX + padding; j++) {
                        if (abs(i - targetY) + abs(j - targetX) <= padding) {
                            // Está dentro do elemento estruturante em forma de diamante
                            if (img[i][j].px.red == 255) {
                                px.red = 255;
                                px.alpha = 255;
                                pixelFound = true;
                                break;
                            }
                        }
                    }
                    if (pixelFound) break;
                }
            }
            case 3: // Line X
                for(int i = targetX-padding; i <= targetX+padding; i++){
                    if (img[targetY][i].px.red == 255){
                        px.red = 255;
                        px.alpha = 255;
                        break;
                    }
                }
                break;
            case 4: // Line Y
                for(int i = targetY-padding; i <= targetY+padding; i++){
                    if (img[i][targetX].px.red == 255){
                        px.red = 255;
                        px.alpha = 255;
                        break;
                    }
                }
                break;
        }
    }

    void morphoErosion(vector<vector<pixel>>& img, int padding, int targetY, int targetX, int type){
        px.red = 255;
        px.alpha = 255;

        switch (type){
            case 1: // Square
                for(int i = targetY-padding; i <= targetY+padding; i++){
                    for (int j = targetX-padding; j <= targetX+padding; j++){
                        if (img[i][j].px.red != 255){
                            px.red = 0;
                            px.alpha = 255;
                            break;
                        }
                    } 
                }
                break;
            case 2: {// Diamond
                bool pixelFound = false;

                for (int i = targetY - padding; i <= targetY + padding; i++) {
                    for (int j = targetX - padding; j <= targetX + padding; j++) {
                        if (abs(i - targetY) + abs(j - targetX) <= padding) {
                            // Está dentro do elemento estruturante em forma de diamante
                            if (img[i][j].px.red != 255) {
                                px.red = 0;
                                px.alpha = 0;
                                pixelFound = true;
                                break;
                            }
                        }
                    }
                    if (pixelFound) break;
                }
            }   
            case 3: // Line X
                for(int i = targetX-padding; i <= targetX+padding; i++){
                    if (img[targetY][i].px.red != 255){
                        px.red = 0;
                        px.alpha = 255;
                        break;
                    }
                }
                break;
            case 4: // Line Y
                for(int i = targetY-padding; i <= targetY+padding; i++){
                    if (img[i][targetX].px.red != 255){
                        px.red = 0;
                        px.alpha = 255;
                        break;
                    }
                }
                break;
        }
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
        
        float upper = cfd[i] - cfd[0] > 0 ? cfd[i] - cfd[0] : 1;
        float under = ((width*height)-cfd[0]);

        float tempValue = upper/under;
        newPixelValue[i] = tempValue * 255;

    }


    for(size_t i = 0; i < img.size(); i++){
        for(size_t j = 0; j < img[i].size(); j++){
            imgResult[i][j].equalizeHistogram(newPixelValue[(int) img[i][j].px.red]);
        }
    }

    return imgResult;
}

vector<vector<pixel>> convolutionMean(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding, int kernel){

    int area = kernel*kernel;

    for(size_t i = padding; i < imgResult.size() + padding; i++){
        for(size_t j = padding; j < imgResult[0].size() + padding; j++){
            imgResult[i-padding][j-padding].convolutionMean(img, padding, i, j, area);
        }
    }

    return imgResult;
}

vector<vector<pixel>> convolutionMax(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding){
    
    
    for(size_t i = padding; i < imgResult.size() + padding; i++){
        for(size_t j = padding; j < imgResult[0].size() + padding; j++){
            imgResult[i-padding][j-padding].convolutionMax(img, padding, i, j);
        }
    }

    return imgResult;
}


vector<vector<pixel>> convolutionMin(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding){
    
    for(size_t i = padding; i < imgResult.size() + padding; i++){
        for(size_t j = padding; j < imgResult[0].size() + padding; j++){
            imgResult[i-padding][j-padding].convolutionMin(img, padding, i, j);
        }
    }

    return imgResult;
}

vector<vector<pixel>> convolutionMedian(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding){

    for(size_t i = padding; i < imgResult.size() + padding; i++){
        for(size_t j = padding; j < imgResult[0].size() + padding; j++){
            imgResult[i-padding][j-padding].convolutionMedian(img, padding, i, j);
        }
    }

    return imgResult;
}

vector<vector<pixel>> convolutionOrder(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding, int selectedValue){

    for(size_t i = padding; i < imgResult.size() + padding; i++){
        for(size_t j = padding; j < imgResult[0].size() + padding; j++){
            imgResult[i-padding][j-padding].convolutionOrder(img, padding, i, j, selectedValue);
        }
    }

    return imgResult;
}

vector<vector<pixel>> convolutionConservativeSmoothing(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding){

    for(size_t i = padding; i < imgResult.size() + padding; i++){
        for(size_t j = padding; j < imgResult[0].size() + padding; j++){
            imgResult[i-padding][j-padding].convolutionConservativeSmoothing(img, padding, i, j);
        }
    }

    return imgResult;
}

vector<vector<pixel>> prewitBorderDetection(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding){

    for(size_t i = padding; i < imgResult.size() + padding; i++){
        for(size_t j = padding; j < imgResult[0].size() + padding; j++){
            imgResult[i-padding][j-padding].prewitBorderDetection(img, padding, i, j);
        }
    }

    return imgResult;
}

vector<vector<pixel>> sobelBorderDetection(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding){

    for(size_t i = padding; i < imgResult.size() + padding; i++){
        for(size_t j = padding; j < imgResult[0].size() + padding; j++){
            imgResult[i-padding][j-padding].sobelBorderDetection(img, padding, i, j);
        }
    }

    return imgResult;
}


vector<vector<pixel>> laplacianBorderDetection(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding){

    for(size_t i = padding; i < imgResult.size() + padding; i++){
        for(size_t j = padding; j < imgResult[0].size() + padding; j++){
            imgResult[i-padding][j-padding].laplacianBorderDetection(img, padding, i, j);
        }
    }

    return imgResult;
}

vector<vector<pixel>> morphoDilation(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding, int type){

    for(size_t i = padding; i < imgResult.size() + padding; i++){
        for(size_t j = padding; j < imgResult[0].size() + padding; j++){
            imgResult[i-padding][j-padding].morphoDilation(img, padding, i, j, type);
        }
    }

    return imgResult;
}

vector<vector<pixel>> morphoErosion(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding, int type){

    for(size_t i = padding; i < imgResult.size() + padding; i++){
        for(size_t j = padding; j < imgResult[0].size() + padding; j++){
            imgResult[i-padding][j-padding].morphoErosion(img, padding, i, j, type);
        }
    }

    return imgResult;
}

vector<vector<pixel>> morphoOpening(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding, int type){
    vector<vector<pixel>> imgErosion = imgResult;

    for(size_t i = padding; i < imgResult.size() + padding; i++){
        for(size_t j = padding; j < imgResult[0].size() + padding; j++){
            imgErosion[i-padding][j-padding].morphoErosion(img, padding, i, j, type);
        }
    }

    for(size_t i = padding; i < imgResult.size() + padding; i++){
        for(size_t j = padding; j < imgResult[0].size() + padding; j++){
            imgResult[i-padding][j-padding].morphoDilation(imgErosion, padding, i, j, type);
        }
    }

    return imgResult;
}

vector<vector<pixel>> morphoClosing(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding, int type){
    vector<vector<pixel>> imgDilation = imgResult;

    for(size_t i = padding; i < imgResult.size() + padding; i++){
        for(size_t j = padding; j < imgResult[0].size() + padding; j++){
            imgDilation[i-padding][j-padding].morphoDilation(img, padding, i, j, type);
        }
    }

    for(size_t i = padding; i < imgResult.size() + padding; i++){
        for(size_t j = padding; j < imgResult[0].size() + padding; j++){
            imgResult[i-padding][j-padding].morphoDilation(imgDilation, padding, i, j, type);
        }
    }

    return imgResult;
}

vector<vector<pixel>> morphoOutline(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding, int type){
    vector<vector<pixel>> imgErosion = imgResult;

    for(size_t i = padding; i < imgResult.size() + padding; i++){
        for(size_t j = padding; j < imgResult[0].size() + padding; j++){
            imgErosion[i-padding][j-padding].morphoErosion(img, padding, i, j, type);
        }
    }

    return subtImages(imgResult, imgErosion);
        
}

vector<vector<pixel>> convolutionGaussian(vector<vector<pixel>>& img, vector<vector<pixel>>& imgResult, int padding, vector<vector<float>>& gaussianKernel){

    for(size_t i = padding; i < imgResult.size() + padding; i++){
        for(size_t j = padding; j < imgResult[0].size() + padding; j++){
            imgResult[i-padding][j-padding].gaussian(img, padding, i, j, gaussianKernel);
        }
    }

    return imgResult;
}

vector<vector<float>> getGaussianModel(int kernel, float sigma){
    vector<vector<float>> gaussianKernel(kernel);

    for (int i = 0; i < kernel; i++) {
        gaussianKernel[i].resize(kernel); 
    }

    float total =0;
    int center = kernel / 2;

    for(int i = 0; i < kernel; i++){
        for (int j = 0; j < kernel; j++){
            int x = i - center;
            int y = j - center;
            gaussianKernel[i][j] =  (1 / (2 * M_PI * (sigma * sigma))) * (exp(-(x * x + y * y) / (2 * (sigma * sigma))));
            total += gaussianKernel[i][j];
        }
    }


    for(int i = 0; i < kernel; i++){
        for (int j = 0; j < kernel; j++){
            gaussianKernel[i][j] /= total;
        }
    }

    return gaussianKernel;

}

vector<vector<pixel>> transformKernelModelToImg(vector<vector<float>>& kernel, int size){
    vector<vector<pixel>> result;
    pixel pxl;


    for (int i=0; i<size; i++){
        vector<pixel> line;
        for(int j=0; j<size; j++){ 
            int value = kernel[i][j]*255;
            pxl.SetPixelRed(value);
            line.push_back(pxl);
        }
        result.push_back(line);
    }
    return result;
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
    if(!req.has_param("kernel")){
        res.set_content(req.body, "application/json");
        return;
    }

    int kernel = stoi(req.get_param_value("kernel"));

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);

    vector<vector<pixel>> result = edgeAdjust(img, kernel);
    img = convolutionMean(result, img, kernel/2, kernel);

    string responseIMG = imgToString(img, false);


    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/convolutional/max", [](const httplib::Request& req, httplib::Response& res){
    if(!req.has_param("kernel")){
        res.set_content(req.body, "application/json");
        return;
    }

    int kernel = stoi(req.get_param_value("kernel"));

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);

    vector<vector<pixel>> result = edgeAdjust(img, kernel);
    img = convolutionMax(result, img, kernel/2);

    string responseIMG = imgToString(img, false);


    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/convolutional/min", [](const httplib::Request& req, httplib::Response& res){
    if(!req.has_param("kernel")){
        res.set_content(req.body, "application/json");
        return;
    }

    int kernel = stoi(req.get_param_value("kernel"));

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);

    vector<vector<pixel>> result = edgeAdjust(img, kernel);
    img = convolutionMin(result, img, kernel/2);

    string responseIMG = imgToString(img, false);


    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/convolutional/median", [](const httplib::Request& req, httplib::Response& res){
    if(!req.has_param("kernel")){
        res.set_content(req.body, "application/json");
        return;
    }

    int kernel = stoi(req.get_param_value("kernel"));

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);

    vector<vector<pixel>> result = edgeAdjust(img, kernel);
    img = convolutionMedian(result, img, kernel/2);

    string responseIMG = imgToString(img, false);


    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/convolutional/order", [](const httplib::Request& req, httplib::Response& res){
    if(!req.has_param("kernel") || !req.has_param("selectedvalue")){
        res.set_content(req.body, "application/json");
        return;
    }

    int kernel = stoi(req.get_param_value("kernel"));
    int selectedValue = stoi(req.get_param_value("selectedvalue"));
    selectedValue -= 1;

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);

    vector<vector<pixel>> result = edgeAdjust(img, kernel);
    img = convolutionOrder(result, img, kernel/2, selectedValue);

    string responseIMG = imgToString(img, false);


    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/convolutional/conservativeSmoothing", [](const httplib::Request& req, httplib::Response& res){
    if(!req.has_param("kernel")){
        res.set_content(req.body, "application/json");
        return;
    }

    int kernel = stoi(req.get_param_value("kernel"));

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);

    vector<vector<pixel>> result = edgeAdjust(img, kernel);
    img = convolutionConservativeSmoothing(result, img, kernel/2);

    string responseIMG = imgToString(img, false);


    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/convolutional/gaussian", [](const httplib::Request& req, httplib::Response& res){
    if(!req.has_param("kernel") || !req.has_param("sigma")){
        res.set_content(req.body, "application/json");
        return;
    }
    

    int kernel = stoi(req.get_param_value("kernel"));
    float sigma = stof(req.get_param_value("sigma"));

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    vector<vector<float>> gaussianKernel = getGaussianModel(kernel, sigma);

    vector<vector<pixel>> result = edgeAdjust(img, kernel);
    img = convolutionGaussian(result, img, kernel/2, gaussianKernel);

    vector<vector<pixel>> gaussianModelResult = transformKernelModelToImg(gaussianKernel, kernel);


    string responseIMG = imgToString(img, false);
    string responseGaussianModel = imgToString(gaussianModelResult, true);
  
    stringstream json;
    json   << "{\n \"image\": " << responseIMG 
           << ",\n \"kernelModel\": " << responseGaussianModel << "\n}";

    res.set_content(json.str(), "application/json");
  });

  svr.Post("/borderDetection/prewit", [](const httplib::Request& req, httplib::Response& res){

    int kernel = 3;

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    vector<vector<pixel>> result = edgeAdjust(img, kernel);

    // Tratamento
    img = convolutionMedian(result, img, kernel/2);
    result = edgeAdjust(img, kernel);

    result = convertTo8Bit(result);
    img = convertTo8Bit(img);

    img = prewitBorderDetection(result, img, kernel/2);

    string responseIMG = imgToString(img, true);
  
    res.set_content(responseIMG, "application/json");
  });

    svr.Post("/borderDetection/sobel", [](const httplib::Request& req, httplib::Response& res){

    int kernel = 3;

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    vector<vector<pixel>> result = edgeAdjust(img, kernel);

    // Tratamento
    img = convolutionMedian(result, img, kernel/2);
    result = edgeAdjust(img, kernel);

    result = convertTo8Bit(result);
    img = convertTo8Bit(img);

    img = sobelBorderDetection(result, img, kernel/2);

    string responseIMG = imgToString(img, true);
  
    res.set_content(responseIMG, "application/json");
  });

   svr.Post("/borderDetection/laplacian", [](const httplib::Request& req, httplib::Response& res){

    int kernel = 3;

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    vector<vector<pixel>> result = edgeAdjust(img, kernel);

    // Tratamento
    img = convolutionMedian(result, img, kernel/2);
    result = edgeAdjust(img, kernel);

    result = convertTo8Bit(result);
    img = convertTo8Bit(img);

    img = laplacianBorderDetection(result, img, kernel/2);

    string responseIMG = imgToString(img, true);
  
    res.set_content(responseIMG, "application/json");
  });

  /*
    1 - Square
    2 - Diamond
    3 - Line X
    4 - Line Y

  */

  svr.Post("/morphological/dilation", [](const httplib::Request& req, httplib::Response& res){
    if(!req.has_param("kernel") || !req.has_param("type")){
        res.set_content(req.body, "application/json");
        return;
    }

    int kernel = stoi(req.get_param_value("kernel"));
    int type = stoi(req.get_param_value("type"));

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    vector<vector<pixel>> result = edgeAdjust(img, kernel);

    // Tratamento
    result = edgeAdjust(img, kernel);
    result = convertTo8Bit(result);
    result = convertTo1Bit(result, 128);

    img = morphoDilation(result, img, kernel/2, type);

    string responseIMG = imgToString(img, true);
  
    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/morphological/erosion", [](const httplib::Request& req, httplib::Response& res){

    int kernel = stoi(req.get_param_value("kernel"));
    int type = stoi(req.get_param_value("type"));

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    vector<vector<pixel>> result = edgeAdjust(img, kernel);

    // Tratamento
    result = edgeAdjust(img, kernel);
    result = convertTo8Bit(result);
    result = convertTo1Bit(result, 128);

    img = morphoErosion(result, img, kernel/2, type);

    string responseIMG = imgToString(img, true);
  
    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/morphological/opening", [](const httplib::Request& req, httplib::Response& res){

    int kernel = stoi(req.get_param_value("kernel"));
    int type = stoi(req.get_param_value("type"));

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    vector<vector<pixel>> result = edgeAdjust(img, kernel);

    // Tratamento
    result = edgeAdjust(img, kernel);
    result = convertTo8Bit(result);
    result = convertTo1Bit(result, 128);

    img = morphoOpening(result, img, kernel/2, type);

    string responseIMG = imgToString(img, true);
  
    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/morphological/closing", [](const httplib::Request& req, httplib::Response& res){

    int kernel = stoi(req.get_param_value("kernel"));
    int type = stoi(req.get_param_value("type"));

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    vector<vector<pixel>> result = edgeAdjust(img, kernel);

    // Tratamento
    result = edgeAdjust(img, kernel);
    result = convertTo8Bit(result);
    result = convertTo1Bit(result, 128);

    img = morphoClosing(result, img, kernel/2, type);

    string responseIMG = imgToString(img, true);
  
    res.set_content(responseIMG, "application/json");
  });

  svr.Post("/morphological/outline", [](const httplib::Request& req, httplib::Response& res){

    int kernel = stoi(req.get_param_value("kernel"));
    int type = stoi(req.get_param_value("type"));

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    vector<vector<pixel>> result = edgeAdjust(img, kernel);

    // Tratamento
    result = edgeAdjust(img, kernel);
    result = convertTo8Bit(result);
    result = convertTo1Bit(result, 128);

    img = morphoOutline(result, img, kernel/2, type);

    string responseIMG = imgToString(img, true);
  
    res.set_content(responseIMG, "application/json");
  });

   svr.listen("localhost", 8080);
   return 0;
}