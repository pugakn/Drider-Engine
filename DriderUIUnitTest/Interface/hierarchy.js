

var hierarchy = document.getElementById('idSorteableHierarchy');

function addGO(newNode, nodeFather) {
   var entry = document.createElement('li');
   var divName = document.createElement('div');
   var divPlus = document.createElement('div');
   var list = document.createElement('ul');
   list.setAttribute("class", "sortableHierarchy");
   entry.setAttribute("class", "ui-state-default");
   entry.setAttribute("data-id", newNode.id);

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
      var text = $(event.toElement)[0].innerText.slice(0,-1);
      var list = $(event.target).parent().parent().find("ul")[0];
      var element = $(event.toElement).parent()[0].cloneNode(true);
      console.log(element);
      $(event.toElement).parent()[0].remove();
      element.setAttribute("style", "position: static;");
      list.appendChild(element);
      updateDropables();
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

updateDropables();
/*
$(document).ready( function() {
  addGO("item1", hierarchy);
  addGO("item2", hierarchy);
  addGO("item3", hierarchy);
  addGO("item4", hierarchy);
});
*/
function addChilds(childs, father) {
  for (var i = 0; i < childs.length; i++) {
    var sonList = addGO(childs[i], father);
    addChilds(childs[i].childs, sonList);
  }
}
function JS_InfoHierarchy(data) {
  var nodes = JSON.parse(data.replace(/\'/g, '"'));
  console.log(nodes);
  addChilds(nodes.childs, hierarchy);
}

function HierarchyUpdate() {
  C_HierarchyUpdate();
  //JS_InfoHierarchy("{'id':1,'name':'root','childs': [{'id':1,'name':'Model','childs': [{'id':2,'name':'Model','childs': []},{'id':3,'name':'Floor','childs': []}]},{'id':1,'name':'Floor','childs': []}]}");
}

HierarchyUpdate();
