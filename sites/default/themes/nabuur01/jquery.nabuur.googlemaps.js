/* 
 * This file is based on :  
 * jQuery googleMap Copyright Dylan Verheul <dylan@dyve.net>
 * Licensed like jQuery, see http://docs.jquery.com/License
 * 
 * Some initial modifications made by Peter Brownell for Code Positive and
 * School of Everything. 
 *
 * We have made some modifications to the original file, but not that many. 
 * There is a good amount of work to do to tie this into Dupal and make
 * it configurable and themeable. 
 * 
 * For now, unless you want to patch the source, there is not all that
 * much to can do to change things. 
 * 
 * Ideas and plans welcome. 
 */

$(document).ready(function()
{
  $("#map").googleMap(null,null,13 , { markers: $(".geo"),controls: ["GSmallZoomControl"]});
});

$.googleMap = {
  maps: {},
  marker: function(m) {
    if (!m) {
      return null;
    } 
    else if (m.lat == null && m.lng == null) {
      return $.googleMap.marker($.googleMap.readFromGeo(m));
    } 
    else {
      var localIcon = new GIcon(G_DEFAULT_ICON);
      markerOptions = { icon:localIcon };
      var marker = new GMarker(new GLatLng(m.lat, m.lng), markerOptions );
      if (m.txt) {
        GEvent.addListener(marker, "click", function() {
        marker.openInfoWindowHtml(m.txt);
        });
      }
      if (m.link) {
        GEvent.addListener(marker, "dblclick", function() {
          window.location=m.link; 
        });
      } 
      return marker;
    }
  },
  readFromGeo: function(elem) {
    var latElem = $(".latitude", elem)[0];
    var lngElem = $(".longitude", elem)[0];
    var title = ''; 
    var link = '';
    if (latElem && lngElem) {
      if ( $(elem).attr("title") ) {
        if ( $(elem).attr("nid") > 0 ) {
          link = '/node/'+$(elem).attr("nid");
          title = '<h4 class="maptitle"><a href="'+link+'">'+$(elem).attr("title")+'</a></h4>'; 
          if ( $(elem).attr("info") ) { title += $(elem).attr("info"); }
        } 
        else {
          title = $(elem).attr("title");
        }
      }
      return { lat:parseFloat($(latElem).attr("title")), lng:parseFloat($(lngElem).attr("title")), txt:title, link: link }
    } 
    else {
      return null;
    }
  },
  mapNum: 1
};

 
$.fn.googleMap = function(lat, lng, zoom, options) {

  // If we aren't supported, we're done
  if (!window.GBrowserIsCompatible || !GBrowserIsCompatible()) return this;
  // Default values make for easy debugging
  if (lat == null) lat = 51.52177;
  if (lng == null) lng = -0.20101;
  if (!zoom) zoom = 1;
  // Sanitize options
  if (!options || typeof options != 'object')	options = {};
  options.mapOptions = options.mapOptions || {};
  options.markers = options.markers || [];
  options.controls = options.controls || {};

  // Map all our elements
  return this.each(function() {
    // Make sure we have a valid id
    if (!this.id) this.id = "gMap" + $.googleMap.mapNum++;
    // find our markers
    var marker = null;
    var markers = new Array();
    var bounds = null;
    for (var i = 0; i < options.markers.length; i++) {
      if (marker = $.googleMap.marker(options.markers[i])) {
        markers[i] = marker;
        if (!bounds) { 
          var bounds = new GLatLngBounds(marker.getPoint());
        } 
        else {
          // extend bounds with marker.getPoint();
          bounds.extend(marker.getPoint());
        }
      }
    }
    // we only want a map if we have markers
    if (markers.length) {
      $(this).css('display','block');
      // Create a map and a shortcut to it at the same time
      var map = $.googleMap.maps[this.id] = new GMap2(this, options.mapOptions); 
      // Center and zoom the map
      map.setCenter(new GLatLng(lat, lng), zoom);
      // Add controls to our map
      for (var i = 0; i < options.controls.length; i++) {
        var c = options.controls[i];
        eval("map.addControl(new " + c + "());");
      }
      for (var i = 0; i < markers.length; i++) {
        map.addOverlay(markers[i]);
      }
      // time to zoom the map
      var distance = 2.0;
      /*if ( markers.length == 1 ) {
        // if we only have one marker, we move out a bit more
        distance = 0.5;
      }*/
      // Moving the map to show our markers
      // We have to set the centre to get the bounds properly
      
      map.setCenter(bounds.getCenter(), map.getBoundsZoomLevel(bounds));
      var southWest=bounds.getSouthWest();
      var northEast=bounds.getNorthEast();
      bounds.extend(new GLatLng(southWest.lat() - distance, southWest.lng() - distance ));
      bounds.extend(new GLatLng(northEast.lat() + distance, northEast.lng() + distance ));								 
      map.setCenter(bounds.getCenter(), map.getBoundsZoomLevel(bounds));		
      map.enableScrollWheelZoom();
    }
  });
};
