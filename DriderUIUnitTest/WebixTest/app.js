var gl; // Un variable global para el contexto WebGL

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

function start() {
  var canvas = document.getElementById("glcanvas");
  gl = initWebGL(canvas);      // Inicializar el contexto GL
  
  // Solo continuar si WebGL esta disponible y trabajando
  
  if (gl) {
    gl.clearColor(0.0, 0.0, 0.0, 1.0);                      // Establecer el color base en negro, totalmente opaco
    gl.enable(gl.DEPTH_TEST);                               // Habilitar prueba de profundidad
    gl.depthFunc(gl.LEQUAL);                                // Objetos cercanos opacan objetos lejanos
    gl.clear(gl.COLOR_BUFFER_BIT|gl.DEPTH_BUFFER_BIT);      // Limpiar el buffer de color asi como el de profundidad
	
	gl.viewport(0, 0, canvas.width, canvas.height);
  }
  
}



function myFunction() {
  console.log("hola");
  lol("caradechango");
}
function addFile(filename) {
  console.log(filename);
  //$$("files").data.add({id:filename, value:  filename, type:   "folder",  date:   666, size: 0}, 0);
}

//webix.ready() function ensures that the code will be executed when the page is loaded
webix.ready(function(){
	webix.markup.init();
	webixReady();
	
	webix.ui({
	container:"toolbarView",
	id:"toolbar",
	view:"toolbar", padding:0,width:1611,margin:0,
	cols: [
		{ view:"button", type:"imageTop", label:"File", 
			image:"../common/imgs/18/save.gif", width:60},
		{ view:"button", type:"imageTop", label:"Project", 
			image:"../common/imgs/18/paste.gif", width:60},
		{ view:"button", type:"imageTop", label:"Help DX",
			image:"../common/imgs/18/copy.gif", width:60}
	]
	});
	
	
	
	
	var propertysheet_1 = {
	view:"property",  id:"sets", width:300,
	elements:[
		{ label:"GameObject", type:"label" },
		{ label:"Width", type:"text", id:"width"},
		{ label:"Height", type:"text", id:"height"},
		{ label:"Material", type:"label" },
		{ label:"Color" },
		{ label:"Background", 	type:"color", id:"col", cols:20, rows:20},
		{ label:"BorderColor", 	type:"color", id:"col2"},
		{ label:"FontColor", 	type:"color", id:"col3", cols:5, rows:5}
	]
	};
	
	webix.ui({
		id: "prop",
		container:"propView",
		cols:[
			{ template:"left" },
			{rows:[
				propertysheet_1
			]}
		]
	});

	$$("sets").setValues({
		width:250,
		height:480,
		url:"https://webix.com/data",
		type:"json",
		col1 : "#46C200",
		col2 :"#07FF2A",
		col3 :"#000000"
	});
	
	
	
	
	
	
	var bigtreedata = [
    {id:"root", value:"Films data", open:true, data:[
		{ id:"1", open:true, value:"The Shawshank Redemption", data:[
				"page1","page2","page3","page4","page5","page6","page7","page8","page9","page10",
				"page11","page12","page13","page14","page15","page16","page17","page18","page19","page20",
				"page21","page22","page23","page24","page25","page26","page27","page28","page29","page30",
				"page31","page32","page33","page34","page35","page36","page37","page38","page39","page40",
				"page41","page42","page43","page44","page45","page46","page47","page48","page49","page50"
		]},
		{ id:"2", open:true, value:"The Godfather", data:[
			{ id:"2.1", value:"Part 1" },
			{ id:"2.2", value:"Part 2" }
		]}
	]}
	];
	treeb = webix.ui({
		container:"treeView",
		view:"tree",
		type:"lineTree",
		id:"tree",
		select:'multiselect',
		drag:true,
		editable:true,
		editor:"text",
		editValue:"value",
		data:bigtreedata
	});	
	//$$("tree").data.add({id: "GameObject", value:  "GameObject"}, 0);
	
	
    //object constructor
    webix.ui({
        view:"filemanager",
        id:"files",
		container: "fman"
		
		//save: "cache->mydata",
		//url: "cache->mydata"
    });
    // loading data source
	/*$$("files").parse([
			{id: "assets", value: "Assets", open: true,  type: "folder", date:  666, data:[
				{ id: "scripts", value: "Scripts", date:  666,  type: "folder", open: true, data:[
	
				]},
				{ id: "textures", value: "Textures", type: "folder", date:  666, open: true, data:[

				]},
				{ id: "sounds", value: "Sounds", type: "folder", date:  666, data:[
					{id: "video1", value: "New Year 2013.avi", icon: "file-video-o", type:"video", date:  666, size: "25030000", pId: "video" },
					{id: "video2", value: "Presentation.avi", icon: "file-video-o",type:"video", date: 666, size: "11072000" , pId: "video"},
					{id: "timg", value: "timg.png", icon: "file-image-o", type:"image", date:  666, size: "31256000", pId: "image" }
				]}
			]}
		]);
	console.log($$("files").data.serialize());*/
	
	$$("files").attachEvent("onBeforeRun",function(id){
			webix.confirm({
				text:"Do you want to download this file?",
				ok:"Yes",
				cancel:"No",
				callback:function(result){
					if(result)
						$$("files").download(id);
				}
			});
			return false;
		});
		
	webix.attachEvent("addFile", function(filename,parent,extension){
		console.log(parent);
		var ftype;
		var fico;
		if (extension == ".html"){
			ftype = "code";
			fico = "file-code-o";
		} else if (extension == ".png") { 
			ftype = "image";
			fico = "file-image-o";
		}
		else if (extension == ".folder") {
			ftype = "folder";
			fico = "file-folder-o";
		}
		
		if (parent == filename) {
			$$("files").data.add({id:filename, value:  filename, type:   ftype, icon: fico, date:   666, size: 0}, 0);
		}	   
		else {
			$$("files").data.add({id:filename, value:  filename, type:   ftype, icon: fico, date:   666, size: 0}, 0,parent);
		}

	});
	
	$$("files").attachEvent("onBeforeCreateFolder",function(id){
		// your code
		return true;
	});
	
});





