<?php
// $Id: nodeprofile_workflow_ng.inc,v 1.1.2.5 2008/03/02 15:53:00 fago Exp $

/**
 * @file
 * Some workflow-ng conditions/actions
 */

/*
 * Implementation of hook_condition_info()
 */
function nodeprofile_condition_info() {
  return array(
    'nodeprofile_user_has_profile_condition' => array(
      '#label' => t('User has nodeprofile'),
      '#arguments' => array(
        'user' => array('#entity' => 'user', '#label' => t('User')),
      ),
      '#module' => 'Node Profile',
    ),
  );
}

function nodeprofile_user_has_profile_condition($user, $settings) {
  $node = nodeprofile_load($settings['type'], $user->uid);
  return (bool)$node;
}

function nodeprofile_user_has_profile_condition_form($settings) {
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Node Profile Content Type'),
    '#options' => nodeprofile_get_types('names'),
    '#default_value' => isset($settings['type']) ? $settings['type'] : array(),
    '#description' => t('Select the node profile content type, which the user must have created.'),
    '#required' => TRUE,
  );
  return $form;
}

function nodeprofile_user_has_profile_condition_submit($form_id, $form_values) {
  return array('type' => $form_values['type']);
}

/**
 * Implementation of hook_action_info()
 */
function nodeprofile_action_info() {
  return array(
    'nodeprofile_action_load' => array(
      '#label' => t('Load node profile'),
      '#arguments' => array(
        'user' => array('#entity' => 'user', '#label' => t('User, whose profile should be loaded')),
      ),
      '#new arguments' => array(
        'node_loaded' => array('#entity' => 'node', '#label' => t('Loaded node profile')),
      ),
      '#module' => 'Node Profile',
    ),
  );
}

/**
 * Loads a node profile
 */
function nodeprofile_action_load($user, $settings, &$arguments, &$log) {
  if ($node = nodeprofile_load($settings['type'], $user->uid)) {
    return array('#new arguments' => array('node_loaded' => $node));
  }
}

function nodeprofile_action_load_form($settings) {
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Node Profile Content Type'),
    '#options' => nodeprofile_get_types('names'),
    '#default_value' => isset($settings['type']) ? $settings['type'] : array(),
    '#description' => t('Select the node profile content type to load.'),
    '#required' => TRUE,
  );
  return $form;
}

function nodeprofile_action_load_submit($form_id, $form_values) {
  return array('type' => $form_values['type']);
}
