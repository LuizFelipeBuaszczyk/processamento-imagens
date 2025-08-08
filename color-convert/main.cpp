#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <cstdlib>
#include "httplib.h" 

using namespace std;

struct hsv{
    float hue;
    float saturation;
    float value;
};

struct rgb{
    float red;
    float green;
    float blue;
};

struct cmyk{
    float cyan;
    float magenta;
    float yellow;
    float key;
};

float searchMax(float r, float g, float b){
    float rgb[3] = {r,g,b};
    float maxValue = rgb[0];

    for(int i=1; i<=2; i++){
        if(rgb[i]>maxValue){
            maxValue = rgb[i];
        }
    }
    return maxValue;
}

float searchMin(float r, float g, float b){
    float rgb[3] = {r,g,b};
    float minValue = rgb[0];

    for(int i=1; i<=2; i++){
        if(rgb[i]<minValue){
            minValue = rgb[i];
        }
    }
    return minValue;
}

rgb normalizeRGB(rgb RGB){
    return rgb{RGB.red/255.0, RGB.green/255.0, RGB.blue/255.0};  // Garantir que a divis�o seja em ponto flutuante
}

hsv convertRGBtoHSV(rgb RGB){
    rgb RGBnormalized = normalizeRGB(RGB);

    float r = RGBnormalized.red;
    float g = RGBnormalized.green;
    float b = RGBnormalized.blue;

    // Pegar valor m�ximo e m�nimo e subtrai-los
    float maxValue = searchMax(r, g, b);
    float minValue = searchMin(r, g, b);

    float delta = maxValue - minValue;

    // Calcular matiz
    float h;

    if(delta==0){
        h = 0;
    }else if(maxValue == r){
        h = 60 * fmod(((g-b)/delta),6);
    }else if(maxValue == g){
        h = 60 * (((b-r)/delta)+2);
    }else if(maxValue == b){
        h = 60 * (((r-g)/delta)+4);
    }

    // Calcular Satura��o
    float s = (maxValue == 0) ? 0 : (delta / maxValue);

    // Calcular valor
    float v = maxValue;
    // Exibir resultado
    return hsv{h,s,v};
}

rgb convertHSVtoRGB(hsv HSV){
    float r,g,b;

    float c = HSV.value * HSV.saturation;
    float x = c * (1- abs((fmod((HSV.hue/60),2)-1)));
    float m = HSV.value - c;

    if(HSV.hue >= 0 && HSV.hue<60){
         r = c;
         g = x;
         b = 0;
    } else if(HSV.hue >= 60 && HSV.hue<120){
         r = x;
         g = c;
         b = 0;
    }else if( HSV.hue >= 120 && HSV.hue<180){
         r = 0;
         g = c;
         b = x;
    }else if( HSV.hue >= 180 && HSV.hue<240){
         r = 0;
         g = x;
         b = c;
    }else if( HSV.hue >= 240 && HSV.hue<300){
         r = x;
         g = 0;
         b = c;
    }else if( HSV.hue >= 300 && HSV.hue<360){
         r = c;
         g = 0;
         b = x;
    }

    return rgb{(r+m)*255, (g+m)*255, (b+m)*255};
}

cmyk convertRGBtoCMYK(rgb RGB){
    rgb rgbNormilized = normalizeRGB(RGB);

    float maxValue = searchMax(rgbNormilized.red, rgbNormilized.blue, rgbNormilized.green);

    float k = 1-maxValue;
    float c = (1-rgbNormilized.red-k)/(1-k);
    float m = (1-rgbNormilized.green-k)/(1-k);
    float y = (1-rgbNormilized.blue-k)/(1-k);

    return cmyk{c,m,y,k};
}


rgb convertCMYKtoRGB(cmyk CMYK){
    rgb RGB;

    RGB.red = 255 * (1-CMYK.cyan) * (1-CMYK.key);
    RGB.green = 255 * (1-CMYK.magenta) * (1-CMYK.key);
    RGB.blue = 255 * (1-CMYK.yellow) * (1-CMYK.key);

    return RGB;
}

int rgbToGrayScale(rgb RGB){
    return (0.2989*RGB.red) + (0.5870 * RGB.green) + (0.1140*RGB.blue);
}


//rgb RGB = {125,100,200};

int main() {
   httplib::Server svr;

   svr.set_default_headers({
   {"Access-Control-Allow-Origin", "*"} // Permite todas as origens
   });

   //NORMALIZAR RGB
   svr.Get("/normalize/RGB", [](const httplib::Request& req, httplib::Response& res){ 
    if (!req.has_param("r") || !req.has_param("g") || !req.has_param("b")){
   	    res.status = 400;
	    res.set_content("Par�metros R, G e B s�o obrigat�rios.", "text/plan");
	    return;
   	}

   	int r = stoi(req.get_param_value("r"));
   	int g = stoi(req.get_param_value("g"));
   	int b = stoi(req.get_param_value("b"));

	rgb RGB = {static_cast<float>(r), static_cast<float>(g), static_cast<float>(b)};
	RGB = normalizeRGB(RGB);

        ostringstream json;
        json << "{\n \"red\": " << RGB.red
	     << ",\n\"green\": " << RGB.green
	     << ",\n\"blue\": " << RGB.blue << "\n}";
	
	res.set_content(json.str(), "application/json");
   });	


   //CONVERTER RGB PARA HSV
   svr.Get("/convert/RGB/HSV", [](const httplib::Request& req, httplib::Response& res){
   	if (!req.has_param("r") || !req.has_param("g") || !req.has_param("b")){
   	    res.status = 400;
	    res.set_content("Par�metros R, G e B s�o obrigat�rios.", "text/plan");
	    return;
   	}

   	int r = stoi(req.get_param_value("r"));
   	int g = stoi(req.get_param_value("g"));
   	int b = stoi(req.get_param_value("b"));


   	rgb RGB = {static_cast<float>(r), static_cast<float>(g), static_cast<float>(b)};
   	hsv HSV = convertRGBtoHSV(RGB);

      	//Resposta
   	ostringstream json;
   	json << "{\n\"hue\": " << HSV.hue
	     << ",\n\"saturation\": " << HSV.saturation
	     << ",\n\"value\": " << HSV.value << "\n}";
 
   	res.set_content(json.str(),"application/json");
   });


   //CONVERTER RGB PARA CMYK
   svr.Get("/convert/RGB/CMYK", [](const httplib::Request& req, httplib::Response& res){
   	if (!req.has_param("r") || !req.has_param("g") || !req.has_param("b")){
   	    res.status = 400;
	    res.set_content("Par�metros R, G e B s�o obrigat�rios.", "text/plan");
	    return;
   	}

   	int r = stoi(req.get_param_value("r"));
   	int g = stoi(req.get_param_value("g"));
   	int b = stoi(req.get_param_value("b"));

	rgb RGB = {static_cast<float>(r), static_cast<float>(g), static_cast<float>(b)};
	cmyk CMYK = convertRGBtoCMYK(RGB);

	ostringstream json;
	json << "{\n\"cyan\": " << CMYK.cyan
	     << ",\n\"magenta\": " << CMYK.magenta
	     << ",\n\"yellow\": " << CMYK.yellow 
	     << ",\n\"key\": " << CMYK.key << "\n}";
	
	res.set_content(json.str(), "application/json");
   });

   //CONVERTER HSV PARA RGB 
   svr.Get("/convert/HSV/RGB", [](const httplib::Request& req, httplib::Response& res){
   	if (!req.has_param("h") || !req.has_param("s") || !req.has_param("v")){
   	    res.status = 400;
	    res.set_content("Parametros H, S e V sao obrigatorios.", "text/plan");
	    return;
   	}
	
   	float h = stof(req.get_param_value("h"));
   	float s = stof(req.get_param_value("s"));
   	float v = stof(req.get_param_value("v"));

	hsv HSV = {h, s, v};
	rgb RGB = convertHSVtoRGB(HSV);

        ostringstream json;
        json << "{\n \"red\": " << round(RGB.red)
	     << ",\n\"green\": " << round(RGB.green)
	     << ",\n\"blue\": " << round(RGB.blue) << "\n}";
	
	res.set_content(json.str(), "application/json");
   });

  //CONVERTER CMYK PARA RGB
  svr.Get("/convert/CMYK/RGB", [](const httplib::Request& req, httplib::Response& res){
   	if (!req.has_param("c") || !req.has_param("m") || !req.has_param("y") || !req.has_param("k")){
   	    res.status = 400;
	    res.set_content("Par�metros C, M, Y e K s�o obrigat�rios.", "text/plan");
	    return;
   	}
  
	float c = stof(req.get_param_value("c"));
   	float m = stof(req.get_param_value("m"));
   	float y = stof(req.get_param_value("y"));
	float k = stof(req.get_param_value("k"));
	
	cmyk CMYK = {static_cast<float>(c), static_cast<float>(m), static_cast<float>(y), static_cast<float>(k)};
	rgb RGB = convertCMYKtoRGB(CMYK);

	ostringstream json;
        json << "{\n \"red\": " << round(RGB.red)
	     << ",\n\"green\": " << round(RGB.green)
	     << ",\n\"blue\": " << round(RGB.blue) << "\n}";

	res.set_content(json.str(), "application/json");
  });

  svr.Get("/convert/RGB/GRAYSCALE", [](const httplib::Request& req, httplib::Response& res){
   	if (!req.has_param("r") || !req.has_param("g") || !req.has_param("b")){
   	    res.status = 400;
	    res.set_content("Par�metros R, G e B s�o obrigat�rios.", "text/plan");
	    return;
   	}

   	int r = stoi(req.get_param_value("r"));
   	int g = stoi(req.get_param_value("g"));
   	int b = stoi(req.get_param_value("b"));

	rgb RGB = {static_cast<float>(r), static_cast<float>(g), static_cast<float>(b)};
	int grayScale = rgbToGrayScale(RGB);

        ostringstream json;
        json << "{\n \"grayscale\": " << grayScale << "\n}";

	res.set_content(json.str(), "application/json");

  });

   svr.listen("localhost", 8080);

   return 0;
}