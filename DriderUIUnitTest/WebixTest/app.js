
// ==================================================================================================================
//                                                  C app
// ==================================================================================================================
function JS_AddSceneGraphNode(name,pname) {
	console.log(name);
	webix.callEvent('WEBIX_AddSceneGraphNode',[name,pname]);
}
function JS_ClearSceneGraphTree() {
	webix.callEvent('WEBIX_ClearSceneGraphTree');
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
		onContext:{},
		on:{
			onBeforeDrop:function(context){
				//drop as the first child
				context.parent = context.target;
				context.index = 0;
			}
			
		},
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
			//id: list.getItem(listId).title
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





