$(document).click(function(e){
  var active = "0";
    if (e.clientX > document.querySelector('#scene').offsetLeft &&
        e.clientX < document.querySelector('#scene').offsetLeft +
        document.querySelector('#scene').offsetWidth) {
      if (e.clientY > document.querySelector('#scene').offsetTop &&
          e.clientY < document.querySelector('#scene').offsetTop +
          document.querySelector('#scene').offsetHeight) {
        active = "1";
      }
    }
    C_ActiveScene(active);
});
