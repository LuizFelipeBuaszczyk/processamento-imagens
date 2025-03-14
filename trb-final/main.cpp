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

    void rgbTo8Bit(){
        px.red = (0.2989*px.red) + (0.5870 * px.green) + (0.1140*px.blue);
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

    void rgbTo1Bit(int threshold){
        px.red >= threshold ? px.red = 255 : px.red = 0; 
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

vector<vector<pixel>> addValue(vector<vector<pixel>>& imgMatrix, int value){

    for(size_t i = 0; i < imgMatrix.size(); i++){
        for(size_t j = 0; j < imgMatrix[i].size(); j++){
            imgMatrix[i][j].addValue(value);
        }
    }

    return imgMatrix;
}

vector<vector<pixel>> subtValue(vector<vector<pixel>>& imgMatrix, int value){

    for(size_t i = 0; i < imgMatrix.size(); i++){
        for(size_t j = 0; j < imgMatrix[i].size(); j++){
            imgMatrix[i][j].subtValue(value);
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
            imgMatrix[i][j].rgbTo1Bit(100);
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
    img = addValue(img, value);

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
    img = subtValue(img, value);

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

    string body = req.body;
    vector<vector<pixel>> img = parse_json_pixels(body);
    img = convertTo1Bit(img, 100);

    string responseIMG = imgToString(img, true);


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
    vector<vector<pixel>> result = subtImages(imgA, imgB);

    string responseIMG = imgToString(result, false);

    res.set_content(responseIMG, "application/json");
  });



   svr.listen("localhost", 8080);
   return 0;
}