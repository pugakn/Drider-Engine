var height = $( window ).height();
var width = $( window ).width();
var sizeHeader = 27;

$(document).ready( function() {
  $('#container').css("height",$( window ).height());
});

$( window ).resize(function() {
  var tempPos,
    actualHeight = $( window ).height(),
    actualWidth = $( window ).width();
  $('#container').css("height",$( window ).height());
  tempPos = $("#scene").position();
  $("#scene").css({left: actualWidth * tempPos.left / width, top: actualHeight * tempPos.top / height});
  tempPos = $("#inspector").position();
  $("#inspector").css({left: actualWidth * tempPos.left / width, top: actualHeight * tempPos.top / height});
  tempPos = $("#project").position();
  $("#project").css({left: actualWidth * tempPos.left / width, top: actualHeight * tempPos.top / height});
  tempPos = $("#hierarchy").position();
  $("#hierarchy").css({left: actualWidth * tempPos.left / width, top: actualHeight * tempPos.top / height});
  height = actualHeight;
  width = actualWidth;
});


var unitHeight = $( window ).height() * .66;
var unitWidth = $( window ).width() * .25;
$("#scene").css({left: unitWidth});
$("#inspector").css({left: unitWidth*3});
$("#project").css({top: unitHeight});
$('#fileTree')[0].style.height = $( window ).height() * .34 - sizeHeader + "px";
$('#divHierarchy')[0].style.height = $( window ).height() * .65 - sizeHeader + "px";
$('#divInspector')[0].style.height = $( window ).height() - sizeHeader + "px";


$( ".section" ).each(function( section ) {
  $(this).draggable({
    containment: "parent",
    snap: ".snapTarget",
    handle: "h3"
  });
  $(this).mousedown(function(){
    $('.front').removeClass('front');
    $(this).addClass('front');
  });
});

$( ".subSection" ).each(function(  ) {
  $(this).resizable({
    containment: "#container",
    resize: function( event, ui ) {
      $($(ui.element[0]).parent()[0]).css("height", ui.size.height);
      $($(ui.element[0]).parent()[0]).css("width", ui.size.width);
    }
  });
});

$("#scene").draggable({
  containment: "parent",
  snap: ".snapTarget",
  handle: "h3",
  drag: function( event, ui ) {
    CFSetSceneAreaViewport();
  }
});
$("#sceneDiv").resizable({
  resize: function( event, ui ) {
    $($(ui.element[0]).parent()[0]).css("height", ui.size.height);
    $($(ui.element[0]).parent()[0]).css("width", ui.size.width);
    CFSetSceneAreaViewport();
  }
});

$("#projectDiv").resizable({
  resize: function( event, ui ) {
    $($(ui.element[0]).parent()[0]).css("height", ui.size.height);
    $($(ui.element[0]).parent()[0]).css("width", ui.size.width);
    $(event.target).find("#fileTree").css("height", ui.size.height - sizeHeader);
  }
});

$("#hierarchyDiv").resizable({
  resize: function( event, ui ) {
    $($(ui.element[0]).parent()[0]).css("height", ui.size.height);
    $($(ui.element[0]).parent()[0]).css("width", ui.size.width);
    $(event.target).find("#divHierarchy").css("height", ui.size.height - sizeHeader);
  }
});

function CFSetSceneAreaViewport() {
  C_SetSceneAreaViewport($("#scene").position().top + sizeHeader, $("#scene").position().left, $("#scene").width(), $("#scene").height() - sizeHeader);
}

CFSetSceneAreaViewport();
