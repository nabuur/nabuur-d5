<?php
/* $Id: geonames_cck.theme.inc,v 1.1 2008/03/25 10:14:42 thegreenman Exp $ */

/**
 * Theme functions for the geonames_cck module
 */


/**
 * Theme the main disambiguation page. 
 *  
 *
 * 
 */
function theme_geonames_cck_disambiguate_page($original_place, $original_country, $locations, $nid, $field, $index) { 
  if ($original_place) {
    $values = array(
        '!location' => $original_place .', '. geoapi_countries($original_country),
        '@resultcount' => $locations -> total_results_count,
      );
    //How many matches do we have for the given search string?
    switch (sizeof($locations -> results)) {
      case 0:  // no matches
        $output = _geonames_cck_text('disambiguation_noresults', $values);
        break;
      case 1:  // only one match
        $output = _geonames_cck_text('disambiguation_oneresult', $values);
        $output .= theme('geonames_cck_disambiguate_location', $original_place, $original_country, $locations, $nid, $field, $index);
        break;
      default:  // multiple matches	
        $output .= _geonames_cck_text('disambiguation_header', $values);  
        $output .= theme('geonames_cck_disambiguate_location', $original_place, $original_country, $locations, $nid, $field, $index);
        break;
    } 
  } 
  else {
    // no place data 
    $output = _geonames_cck_text('disambiguation_empty', $values);
  }
  $output .= drupal_get_form('geonames_cck_disambiguate_form', $original_place, $original_country,  $nid, $field, $index);  
  $output .= _geonames_cck_text('disambiguation_footer', $values);
  $output = '<div id="disambiguation">'. $output .'</div>';
  return $output;
} 

/**
 * Produce disambiguation data for a specific location
 */
function theme_geonames_cck_disambiguate_location($original_place, $original_country, $locations, $nid, $field, $index) {
  //print_r($locations);  
  foreach ($locations->results as $location) {
    $items[] = theme('geonames_cck_disambiguate_location_item', $location, $nid, $field, $index);    
  } 
  if (sizeof($items)<1) {
    $title = t('Locations matching !location', array('!location' => $original_place .', '. geoapi_countries($original_country)));
  } 
  else {
    $title = t('Location matching !location', array('!location' => $original_place .', '. geoapi_countries($original_country)));
  }
  $output = theme( 'item_list', $items, $title , 'ul', array('class' => 'disambiguation-list'));
  return $output;
}

/**
 * Theme one item of the disambiguation list
 */
function theme_geonames_cck_disambiguate_location_item($location, $nid, $field, $index) {
  static $counter = 0; 
  $location_string = theme('geonames_cck_location_name', $location);                          
  $action_url = 'services/location/geonames/insert/'. $nid .'/'. $field .'/'. $index .'/'. $location['geonameid'];
  $output = ++$counter .' - '. l($location_string, $action_url, array(), null, null, false, true );
  $output .= theme('geonames_hidden', $location['lat'], $location['lng'], $counter .' - '. $location_string); 
  return $output;     
}

/**
 * Theme the full name for a location
 */
function theme_geonames_cck_location_name($location) {
  $location_name[] = $location['name']; 
  $admin2 = geonames_cck_get_admin_region($location['adminname2'], $location['lat'], $location['lng']);
  if ($admin2 and $admin2 != $location['name']) $location_name[] = $admin2;  // Fix the 'London, London' issue
  if ($location['adminname1']) $location_name[] = $location['adminname1'];
  $result = implode(', ', $location_name);                      
  return $result; 
}


function theme_geonames_location($lat, $lng, $name='', $item=NULL, $node=NULL) {  
  $location_name = array();

  if ($name != '') {
    $location_name[] = $name;
  }
  if (is_object($node)) {
    $title = $node->title;   // we have data, but no title 
    $attributes = 'nid="'. $node->nid .'"';
  }
  if (is_array($item)) {
    // this is a cck item
    if (geoapi_countries($item['country'])) {
      $location_name[] = geoapi_countries($item['country']);
  } 
  }
  $output = check_plain(implode(', ', $location_name));
  if ($lat and $lng) {
    $output .= '<span class="latitude" title="'. $lat .'"></span> 
                <span class="longitude" title="'. $lng .'"></span>';
    $output = '<div class="geo" title="'. $title .'" '. $attributes .' >'. $output .'</div>';  
  }                     
  return $output;
}



function theme_geonames_hidden($lat, $lng, $name ='', $item=NULL, $node=NULL) {
  if ($node) $title = $node -> title;  
  if ($lat and $lng) $output = '<div class="geo" title="'. check_plain($title) .'" nid="'. $node -> nid .' ">
                   <span class="latitude" title="'. $lat .'"></span> 
                   <span class="longitude" title="'. $lng .'"></span>
                   </div>';
  return $output;
}

/**
 * Theme the display of the locations 
 */
function theme_geonames_cck_location( $node = null, $disambiguate_links = false ) {     
if (!$node && arg(0) == 'node' && is_numeric(arg(1))) {
  $node = node_load(arg(1));
  }
  if ($node) {
    // make the geonames fields work 
    $geo_fields = geonames_cck_get_all_fields();
    // look for all the defined fields
    foreach ($geo_fields as $field_name => $field_data) {
      if (is_array($node->$field_name)) {
        // this node uses this field
        foreach ($node -> $field_name as $index => $field_data) {
          $locations['geonames'][] = theme('geonames_cck_location_item', $field_data, $node, $disambiguate_links, $disambiguate_links); 
        }
      }
    } 
    if (module_exists('token')) token_replace($title, 'node', $node);
    if (is_array($locations['geonames'])) {
      $output = $title .'<div class="locations-geonames">'. implode(', ', $locations['geonames']) .'</div>';
    }  
  }   
  if ($output) return '<div class="geonames-locations">'. $output .'</div>';  
}

/**
 * Theme a geonames entry in the locations list.
 */
function theme_geonames_cck_location_item($location, $node, $disambiguate_links = false) {
  global $user;   
  if ($location['name'] and $location['lat']) {
    // we have location data       
    $values = array(
      '!location' => theme('geonames_location', $location['lat'], $location['lng'], theme('geonames_cck_location_name', $location), $location),
      '@country' => geoapi_countries($location['country']),
      '@url' => url('node/'. $node -> nid .'/disambiguate'), 
    );
    if ( $disambiguate_links && node_access ('update', $node)) {
      $output = _geonames_cck_text('location_known', $values);     
    }
    else {
      $output = theme('geonames_location', $location['lat'], $location['lng'], theme('geonames_cck_location_name', $location), $location);   
    }       
  } 
  else if ($location['name'] and !$location['lat']) {
    // we have a name, but can't work out where it is
    $values = array(
      '!location' => $location['name'],
      '@country' => geoapi_countries($location['country']),
      '@url' => url('node/'. $node -> nid .'/disambiguate'), 
    );
    if ( $disambiguate_links && node_access('update', $node)) {
      $output = _geonames_cck_text('location_unknown', $values);
    }
    else {
      $output = geoapi_countries($location['country']);
    }     
  } 
  else {
    //no location data has been provided
    if ( $disambiguate_links && node_access('update', $node)) {
      $values = array(
        '@country' => geoapi_countries($location['country']),
        '@url' => url('node/'. $node->nid .'/disambiguate'), 
      );
      $output = _geonames_cck_text('location_nodata', $values);
    } 
    else {
      $output = geoapi_countries($location['country']); 
    }        
  }    
  return $output;
}


/**
 * Very rough implementation of a place page
 *
 */
function theme_place_page($country, $location, $nodes, $geodata) {
    $output = ''; 
    foreach ($nodes as $node) {
      $node = node_load($node -> nid); 
      $output .= node_view($node, true, false, false);
    }
    return $output;
}
