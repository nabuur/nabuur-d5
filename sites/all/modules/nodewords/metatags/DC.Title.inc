<?php
// $Id: DC.Title.inc,v 1.1.2.3 2008/08/12 06:50:00 robrechtj Exp $

/**
 * @file
 * Support file for DC.Title meta tag.
 */

function nodewords_dc_title_prepare($type, $ids, $value, $settings) {
  if (!isset($value) || empty($value)) {
    // http://drupal.org/node/103399 : we are unable to drupal_get_title(),
    // so unfortunately, only node-pages can get a DC.title.
    if ($type == 'node' && count($ids) == 1) {
      $node = node_load($ids[0]);
      if (node_access('view', $node)) {
        $value = $node->title;
      }
    }
    elseif ($type == 'panels' && count($ids) == 1) {
      if (function_exists('panels_api_version') && module_exists('panels_page')) {
        $version = panels_api_version();
        if ($version[0] == 2 && $version[1] == 0) {
          $panel = db_fetch_object(db_query("SELECT * FROM {panels_page} WHERE did = %d", $ids[0]));
        }
      }
      else if (db_table_exists('panels_info')) {
        $panel = db_fetch_object(db_query("SELECT * FROM {panels_info} WHERE did = %d", $ids[0]));
      }
      if ($panel) {
        $value = $panel->title;
      }
    }
    else {
      $value = variable_get('site_name', 'Drupal');
      if (variable_get('site_slogan', '')) {
        $value .= ' | '. variable_get('site_slogan', '');
      }
    }
    return $value;
  }
}

function nodewords_dc_title_form($type, $value, $settings) {
  return array(
    '#type' => 'textfield',
    '#title' => t('DC.Title'),
    '#default_value' => $value,
    '#description' => t('Enter a value to be used as DC.Title meta tag.'),
  );
}

