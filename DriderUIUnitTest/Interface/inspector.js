$('<option value="1">Item 4</option>').appendTo($('#tagSelector'));
console.log("Ayuda");


$( function() {
  $( "#accordionInspector" )
    .accordion({
      header: "> div > h3",
      collapsible: true
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

function addComponent(data) {
  console.log(data);
  var element = '<div class="group"><h3>' + data.name + '</h3><div>';

  for (var i = 0; i < data.inputs.length; i++) {
    var type = data.inputs[i].type;
    if (type === "selectable") {
      element += addSelectable(data.inputs[i]);
    }
    else if (type === "droppableFile") {
      element += addDroppableFile(data.inputs[i]);
    }
    else if (type === "droppableGO") {
      element += addDroppableGO(data.inputs[i]);
    }
    else {
      element += addInput(data.inputs[i]);
    }
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
      console.log(event);
      console.log(ui);
      console.log(ui.draggable[0].dataset.url);
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
      console.log(event);
      console.log(ui);
      console.log(ui.draggable[0].baseURI);
      console.log(ui.draggable[0].innerText);
      event.target.value = ui.draggable[0].innerText;
      updateInput(event.target);
    },
    classes: {
      "ui-droppable-hover": "ui-state-hover"
    },
    tolerance: "pointer"
  });
}

function addInput(dataInput) {
  var component = '<div>'
  component += '<label for="objectName' + dataInput.name + '">'+  dataInput.name + ': </label>';
  component += '<input oninput="updateInput(this)" name:"objectName' + dataInput.name + '" type="' + dataInput.type;
  if (dataInput.type == "checkbox") {
    component += '" checked="'+ dataInput.value + '">';
  }
  else {
    component += '" value="'+ dataInput.value + '">';
  }
  component += '</div>'
  return component;
}

function addDroppableFile(dataInput) {
  var component = '<div>'
  component += '<label for="objectName' + dataInput.name + '">'+  dataInput.name + ': </label>';
  component += '<input disabled="true" class="droppableElementFile" oninput="updateInput(this)" name:"objectName' + dataInput.name + '" value="'+ dataInput.value + '">';
  component += '</div>'
  return component;
}

function addDroppableGO(dataInput) {
  var component = '<div>'
  component += '<label for="objectName' + dataInput.name + '">'+  dataInput.name + ': </label>';
  component += '<input disabled="true" class="droppableElementGO" oninput="updateInput(this)" name:"objectName' + dataInput.name + '" value="'+ dataInput.value + '">';
  component += '</div>'
  return component;
}

function addSelectable(dataInput) {
  var component = '<div>'
  component += '<label for="objectName' + dataInput.name + '">'+  dataInput.name + ': </label>';
  component += '<select oninput="updateInput(this)" name:"objectName' + dataInput.name + '">';
  for (var i = 0; i < dataInput.options.length; i++) {
    component += '  <option value="' + i + '"';
    if (dataInput.selected === i) {
      component += ' selected'
    }
    component +='>' + dataInput.options[i].name + '</option>';
  }
  component += '</select></div>'
  return component;
}

var componentes = {
  "object": "idobject",
  "components": [
    {
      "name": "Render",
      "inputs": [
        {
          "type": "text",
          "name": "nombre",
          "value": "lucas"
        },
        {
          "type": "number",
          "name": "altura",
          "value": "0"
        },
        {
          "type": "checkbox",
          "name": "activado",
          "value": "true"
        },
        {
          "type": "selectable",
          "name": "colores",
          "selected": "1",
          "options": [
            {"name": "rojos"},
            {"name": "rojo"},
            {"name": "negro"},
            {"name": "verde"}
          ]
        }
      ]
    },
    {
      "name": "Posicion",
      "inputs": [
        {
          "type": "number",
          "name": "Y",
          "value": 3
        },
        {
          "type": "color",
          "name": "X",
          "value": "#ff0000"
        }
      ]
    },
    {
      "name": "Material",
      "inputs": [
        {
          "type": "droppableFile",
          "name": "Albedo",
          "value": "colo.x"
        },
        {
          "type": "droppableGO",
          "name": "colider",
          "value": "piso"
        }
      ]
    }
  ]

}

function updateInput(input){
  console.log("ASdasdasdasdas");
  console.log(input);
  console.log(input.checked);
  console.log(input.value);
}

function JS_UpdateComponents(data) {
  while ($('#accordionInspector')[0].firstChild) {
    $('#accordionInspector')[0].removeChild($('#accordionInspector')[0].firstChild);
  }
  var components = JSON.parse(data.replace(/\'/g, '"'));
  for (var i = 0; i < components.components.length; i++) {
    addComponent(components.components[i]);
  }
}

JS_UpdateComponents("{'id':'3','components': [{'name':'RenderComponent', 'inputs':[{'type': 'text','name': 'nombre','id': '0','value': 'lucas'},{'type': 'number','name': 'altura','id': '1','value': '0'},{'type': 'checkbox','name': 'activado','id': '2','value': 'true'},{'type': 'checkbox','name': 'activado','id': '3','value': 'false'},{'type': 'color','name': 'color','id': '4','value': '#ff0000'},{'type': 'droppableFile','name': 'Albedo','id': '5','value': 'colo.x'},{'type': 'droppableGO','name': 'colider','id': '5','value': 'piso'},{'type': 'selectable','name': 'Tipos de campos','id': '5','selected': 'colider','options': [{'name': 'text'},{'name': 'number'},{'name': 'checkbox'},{'name': 'color'},{'name': 'droppableFile'},{'name': 'droppableGO'}]}]},{'name':'AABBCollider', 'inputs':[]}]}");
