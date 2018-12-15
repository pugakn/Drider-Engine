var para = document.createElement("p");
var node = document.createTextNode("This is new.");
para.appendChild(node);

var data = ["tex1", "tex2", "tex3"];
function loadDropdownMaterials(data) {
  while ($('#materialMenuDiv')[0].firstChild) {
    $('#materialMenuDiv')[0].removeChild($('#materialMenuDiv')[0].firstChild);
  }
  var element = '<select class="selectOptionMaterial" id="selectedMaterial">';
  element += '<option oninput="changeMaterial(this) value="selectedTexture">Selecciona Textura</option>';
  for (var i = 0; i < data.length; i++) {
    element += '<option value="' + data[i] + '">' + data[i] + '</option>';
  }
  element += '</select>';
  element += '<br><label for="AlbedoTex">AlbedoTex: </label>';
  element += '<input disabled="true" class="droppableElementFileMaterial" id="Albedo" name:"AlbedoTex" value="">';
  element += '<br><label for="NormalTex">NormalTex: </label>';
  element += '<input disabled="true" class="droppableElementFileMaterial" id="Normal" name:"NormalTex" value="">';
  element += '<br><label for="EmissiveTex">EmissiveTex: </label>';
  element += '<input disabled="true" class="droppableElementFileMaterial" id="Emisivity" name:"EmissiveTex" value="">';
  element += '<br><label for="Metallic">Metallic: </label>';
  element += '<input disabled="true" class="droppableElementFileMaterial" id="Metallic" name:"Metallic" value="">';
  element += '<br><label for="Roughness">Roughness: </label>';
  element += '<input disabled="true" class="droppableElementFileMaterial" id="Roughness" name:"Roughness" value="">';
  $(element).appendTo($('#materialMenuDiv'));
  updateDropablesMaterial();
}

function updateDropablesMaterial() {
  $(".droppableElementFileMaterial" ).droppable({
    accept: ".file",
    drop: function( event, ui ) {
      event.target.value = ui.draggable[0].innerText;
      updateMaterial(event.target);
    },
    classes: {
      "ui-droppable-hover": "ui-state-hover"
    },
    tolerance: "pointer"
  });
  $( ".selectOptionMaterial" ).selectmenu({
    select: function() {
      changeMaterial();
    }
  });
}

function changeMaterial() {
  var name = document.querySelector("#selectedMaterial").value;
  if (name !== "Selecciona Textura") {
    C_InfoMaterial(name);
  }
}
function updateMaterial(input) {
  var name = document.querySelector("#selectedMaterial").value;
  if (name !== "Selecciona Textura") {
    console.log(input.id);
    console.log(name);
    console.log(input.value);
    C_ChangeTexture(input.id, name, input.value);
  }

}

function JS_UpdateTextureMaterial(data) {
  for (var i = 0; i < data.length; i++) {
    var input = "#" + data[i];
    i++;
    document.querySelector(input).value = data[i];
  }
}

function JS_UpdateMaterials(materials) {
  loadDropdownMaterials(materials);
}
function loadMaterialsSection() {
  C_LoadMaterials();
}
C_LoadMaterials();
