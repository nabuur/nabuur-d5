<?php
// $Id: copyright.inc,v 1.1.2.1 2008/01/08 18:48:33 robrechtj Exp $

/**
 * @file
 * Support file for COPYRIGHT meta tag.
 */

function nodewords_copyright_prepare($type, $ids, $value, $settings) {
  if ((!isset($value) || $value == '')) {
    $value = $settings['global']['copyright'];
  }
  return $value;
}

function nodewords_copyright_form($type, $value, $settings) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Copyright'),
    '#default_value' => $value,
    '#size' => 60,
    '#maxlength' => $settings['max_size'],
    '#description' => t('Enter a short copyright statement for this page.'),
  );
}
