var corModel = 'rgb';
var rgb = {r: 0, g: 0, b: 0}

document.addEventListener("DOMContentLoaded", () => {
    document.getElementById("sendButton").addEventListener("click", executeFunction);

    document.querySelectorAll('.color-button').forEach(button => {
        button.addEventListener('click',function(event){
            event.preventDefault();
            corModel = event.target.id.replace('ButtonSelect','')
            updateInputFields(corModel);
        });
    });
});

window.onload = function(){
    updateInputFields('rgb')
}

function updateInputFields(select){

    const divFields = document.getElementById('input-fields')

    divFields.innerHTML = ''; //Limpar campos

    switch(select){
        case 'rgb':
            divFields.innerHTML = ` <label>R:<input type="number" id="r" min="0" max="255"></label>
                                    <label>G:<input type="number" id="g" min="0" max="255"></label>
                                    <label>B:<input type="number" id="b" min="0" max="255"></label>`;
            break;
        case 'hsv':
            divFields.innerHTML = ` <label>H:<input type="number" id="h" min="0" max="360"></label>
                                    <label>S:<input type="number" id="s" min="0" max="100"></label>
                                    <label>V:<input type="number" id="v" min="0" max="100"></label>`;
            break;
        case 'cmyk':
            divFields.innerHTML = ` <label>C:<input type="number" id="c" min="0" max="100"></label>
                                    <label>M:<input type="number" id="m" min="0" max="100"></label>
                                    <label>Y:<input type="number" id="y" min="0" max="100"></label>
                                    <label>K:<input type="number" id="k" min="0" max="100"></label>`
            break;
    } 

    updateFeatures(select)
}

function updateFeatures(select){
    const selectOptions = document.getElementById('select-features')

    selectOptions.innerHTML = '';

    switch(select){
        case 'rgb':
            selectOptions.innerHTML = ` <option value="hsv">HSV</option>
                                        <option value="cmyk">CMYK</option>
                                        <option value="grayscale">Escala de Cinza</option>`;
            break;
        case 'hsv':
            selectOptions.innerHTML = ` <option value="rgb">RGB</option>`;
            break;
        case 'cmyk':
            selectOptions.innerHTML = ` <option value="rgb">RGB</option>`;
            break;
    }
}

function executeFunction(){
    const feature = document.getElementById('select-features').value;
    document.getElementById('inputErroMSG').innerHTML = ``;
    switch (corModel){
        case 'rgb':
            resetStyle(['r','g','b']);
            switch (feature){
                case 'hsv':
                    convertRGBtoHSV()
                    break;
                case 'cmyk':
                    convertRGBtoCMYK()
                    break;
                case 'grayscale':
                    convertRGBtoGrayScale()
                    break;
            }
            break;
        case 'hsv':
            resetStyle(['h','s','v']);
            switch(feature){
                case 'rgb':
                    convertHSVtoRGB()
                    break;
            }
            break;
        case 'cmyk':
            resetStyle(['c','m','y','k']);
            switch(feature){
                case 'rgb':
                    convertCMYKtoRGB()
                    break;
            }
            break;
    }
}

function normalizeRGB() {
    const r = document.getElementById('r');
    const g = document.getElementById('g');
    const b = document.getElementById('b');

    rgb = {r: parseInt(r.value), g: parseInt(g.value), b: parseInt(b.value)};

    if(!rgbIsValid(rgb)){
        rgbInputErrorStyle();
        errorInputMessage();
    }
    else {
        fetch(`http://localhost:8080/normalize/RGB?r=${rgb.r}&g=${rgb.g}&b=${rgb.b}`, {
            method: 'GET'
        })
        .then(response => response.json())
        .then(data => {
            document.getElementById('rgbNormalized').innerHTML = `Cor RGB normalizada: R = ${data.red}<br>G = ${data.green}<br>B = ${data.blue}`;
        })
            .catch(error => {
                document.getElementById('rgbNormalized').textContent = 'Ooops! Ocorreu um erro na conversão.';
                console.error('Erro:', error);
            });
        updateViewColor();
        updateHexDecimal();
    }
}

function convertRGBtoHSV() {
    const r = document.getElementById('r');
    const g = document.getElementById('g');
    const b = document.getElementById('b');

    rgb = {r: parseInt(r.value), g: parseInt(g.value), b: parseInt(b.value)};

    if(!rgbIsValid(rgb)){
        rgbInputErrorStyle();
        errorInputMessage();
    }
    else{
        fetch(`http://localhost:8080/convert/RGB/HSV?r=${rgb.r}&g=${rgb.g}&b=${rgb.b}`, {
            method: 'GET'
        })
        .then(response => response.json())
        .then(data => {
            document.getElementById('response').innerHTML =`Hue = ${data.hue}<br>Saturation = ${data.saturation*100}<br>Value = ${data.value*100}`;
        })
            .catch(error => {
                document.getElementById('response').textContent = 'Ooops! Ocorreu um erro na conversão.';
                console.error('Erro:', error);
            });

        normalizeRGB()
        updateViewColor();
        updateHexDecimal();
    }
}

function convertRGBtoCMYK(){
    const r = document.getElementById('r');
    const g = document.getElementById('g');
    const b = document.getElementById('b');

    rgb = {r: parseInt(r.value), g: parseInt(g.value), b: parseInt(b.value)};

    if(!rgbIsValid(rgb)){
        rgbInputErrorStyle();
        errorInputMessage();
    }
    else{

        fetch(`http://localhost:8080/convert/RGB/CMYK?r=${rgb.r}&g=${rgb.g}&b=${rgb.b}`, {
            method: 'GET'
        })
        .then(response => response.json())
        .then(data => {
            document.getElementById('response').innerHTML =`Cyan = ${data.cyan*100}<br>Magenta = ${data.magenta*100}<br>Yellow = ${data.yellow*100}<br>Key = ${data.key*100}`;
        })
            .catch(error => {
                document.getElementById('response').textContent = 'Ooops! Ocorreu um erro na conversão.';
                console.error('Erro:', error);
            });

        normalizeRGB()
        updateViewColor();
        updateHexDecimal();    
    }
}

function convertRGBtoGrayScale(){
    const r = document.getElementById('r');
    const g = document.getElementById('g');
    const b = document.getElementById('b');

    rgb = {r: parseInt(r.value), g: parseInt(g.value), b: parseInt(b.value)};

    if(!rgbIsValid(rgb)){
        rgbInputErrorStyle();
        errorInputMessage();
    }
    else{
        fetch(`http://localhost:8080/convert/RGB/GRAYSCALE?r=${rgb.r}&g=${rgb.g}&b=${rgb.b}`, {
            method: 'GET'
        })
        .then(response => response.json())
        .then(data => {
            document.getElementById('response').textContent =`Escala de Cinza = ${data.grayscale}`;
        })
            .catch(error => {
                document.getElementById('response').textContent = 'Ooops! Ocorreu um erro na conversão.';
                console.error('Erro:', error);
            });

        normalizeRGB()
        updateViewColor();
        updateHexDecimal();
    }
}

function convertHSVtoRGB(){
    const h = document.getElementById('h')
    const s = document.getElementById('s')
    const v = document.getElementById('v')

    const hsv = {h: parseFloat(h.value), s: parseFloat(s.value), v: parseFloat(v.value)}

    if(!hsvIsValid(hsv)){
        hsvInputErrorStyle(hsv);
        errorInputMessage();
    }else{
        fetch(`http://localhost:8080/convert/HSV/RGB?h=${hsv.h}&s=${hsv.s/100}&v=${hsv.v/100}`, {
            method: 'GET'
        })
        .then(response => response.json())
        .then(data => {
            rgb = {r: parseInt(data.red), g: parseInt(data.green), b: parseInt(data.blue)};
            document.getElementById('response').innerHTML = `Red = ${data.red}<br>Green = ${data.green}<br>Blue = ${data.blue}`;
            updateViewColor();
            updateHexDecimal();
        })
            .catch(error => {
                document.getElementById('response').textContent = 'Ooops! Ocorreu um erro na conversão.';
                console.error('Erro:', error);
            });            
    }
}

function convertCMYKtoRGB(){
    const c = document.getElementById('c')
    const m = document.getElementById('m')
    const y = document.getElementById('y')
    const k = document.getElementById('k')

    const cmyk = {c: parseFloat(c.value), m: parseFloat(m.value), y: parseFloat(y.value), k: parseFloat(k.value)}

    if(!cmykIsValid(cmyk)){
        cmykInputErrorStyle(cmyk);
        errorInputMessage();
    }else{
        fetch(`http://localhost:8080/convert/CMYK/RGB?c=${cmyk.c/100}&m=${cmyk.m/100}&y=${cmyk.y/100}&k=${cmyk.k/100}`, {
            method: 'GET'
        })
        .then(response => response.json())
        .then(data => {
            rgb = {r: parseInt(data.red), g: parseInt(data.green), b: parseInt(data.green)};
            document.getElementById('response').innerHTML = `Red = ${data.red}<br>Green = ${data.green}<br>Blue = ${data.blue}`;
            updateViewColor();
            updateHexDecimal();
        })
            .catch(error => {
                document.getElementById('response').textContent = 'Ooops! Ocorreu um erro na conversão.';
                console.error('Erro:', error);
            });
    }
}

function updateViewColor(){
    const viewColor = document.getElementById("viewColor");
    viewColor.style.backgroundColor = `rgb(${rgb.r}, ${rgb.g}, ${rgb.b})`;
}

function updateHexDecimal(){
    const red = rgb.r.toString(16).padStart(2, '0').toUpperCase();
    const green = rgb.g.toString(16).padStart(2, '0').toUpperCase();
    const blue = rgb.b.toString(16).padStart(2, '0').toUpperCase();
    const hexDecimalRGB = document.getElementById('hexDecimalRGB').textContent = `#${red}${green}${blue}`;
}

function rgbIsValid(rgb){
    if(isNaN(rgb.r) || isNaN(rgb.g) ||isNaN(rgb.b)){
        return false;
    }
    if(rgb.r < 0 || rgb.r > 255 || rgb.g < 0 || rgb.g > 255 || rgb.b < 0 || rgb.b > 255){
        return false;
    }
    return true;
}

function rgbInputErrorStyle(){
    const r = document.getElementById('r')
    const g = document.getElementById('g')
    const b = document.getElementById('b')

    if(rgb.r < 0 || rgb.r > 255 || isNaN(rgb.r)){
        r.style.backgroundColor = `red`;
    }
    if(rgb.g < 0 || rgb.g > 255 || isNaN(rgb.g)){
        g.style.backgroundColor = `red`;
    }
    if(rgb.b < 0 || rgb.b > 255 || isNaN(rgb.b)){
        b.style.backgroundColor = `red`;
    }
}

function cmykIsValid(cmyk){
    if(isNaN(cmyk.c) || isNaN(cmyk.m) || isNaN(cmyk.y) || isNaN(cmyk.k)){
        return false;
    }
    if(cmyk.c < 0 || cmyk.c > 100 || cmyk.m < 0 || cmyk.m > 100 || cmyk.y < 0 || cmyk.y > 100 || cmyk.k < 0 || cmyk.k > 100){
        return false;
    }
    return true;
}

function cmykInputErrorStyle(cmyk){
    const c = document.getElementById('c')
    const m = document.getElementById('m')
    const y = document.getElementById('y')
    const k = document.getElementById('k')

    if(cmyk.c < 0 || cmyk.c > 100 || isNaN(cmyk.c)){
        c.style.backgroundColor = `red`;
    }
    if(cmyk.m < 0 || cmyk.m > 100 || isNaN(cmyk.m)){
        m.style.backgroundColor = `red`;
    }
    if(cmyk.y < 0 || cmyk.y > 100 || isNaN(cmyk.y)){
        y.style.backgroundColor = `red`;
    }
    if(cmyk.k < 0 || cmyk.k > 100 || isNaN(cmyk.k)){
        k.style.backgroundColor = `red`;
    }
}

function hsvIsValid(hsv){
    if(isNaN(hsv.h) || isNaN(hsv.s) || isNaN(hsv.v)){
        return false;
    }
    if(hsv.h < 0 || hsv.h > 360 || hsv.s < 0 || hsv.s > 100|| hsv.v < 0 || hsv.v > 100){
        return false;
    }
    return true;
}

function hsvInputErrorStyle(hsv){
    const h = document.getElementById('h')
    const s = document.getElementById('s')
    const v = document.getElementById('v')

    if(hsv.h < 0 || hsv.h > 360 || isNaN(hsv.h)){
        h.style.backgroundColor = `red`;
    }
    
    if(hsv.s < 0 || hsv.s > 100 || isNaN(hsv.s)){
        s.style.backgroundColor = `red`;
    }
    
    if(hsv.v < 0 || hsv.v > 100 || isNaN(hsv.v)){
        v.style.backgroundColor = `red`;
    }
}

function resetStyle(idHTML){

    idHTML.forEach(id => {
        const element = document.getElementById(id);
        if(element){
            element.style.backgroundColor = `white`;
        }
    });
}

function errorInputMessage(){
    const errorMessage = document.getElementById('inputErroMSG');
    const inputFields = document.getElementById('input-fields');
    
    inputFields.style.marginBottom = `0px`;
    errorMessage.textContent = `Valores inválidos! Por favor insira os valores corretamente.`;
    errorMessage.style.color = `red`;
    errorMessage.style.fontSize = `12px`;
}