
// ==================================================================================================================
//                                                  C app
// ==================================================================================================================
function JS_AddSceneGraphNode(name,pname) {
	//console.log(name);
	webix.callEvent('WEBIX_AddSceneGraphNode',[name,pname]);
}
function JS_ClearSceneGraphTree() {
	webix.callEvent('WEBIX_ClearSceneGraphTree');
}
function JS_ChangeGMOFocus(name,posX,posY,posZ,scX,scY,scZ,rtX,rtY,rtZ) {
	webix.callEvent('WEBIX_ChangeGMOPropFocus',[name,posX,posY,posZ,scX,scY,scZ,rtX,rtY,rtZ]);
}
function JS_AddComponent_Render(objName,componentName) {
	webix.callEvent('WEBIX_AddComponent_Render',[objName,componentName]); 
}
function JS_AddComponent_Script(componentName, modelName) {
	webix.callEvent('WEBIX_AddComponent_Script',[componentName,modelName]); 
}
function JS_ClearPropertySheetUI() {
	webix.callEvent('WEBIX_ClearPropertySheetUI');
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

function getViewport(el) {
  var viewport = el.getBoundingClientRect();
  return {
	top: viewport.top,
    left: viewport.left,
	width: viewport.width,
	height: viewport.height,
  };
}

document.addEventListener('contextmenu', event => event.preventDefault());

//===================================================================================================================
//                                                  WEBIX
// ==================================================================================================================
//webix.ready() function ensures that the code will be executed when the page is loaded
webix.ready(function(){
	webix.markup.init();
	
	
    //=======================================================================================================
	//                                             Toobar
	//=======================================================================================================
	var toolbar_UI = {
	id:"toolbar",
	view:"toolbar", padding:0,width:1611,margin:0,
	cols: [
		{ view:"button", type:"imageTop", label:"File", 
			image:"../common/imgs/18/save.gif", width:60},
		{ view:"button", type:"imageTop", label:"Project", 
			image:"../common/imgs/18/paste.gif", width:60},
		{ view:"button", type:"imageTop", label:"Help",
			image:"../common/imgs/18/copy.gif", width:60},
		{ view:"button", type:"imageTop", label:"View",
			image:"../common/imgs/18/copy.gif", width:60},
		{ view:"button", type:"imageTop", label:"Play",
			image:"../common/imgs/18/copy.gif", width:60,margin:200}
	]
	};
	
	
	

	
	//=======================================================================================================
	//                                          Propertysheet
	//=======================================================================================================
	
	var transform_UI = {
	cols:[
		{
			view:"fieldset", id:"myform1", label:"Propiedades", drag:true,body:{
			rows:[
				{rows:[
				    { template:"Name", type:"header", id: "prop_title" },
					{ type:"section", template:" Scale" },
					{ height: 30, margin:0,padding:0, cols:[
						{ view:"text",   label:"x", inputWidth:80 , labelWidth:20, inputHeight:24,autowidth:false ,id:"scX"},
						{ view:"text",   label:"y", inputWidth:80 , labelWidth:20, inputHeight:24,autowidth:false ,id:"scY"},
						{ view:"text",   label:"z", inputWidth:80 , labelWidth:20, inputHeight:24,autowidth:false ,id:"scZ"},
					]},
					{ type:"section", template:" Rotation" },
					{ height: 30, margin:0,padding:0, cols:[
						{ view:"text",   label:"x", inputWidth:80 , labelWidth:20, inputHeight:24,autowidth:false ,id:"rtX"},
						{ view:"text",   label:"y", inputWidth:80 , labelWidth:20, inputHeight:24,autowidth:false ,id:"rtY"},
						{ view:"text",   label:"z", inputWidth:80 , labelWidth:20, inputHeight:24,autowidth:false ,id:"rtZ"},
					]},
					{ type:"section", template:" Position" },
					{ height: 30, margin:0,padding:0, cols:[
						{ view:"text",   label:"x", inputWidth:80 , labelWidth:20, inputHeight:24,autowidth:false ,id:"posX",drag:true,},
						{ view:"text",   label:"y", inputWidth:80 , labelWidth:20, inputHeight:24,autowidth:false ,id:"posY"},
						{ view:"text",   label:"z", inputWidth:80 , labelWidth:20, inputHeight:24,autowidth:false ,id:"posZ"},
					]},
				]}
			]
		}
		}
	],
	margin:0,
	padding:0,
	id:"propTransform"
	};

		
	var prop_UI = {
		//container:"propView",
		margin:0,
		padding:0,
		id:"prop",
		rows:[]
	};
	/*var model_options = [
	  {id:1, value:"Sphere"},
	];*/
	
	
	webix.attachEvent("WEBIX_ClearPropertySheetUI", function(){
		$$("prop").reconstruct();
	});
	webix.attachEvent("WEBIX_AddComponent_Render", function(compName,modelName){
	    $$("prop").addView(
		{
		rows:[{
			view:"fieldset", label:"Render Component", body:{
				rows:[
				    { value: compName, view:"text", id: "prop_comp_" + compName},
					{ value: modelName,view:"text",   label:"Model", inputWidth:0 , labelWidth:80, inputHeight:24,autowidth:false ,id:"aaaaaaa" + compName},
					{ view:"text",   label:"Material", inputWidth:0 , labelWidth:80, inputHeight:24,autowidth:false ,id:"bbbbbbb" + compName},
				]}
		}
	]}, 0);
	});
	webix.attachEvent("WEBIX_AddComponent_Script", function(objName,compName){
		console.log(compName);
		//{ label: "Model", type:"combo", options:model_options, id:"model_prop"+ "modelName"},
	});
	
	webix.attachEvent("WEBIX_ChangeGMOPropFocus", function(name,posX,posY,posZ,scX,scY,scZ,rtX,rtY,rtZ){
		$$("prop_title").define("template",name);
		$$("prop_title").refresh();
		
	    $$("scX").define("value",scX);
		$$("scX").refresh();
		$$("scY").define("value",scY);
		$$("scY").refresh();
		$$("scZ").define("value",scZ);
		$$("scZ").refresh();

		$$("posX").define("value",posX);
		$$("posX").refresh();
	    $$("posY").define("value",posY);
		$$("posY").refresh();
		$$("posZ").define("value",posZ);
		$$("posZ").refresh();
		
		$$("rtX").define("value",rtX);
		$$("rtX").refresh();
		$$("rtY").define("value",rtY);
		$$("rtY").refresh();
		$$("rtZ").define("value",rtZ);
		$$("rtZ").refresh();
	});
	
	var propertysheet_1 = {
	view:"property",  id:"sets", width:300,
	elements:[
		{ label:"Transform", type:"label" },
		{ label:"Scale", type:"text", id:"scale"},
		{ label:"Height", type:"text", id:"height"},
		{ label:"Material", type:"label" },
		{ label:"Color" },
		{ label:"Background", 	type:"color", id:"col", cols:20, rows:20},
		{ label:"BorderColor", 	type:"color", id:"col2"},
		{ label:"FontColor", 	type:"color", id:"col3", cols:5, rows:5},
		{ label:"asd", 	type:"textg"}
	]
	};
	
	//=======================================================================================================
	//                                           Scene Graph
	//=======================================================================================================
	webix.protoUI({
		name:"edittree",
	}, webix.EditAbility, webix.ui.tree);
	var treeb_UI = {
		//container:"treeView",
		clipboard: "insert",
		view:"edittree",
		type:"lineTree",
		id:"tree",
		select:'multiselect',
		drag:true,
		editable:true,
		editor:"text",
		editValue:"value",
		editaction:"custom",
		onContext:{},
		on:{
			onBeforeDrag:function(context,native_event){
				if("ROOT_NODE_X" == context.source[0]) return false;
				return true;
			},
			onBeforeDrop:function(context){

				//drop as the first child
				//context.parent = context.target;
				//context.index = 0;
				//TODO: for all selected items
				var src = context.source[0];
				C_ChangeNodeParent(src,context.target);
			},
			onSelectChange:function () {
				//this.getItem(this.getSelectedId(true)[0]).value
				C_ChangeSceneGraphNodeSelection(this.getSelectedId(true)[0]);
			}
			
			
		},
	};	

	//$$("tree").data.add({id: "ROOT_NODE_X", value:  "ROOT_NODE_X", icon:"home"}, 0);
	webix.attachEvent("WEBIX_AddSceneGraphNode", function(name,parentName){
		if (name == "ROOT_NODE_X"){
			$$("tree").data.add({id: name, value:  name, open: true}, 0);
		}else {
			$$("tree").data.add({id: name, value:  name, open: true}, 0, parentName);
		}
	});
	
	
	webix.attachEvent("WEBIX_ClearSceneGraphTree", function(){
		$$("tree").clearAll();
	});

	//=======================================================================================================
	//                                           File Manager
	//=======================================================================================================
    var fman_UI = {
        view:"filemanager",
        id:"files",
		//container: "fman"
		//save: "cache->mydata",
		//url: "cache->mydata"
    };
	

		
	webix.attachEvent("WEBIX_AddFile", function(filename,parent,extension){
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

	
	
	
	var editorCells = [
		{
			header:"Scene",
			id:"editor_scene",
			rows:[],
			onContext:{},
			body:{ view:"edittree",id:"editor_grid",
					drag:true,
			}
			
		},
		{
			header:"Game",
			body:{ template:"Game" }
		}
	];

	var editor_UI = {
		view:"tabview",
		animate:false,
		tabbar:{
			close:true
		},
		cells:editorCells,
	};
	
	var propCells =[
	{
		header:"Object Editor",
		body:{ rows: [transform_UI,prop_UI] }
	},
	{
		header:"Material Editor",
		body:{ template:"Material Editor" }
	},
	{
		header:"Texture Editor",
		body:{ template:"Texture Editor" }
	}
	];

	var propCells_UI = {
		view:"tabview",
		animate:false,
		tabbar:{
			close:false
		},
		cells:propCells
	};
	
	var grid = {
	view:"dashboard", id:"grid",
	gridColumns:4, gridRows:4,
	cellHeight: 650/4, cellWidth: 1220/4,
	padding:6, margin:6,
	 cells:[
		{ view:"panel", x:3, y:0, dx:1, dy:4,resize:true, body:{view:"scrollview",body:propCells_UI},onContext:{},id:"prop_grid"},
		{ view:"panel", x:2, y:0, dx:1, dy:3,resize:true, header:"Scene Graph", body:{view:"scrollview",body:treeb_UI, onContext:{},id:"tree_grid"} },
		{ view:"panel", x:0, y:3, dx:3, dy:1,resize:true, body:fman_UI},
		{ view:"panel", x:0, y:0, dx:2, dy:3,resize:true,  body: editor_UI},
	],
	onContext:{},
	};
	
	webix.ui({view:"scrollview", body: {rows:[toolbar_UI,grid]}});
	
	$$("grid").attachEvent("onChange",function(){
		var pos = getViewport($$("editor_grid").$view);
		C_SetSceneAreaViewport(pos.top,pos.left,pos.width,pos.height);
	});
	
	//Property sheet
	webix.ui({
		view:"contextmenu",
		id:"general_prop_ctx_menu",
		data:[{value:"Add Component" ,submenu: {
		data:[ 
		"Render", "Animator", "Script", "Collider"],
		id:"general_prop_ctx_menu_sub",
		view:"contextmenu",
		},
		config:{
			on:{
			onItemClick:function(id){
				console.log(this.getItem(id).value);

				if (this.getItem(id).value == "Render"){
					C_AddRenderComponent("default");
				}
			}
			},
		}
		}],
		master:$$("prop_grid").$view,
		openAction:"click",

	});
	
	//Tree View
	webix.ui({
	view:"contextmenu",
	id:"item_tree_ctx_menu",
	data:["Rename",{ $template:"Separator" },"Delete"],
	master:$$("tree"),
	on:{
		onItemClick:function(id){
			var context = this.getContext();
			var list = context.obj;
			var listId = context.id;
			webix.message(this.getItem(id).value);
			if (this.getItem(id).value == "Delete"){
				if("ROOT_NODE_X" == list.getItem(listId).id) return;
				C_DeleteSceneGraphNode(list.getItem(listId).id);
			}else if (this.getItem(id).value == "Rename"){
				$$("tree").edit(list.getItem(listId).id);
			}
		}
	}
    });
		
    webix.ui({
        view:"contextmenu",
	    id:"general_tree_ctx_menu",
        data:["Add GameObject",{ $template:"Separator" },"Start WW3"],
		master:$$("tree_grid").$view,
        on:{
            onItemClick:function(id){
				if (this.getItem(id).value == "Add GameObject"){
					var name = "GameObject";
					var count = 0;
					while ($$("tree").exists(name + "_" +count)) { count++;}
					C_AddSceneGraphNode(name + "_" + count, "ROOT_NODE_X");
					//$$("tree").data.add({id: "GameObject", value:  "GameObject", icon:"home"}, 0, "ROOT_NODE_X");
				}

            }
        }
    });
	//Position
	$$("posX").attachEvent("onChange", function(newv, oldv){
		webix.message("Value changed from: "+oldv+" to: "+newv);
		C_ChangeXPos(parseInt(newv));
	});
	$$("posY").attachEvent("onChange", function(newv, oldv){
		webix.message("Value changed from: "+oldv+" to: "+newv);
		C_ChangeYPos(parseInt(newv));
	});
	$$("posZ").attachEvent("onChange", function(newv, oldv){
		webix.message("Value changed from: "+oldv+" to: "+newv);
		C_ChangeZPos(parseInt(newv));
	});
	
	$$("scX").attachEvent("onChange", function(newv, oldv){
		webix.message("Value changed from: "+oldv+" to: "+newv);
		C_ChangeXScale(parseInt(newv));
	});
	$$("scY").attachEvent("onChange", function(newv, oldv){
		webix.message("Value changed from: "+oldv+" to: "+newv);
		C_ChangeYScale(parseInt(newv));
	});
	$$("scZ").attachEvent("onChange", function(newv, oldv){
		webix.message("Value changed from: "+oldv+" to: "+newv);
		C_ChangeZScale(parseInt(newv));
	});
	
	
	$$("rtX").attachEvent("onChange", function(newv, oldv){
		webix.message("Value changed from: "+oldv+" to: "+newv);
		C_ChangeXRot(parseInt(newv));
	});
	$$("rtY").attachEvent("onChange", function(newv, oldv){
		webix.message("Value changed from: "+oldv+" to: "+newv);
		C_ChangeYRot(parseInt(newv));
	});
	$$("rtZ").attachEvent("onChange", function(newv, oldv){
		webix.message("Value changed from: "+oldv+" to: "+newv);
		C_ChangeZRot(parseInt(newv));
	});
	
	
	$$("posX").attachEvent("onSwipeX",function(c1,c2){
	   if( c2.x - c1.x > 0 ){
			webix.message("ss: ");
		}
	   else{
		   // swipe from right to left
	   }
	});
	//
	webix.UIManager.addHotKey("enter", function(view){
	var pos = $$("tree").getSelectedId();
	$$("tree").edit(pos);
    }, $$("tree"));
	$$("tree").attachEvent("onBeforeEditStart", function(id){
		if("ROOT_NODE_X" == id) return false;
	});
	$$("tree").attachEvent("onAfterEditStop", function(state, editor, ignoreUpdate){
		C_ChangeSceneGraphNodeName(state.old,state.value);
		console.log(state.value);
	});
	
	//Fman
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
	
	$$("files").attachEvent("onBeforeCreateFolder",function(id){
	// your code
	return true;
	});
	webixReady();
	var pos = getViewport($$("editor_grid").$view);
	C_SetSceneAreaViewport(pos.top,pos.left,pos.width,pos.height);
});





