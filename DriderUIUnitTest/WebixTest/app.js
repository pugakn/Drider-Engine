
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
function JS_UpdatePropertySheetUI() {
	webix.callEvent('WEBIX_UpdatePropertySheetUI'); //TODO: ADLASD
}
function JS_AddComponent_Render(objName,componentName) {
	webix.callEvent('WEBIX_AddComponent_Render',[objName,componentName]); 
}
//===================================================================================================================
//                                                  WEBIX
// ==================================================================================================================
//webix.ready() function ensures that the code will be executed when the page is loaded
webix.ready(function(){
	webix.markup.init();
	webixReady();
	
	
    //=======================================================================================================
	//                                             Toobar
	//=======================================================================================================
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
	
	
	
	//=======================================================================================================
	//                                          Propertysheet
	//=======================================================================================================
	
	var transform_UI = webix.ui({
	cols:[
		{
			view:"fieldset", id:"myform1", label:"Propiedades", body:{
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
						{ view:"text",   label:"x", inputWidth:80 , labelWidth:20, inputHeight:24,autowidth:false ,id:"posX"},
						{ view:"text",   label:"y", inputWidth:80 , labelWidth:20, inputHeight:24,autowidth:false ,id:"posY"},
						{ view:"text",   label:"z", inputWidth:80 , labelWidth:20, inputHeight:24,autowidth:false ,id:"posZ"},
					]},
				]}
			]
		}
		}
	],
	container:"propView",
	margin:0,
	padding:0,
	id:"propTransform"
	});
	
	
	
	webix.attachEvent("WEBIX_AddComponent_Render", function(objName,compName){
		console.log(compName);
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
	
	
	
	
	webix.ui({
	view:"contextmenu",
	id:"general_prop_ctx_menu",
	data:[{value:"Add Component" ,submenu:[ 
		  "Render", "Animator", "Script", "Collider"]}],
	master:"propView",
	on:{
		onItemClick:function(id){
			if (this.getItem(id).value == "Render"){

			}
		}
	},
	openAction:"click",
	});
	//=======================================================================================================
	//                                           Scene Graph
	//=======================================================================================================
	webix.protoUI({
		name:"edittree",
	}, webix.EditAbility, webix.ui.tree);
	treeb = webix.ui({
		container:"treeView",
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
	});	
	webix.UIManager.addHotKey("enter", function(view){
		var pos = treeb.getSelectedId();
		treeb.edit(pos);
    }, $$("tree"));
	treeb.attachEvent("onBeforeEditStart", function(id){
		if("ROOT_NODE_X" == id) return false;
	});
	treeb.attachEvent("onAfterEditStop", function(state, editor, ignoreUpdate){
		C_ChangeSceneGraphNodeName(state.old,state.value);
		console.log(state.value);
	});

	
    webix.ui({
        view:"contextmenu",
	    id:"general_tree_ctx_menu",
        data:["Add GameObject",{ $template:"Separator" },"Start WW3"],
		master:"treeView",
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
				treeb.edit(list.getItem(listId).id);
			}
		}
	}
    });

	
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
    webix.ui({
        view:"filemanager",
        id:"files",
		container: "fman"
		//save: "cache->mydata",
		//url: "cache->mydata"
    });
	
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
	$$("files").attachEvent("onBeforeCreateFolder",function(id){
		// your code
		return true;
	});
	
});





