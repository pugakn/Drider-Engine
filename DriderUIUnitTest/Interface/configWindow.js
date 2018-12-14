var height = $( window ).height();
var width = $( window ).width();
var menuHeight;
var headerSectionsHeight;

$(document).ready( function() {
  headerSectionsHeight = document.querySelector('.titleSection').offsetHeight;
  menuHeight = document.querySelector('.navbar').offsetHeight;
  var unitHeight = ($( window ).height() - menuHeight) / 3;
  var unitWidth = $( window ).width() * .25;
  $('#container').css("height",$( window ).height() - menuHeight);
  $('#divHierarchy')[0].style.height = $( window ).height() - menuHeight - headerSectionsHeight - unitHeight + "px";
  $('#hierarchy')[0].style.height = $( window ).height() - menuHeight - unitHeight  + "px";

  $('#scene')[0].style.height = $( window ).height() - menuHeight  - unitHeight + "px";
  $('#sceneDiv')[0].style.height = $( window ).height() - menuHeight - unitHeight  + "px";
  $("#scene").css({left: unitWidth});

  $("#inspector").css({left: (unitWidth*3)});
  $('#divInspector')[0].style.height = $( window ).height() - menuHeight + "px";

  $("#project").css({top: (unitHeight * 2) + menuHeight});
  $("#project")[0].style.height = unitHeight  + "px";
  $('#fileTree')[0].style.height = unitHeight - + headerSectionsHeight + "px";
});

$( window ).resize(function() {
  var tempPos,
    actualHeight = $( window ).height() - 56,
    actualWidth = $( window ).width();
  $('#container').css("height",$( window ).height() - 56);
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

/*
var unitHeight = $( window ).height() * .66;
var unitWidth = $( window ).width() * .25;
$("#scene").css({left: unitWidth});
$("#inspector").css({left: unitWidth*3});
$("#project").css({top: unitHeight});
$('#fileTree')[0].style.height = $( window ).height() * .34 - menuHeight + "px";
$('#divHierarchy')[0].style.height = $( window ).height() * .65 - menuHeight + "px";
$('#divInspector')[0].style.height = $( window ).height() - menuHeight + "px";*/


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
    $(event.target).find("#fileTree").css("height", ui.size.height - menuHeight);
  }
});

$("#hierarchyDiv").resizable({
  resize: function( event, ui ) {
    $($(ui.element[0]).parent()[0]).css("height", ui.size.height);
    $($(ui.element[0]).parent()[0]).css("width", ui.size.width);
    $(event.target).find("#divHierarchy").css("height", ui.size.height - menuHeight);
  }
});

function CFSetSceneAreaViewport() {
  C_SetSceneAreaViewport($("#scene").position().top + headerSectionsHeight, $("#scene").position().left, $("#scene").width(), $("#scene").height() - headerSectionsHeight);
}
setTimeout(function(){ CFSetSceneAreaViewport(); }, 0);
