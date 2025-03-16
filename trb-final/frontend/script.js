var rgb = {r: 0, g: 0, b: 0, a:255}


document.getElementById('inputImage').addEventListener('change', loadImageToCanvas('inputImage','showInputImage'));
document.getElementById('inputImage2').addEventListener('change', loadImageToCanvas('inputImage2','showInputImage2'));

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
                    canvas.width = img.width;
                    canvas.height = img.height;

                    // Desenha a imagem no canvas
                    ctx.drawImage(img, 0, 0, img.width, img.height);
                };
                img.src = e.target.result;
            };
            reader.readAsDataURL(file);
        }
    });
}

document.getElementById("button").addEventListener("click", executeFeature);
document.getElementById("arithmeticButton").addEventListener("click", updateFeature('arithmetic'));

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

function updateFeature(selectedOption){
    const selectOptions = document.getElementById("selectFeatures");

    switch (selectedOption){
        case 'arithmetic':
        selectOptions.innerHTML =  `<option value="addValue">Adicionar</option>
                                    <option value="subtValue">Subtrair</option>
                                    <option value="multValue">Multiplicação</option>
                                    <option value="divValue">Divisão</option>`;
        break;
    }
}

function executeFeature(){
    const selected = document.getElementById("selectFeatures").value;

    switch (selected) {
        case('addValue'):
            operationValueToIMG('+', 100);
            break;
        case('subtValue'):
            operationValueToIMG('-', 100);
            break;
        case('multValue'):
            operationValueToIMG('*', 2);
            break;
        case('divValue'):
            operationValueToIMG('/', 2);
            break;
        case('grayScale'):
            convertIMGtoGrayScale();
            break;
        case('1bit'):
            convertIMGto1Bit();
            break;
        case('addImages'):
            addImages();
            break;
        case('subtImages'):
            subtImages();
            break;
    }
}

function operationValueToIMG(op, value){
    matrixJSON = transformIMGtoMATRIX(document.getElementById('showInputImage'));

    if(false){

    } else{
        let endpoint;

        console.log(op);

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
    const value = 100;

    if(false){

    } else{
        fetch(`http://localhost:8080/convert/1bit`, {
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
            console.error('Erro:', error);
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