<?php
// $Id: nodeprofile_panels.inc,v 1.1.2.1 2008/02/04 11:29:32 fago Exp $

/**
 * @file
 * Panels integration
 */

/**
 * Plugin to provide an relationship handler for node from user
 */
function nodeprofile_panels_relationships() {
  $args['node_from_user'] = array(
    'title' => t("Nodeprofile from user"),
    'keyword' => 'nodeprofile',
    'description' => t('Adds a nodeprofile from user context'),
    'required context' => new panels_required_context(t('User'), 'user'),
    'context' => 'nodeprofile_panels_context',
    'settings form' => 'nodeprofile_panels_settings_form',
    'settings form validate' => 'nodeprofile_panels_settings_form_validate',
  );
  return $args;
}

/**
 * Return a new context based on an existing context
 */
function nodeprofile_panels_context($context = NULL, $conf) {
  // If unset it wants a generic, unfilled context, which is just NULL
  if (empty($context->data)) {
    return panels_context_create_empty('node', NULL);
  }

  if (isset($context->data->uid)) {
    // Load the node for the requested type
    $uid = $context->data->uid;
    $nodeprofilenode = nodeprofile_load($conf['type'], $uid);
   
    // Send it to panels
    return panels_context_create('node', $nodeprofilenode);
  }
  else {
    return panels_context_create_empty('node', NULL);
  }   
}

/**
 * Settings form for the relationship
 */
function nodeprofile_panels_settings_form($conf) {
  $options = nodeprofile_get_types('names');
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Relationship type'),
    '#options' => $options,
    '#default_value' => $conf['type']
  );

  return $form;
}
