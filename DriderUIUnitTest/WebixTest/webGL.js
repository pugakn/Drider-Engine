var gl; // Un variable global para el contexto WebGL
var g_EditorTexture;
function C_UpdateEditor(buffer,w,h){
	updateTexture(g_EditorTexture,buffer,w,h);
}

//Get the canvas size on C app
function C_GetSceneViewSize() {
	var canvas = document.getElementById('glcanvas');
	var width = canvas.width;
	var height = canvas.height;
	JS_GetSceneViewSize(width,height);
}

function createTexture(w,h) {
  var texture = gl.createTexture();
  gl.bindTexture(gl.TEXTURE_2D, texture);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
  
  gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
  gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, w, h, 0, gl.RGBA, gl.UNSIGNED_BYTE, null);
  //gl.bindTexture(gl.TEXTURE_2D, null);
  return texture;
}

function updateTexture(texture,buffer,w,h) {
	var result = [];
	for(var i = 0; i < buffer.length; i+=1)
	{
		result.push(buffer.charCodeAt(i));
	}
	result = Uint8Array.from(result)
  //var enc = new TextEncoder();
  //arr = enc.encode(buffer);
  //console.log(result[0]);
  //gl.bindTexture(gl.TEXTURE_2D, texture);
  ///gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
  //gl.pixelStorei(gl.UNPACK_ALIGNMENT, 4);
  ///gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, w, h, 0, gl.RGBA, gl.UNSIGNED_BYTE, result);
  //gl.bindTexture(gl.TEXTURE_2D, null);
}

function initShaderProgram(gl, vsSource, fsSource) {
  const vertexShader = loadShader(gl, gl.VERTEX_SHADER, vsSource);
  const fragmentShader = loadShader(gl, gl.FRAGMENT_SHADER, fsSource);

  // Create the shader program

  const shaderProgram = gl.createProgram();
  gl.attachShader(shaderProgram, vertexShader);
  gl.attachShader(shaderProgram, fragmentShader);
  gl.linkProgram(shaderProgram);

  // If creating the shader program failed, alert

  if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
    alert('Unable to initialize the shader program: ' + gl.getProgramInfoLog(shaderProgram));
    return null;
  }

  return shaderProgram;
}

//
// creates a shader of the given type, uploads the source and
// compiles it.
//
function loadShader(gl, type, source) {
  const shader = gl.createShader(type);

  // Send the source to the shader object

  gl.shaderSource(shader, source);

  // Compile the shader program

  gl.compileShader(shader);

  // See if it compiled successfully

  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    alert('An error occurred compiling the shaders: ' + gl.getShaderInfoLog(shader));
    gl.deleteShader(shader);
    return null;
  }

  return shader;
}


function initWebGL(canvas) {
  gl = null;
  
  try {
    // Tratar de tomar el contexto estandar. Si falla, retornar al experimental.
    gl = canvas.getContext("webgl") || canvas.getContext("experimental-webgl");
  }
  catch(e) {}
  
  // Si no tenemos ningun contexto GL, date por vencido ahora
  if (!gl) {
    alert("Imposible inicializar WebGL. Tu navegador puede no soportarlo.");
    gl = null;
  }
  
  return gl;
}


function initBuffers(gl) {

  // Create a buffer for the square's positions.

  const positionBuffer = gl.createBuffer();

  // Select the positionBuffer as the one to apply buffer
  // operations to from here out.

  gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

  // Now create an array of positions for the square.

  const positions = [
     1.0,  1.0, 0.0 ,1.0, 1.0, 0.0,
    -1.0,  1.0, 0.0 ,1.0, 0.0, 0.0,
     1.0, -1.0, 0.0 ,1.0, 1.0, 1.0,
    -1.0, -1.0, 0.0 ,1.0, 0.0, 1.0,
  ];

  // Now pass the list of positions into WebGL to build the
  // shape. We do this by creating a Float32Array from the
  // JavaScript array, then use it to fill the current buffer.

  gl.bufferData(gl.ARRAY_BUFFER,
                new Float32Array(positions),
                gl.STATIC_DRAW);

  return {
    position: positionBuffer,
  };
}

function drawScene(gl, programInfo, buffers) {
  gl.clearColor(0.0, 0.0, 0.0, 1.0);  // Clear to black, fully opaque
  gl.clearDepth(1.0);                 // Clear everything
  gl.enable(gl.DEPTH_TEST);           // Enable depth testing
  gl.depthFunc(gl.LEQUAL);            // Near things obscure far things

  // Clear the canvas before we start drawing on it.

  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

  // Tell WebGL how to pull out the positions from the position
  // buffer into the vertexPosition attribute.
  {
    const stride = 24;
    var offset = 0; 
    gl.bindBuffer(gl.ARRAY_BUFFER, buffers.position);
    gl.vertexAttribPointer(programInfo.attribLocations.vertexPosition,4,gl.FLOAT,false,stride,offset);
    gl.enableVertexAttribArray(programInfo.attribLocations.vertexPosition);
	offset += 16;
	gl.vertexAttribPointer(programInfo.attribLocations.textureCoord, 2, gl.FLOAT, false, stride, offset);
    gl.enableVertexAttribArray(programInfo.attribLocations.textureCoord);
  }

  // Tell WebGL to use our program when drawing

  gl.useProgram(programInfo.program);
  
  // Bind texture
  gl.activeTexture(gl.TEXTURE0);
  gl.bindTexture(gl.TEXTURE_2D, g_EditorTexture);
  gl.uniform1i(gl.getUniformLocation(programInfo.program, 'uSampler'), 0);
  
  
  {
    const offset = 0;
    const vertexCount = 4;
    gl.drawArrays(gl.TRIANGLE_STRIP, offset, vertexCount);
  }
}

function start() {
  var canvas = document.getElementById("glcanvas");
  gl = initWebGL(canvas);      // Inicializar el contexto GL
  g_EditorTexture = createTexture(128,128);
  
  // Solo continuar si WebGL esta disponible y trabajando
  
  if (gl) {
    gl.clearColor(0.0, 0.0, 0.0, 1.0);                      // Establecer el color base en negro, totalmente opaco
    gl.enable(gl.DEPTH_TEST);                               // Habilitar prueba de profundidad
    gl.depthFunc(gl.LEQUAL);                                // Objetos cercanos opacan objetos lejanos
    gl.clear(gl.COLOR_BUFFER_BIT|gl.DEPTH_BUFFER_BIT);      // Limpiar el buffer de color asi como el de profundidad
	gl.viewport(0, 0, canvas.width, canvas.height);
	
	//Init Shaders
	const vsSource = `
	attribute vec4 aVertexPosition;
	attribute vec2 aTextureCoord;
	varying highp vec2 vTextureCoord;
	
	void main() {
	  gl_Position = aVertexPosition;
	  vTextureCoord = aTextureCoord;
	}
	`;
	const fsSource = `
	varying highp vec2 vTextureCoord; 
	uniform sampler2D uSampler;
	void main() {
	  //gl_FragColor = vec4(vTextureCoord.x, vTextureCoord.y, 0.0, 1.0);
	  gl_FragColor = vec4(texture2D(uSampler, vTextureCoord.st).xyz,1.0);
	}
  `;
	const shaderProgram = initShaderProgram(gl, vsSource, fsSource);
	
	const programInfo = {
		program: shaderProgram,
		attribLocations: {
		  vertexPosition: gl.getAttribLocation(shaderProgram, 'aVertexPosition'),
		  textureCoord: gl.getAttribLocation(shaderProgram, 'aTextureCoord'),
		},
	};
	var buffers = initBuffers(gl);
	
	canvasReady();
	var t = setInterval(function() {drawScene(gl, programInfo, buffers)},15);
	//clearInterval(t);
  }
  
}




