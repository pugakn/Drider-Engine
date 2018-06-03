var height = $( window ).height();
var width = $( window ).width();
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

$( ".section" ).each(function( section ) {
  $(this).draggable({
    containment: "parent",
    snap: ".snapTarget"
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

$("#scene").resizable({
  resize: function( event, ui ) {
    C_SetSceneAreaViewport($(event.target).position().top, $(event.target).position().left, $(event.target).width(), $(event.target).height());
  }
});

C_SetSceneAreaViewport($("#scene").position().top, $("#scene").position().left, $("#scene").width(), $("#scene").height());
