
var IdSelected,
  componentSelected = false;

$( function() {
    $( "#accordion" ).accordion({
      collapsible: true
    });
    $("#accordion").accordion( "option", "active", 2 );

  } );

$(document).ready(function(){
  $("#divInspector").oncontextmenu = function (e) {
    showMenuInspector(e);
  };
  $("#menuInspector").hide();
  $(document).click(function(e){
    if(e.button == 0){
      $("#menuInspector").css("display", "none");
    }
  });
  $(document).keydown(function(e){
    if(e.keyCode == 27){
      $("#menuInspector").css("display", "none");
    }
  });

  //controlamos los botones del menú
  $("#menuInspector").click(function(e){
    // El switch utiliza los IDs de los <li> del menú
    /*
    switch(e.target.id){
      case "eliminar":
        DeleteSceneGraphNode();
      break;
    }
    */

  });

  $("#generalInspector")[0].hidden = true;
});

function showMenuInspector(e) {
  $("#menuInspector").css({'display':'block', 'left':e.pageX, 'top':e.pageY});
  return false;
}

$( function() {
  $( "#accordionInspector" )
    .accordion({
      header: "> div > h3",
      collapsible: true,
      animate: false,
      activate: function( event, ui ) {
        if ($("#accordionInspector").accordion( "option", "active" )) {
          componentSelected = $("#accordionInspector").accordion( "option", "active" );
        }
      }
    })
    .sortable({
      axis: "y",
      handle: "h3",
      stop: function( event, ui ) {
        // IE doesn't register the blur when sorting
        // so trigger focusout handlers to remove .ui-state-focus
        ui.item.children( "h3" ).triggerHandler( "focusout" );

        // Refresh accordion to handle new order
        $( this ).accordion( "refresh" );
      }
    });
} );

$( "#tagSelector" ).selectmenu({
  select: function( event, ui ) {
    if (ui.item.index === 0) {
      AddSceneGraphNode();
    }
  }
});

function addComponent(data) {
  var element = '<div class="group"><h3>' + data.name + '</h3><div>';

  for (var i = 0; i < data.inputs.length; i++) {
    var type = data.inputs[i].type;
    var component = '<div data-id="' + data.inputs[i].id + '" data-namecomponent="'+ data.name +'"" >';
    if (type === "selectable") {
      element += addSelectable(data.inputs[i], component);
    }
    else if (type === "droppableFile") {
      element += addDroppableFile(data.inputs[i], component);
    }
    else if (type === "droppableGO") {
      element += addDroppableGO(data.inputs[i], component);
    }
    else {
      element += addInput(data.inputs[i], component);
    }
    component += '</div>';
  }
  element += '</div></div>'
  $(element).appendTo($('#accordionInspector'));

  activateDropablesAreas();
  $( "#accordionInspector" ).accordion( "refresh" );
}

function activateDropablesAreas() {
  $(".droppableElementFile" ).droppable({
    accept: ".file",
    drop: function( event, ui ) {
      event.target.value = ui.draggable[0].innerText;
      updateInput(event.target);
    },
    classes: {
      "ui-droppable-hover": "ui-state-hover"
    },
    tolerance: "pointer"
  });

  $(".droppableElementGO" ).droppable({
    accept: ".gameObjectElement",
    drop: function( event, ui ) {
      event.target.value = ui.draggable[0].innerText;
      updateInput(event.target);
    },
    classes: {
      "ui-droppable-hover": "ui-state-hover"
    },
    tolerance: "pointer"
  });

  $( ".selectOption" ).selectmenu({
    select: function( event, ui ) {
      updateInput(ui.item.element[0].parentElement);
    }
  });
}

function addInput(dataInput, component) {
  component += '<label for=' + dataInput.name + '">'+  dataInput.name + ': </label>';
  if (dataInput.type === "numberFloat") {
    component += '<input  oninput="updateInput(this)" name:"' + dataInput.name + '" type="number" step="any';
  }
  else {
    component += '<input  oninput="updateInput(this)" name:"' + dataInput.name + '" type="' + dataInput.type;
  }
  if (dataInput.type == "checkbox") {
    component += '" checked="'+ dataInput.value + '">';
  }
  else {
    component += '" value="'+ dataInput.value + '">';
  }
  return component;
}

function addDroppableFile(dataInput, component) {
  component += '<label for="' + dataInput.name + '">'+  dataInput.name + ': </label>';
  component += '<input disabled="true" class="droppableElementFile" oninput="updateInput(this)" name:"' + dataInput.name + '" value="'+ dataInput.value + '">';
  return component;
}

function addDroppableGO(dataInput, component) {
  component += '<label for="' + dataInput.name + '">'+  dataInput.name + ': </label>';
  component += '<input disabled="true" class="droppableElementGO" oninput="updateInput(this)" name:"' + dataInput.name + '" value="'+ dataInput.value + '">';
  return component;
}

function addSelectable(dataInput, component) {
  component += '<label for="' + dataInput.name + '">'+  dataInput.name + ': </label>';
  component += '<select class="selectOption" oninput="updateInput(this)" name:"' + dataInput.name + '">';
  for (var i = 0; i < dataInput.options.length; i++) {
    component += '  <option value="' + i + '"';
    if ( parseInt(dataInput.selected, 10) === i) {
      component += ' selected'
    }
    component +='>' + dataInput.options[i].name + '</option>';
  }
  component += '</select>';
  return component;
}

function updateInput(input){
  if (input.type === 'checkbox') {
    input.value = input.checked;
  }
  C_InputChange(IdSelected,
                input.parentElement.dataset.namecomponent,
                input.parentElement.dataset.id,
                input.value);
}

function updateGeneralInput(id, input) {

  C_InputGeneralChange(IdSelected,
                       id.toString(),
                       input);
}

function AddComponenenGameObject(value) {
  C_AddComponent(IdSelected, value);
  C_ChangeSceneGraphNodeSelection(IdSelected);
}
function JS_UpdateInspector() {
  C_ChangeSceneGraphNodeSelection(IdSelected);
}
function JS_UpdateComponents(data) {
  while ($('#accordionInspector')[0].firstChild) {
    $('#accordionInspector')[0].removeChild($('#accordionInspector')[0].firstChild);
  }
  var gameObject = JSON.parse(data.replace(/\'/g, '"'));
  IdSelected = gameObject.id;
  for (var i = 0; i < gameObject.components.length; i++) {
    addComponent(gameObject.components[i]);
  }
  $("#objectName")[0].value = gameObject.name;
  $("#positionX")[0].value = gameObject.position[0];
  $("#positionY")[0].value = gameObject.position[1];
  $("#positionZ")[0].value = gameObject.position[2];
  $("#rotX")[0].value = gameObject.rotation[0];
  $("#rotY")[0].value = gameObject.rotation[1];
  $("#rotZ")[0].value = gameObject.rotation[2];
  $("#scaleX")[0].value = gameObject.scale[0];
  $("#scaleY")[0].value = gameObject.scale[1];
  $("#scaleZ")[0].value = gameObject.scale[2];
  $("#accordionInspector").accordion( "option", "active", componentSelected);

}
