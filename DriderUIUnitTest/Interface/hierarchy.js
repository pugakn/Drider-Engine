

var hierarchy = document.getElementById('idSorteableHierarchy');

function addGO(newNode, nodeFather) {
   var entry = document.createElement('li');
   var divName = document.createElement('div');
   var divPlus = document.createElement('div');
   var list = document.createElement('ul');
   list.setAttribute("class", "sortableHierarchy");
   entry.setAttribute("class", "ui-state-default");
   divName.appendChild(document.createTextNode(newNode));
   divPlus.setAttribute("class", "droppableHierarchy");
   divPlus.appendChild(document.createTextNode("+"));

   divName.appendChild(divPlus);
   entry.appendChild(divName);
   entry.appendChild(list);
   nodeFather.appendChild(entry);
   updateDropables();
}

function updateDropables() {
  $(".droppableHierarchy" ).droppable({
    drop: function( event, ui ) {
      console.log(event, ui);
      console.log(ui.draggable.context.outerHTML);
      var text = $(event.toElement)[0].innerText.slice(0,-1);
      var list = $(event.target).parent().parent().find("ul")[0];
      //$(event.toElement).parent()[0].appendChild(list);
      //console.log($(event.toElement).parent()[0]);
      var element = $(event.toElement).parent()[0].cloneNode(true);
      $(event.toElement).parent()[0].remove();
      console.log(element);
      console.log(list);
      list.appendChild(element);
      //list.appendChild($(event.toElement).parent()[0]);
      //addGO(text, list);
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

$(document).ready( function() {
  addGO("item1", hierarchy);
  addGO("item2", hierarchy);
  addGO("item3", hierarchy);
  addGO("item4", hierarchy);
});
