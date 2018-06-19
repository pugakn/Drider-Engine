

var hierarchy = document.getElementById('idSorteableHierarchy'),
  index = 0,
  gameObjectSelected = hierarchy;

function selectGO(ui) {
  $('.selectedGameobject').removeClass('selectedGameobject');
  $(ui.path[1]).addClass('selectedGameobject');
  gameObjectSelected = ui.path[1];
}

function addGO(newNode, nodeFather) {
   var entry = document.createElement('li');
   var divName = document.createElement('div');
   var divPlus = document.createElement('div');
   var list = document.createElement('ul');
   list.setAttribute("class", "sortableHierarchy");
   entry.setAttribute("class", "ui-state-default");
   entry.setAttribute("class", "ui-widget-content");
   entry.setAttribute("data-id", newNode.id);
   entry.addEventListener("click", selectGO);
   divName.appendChild(document.createTextNode(newNode.name));
   divPlus.setAttribute("class", "droppableHierarchy");
   divPlus.appendChild(document.createTextNode("+"));

   divName.appendChild(divPlus);
   entry.appendChild(divName);
   entry.appendChild(list);
   nodeFather.appendChild(entry);
   updateDropables();
   return list;
}

function updateDropables() {
  $(".droppableHierarchy" ).droppable({
    drop: function( event, ui ) {
      var element = $(event.toElement).parent()[0],
        idParent = $(event.target).parent().parent()[0].dataset.id;
        idSon = element.dataset.id;
      if (!idParent) {
        idParent = hierarchy.dataset.id;
      }
      ChangeNodeParent(idParent, idSon);
    },
    classes: {
      "ui-droppable-hover": "ui-state-hover"
    },
    tolerance: "pointer"
  });
  $( function() {
     $( ".sortableHierarchy" ).sortable({
        placeholder: "ui-state-highlight",
        connectWith: ".connectedSortable"
      }).disableSelection();
   } );
}

function addChilds(childs, father) {
  for (var i = 0; i < childs.length; i++) {
    var sonList = addGO(childs[i], father);
    addChilds(childs[i].childs, sonList);
  }
}

function JS_InfoHierarchy(data) {
  while (hierarchy.firstChild) {
    hierarchy.removeChild(hierarchy.firstChild);
  }
  var nodes = JSON.parse(data.replace(/\'/g, '"'));
  hierarchy.setAttribute("data-id", nodes.id);
  hierarchy.setAttribute("data-name", nodes.name);

  addChilds(nodes.childs, hierarchy);
}

$( "#optionsHierarchy" ).selectmenu({
  select: function( event, ui ) {
    if (ui.item.index === 0) {
      AddSceneGraphNode();
    }
  }
});

function HierarchyUpdate() {
  //C_HierarchyUpdate();
  JS_InfoHierarchy("{'id':0,'name':'ROOT_NODE_X','childs': [{'id':3,'name':'Model','childs': []},{'id':4,'name':'Floor','childs': []}]}");
}

function AddSceneGraphNode() {
  var name = "gameObject" + index,
    id = gameObjectSelected.dataset.id;
  index++;
  C_AddSceneGraphNode(name, id);
}

function ChangeNodeParent(idParent, idSon) {
  C_ChangeNodeParent(idParent, idSon);
}
updateDropables();
HierarchyUpdate();
