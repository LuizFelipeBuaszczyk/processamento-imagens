var rgb = {r: 0, g: 0, b: 0, a:255}
var selectedFeature;

document.getElementById('inputImage').addEventListener('change', loadImageToCanvas('inputImage','showInputImage'));

document.getElementById('inputImage2').addEventListener('change', loadImageToCanvas('inputImage2','showInputImage2'));

document.getElementById('rangeForm').innerHTML = `  <label class = "label" id = "labelRange">Valor: 128</label>
                                                    <input type= "range" id="range" min="0" max="255">`;

document.getElementById('selectConvertion').addEventListener('change', showThresholdRange);
document.getElementById('selectFeatures').addEventListener('change', updateRange);


function loadImageToCanvas(inputId, canvasId) {
    document.getElementById(inputId).addEventListener('change', function(event) {
        var file = event.target.files[0];
        if (file) {
            var reader = new FileReader();
            reader.onload = function(e) {
                var img = new Image();
                img.onload = function() {
                    var canvas = document.getElementById(canvasId);
                    var ctx = canvas.getContext('2d');

                    // Ajusta o tamanho do canvas para a imagem
                    canvas.width = 250;
                    canvas.height = 250;

                    // Desenha a imagem no canvas
                    ctx.drawImage(img, 0, 0, canvas.width, canvas.height);

                    if(inputId == 'inputImage'){
                        getHistogram('showInputImage', 'originalHistogram');
                    }
                };
                img.src = e.target.result;
            };
            reader.readAsDataURL(file);
        }
    });

}

document.getElementById("processButton").addEventListener("click", executeFeature);
document.getElementById("convertButton").addEventListener("click", executeConvert);
document.getElementById('equalizeHistogramButton').addEventListener('click', equalizeHistogram);

document.getElementById("enable2Images").addEventListener("change", function(){
    updateButtons();
    updateFeature('arithmeticButton');
});

document.addEventListener("DOMContentLoaded", () => {
    document.querySelectorAll('.featureButton').forEach(button => {
        button.addEventListener('click',function(event){
            event.preventDefault();
            selectedFeature = event.target.id.replace('ButtonSelect','')
            updateFeature(selectedFeature);
        });
    });
});

if(document.getElementById('range')){
    document.getElementById('range').addEventListener('input', () => {
        document.getElementById('labelRange').textContent = "Valor: " + document.getElementById('range').value;
    });
}

document.getElementById('saveImage').addEventListener('click', function(){
    saveImage();
});

// Função para salvar imagens
function saveImage(){
    const canvas = document.getElementById('canvas');
    const ctx = canvas.getContext('2d');

      
    const imagemDataURL = canvas.toDataURL('image/png');

    const link = document.createElement('a');
    link.href = imagemDataURL;
    link.download = 'imagem_canvas.png'; 
    link.click(); 
}

function showThresholdRange(){
    const selected = document.getElementById('selectConvertion').value;

    if(selected == '1bit'){
        document.getElementById('rangeThreshold').innerHTML = ` <label class = "label" id= "labelRangeConvertion">Valor: 128</label>
                                                                <input type= "range" id="rangeConvertion" min="0" max="255" oninput="updateConvertLabelValue()">`
    }else{
        document.getElementById('rangeThreshold').innerHTML = ``;
    }
}

function updateConvertLabelValue(){
    document.getElementById('labelRangeConvertion').textContent = "Valor: " + document.getElementById('rangeConvertion').value; 
}

function transformIMGtoMATRIX(canvas) {
    var ctx = canvas.getContext('2d');

    // Aguardar a imagem ser carregada antes de processá-la
    var imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);
        var data = imageData.data;

        // Criar a matriz
        var matrix = [];
        for (var y = 0; y < canvas.height; y++) {
            var row = [];
            for (var x = 0; x < canvas.width; x++) {
                // Cada pixel é representado por um array [r, g, b, a]
                var index = (y * canvas.width + x) * 4;
                var rgba = [
                    data[index],     
                    data[index + 1], 
                    data[index + 2], 
                    data[index + 3]  
                ];
                row.push(rgba);
            }
            matrix.push(row);
        }

        // Exibir a matriz no formato JSON 
        matrixJSON = JSON.stringify(matrix, null, 2);
        return matrixJSON;
        //JSON ordem: [WIDTH][HEIGHT]
}

function drawImage(imageData){
    var canvas = document.getElementById('canvas');
    var ctx = canvas.getContext('2d');

    const height = imageData.length;
    const width = imageData[0].length;
    canvas.width = width;
    canvas.height = height;

    const imageDataObj = ctx.createImageData(width, height);

    let i = 0;
    for(let y = 0; y < height; y++){
        for(let x = 0; x < width; x++){
            let pixel = imageData[y][x];
            imageDataObj.data[i++] = pixel[0]; // Red
            imageDataObj.data[i++] = pixel[1]; // Green
            imageDataObj.data[i++] = pixel[2]; // Blue
            imageDataObj.data[i++] = pixel[3]; // Alpha
        }
    }

    ctx.putImageData(imageDataObj, 0, 0);   
}

function draw8bitImage(imageData){
    var canvas = document.getElementById('canvas');
    var ctx = canvas.getContext('2d');

    const height = imageData.length;
    const width = imageData[0].length;
    canvas.width = width;
    canvas.height = height;

    const imageDataObj = ctx.createImageData(width, height);

    let i = 0;
    for(let y = 0; y < height; y++){
        for(let x = 0; x < width; x++){
            let pixel = imageData[y][x];
            imageDataObj.data[i++] = pixel;
            imageDataObj.data[i++] = pixel; 
            imageDataObj.data[i++] = pixel; 
            imageDataObj.data[i++] = 255;  
        }
    }

    ctx.putImageData(imageDataObj, 0, 0);   
}

function drawHistogram(histogram, labels, values){
    const canvas = document.getElementById(histogram);
    const ctx = canvas.getContext("2d");

    const width = canvas.width;
    const height = canvas.height;
    const margin = 40;
    const barLarge = (width - 2 * margin) / labels.length;

    const maxValue = Math.max(...values);
    const heightScale = (height - 2 * margin) / maxValue;

    ctx.clearRect(0, 0, width, height);

    ctx.beginPath();
    ctx.moveTo(margin, margin);
    ctx.lineTo(margin, height - margin);
    ctx.lineTo(width - margin, height - margin);
    ctx.stroke();

    labels.forEach((label, i) => {
        const x = margin + i * barLarge;
        const h = values[i] * heightScale;
        const y = height - margin - h;

        ctx.fillStyle = "#2196f3";
        ctx.fillRect(x, y, barLarge, h);
  
        ctx.fillStyle = "#000";
        ctx.fillText(label, x + barLarge / 4, height - margin + 12);
    });
}

function updateButtons(){
    const selected = document.getElementById("enable2Images").checked;
    
    if(selected){
         document.getElementById("featureButtons").innerHTML = ` <button value="arithmetic" id="arithmeticButton" class="featureButton">Aritmética</button>
                                                                 <button value="logic" id="logicButton" class="featureButton">Lógica</button>
                                                                 <button value="diff" id="diffButton" class="featureButton">Diferença</button>
                                                                 <button value="linear" id="linearButton" class="featureButton">Linear</button>`;
    }else{
        document.getElementById("featureButtons").innerHTML = `  <button value="arithmetic" id="arithmeticButton" class="featureButton">Aritmética</button>
                                                                 <button value="invert" id="invertButton" class="featureButton">Inverção</button>
                                                                 <button value="convolutional" id="convolutionalButton" class="featureButton">Convolução</button>`;
    }

    document.querySelectorAll('.featureButton').forEach(button => {
        button.addEventListener('click',function(event){
            event.preventDefault();
            selectedFeature = event.target.id.replace('ButtonSelect','')
            updateFeature(selectedFeature);
        });
    });

}


function updateFeature(selectedOption){
    const selectOptions = document.getElementById("selectFeatures");

    const checked = document.getElementById("enable2Images").checked;

    if (checked){
        switch(selectedFeature){
            case 'arithmeticButton':
                showRange(false);
                showConvolutionRange(false);
                selectOptions.innerHTML     =   `   <option value="addValue">Adicionar</option>
                                                    <option value="subtValue">Subtrair</option>`;
                break;
            case 'logicButton':
                    showRange(false);
                    showConvolutionRange(false);
                    selectOptions.innerHTML =  `    <option value="and">AND</option>
                                                    <option value="or">OR</option>
                                                    <option value="xor">XOR</option>
                                                    <option value="not">NOT</option>`;
    
                break;
            case 'diffButton':
                showRange(false);
                showConvolutionRange(false);
                selectOptions.innerHTML =  ``;
                break;
            case 'linearButton':
                showRange(true);
                showConvolutionRange(false);
                selectOptions.innerHTML =  `<option value="average">Média</option>
                                            <option value="blending" class="percentRange">Blending</option>`;
                break;
        }
    } else{
        switch(selectedFeature){
            case 'arithmeticButton':
                showRange(true);
                selectOptions.innerHTML =  `<option value="addValue" class="valueRange">Adicionar</option>
                                            <option value="subtValue" class="valueRange">Subtrair</option>
                                            <option value="multValue" class="valueRange">Multiplicação</option>
                                            <option value="divValue" class="valueRange">Divisão</option>`;
                break;
            case 'invertButton':
                showRange(false);
                showConvolutionRange(false);
                selectOptions.innerHTML =  `<option value="horizontal">Horizontal</option>
                                            <option value="vertical">Vertical</option>`;
                break;
            
            case 'convolutionalButton':
                showConvolutionRange(true);
                selectOptions.innerHTML =  `<option value="mean" class="convolutional">MEAN</option>
                                            <option value="min" class="convolutional">MIN</option>
                                            <option value="max" class="convolutional">MAX</option>
                                            <option value="median" class="convolutional">MEDIANA</option>
                                            <option value="order" class="convolutional">ORDEM</option>
                                            <option value="conservativeSmoothing" class="convolutional">SUAVIZAÇÃOO CONSERVATIVA</option>
                                            <option value="gaussian" class="convolutional">Gaussiano</option>`;
                break;
            case 'borderDetectionButton':
                showRange(false);
                showConvolutionRange(false);
                selectOptions.innerHTML =  `<option value="prewit">Prewit</option>
                                            <option value="sobel">Sobel</option>
                                            <option value="laplacian">Laplaciano</option>`;
                break;
        }

    }
}

function updateRangeLabel(){
    document.getElementById("labelRange").innerHTML= `<label class = "label" id= "labelRange">Valor: ${document.getElementById("range").value}</label>`
}

function updateRange() {
    const selectElement = document.getElementById('selectFeatures');
    const selectedOption = selectElement.options[selectElement.selectedIndex];
    const selectedClass = selectedOption.className;

    switch (selectedClass) {
        case 'valueRange':
            showRange(true);
            break;
        case 'convolutional':
            showConvolutionRange(true);
            break;
        case 'percentRange':
            showRange(true);
            break;
        default:
            showRange(false);
    }
}

function showRange(show){
    if (show){
        document.getElementById('rangeForm').innerHTML = `  <label class = "label" id= "labelRange">Valor: 128</label>
                                                            <input type= "range" id="range" min="0" max="255" oninput="updateRangeLabel()">`;   
    }else {
        document.getElementById('rangeForm').innerHTML = ``;                                 
    }
}

function showConvolutionRange(show){
    selected = document.getElementById('selectFeatures').value;

    if (show) {
        let html = `<label class = "label" id= "labelRange">Valor: 7</label>
                    <input type= "range" id="range" min="3" max="11" step="2" oninput="updateRangeLabel()">`;
       
        if (selected == 'order'){
            html += `<input type="number" name="number" id="inputValue" max="5">`;   
        }

        if (selected == 'gaussian'){
            html += `<input type="number" name="number" id="inputValue" max="100" step="0.01">`
        }
                    
        document.getElementById('rangeForm').innerHTML = html;
    }else {
        document.getElementById('rangeForm').innerHTML = ``;                                 
    }
}

function executeFeature(){
    const selected = document.getElementById("selectFeatures").value;

    switch(selectedFeature){
        case 'arithmeticButton':
            const arithmeticWith2images = document.getElementById("enable2Images").checked;
            if(arithmeticWith2images){
                switch (selected) {
                    case('addValue'):
                        operationWith2Images('+');
                        break;
                    case('subtValue'):
                        subtImages('-');
                        break;
                }
            }else {
                const value = document.getElementById('range').value;

                switch(selected){
                    case('addValue'):
                        operationValueToIMG('+', value);
                        break;
                    case('subtValue'):
                        operationValueToIMG('-', value);
                        break;
                    case('multValue'):
                        operationValueToIMG('*', value);
                        break;
                    case('divValue'):
                        operationValueToIMG('/', value);
                        break;
                }
            }
            break;
        case 'logicButton':
            logic(selected);
            break;
        case 'invertButton':
            switch(selected){
                case('horizontal'):
                    invertImage('h');
                    break;
                case('vertical'):
                    invertImage('v')
                    break;
            }
            break;
        case 'diffButton':
            operationWith2Images('d');
            break;
        case 'linearButton':
            switch(selected){
                case('blending'):
                    linear('b');
                    break;
                case('average'):
                    linear('a');
                    break;
            }
            break;  
        
        case 'convolutionalButton': 
            convolutional(selected);
        case 'borderDetectionButton':
            borderDetection(selected);
    }       
}

function executeConvert(){
    const selected = document.getElementById("selectConvertion").value;
    
    switch(selected){
        case('grayScale'):
            convertIMGtoGrayScale()
            break;
        case('1bit'):
            convertIMGto1Bit()
            break;
    }
}

function operationValueToIMG(op, value){
    matrixJSON = transformIMGtoMATRIX(document.getElementById('showInputImage'));

    if(false){

    } else{
        let endpoint;

        switch (op) {//Verify operation
            case '+':
                endpoint = `http://localhost:8080/process/add?value=${value}`;
                break;
            case '-':
                endpoint = `http://localhost:8080/process/subt?value=${value}`;
                break;
            case '*':
                endpoint = `http://localhost:8080/process/mult?value=${value}`;
                break;
            case '/':
                endpoint = `http://localhost:8080/process/div?value=${value}`;
                break;
            default:
                endpoint = undefined;
        }

        if(endpoint){
            fetch(endpoint, {
                method: 'POST',
                body: matrixJSON
            })
            .then(response => response.json())
            .then(data => {
                drawImage(data);
            })
            .catch(error => {
                console.error('Erro:', error);
            });
        }
    }
}

function convertIMGtoGrayScale(){
    matrixJSON = transformIMGtoMATRIX(document.getElementById('showInputImage'));
    const value = 100;

    if(false){

    } else{
        fetch(`http://localhost:8080/convert/8bit`, {
            method: 'POST',
            body: matrixJSON
        })
        .then(response => response.json())
        .then(data => {
            draw8bitImage(data);
        })
        .catch(error => {
            console.error('Erro:', error);
        });
    }
}

function convertIMGto1Bit(){
    matrixJSON = transformIMGtoMATRIX(document.getElementById('showInputImage'));
    const value = document.getElementById('rangeConvertion').value;

    if(false){

    } else{
        fetch(`http://localhost:8080/convert/1bit?value=${value}`, {
            method: 'POST',
            body: matrixJSON
        })
        .then(response => response.json())
        .then(data => {
            draw8bitImage(data);
        })
        .catch(error => {
            console.error('Erro:', error);
        });
    }
}

function invertImage(op){
    matrixJSON = transformIMGtoMATRIX(document.getElementById('showInputImage'));

    if(false){

    } else{
        endpoint = op == 'h' ? `http://localhost:8080/invert/horizontal` : `http://localhost:8080/invert/vertical`; 
        fetch(endpoint, {
            method: 'POST',
            body: matrixJSON
        })
        .then(response => response.json())
        .then(data => {
            drawImage(data);
        })
        .catch(error => {
            console.error('Erro: ', error);
        });
    }
}

function addImages(){
    const matrixJSON = transformIMGtoMATRIX(document.getElementById('showInputImage'));
    const matrixJSON2 = transformIMGtoMATRIX(document.getElementById('showInputImage2'));
    json = matrixJSON + '\n' + 'S' + matrixJSON2;


    if(false){

    } else{
        fetch(`http://localhost:8080/add`, {
            method: 'POST',
            body: json
        })
        .then(response => response.json())
        .then(data => {
            drawImage(data);
        })
        .catch(error => {
            console.error('Erro: ', error);
        });
    }
}

function subtImages(){
    const matrixJSON = transformIMGtoMATRIX(document.getElementById('showInputImage'));
    const matrixJSON2 = transformIMGtoMATRIX(document.getElementById('showInputImage2'));
    json = matrixJSON + '\n' + 'S' + matrixJSON2;

    if(false){

    } else{
        fetch(`http://localhost:8080/subt`, {
            method: 'POST',
            body: json
        })
        .then(response => response.json())
        .then(data => {
            drawImage(data);
        })
        .catch(error => {
            console.error('Erro:', error);
        });
    }
}

function operationWith2Images(op){
    const matrixJSON = transformIMGtoMATRIX(document.getElementById('showInputImage'));
    const matrixJSON2 = transformIMGtoMATRIX(document.getElementById('showInputImage2'));
    json = matrixJSON + '\n' + 'S' + matrixJSON2;

    if(false){

    }else{
        let endpoint;
        
        switch (op){
            case '+':
                endpoint = `http://localhost:8080/add`;
                break;
            case '-':
                endpoint = `http://localhost:8080/subt`;
                break;
            case 'd':
                endpoint = `http://localhost:8080/diference`
                break;
            default:
                endpoint = undefined;
        }

        if(endpoint) {
            fetch(endpoint, {
                method: 'POST',
                body: json
            })
            .then(response => response.json())
            .then(data => {
                drawImage(data);
            })
            .catch(error => {
                console.error('Erro:', error);
            });
        }        
    }
}


function logic(op){
    const matrixJSON = transformIMGtoMATRIX(document.getElementById('showInputImage'));
    if(op!= 'not'){
        const matrixJSON2 = transformIMGtoMATRIX(document.getElementById('showInputImage2'));
        json = matrixJSON + '\n' + 'S' + matrixJSON2;
    }else{
        json = matrixJSON;
    }
    if(false){

    }else{
        let endpoint;
        
        endpoint = `http://localhost:8080/logic/` + op;

        if(endpoint) {
            fetch(endpoint, {
                method: 'POST',
                body: json
            })
            .then(response => response.json())
            .then(data => {
                draw8bitImage(data);
            })
            .catch(error => {
                console.error('Erro:', error);
            });
        }        
    }
}

function linear(op){
    const matrixJSON = transformIMGtoMATRIX(document.getElementById('showInputImage'));
    const matrixJSON2 = transformIMGtoMATRIX(document.getElementById('showInputImage2'));
    json = matrixJSON + '\n' + 'S' + matrixJSON2;

    if(false){

    }else{
        let endpoint;
        
        switch (op){
            case 'b': //Valor personalizado
                let value = document.getElementById("range").value;
                endpoint = `http://localhost:8080/linear/blending?value=${value}`;
                break;
            case 'a':
                endpoint = `http://localhost:8080/linear/average`;
                break;
            default:
                endpoint = undefined;
        }

        if(endpoint) {
            fetch(endpoint, {
                method: 'POST',
                body: json
            })
            .then(response => response.json())
            .then(data => {
                drawImage(data);
            })
            .catch(error => {
                console.error('Erro:', error);
            });
        }        
    }
}

function getHistogram(img, histogram){
    matrixJSON = transformIMGtoMATRIX(document.getElementById(img));

    endpoint = `http://localhost:8080/histogram`;
    fetch(endpoint, {
        method: 'POST',
        body: matrixJSON
    })
    .then(response => response.json())
    .then(data => {
        const labels = Array.from({length: 256}, (_,i) => i);
        drawHistogram(histogram, labels, data);
    })
    .catch(error =>{
        console.error('Erro: ', error);
    })

}

function  convolutional(option){
    matrixJSON = transformIMGtoMATRIX(document.getElementById('showInputImage'));
    const kernel = document.getElementById('range').value;
    let invalid = false;
    let msgError = '';

    if (option == 'order'){
        const inputValue = document.getElementById('inputValue').value;
        invalid = !verifityValueIsInRange(inputValue, 1, kernel*kernel);
        msgError += `\nInsira um valor dentro do range (1, ${kernel*kernel})`;
    }

    if(invalid){
        alert(msgError);
    } else {
        endpoint = `http://localhost:8080/convolutional/${option}?kernel=${kernel}`; 
        if (option == 'order'){
            endpoint += `&selectedvalue=${document.getElementById('inputValue').value}`;
        }
        else if (option == 'gaussian'){
            const value = document.getElementById('inputValue').value.replace(",", ".");
            endpoint += `&sigma=${value}`;
        }
        
        fetch(endpoint, {
            method: 'POST',
            body: matrixJSON
        })
        .then(response => response.json())
        .then(data => {     
            if (option == 'gaussian'){
                drawImage(data.image);
            } 
            else{
                drawImage(data);
            }
        })
        .catch(error => {
            console.error('Erro: ', error);
        });
    }
}

function borderDetection(option){
    matrixJSON = transformIMGtoMATRIX(document.getElementById('showInputImage'));


    if(false){
        
    } else {
        endpoint = `http://localhost:8080/borderDetection/${option}`; 
        
        fetch(endpoint, {
            method: 'POST',
            body: matrixJSON
        })
        .then(response => response.json())
        .then(data => {     
            draw8bitImage(data);
        })
        .catch(error => {
            console.error('Erro: ', error);
        });
    }
}


function equalizeHistogram(){
    matrixJSON = transformIMGtoMATRIX(document.getElementById('showInputImage'));

    if(false){

    } else{
        endpoint = `http://localhost:8080/histogram/equalize`; 
        fetch(endpoint, {
            method: 'POST',
            body: matrixJSON
        })
        .then(response => response.json())
        .then(data => {
            draw8bitImage(data.image);
            const labels = Array.from({length: 256}, (_,i) => i);
            drawHistogram('equalizedHistogram', labels, data.histogram);
        })
        .catch(error => {
            console.error('Erro: ', error);
        });
    }
}


/// Verificações
function verifityValueIsInRange(value, min, max){
    return ((value<=max) && (value>=min));
}