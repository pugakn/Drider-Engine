// jQuery File Tree Plugin
//
// Version 1.01
//
// Cory S.N. LaViska
// A Beautiful Site (http://abeautifulsite.net/)
// 24 March 2008
//
// Visit http://abeautifulsite.net/notebook.php?article=58 for more information
//
// Usage: $('.fileTreeDemo').fileTree( options, callback )
//
// Options:  root           - root folder to display; default = /
//           script         - location of the serverside AJAX file to use; default = jqueryFileTree.php
//           folderEvent    - event to trigger expand/collapse; default = click
//           expandSpeed    - default = 500 (ms); use -1 for no animation
//           collapseSpeed  - default = 500 (ms); use -1 for no animation
//           expandEasing   - easing function to use on expand (optional)
//           collapseEasing - easing function to use on collapse (optional)
//           multiFolder    - whether or not to limit the browser to one subfolder at a time
//           loadMessage    - Message to display while initial tree loads (can be HTML)
//
// History:
//
// 1.01 - updated to work with foreign characters in directory/file names (12 April 2008)
// 1.00 - released (24 March 2008)
//
// TERMS OF USE
//
// This plugin is dual-licensed under the GNU General Public License and the MIT License and
// is copyright 2008 A Beautiful Site, LLC.
//
var globalDirectory, urlDirectory, o;
$(document).ready( function() {
    o = $('#fileTree').fileTree({ }, function(file) {});
});

if(jQuery) (function($){

	$.extend($.fn, {
    showTree:function(c, t, data, o) {
      $(c).addClass('wait');
      $(".jqueryFileTree.start").remove();
      $(c).find('.start').html('');
      $(c).removeClass('wait').append(data);
      if( o.root == t ) $(c).find('UL:hidden').show(); else $(c).find('UL:hidden').slideDown({ duration: o.expandSpeed, easing: o.expandEasing });
      $('#fileTree').bindTree(c, o);
    },
    bindTree:function(t, o) {
      $(t).find('LI A').bind(o.folderEvent, function() {
        if( $(this).parent().hasClass('directory') ) {
          if( $(this).parent().hasClass('collapsed') ) {
            // Expand
            if( !o.multiFolder ) {
              $(this).parent().parent().find('UL').slideUp({ duration: o.collapseSpeed, easing: o.collapseEasing });
              $(this).parent().parent().find('LI.directory').removeClass('expanded').addClass('collapsed');
            }
            $(this).parent().find('UL').remove(); // cleanup
            $('#fileTree').SetDirectory( $(this).parent(), escape($(this).attr('rel').match( /.*\// )) );
            $(this).parent().removeClass('collapsed').addClass('expanded');
            CFUIReady(urlDirectory);
          } else {
            // Collapse
            $(this).parent().find('UL').slideUp({ duration: o.collapseSpeed, easing: o.collapseEasing });
            $(this).parent().removeClass('expanded').addClass('collapsed');
          }
        } else {
          h($(this).attr('rel'));
        }
        return false;
      });
      // Prevent A from triggering the # on non-click events
      if( o.folderEvent.toLowerCase != 'click' ) $(t).find('LI A').bind('click', function() { return false; });
    },
    SetDirectory: function(dir, url) {
      urlDirectory = url;
      globalDirectory = dir;
    },
		fileTree: function(o, h) {
			// Defaults
			if( !o ) var o = {};
			if( o.folderEvent == undefined ) o.folderEvent = 'click';
			if( o.expandSpeed == undefined ) o.expandSpeed= 500;
			if( o.collapseSpeed == undefined ) o.collapseSpeed= 500;
			if( o.expandEasing == undefined ) o.expandEasing = null;
			if( o.collapseEasing == undefined ) o.collapseEasing = null;
			if( o.multiFolder == undefined ) o.multiFolder = true;
			if( o.loadMessage == undefined ) o.loadMessage = 'Loading...';

      $(this).html('<ul class="jqueryFileTree start"><li class="wait">' + o.loadMessage + '<li></ul>');
      // Get the initial file list
      $('#fileTree').SetDirectory($(this), escape(o.root));
      return o;
    }
  });

})(jQuery);

function JS_InfoTreeFile( root, datos) {
  var r = '<ul class="jqueryFileTree" style="display: none;">';
  var files = JSON.parse(datos.replace(/\'/g, '"')).items;
  var index, len = files.length;
  for (index = 0; index < len; ++index) {
    if (files[index].type === "folder") {
      r += '<li class="directory collapsed"><a href="#" rel="' + files[index].parent  + '/">' + files[index].name + '</a></li>';
    } else {
      r += '<li class="file ext_' + files[index].type + '"><a href="#" rel='+ files[index].parent + '>' + files[index].name + '</a></li>';
    }
  }
  $('#fileTree').showTree(globalDirectory, urlDirectory, r, o);
}

function CFUIReady(url) {
  C_FileTree(url);

  /*function C_FileTree () {setTimeout(function(){
  JS_InfoTreeFile("Assets","{'items':[{'name':'Models','parent':'Assets/Models','type':'folder'},{'name':'Scripts','parent':'Assets/Scripts','type':'folder'}]}"); }, 1000);
};*/
}

CFUIReady("Assets")
