<?php
// $Id: abstract.inc,v 1.1.2.1 2008/01/08 18:48:33 robrechtj Exp $

/**
 * @file
 * Support file for ABSTRACT meta tag.
 */

function nodewords_abstract_form($type, $value, $settings) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Abstract'),
    '#default_value' => $value,
    '#size' => 60,
    '#maxlength' => $settings['max_size'],
    '#description' => t('Enter a short abstract for this page. Typically it is one sentence.'),
  );
}
