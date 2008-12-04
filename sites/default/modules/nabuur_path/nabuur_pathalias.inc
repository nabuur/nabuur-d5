<?php
// $Id: nabuur_pathalias.inc,v 1.1.2.10 2008/11/24 12:41:09 fransk Exp $

/**
 * @file nabuur_pathalias.inc - the custom url rewrite rules for nabuursite
 */

//require_once('../../includes/path.inc');

/**
 * Returns url path pieces used by nabuur links.
 *
 * Strange construct is here to keep the details in one location, but not to call t() before it is declared.
 */
function _nabuur_path_settings($d = TRUE) {
  // path pieces
  $types = array( 'village', 'group', );
  $subpages = array( '', 'background', 'images', 'news', 'project', 'resources', 'neighbours',); //'chat' );

  if (! $d) {
    return array($types, $subpages);
  }

  // Description, menu info, any other data required
  // content type used to decide which to give 'add' links to on pages
  // order is important on the 'Our Project' page
  $types_data = array(t('Villages'), t('Groups'));
  $subpages_data = array(
    array('title' => t('Overview'), 'weight' => -15,),
    array('title' => t('Background'), 'weight' => -10, 'content-type' => array('villagebackground'),), 
    array('title' => t('Gallery'), 'weight' => 0, 'content-type' => array('image'),), 
    array('title' => t('News'), 'weight' => -2, 'content-type' => array('news_item','newsletter','feedapi_node'),), 
    array('title' => t('Project room'), 'weight' => -8, 'content-type' => array('project_outline', 'task', 'task_wiki'),),
    array('title' => t('Resources'), 'weight' => 10),
    array('title' => t('Neighbours'), 'weight' => -5),
//    array('title' => t('Chat'), 'content-type' => array('chatroom'), ),
  );

  return array(array_combine($types, $types_data), array_combine($subpages, $subpages_data));
}

/**
 * Local version of custom_url_rewrite().
 */
function custom_url_rewrite($op, $result, $path) {
  // @todo drupal doesn't cache this, we could use static variable here
  list($types, $subpages) = _nabuur_path_settings(FALSE); 
  $types_or = implode($types, '|');
  $subpages_or = implode($subpages, '|');

  if ($op == 'alias') {
    $result = i18n_url_rewrite($op, $result, $path);
    $lang = i18n_get_lang_prefix($result, TRUE);
    if (preg_match("@og/users/([0-9]+)/?(faces|add)?$@", $path, $path_pieces)) {
      // go to nabuur neighbours page rather than standard og/users pages
      $path = 'node/'. $path_pieces[1] .'/neighbours';
      if ($path_pieces[2] == 'add') {
        $path .= '/add';
      }
    }
    elseif (preg_match("@node/([0-9]+)/($subpages_or)(?:/feed)?$@", $path, $path_pieces)) {
      // [group|village]/[background|images|...]
      $group_node = 'node' .'/'. $path_pieces[1];
      $group_path = drupal_lookup_path('alias', $group_node);
      if ($group_path != '') {
        $result = $group_path .'/'. $path_pieces[2];
      } 
    }
    elseif (preg_match("@user/([0-9]+)(?:/[0-9]+)?$@", $path, $path_pieces)) {
      // can now gete nodeprofile natively within panel from relationship -- @todo check, remove
      /* user page, make profile node context available for panels if created
      if (is_numeric($path_pieces[1])) {
        $user_profile = nabuur_users_nodeprofile_load((int) $path_pieces[1]);
        if (is_object($user_profile)) {
          $result = $path .'/'. $user_profile->nid;
        }
      }*/
    }
    elseif (preg_match('@^facet_search/(.*)@', $path, $path_pieces)) {
      $result = 'search/'. $path_pieces[1];
    }
    $result = $lang ? $lang .'/'. $result : $result;
  }

  if ($op == 'source') {
    $result = i18n_url_rewrite($op, $result, $path);
    if (preg_match("@og/users/([0-9]+)/?(faces|add)?$@", $result, $path_pieces)) {
      $result = 'node/'. $path_pieces[1] .'/neighbours';
      if ($path_pieces[2] == 'add') {
        $result .= '/add';
      }
    }     
    if (preg_match("@($types_or)/([^/\?\s]+)/($subpages_or)(?:/feed)?$@", $result, $path_pieces)) {
      // [group|village]/[background|images|...]
      $group_type = $path_pieces[1];
      $group_path = $group_type .'/'. $path_pieces[2];
      if (function_exists('drupal_lookup_path')) {
        $group_node = drupal_lookup_path('source', $group_path);
      }
      if ($group_node != '') {
        $result = $group_node .'/'. $path_pieces[3];
      } 
    }
    // @todo user page, allow user/uid/nid to map to user/username too
    else {
      // if it's a search change to a facet search
      $result = preg_replace('@^search/@', 'facet_search/', $result);
    }
    $result = i18n_url_rewrite($op, $result, $path);
  }

  return $result;
}
