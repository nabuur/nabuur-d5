<?php
// $Id: buddylist_views.inc,v 1.1.4.4 2007/01/07 18:23:14 fago Exp $

/**
 * This include file implements views functionality on behalf of the
 * buddylist.module 
 */

function buddylist_views_tables() {
  $tables['buddylist'] = array(
    "name" => "buddylist",
    "join" => array(
      "left" => array(
        "table" => "users",
        "field" => "uid",
      ),
      "right" => array(
        "field" => "buddy",
      ),
    ),
  );

  $tables['buddylist_users'] = array(
    'name' => 'users',
    'join' => array(
      'left' => array(
        'table' => 'buddylist',
        'field' => 'uid'
      ),
      'right' => array(
        'field' => 'uid'
      ),
    ),
    'fields' => array(
      'name' => array(
        'name' => t('Buddylist: User Name'),
        'handler' => 'views_handler_field_username',
        'sortable' => true,
        'uid' => 'uid',
        'addlfields' => array('uid'),
        'help' => t('This will display the username of the owner of the buddylist.'),
      ),
      'uid' => array(
        'name' => t('Buddylist: User Picture'),
        'handler' => 'views_handler_field_userpic',
        'sortable' => false,
        'help' => t('This will display the user picture of the owner of the buddylist.'),
      ),
    ),
    'sorts' => array(
      'name' => array(
        'name' => t('Buddylist: Buddy Name'),
        'help' => t('This allows you to sort alphabetically by buddy name.'),
      )
    ),
    'filters' => array(
      'uid' => array(
        'name' => t('Buddylist: Author Is A Buddy Of Username'),
        'operator' => 'views_handler_operator_or',
        'list' => 'views_handler_filter_username',
        'value-type' => 'array',
        'help' => t('This allows you to filter by buddies of a particular user.'),
      ),
      'currentuid' => array(
	'field' => 'uid',
        'name' => t('Buddylist: Author Is A Buddy Of Current User'),
        'operator' => 'views_handler_operator_eqneq',
        'list' => 'views_handler_filter_usercurrent',
        'help' => t('This allows you to filter by buddies of the current user.'),
        'cacheable' => 'no',
      ),
    ),
  );
  if (module_exists('usernode')) {
    $tables['usernode_buddylist'] = array(
      "name" => "buddylist",
      "join" => array(
        "left" => array(
          "table" => "usernode",
          "field" => "uid",
        ),
        "right" => array(
          "field" => "buddy",
        ),
      ),
      'filters' => array(
        'uid' => array(
          'name' => t('Buddylist: Usernode Is A Buddy Of Username'),
          'operator' => 'views_handler_operator_or',
          'list' => 'views_handler_filter_username',
          'value-type' => 'array',
          'help' => t('This allows you to filter by buddies of a particular user.'),
        ),
        'currentuid' => array(
          'field' => 'uid',
          'name' => t('Buddylist: Usernode Is A Buddy Of Current User'),
          'operator' => 'views_handler_operator_eqneq',
          'list' => 'views_handler_filter_usercurrent',
          'help' => t('This allows you to filter by buddies of the current user.'),
        ),
      ),
    );
    $tables['usernode_buddylist_buddyof'] = array(
      "name" => "buddylist",
      "join" => array(
        "left" => array(
          "table" => "usernode",
          "field" => "uid",
        ),
        "right" => array(
          "field" => "uid",
        ),
      ),
      'filters' => array(
        'uid' => array(
          'name' => t('Buddylist: Username Is A Buddy Of Usernode'),
          'operator' => 'views_handler_operator_or',
          'list' => 'views_handler_filter_username',
          'value-type' => 'array',
          'help' => t('This allows you to filter by buddies of a particular user.'),
        ),
        'currentuid' => array(
          'field' => 'uid',
          'name' => t('Buddylist: Current User Is A Buddy Of Usernode'),
          'operator' => 'views_handler_operator_eqneq',
          'list' => 'views_handler_filter_usercurrent',
          'help' => t('This allows you to filter by buddies of the current user.'),
        ),
      ),
    );
    $tables['usernode_buddylist_buddyof'] = array(
      "name" => "buddylist",
      "join" => array(
        "left" => array(
          "table" => "usernode",
          "field" => "uid",
        ),
        "right" => array(
          "field" => "uid",
        ),
      ),
      'filters' => array(
        'buddy' => array(
          'name' => t('Buddylist: Username Is A Buddy Of Usernode'),
          'operator' => 'views_handler_operator_or',
          'list' => 'views_handler_filter_username',
          'value-type' => 'array',
          'help' => t('This allows you to filter by buddies of a particular user.'),
        ),
        'currentuid' => array(
          'field' => 'buddy',
          'name' => t('Buddylist: Current User Is A Buddy Of Usernode'),
          'operator' => 'views_handler_operator_eqneq',
          'list' => 'views_handler_filter_usercurrent',
          'help' => t('This allows you to filter by buddies of the current user.'),
        ),
      ),
    );
  }
  return($tables);
}

function buddylist_views_default_views() {
  $view = new stdClass();
  $view->name = 'buddylist_posts';
  $view->description = 'A list of nodes created by buddies of current user';
  $view->access = array ();
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'Buddylist Content';
  $view->page_header = 'Nodes authored by buddies of current user<br />';
  $view->page_footer = '';
  $view->page_empty = '';
  $view->page_type = 'table';
  $view->url = 'buddies/posts';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '10';
  $view->sort = array (
    array (
      'tablename' => 'node',
      'field' => 'created',
      'sortorder' => 'DESC',
      'options' => '',
    ),
  );
  $view->argument = array (
  );
  $view->field = array (
    array (
      'tablename' => 'users',
      'field' => 'name',
      'label' => 'Buddy',
    ),
    array (
      'tablename' => 'node',
      'field' => 'title',
      'label' => 'Node Title',
      'handler' => 'views_handler_field_nodelink',
    ),
    array (
      'tablename' => 'node',
      'field' => 'type',
      'label' => 'Node Type',
    ),
  );
  $view->filter = array (
    array (
      'tablename' => 'node',
      'field' => 'status',
      'operator' => '=',
      'options' => '',
      'value' => '1',
    ),
    array (
      'tablename' => 'buddylist_users',
      'field' => 'currentuid',
      'operator' => '=',
      'options' => '',
      'value' => '***CURRENT_USER***',
    ),
  );
  $view->exposed_filter = array (
  );
  $view->requires = array(node, users, buddylist_users);
  $views[$view->name] = $view;
  
  
  if (module_exists('usernode')) {
    $view = new stdClass();
    $view->name = 'buddylist';
    $view->description = 'Buddies of the current user';
    $view->access = array();
    $view->view_args_php = '';
    $view->page = TRUE;
    $view->page_title = 'Buddylist';
    $view->page_header = 'Buddies of the current user<br />';
    $view->page_type = 'table';
    $view->url = 'buddies';
    $view->use_pager = TRUE;
    $view->nodes_per_page = '10';
    $view->sort = array (
      array (
        'tablename' => 'node',
        'field' => 'created',
        'sortorder' => 'DESC',
        'options' => 'normal',
      ),
    );
    $view->field = array (
      array (
        'tablename' => 'node',
        'field' => 'title',
        'label' => 'Name',
        'handler' => 'views_handler_field_nodelink',
        'options' => 'link',
      ),
    );
    $view->filter = array (
      array (
         'tablename' => 'node',
        'field' => 'status',
        'operator' => '=',
        'options' => '',
        'value' => '1',
        ),
      array (
        'tablename' => 'usernode_buddylist',
        'field' => 'currentuid',
        'operator' => '=',
        'options' => '',
        'value' => '***CURRENT_USER***',
      ),
      array (
        'tablename' => 'node',
        'field' => 'type',
        'operator' => 'OR',
        'options' => '',
        'value' => array (0 => 'usernode'),
      ),
    );
    $view->exposed_filter = array (
    );
    $view->requires = array(node, usernode_buddylist);
    $views[$view->name] = $view;
  }

  return($views);
}


/**
 * Implementation of hook_views_arguments
 */
function buddylist_views_arguments() {
  if (module_exists('usernode')) {
    $arguments = array(
      'buddylist_buddies' => array(
        'name' => t('Buddylist: Usernode is buddy of UID'),
        'handler' => 'buddylist_views_handler_arg_buddies',
        'help' => t('This will filter usernodes from users, which are buddies of the user with the ID passed to the view as argument.'),
      ),
      'buddylist_buddies_buddyof' => array(
        'name' => t('Buddylist: UID is buddy of Usernode'),
        'handler' => 'buddylist_views_handler_arg_buddies_buddyof',
        'help' => t('This will filter usernodes from users, of which the user with the ID passed to the view as argument is a buddy of.'),
      ),
    );
    return $arguments;
  }
}

/**
 * Callback for buddylist_views_arguments
 */
function buddylist_views_handler_arg_buddies($op, &$query, $argtype, $arg = '') {
  switch($op) {
    case 'summary':
      $table_data = _views_get_tables();
      $joininfo = $table_data['usernode_buddylist_buddyof']['join'];
      $joininfo['type'] = 'inner';
      $query->add_table("usernode_buddylist_buddyof", true, 1, $joininfo);
      
      $query->add_table('users', true);
      $query->add_field('name', 'users');
      $query->add_field('uid', 'users');
      $fieldinfo['field'] = "users.name";
      return $fieldinfo;
      break;
    case 'sort':
      $query->add_orderby('users', 'name', $argtype);
      break;
    case 'filter':
      $uid = intval($arg);
      $table_data = _views_get_tables();
      $joininfo = $table_data['usernode_buddylist']['join'];
      $joininfo['type'] = 'inner';
      $joininfo['extra'] = array('uid' => $uid);
      $query->add_table("usernode_buddylist", true, 1, $joininfo);
      break;
    case 'link':
      $name = ($query->name ? $query->name : variable_get('anonymous', 'Anonymous'));
      return l($name, "$arg/" . intval($query->uid));
    case 'title':
      if (!$query) {
        return variable_get('anonymous', 'Anonymous');
      }
      $user = db_fetch_object(db_query("SELECT name FROM {users} WHERE uid = '%d'", $query));
      return check_plain($user->name);
  }
}


/**
 * Callback for buddylist_views_arguments
 */
function buddylist_views_handler_arg_buddies_buddyof($op, &$query, $argtype, $arg = '') {
  switch($op) {
    case 'summary':
      $table_data = _views_get_tables();
      $joininfo = $table_data['usernode_buddylist']['join'];
      $joininfo['type'] = 'inner';
      $query->add_table("usernode_buddylist", true, 1, $joininfo);
      
      $query->add_table('users', true);
      $query->add_field('name', 'users');
      $query->add_field('uid', 'users');
      $fieldinfo['field'] = "users.name";
      return $fieldinfo;
      break;
    case 'sort':
      $query->add_orderby('users', 'name', $argtype);
      break;
    case 'filter':
      $uid = intval($arg);
      $table_data = _views_get_tables();
      $joininfo = $table_data['usernode_buddylist_buddyof']['join'];
      $joininfo['type'] = 'inner';
      $joininfo['extra'] = array('buddy' => $uid);
      $query->add_table("usernode_buddylist_buddyof", true, 1, $joininfo);
      break;
    case 'link':
      $name = ($query->name ? $query->name : variable_get('anonymous', 'Anonymous'));
      return l($name, "$arg/" . intval($query->uid));
    case 'title':
      if (!$query) {
        return variable_get('anonymous', 'Anonymous');
      }
      $user = db_fetch_object(db_query("SELECT name FROM {users} WHERE uid = '%d'", $query));
      return check_plain($user->name);
  }
}
