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





