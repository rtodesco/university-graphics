var canvas;
var gl;

var program ;

var elapsedTime = 0;
var frameCount = 0;
var lastTime = 0;
var fr = 1;

var arr1 = [];
var arr2 = [];
var arr3 = [];
var arr4 = [];

var tRocket = 0;
var tSun = 0;
var finalSun;

var near = 0.3;
var far = 400.0;
var fovy = 45.0;
var aspect;
var theta = 0.005;

var left = -6.0;
var right = 6.0;
var ytop =6.0;
var bottom = -6.0;

var points;
var colours;
var reg;

var lightPosition2 = vec4(100.0, 100.0, 100.0, 1.0 );
var lightPosition = vec4(0.0, 0.0, 100.0, 1.0 );

var lightAmbient = vec4(0.2, 0.2, 0.2, 1.0 );
var lightDiffuse = vec4( 1.0, 1.0, 1.0, 1.0 );
var lightSpecular = vec4( 1.0, 1.0, 1.0, 1.0 );

var materialAmbient = vec4( 1.0, 0.0, 1.0, 1.0 );
var materialDiffuse = vec4( 1.0, 0.8, 0.0, 1.0 );
var materialSpecular = vec4( 0.4, 0.4, 0.4, 1.0 );
var materialShininess = 30.0;

var ambientColor, diffuseColor, specularColor;

var modelMatrix, viewMatrix ;
var modelViewMatrix, projectionMatrix, normalMatrix;
var modelViewMatrixLoc, projectionMatrixLoc, normalMatrixLoc;
var eye = vec3(30,30,85);
var at = vec3(0.0, 0.0, 0.0);
var up = vec3(0.0, 1.0, 0.0);

var RX = 0 ;
var RY = 0 ;
var RZ = 0 ;

var MS = [] ; // The modeling matrix stack
var TIME = 0.0 ; // Realtime
var TIME = 0.0 ; // Realtime
var resetTimerFlag = true ;
var animFlag = false ;
var prevTime = 0.0 ;
var useTextures = 1 ;

// ------------ Images for textures stuff --------------
var texSize = 64;

var image1 = new Array()
for (var i =0; i<texSize; i++)  image1[i] = new Array();
for (var i =0; i<texSize; i++)
for ( var j = 0; j < texSize; j++)
image1[i][j] = new Float32Array(4);
for (var i =0; i<texSize; i++) for (var j=0; j<texSize; j++) {
    var c = (((i & 0x8) == 0) ^ ((j & 0x8)  == 0));
    image1[i][j] = [c, c, c, 1];
}

// Convert floats to ubytes for texture

var image2 = new Uint8Array(4*texSize*texSize);

for ( var i = 0; i < texSize; i++ )
for ( var j = 0; j < texSize; j++ )
for(var k =0; k<4; k++)
image2[4*texSize*i+4*j+k] = 255*image1[i][j][k];


var textureArray = [] ;



function isLoaded(im) {
    if (im.complete) {
        console.log("loaded") ;
        return true ;
    }
    else {
        console.log("still not loaded!!!!") ;
        return false ;
    }
}

function loadFileTexture(tex, filename)
{
    tex.textureWebGL  = gl.createTexture();
    tex.image = new Image();
    tex.image.src = filename ;
    tex.isTextureReady = false ;
    tex.image.onload = function() { handleTextureLoaded(tex); }
    // The image is going to be loaded asyncronously (lazy) which could be
    // after the program continues to the next functions. OUCH!
}

function loadImageTexture(tex, image) {
    tex.textureWebGL  = gl.createTexture();
    tex.image = new Image();
    //tex.image.src = "CheckerBoard-from-Memory" ;
    
    gl.bindTexture( gl.TEXTURE_2D, tex.textureWebGL );
    //gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, texSize, texSize, 0,
                  gl.RGBA, gl.UNSIGNED_BYTE, image);
    gl.generateMipmap( gl.TEXTURE_2D );
    gl.texParameteri( gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER,
                     gl.NEAREST_MIPMAP_LINEAR );
    gl.texParameteri( gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST );
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE); //Prevents s-coordinate wrapping (repeating)
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE); //Prevents t-coordinate wrapping (repeating)
    gl.bindTexture(gl.TEXTURE_2D, null);

    tex.isTextureReady = true ;

}

function initTextures() {
    
    textureArray.push({}) ;
    loadFileTexture(textureArray[textureArray.length-1],"sun.jpg") ;
	
    textureArray.push({}) ;
    loadFileTexture(textureArray[textureArray.length-1],"mercury.jpg") ;
	
    textureArray.push({}) ;
    loadFileTexture(textureArray[textureArray.length-1],"venus.jpg") ;
	
	textureArray.push({}) ;
    loadFileTexture(textureArray[textureArray.length-1],"earth.jpg") ;
	
	textureArray.push({}) ;
    loadFileTexture(textureArray[textureArray.length-1],"moon.jpg") ;
	
	textureArray.push({}) ;
    loadFileTexture(textureArray[textureArray.length-1],"mars.jpg") ;
	
	textureArray.push({}) ;
    loadFileTexture(textureArray[textureArray.length-1],"jupiter.jpg") ;
	
	textureArray.push({}) ;
    loadFileTexture(textureArray[textureArray.length-1],"saturn.jpg") ;
	
	textureArray.push({}) ;
    loadFileTexture(textureArray[textureArray.length-1],"saturn_ring.jpg") ;
	
	textureArray.push({}) ;
    loadFileTexture(textureArray[textureArray.length-1],"uranus.jpg") ;
	
	textureArray.push({}) ;
    loadFileTexture(textureArray[textureArray.length-1],"neptune.jpg") ;
}


function handleTextureLoaded(textureObj) {
    gl.bindTexture(gl.TEXTURE_2D, textureObj.textureWebGL);
	gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true); // otherwise the image would be flipped upsdide down
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, textureObj.image);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_NEAREST);
    gl.generateMipmap(gl.TEXTURE_2D);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE); //Prevents s-coordinate wrapping (repeating)
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE); //Prevents t-coordinate wrapping (repeating)
    gl.bindTexture(gl.TEXTURE_2D, null);
    console.log(textureObj.image.src) ;
    
    textureObj.isTextureReady = true ;
}

//----------------------------------------------------------------

function setColor(c)
{
    ambientProduct = mult(lightAmbient, c);
    diffuseProduct = mult(lightDiffuse, c);
    specularProduct = mult(lightSpecular, materialSpecular);
    
    gl.uniform4fv( gl.getUniformLocation(program,
                                         "ambientProduct"),flatten(ambientProduct) );
    gl.uniform4fv( gl.getUniformLocation(program,
                                         "diffuseProduct"),flatten(diffuseProduct) );
    gl.uniform4fv( gl.getUniformLocation(program,
                                         "specularProduct"),flatten(specularProduct) );
    gl.uniform4fv( gl.getUniformLocation(program,
                                         "lightPosition"),flatten(lightPosition) );
    gl.uniform1f( gl.getUniformLocation(program, 
                                        "shininess"),materialShininess );
}

function toggleTextures() {
    useTextures = 1 - useTextures ;
    gl.uniform1i( gl.getUniformLocation(program,
                                         "useTextures"), useTextures );
}

function waitForTextures1(tex) {
    setTimeout( function() {
    console.log("Waiting for: "+ tex.image.src) ;
    wtime = (new Date()).getTime() ;
    if( !tex.isTextureReady )
    {
        console.log(wtime + " not ready yet") ;
        waitForTextures1(tex) ;
    }
    else
    {
        console.log("ready to render") ;
        window.requestAnimFrame(render);
    }
               },5) ;
    
}

// Takes an array of textures and calls render if the textures are created
function waitForTextures(texs) {
    setTimeout( function() {
               var n = 0 ;
               for ( var i = 0 ; i < texs.length ; i++ )
               {
                    console.log("boo"+texs[i].image.src) ;
                    n = n+texs[i].isTextureReady ;
               }
               wtime = (new Date()).getTime() ;
               if( n != texs.length )
               {
               console.log(wtime + " not ready yet") ;
               waitForTextures(texs) ;
               }
               else
               {
               console.log("ready to render") ;
               window.requestAnimFrame(render);
               }
               },5) ;
    
}

window.onload = function init() {

    canvas = document.getElementById( "gl-canvas" );
    
    gl = WebGLUtils.setupWebGL( canvas );
	aspect =  canvas.width/canvas.height;
    if ( !gl ) { alert( "WebGL isn't available" ); }

    gl.viewport( 0, 0, canvas.width, canvas.height );
    gl.clearColor( 0.5, 0.5, 1.0, 1.0 );
    
    gl.enable(gl.DEPTH_TEST);

    //
    //  Load shaders and initialize attribute buffers
    //
    program = initShaders( gl, "vertex-shader", "fragment-shader" );
    gl.useProgram( program );
    
 
    // Load canonical objects and their attributes
    Cube.init(program);
    Cylinder.init(9,program);
    Cone.init(9,program) ;
    Sphere.init(36,program) ;

    gl.uniform1i( gl.getUniformLocation(program, "useTextures"), useTextures );

    // record the locations of the matrices that are used in the shaders
    modelViewMatrixLoc = gl.getUniformLocation( program, "modelViewMatrix" );
    normalMatrixLoc = gl.getUniformLocation( program, "normalMatrix" );
    projectionMatrixLoc = gl.getUniformLocation( program, "projectionMatrix" );
    
    // set a default material
    setColor(materialDiffuse) ;
	
	//Get start time for fps counter
	lastTime = new Date().getTime();
	
	//Calculate random numbers for stars
	for(var i = 0; i < 50; i++) {
		//size
		arr1.push((Math.random() * (1.5 - 0.5)) + 0.5);
		//speed
		arr1.push((Math.random() * (5 - 1)) + 1);
		//x or z
		arr1.push(Math.floor(Math.random() * (90 - -90)) + -90);
		//y
		arr1.push(Math.floor(Math.random() * (70 - -120)) + -120);
		//size
		arr2.push((Math.random() * (1.5 - 0.5)) + 0.5);
		//speed
		arr2.push((Math.random() * (5 - 1)) + 1);
		//x or z
		arr2.push(Math.floor(Math.random() * (90 - -90)) + -90);
		//y
		arr2.push(Math.floor(Math.random() * (70 - -120)) + -120);
		//size
		arr3.push((Math.random() * (1.5 - 0.5)) + 0.5);
		//speed
		arr3.push((Math.random() * (5 - 1)) + 1);
		//x or z
		arr3.push(Math.floor(Math.random() * (90 - -90)) + -90);
		//y
		arr3.push(Math.floor(Math.random() * (70 - -120)) + -120);
		//size
		arr4.push((Math.random() * (1.5 - 0.5)) + 0.5);
		//speed
		arr4.push((Math.random() * (5 - 1)) + 1);
		//x or z
		arr4.push(Math.floor(Math.random() * (90 - -90)) + -90);
		//y
		arr4.push(Math.floor(Math.random() * (70 - -120)) + -120);
	}
    
    // set the callbacks for the UI elements
    document.getElementById("sliderXi").oninput = function() {
        RX = this.value ;
        window.requestAnimFrame(render);
    };
    document.getElementById("sliderYi").oninput = function() {
        RY = this.value;
        window.requestAnimFrame(render);
    };
    document.getElementById("sliderZi").oninput = function() {
        RZ =  this.value;
        window.requestAnimFrame(render);
    };
    
    document.getElementById("animToggleButton").onclick = function() {
        if( animFlag ) {
            animFlag = false;
        }
        else {
            animFlag = true  ;
            resetTimerFlag = true ;
            window.requestAnimFrame(render);
        }
    };
    
    document.getElementById("textureToggleButton").onclick = function() {
        toggleTextures() ;
        window.requestAnimFrame(render);
    };

    var controller = new CameraController(canvas);
    controller.onchange = function(xRot,yRot) {
        RX = xRot ;
        RY = yRot ;
        window.requestAnimFrame(render); };
    
    // load and initialize the textures
    initTextures() ;
    
    // Recursive wait for the textures to load
    waitForTextures(textureArray) ;
    //setTimeout (render, 100) ;
    
}

// Sets the modelview and normal matrix in the shaders
function setMV() {
    modelViewMatrix = mult(viewMatrix,modelMatrix) ;
    gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix) );
    normalMatrix = inverseTranspose(modelViewMatrix) ;
    gl.uniformMatrix4fv(normalMatrixLoc, false, flatten(normalMatrix) );
}

// Sets the projection, modelview and normal matrix in the shaders
function setAllMatrices() {
    gl.uniformMatrix4fv(projectionMatrixLoc, false, flatten(projectionMatrix) );
    setMV() ;
    
}

// Draws a 2x2x2 cube center at the origin
// Sets the modelview matrix and the normal matrix of the global program
function drawCube() {
    setMV() ;
    Cube.draw() ;
}

// Draws a sphere centered at the origin of radius 1.0.
// Sets the modelview matrix and the normal matrix of the global program
function drawSphere() {
    setMV() ;
    Sphere.draw() ;
}
// Draws a cylinder along z of height 1 centered at the origin
// and radius 0.5.
// Sets the modelview matrix and the normal matrix of the global program
function drawCylinder() {
    setMV() ;
    Cylinder.draw() ;
}

// Draws a cone along z of height 1 centered at the origin
// and base radius 1.0.
// Sets the modelview matrix and the normal matrix of the global program
function drawCone() {
    setMV() ;
    Cone.draw() ;
}

// Post multiples the modelview matrix with a translation matrix
// and replaces the modelview matrix with the result
function gTranslate(x,y,z) {
    modelMatrix = mult(modelMatrix,translate([x,y,z])) ;
}

// Post multiples the modelview matrix with a rotation matrix
// and replaces the modelview matrix with the result
function gRotate(theta,x,y,z) {
    modelMatrix = mult(modelMatrix,rotate(theta,[x,y,z])) ;
}

// Post multiples the modelview matrix with a scaling matrix
// and replaces the modelview matrix with the result
function gScale(sx,sy,sz) {
    modelMatrix = mult(modelMatrix,scale(sx,sy,sz)) ;
}

// Pops MS and stores the result as the current modelMatrix
function gPop() {
    modelMatrix = MS.pop() ;
}

// pushes the current modelMatrix in the stack MS
function gPush() {
    MS.push(modelMatrix) ;
}

function render() {
    gl.useProgram(program);
    gl.clear( gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    
    // set the projection matrix
    projectionMatrix = perspective(fovy,aspect,near,far);
    
    // set the camera matrix
    viewMatrix = lookAt(eye, at , up);
    
    // initialize the modeling matrix stack
    MS= [] ;
    modelMatrix = mat4() ;
    
    // apply the slider rotations
    gRotate(RZ,0,0,1) ;
    gRotate(RY,0,1,0) ;
    gRotate(RX,1,0,0) ;
    
    // send all the matrices to the shaders
    setAllMatrices() ;
    
    // get real time
    var curTime ;
    if( animFlag )
    {
        curTime = (new Date()).getTime() /1000 ;
        if( resetTimerFlag ) {
            prevTime = curTime ;
            resetTimerFlag = false ;
        }
        TIME = TIME + curTime - prevTime ;
        prevTime = curTime ;
    }

	//Sun
	gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_2D, textureArray[0].textureWebGL);
	gl.uniform1i(gl.getUniformLocation(program, "texture0"),0);
	gPush();
	{	
		setColor(vec4(1,1,0,1.0));
		gRotate(90,1,0,0);
		if (TIME > 41) {
			gScale(finalSun,finalSun,finalSun);
		}
		else if (TIME > 7) {
			gScale(10-tSun,10-tSun,10-tSun);
			finalSun = 10-tSun;
			tSun += 0.05;
		}
		else {
			gScale(10,10,10);
		}
		
		drawSphere();
	}
	gPop();
	
	//Mercury
	gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_2D, textureArray[1].textureWebGL);
	gl.uniform1i(gl.getUniformLocation(program, "texture1"),0);
	drawPlanet(vec4(0.7,0.7,0.7,1.0),12,0.5,0.02,4.1,0);
	gPop();
	
	//Venus
	gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_2D, textureArray[2].textureWebGL);
	gl.uniform1i(gl.getUniformLocation(program, "texture2"),0);
	drawPlanet(vec4(1,0.5,0,1),14,0.75,0.004,1.6,45);
	gPop();
	
	//Earth
	gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_2D, textureArray[3].textureWebGL);
	gl.uniform1i(gl.getUniformLocation(program, "texture3"),0);
	drawPlanet(vec4(0,0,1,1),16,0.75,1,1,90);
	
	//Moon
	gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_2D, textureArray[4].textureWebGL);
	gl.uniform1i(gl.getUniformLocation(program, "texture4"),0);
	drawMoon();
	gPop();
	
	
	//Mars
	gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_2D, textureArray[5].textureWebGL);
	gl.uniform1i(gl.getUniformLocation(program, "texture5"),0);
	drawPlanet(vec4(1,0,0,1),18,0.6,0.9,0.5,135);
	gPop();
	
	//Jupiter
	gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_2D, textureArray[6].textureWebGL);
	gl.uniform1i(gl.getUniformLocation(program, "texture6"),0);
	drawPlanet(vec4(1,0.5,0,1),21.3,2.5,2.4,0.08,180);
	gPop();
	
	//Saturn
	gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_2D, textureArray[7].textureWebGL);
	gl.uniform1i(gl.getUniformLocation(program, "texture7"),0);
	drawPlanet(vec4(1,0.5,0,1),26.4,2,2.2,0.03,225);
	//Rings
	gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_2D, textureArray[8].textureWebGL);
	gl.uniform1i(gl.getUniformLocation(program, "texture8"),0);
	drawRing();
	gPop();
	
	//Uranus
	gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_2D, textureArray[9].textureWebGL);
	gl.uniform1i(gl.getUniformLocation(program, "texture9"),0);
	drawPlanet(vec4(0,0.8,0.8,1),30,1.3,1.4,0.01,270)
	gPop();
	
	//Neptune
	gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_2D, textureArray[10].textureWebGL);
	gl.uniform1i(gl.getUniformLocation(program, "texture10"),0);
	drawPlanet(vec4(0,0,1,1),33,1.3,1.5,0.006,315)
	gPop();

	//No Texture Section
	var temp = useTextures;
	useTextures = 0;
	reg = 0;
    gl.uniform1i( gl.getUniformLocation(program,"useTextures"), useTextures );
	gl.uniform1i( gl.getUniformLocation(program,"regular"), reg );
	
	//Star Section	
	for(var i = 0; i < 200; i+= 4) {
		drawStar(arr1[i],arr1[i+1],arr1[i+2],arr1[i+3],-95);
		drawStar(arr2[i],arr2[i+1],arr2[i+2],arr2[i+3],95);
		drawStar(arr3[i],arr3[i+1],-95,arr3[i+3],arr3[i+2]);
		drawStar(arr4[i],arr4[i+1],95,arr4[i+3],arr4[i+2]);
	}

	//Regular Section
	reg = 1;
	gl.uniform1i( gl.getUniformLocation(program,"regular"), reg );
	//Space
	gPush();
	{
		setColor(vec4(0,0,0,1));
		gScale(200,200,200);
		drawSphere();
	}
	gPop();
	
	if(TIME > 11) {
		drawRocket();
	}
	
	useTextures = temp;
    gl.uniform1i( gl.getUniformLocation(program,"useTextures"), useTextures );
	
	var x = eye[0]
	eye[0] = x*Math.cos(theta) + eye[2]*Math.sin(theta);
	eye[2] = -x*Math.sin(theta) + eye[2]*Math.cos(theta);
	
	framerate();
	
    if( animFlag )
        window.requestAnimFrame(render);
}

// A simple camera controller which uses an HTML element as the event
// source for constructing a view matrix. Assign an "onchange"
// function to the controller as follows to receive the updated X and
// Y angles for the camera:
//
//   var controller = new CameraController(canvas);
//   controller.onchange = function(xRot, yRot) { ... };
//
// The view matrix is computed elsewhere.
function CameraController(element) {
    var controller = this;
    this.onchange = null;
    this.xRot = 0;
    this.yRot = 0;
    this.scaleFactor = 3.0;
    this.dragging = false;
    this.curX = 0;
    this.curY = 0;
    
    // Assign a mouse down handler to the HTML element.
    element.onmousedown = function(ev) {
        controller.dragging = true;
        controller.curX = ev.clientX;
        controller.curY = ev.clientY;
    };
    
    // Assign a mouse up handler to the HTML element.
    element.onmouseup = function(ev) {
        controller.dragging = false;
    };
    
    // Assign a mouse move handler to the HTML element.
    element.onmousemove = function(ev) {
        if (controller.dragging) {
            // Determine how far we have moved since the last mouse move
            // event.
            var curX = ev.clientX;
            var curY = ev.clientY;
            var deltaX = (controller.curX - curX) / controller.scaleFactor;
            var deltaY = (controller.curY - curY) / controller.scaleFactor;
            controller.curX = curX;
            controller.curY = curY;
            // Update the X and Y rotation angles based on the mouse motion.
            controller.yRot = (controller.yRot + deltaX) % 360;
            controller.xRot = (controller.xRot + deltaY);
            // Clamp the X rotation to prevent the camera from going upside
            // down.
            if (controller.xRot < -90) {
                controller.xRot = -90;
            } else if (controller.xRot > 90) {
                controller.xRot = 90;
            }
            // Send the onchange event to any listener.
            if (controller.onchange != null) {
                controller.onchange(controller.xRot, controller.yRot);
            }
        }
    };
}

function drawPlanet(color,distance,size,day,year,start) {
	gPush();
	{
		setColor(color);
		gTranslate(distance,0,0);
		gTranslate(-distance,0,0);
		gRotate(((-TIME*180/3.14159)*year)+start,0,1,0);
		gTranslate(distance,0,0);
		gRotate((-TIME*180/3.14159)*day,0,1,0);
		gRotate(90,1,0,0);
		gScale(size,size,size);
		drawSphere();
	}
}

function drawRing() {
	gPush();
	{
		setColor(vec4(1,1,0,1));
		gRotate(30,1,0,0);
		gScale(1.5,0.1,1.5);
		drawSphere();
	}
	gPop();
	
}

function drawMoon() {
	gPush();
	{
		setColor(vec4(1,1,1,1));
		gTranslate(1.3,0,0);
		gTranslate(-1.3,0,0);
		gRotate((-TIME*180/3.14159)*10,0,1,0);
		gTranslate(1.3,0,0);
		gScale(0.3,0.3,0.3);
		drawSphere();
	}
	gPop()
}

function drawStar(size,speed,x,y,z) {
	
	var intensity = Math.cos(speed*TIME);
	gl.uniform1f( gl.getUniformLocation(program,"intensity"), intensity);
	gPush();
	{
		setColor(vec4(0,0,0,1));
		gTranslate(x,y,z);
		gScale(size,size,size)
		drawSphere();
	}
	gPop();
}

function drawRocket() {
	//Rocket Body
	gPush();
	{
		setColor(vec4(1,1,1,1));
		gTranslate((1-tRocket)*-16 + tRocket*eye[0],tRocket*eye[1],tRocket*eye[2]);
		tRocket += 0.001;
		gRotate(TIME*10.8,0,1,0);
		gScale(0.5,0.5,0.5);
		drawCylinder();
	}
	//Rocket Head
	gPush();
	{	
		
		gTranslate(0.4,0,0.3);
		gRotate(180,1,0,0);
		gScale(0.25,0.25,1);
		drawCone();
	}
	gPop();
	//Rocket Thruster 1
	gPush();
	{	
		
		gTranslate(-0.4,0,0.3);
		gRotate(180,1,0,0);
		gScale(0.25,0.25,1);
		drawCone();
	}
	gPop();
	//Rocket Thruster 2
	gPush();
	{	
		
		gTranslate(0,0.4,0.3);
		gRotate(180,1,0,0);
		gScale(0.25,0.25,1);
		drawCone();
	}
	gPop();
	//Rocket Thruster 3
	gPush();
	{	
		
		gTranslate(0,-0.4,0.3);
		gRotate(180,1,0,0);
		gScale(0.25,0.25,1);
		drawCone();
	}
	gPop();
	//Rocket Thruster 4
	gPush();
	{
		setColor(vec4(1,0,0,1));
		gTranslate(0,0,-1);
		gRotate(180,1,0,0);
		gScale(0.5,0.5,1);
		drawCone();
	}
	gPop();
	gPop();
}

function framerate() {
	var now = new Date().getTime();
	frameCount++;
	elapsedTime += (now - lastTime);
	lastTime = now;
	
	if(elapsedTime >= 1000) {
		fps = frameCount;
		frameCount = 0;
		elapsedTime -= 1000;
		if(fr == 1) {
			console.log("fps: " + fps);
		}
		fr = 1 - fr;
	}
}