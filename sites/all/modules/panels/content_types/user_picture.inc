<?php
// $Id: user_picture.inc,v 1.1.2.3 2008/05/27 20:21:11 sdboyer Exp $


/**
 * Callback function to supply a list of content types.
 */
function panels_user_picture_panels_content_types() {
  $items['user_picture'] = array(
    'title' => t('User picture'),
    'content_types' => 'panels_admin_content_types_user_picture',
    'single' => TRUE,
    'render callback' => 'panels_content_user_picture',
    'title callback' => 'panels_admin_title_user_picture',
  );
  return $items;
}

/**
 * Output function for the 'node' content type. Outputs a node
 * based on the module and delta supplied in the configuration.
 */
function panels_content_user_picture($conf, $panel_args, $context) {
  if (! user_access('access user profiles') && ! user_access('administer users')) {
    return;
  }
  if (! is_object($context->data)) {
    return;
  }

  $account = drupal_clone($context->data);
  $block = new stdClass();
  $block->module = 'term-list';

  $block->title = check_plain($account->name);
  $block->content = theme('user_picture', $account);

  return $block;
}

/**
 * Return all content types available.
 */
function panels_admin_content_types_user_picture() {
  return array(
    'description' => array(
      'title' => t('User picture'),
      'icon' => 'icon_user.png',
      'path' => panels_get_path('content_types/user'),
      'description' => t('The picture of a user.'),
      'required context' => new panels_required_context(t('User'), 'user'),
      'category' => array(t('User context'), -9),
    ),
  );
}

/**
 * Display the administrative title for a panel pane in the drag & drop UI
 */
function panels_admin_title_user_picture($conf, $context) {
  return t('"@s" user picture', array('@s' => $context->identifier));
}

