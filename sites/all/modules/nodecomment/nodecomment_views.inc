<?php
// $Id: nodecomment_views.inc,v 1.2.2.9 2008/07/30 19:28:38 sirkitree Exp $

/**
 * This include file implements views functionality on behalf of relationship.module
 */

function nodecomment_views_tables() {
  $tables['node_comments'] = array(
    'name' => 'node_comments',
    'provider' => 'internal',
    'join' => array(
      'left' => array(
        'table' => 'node',
        'field' => 'nid'
      ),
      'right' => array(
        'field' => 'cid'
      )
    ),
    'fields' => array(
      'nid' => array(
        'name' => t('Node comment: Parent node'),
        'help' => t('Display the node the comment was posted in response to'),
      ),
      'name' => array(
        'name' => t('Node comment: User name'),
        'help' => t('Display the name of the user who posted the comment'),
      ),
      'mail' => array(
        'name' => t('Node comment: Email address'),
        'help' => t('Display the email address of the user who posted the comment'),
      ),
      'homepage' => array(
        'name' => t('Node comment: Homepage'),
        'help' => t('Display the home page of the user who posted the comment'),
      ),
    ),
    'filters' => array(
      'nid' => array(
        'name' => t('Node comment: Parent node'),
        'operator' => views_handler_operator_eqneq(),
        'help' => t('This allows you to filter based on the parent node.'),
      ),
    ),
    'sorts' => array(
      'thread' => array(
        'name' => t('Node comment: Threaded comments'),
        'handler' => 'nodecomment_views_handler_sort_thread',
        'help' => t('Sort by discussion thread.')
      ),
    ),
  );

  $tables['node_comment_statistics'] = array(
    'name' => 'node_comment_statistics',
    'provider' => 'internal',
    'join' => array(
      'left' => array(
        'table' => 'node',
        'field' => 'nid'
      ),
      'right' => array(
        'field' => 'nid'
      ),
    ),
    'fields' => array(
      'last_comment_timestamp' => array(
        'name' => t('Node comment: Last Comment Time'),
        'sortable' => true,
        'handler' => views_handler_field_dates(),
        'option' => 'string',
        'help' => t('This will display the last comment time.'),
       ),
      'last_comment_name' => array(
        'name' => t('Node comment: Last Comment Author'),
        'query_handler' => 'views_query_handler_field_last_comment_name',
        'handler' => 'views_handler_field_last_comment_name',
        'sortable' => true,
        'uid' => 'last_comment_uid',
        'addlfields' => array('last_comment_name', 'last_comment_uid'),
        'help' => t('This will display the name of the last user to comment on the post.'),
      ),
      'comment_count' => array(
        'name' => t('Node comment: Count'),
        'sortable' => true,
        'handler' => array(
          'views_handler_field_int'         => t('Normal'),
          'views_handler_comments_with_new' => t('With New Count')
        ),
        'help' => t('This will display the comment count.'),
      ),
    ),
    'filters' => array(
      'comment_count' => array(
        'name' => t('Node comment: Comment Count'),
        'operator' => 'views_handler_operator_gtlt',
        'option' => 'integer',
        'help' => t('This filter allows you to filter by the amount of comments.'),
      ),
      'last_comment_timestamp' => array(
        'name' => t('Node comment: Last Comment Time'),
        'operator' => 'views_handler_operator_gtlt',
        'value' => views_handler_filter_date_value_form(),
        'handler' => 'views_handler_filter_timestamp',
        'option' => 'string',
        'help' => t('This filter allows nodes to be filtered by the last comment timestamp. Enter dates in the format: CCYY-MM-DD HH:MM:SS. Enter \'now\' to use the current time. You may enter a delta (in seconds) to the option that will be added to the time; this is most useful when combined with now. If you have the jscalendar module from jstools installed, you can use a popup date picker here.'),
      ),
    ),
    'sorts' => array(
      'last_comment_timestamp' => array(
        'name' => t('Node comment: Last Comment Date'),
        'handler' => 'views_handler_sort_date',
        'option' => views_handler_sort_date_options(),
        'help' => t('This will allow you to sort by the date of the most recent comment on a node.')
      ),
      'comment_count' => array(
        'name' => t('Node comment: Comment Count'),
        'help' => t('This filter allows you to sort by the number of comments.'),
      ),
    ),
  );

  return $tables;
}

function nodecomment_views_arguments() {
  $arguments = array(
    'comment_target_nid' => array(
      'name' => t("Node comment: Parent node ID"),
      'handler' => "views_handler_arg_comment_target_nid",
    ),
  );
  return $arguments;
}

function nodecomment_views_default_views() {
  $flatview = new stdClass();
  $flatview->name = 'node_comments';
  $flatview->description = 'Node comments flat';
  $flatview->access = array();
  $flatview->view_args_php = '$args_rev = array_reverse($args);
  if (strtolower($args_rev[0]) == \'desc\') {
    $view->sort[0][\'sortorder\'] = \'DESC\';
  }
  return $args;';
  $flatview->sort = array(
    array(
      'tablename' => 'node',
      'field' => 'sticky',
      'sortorder' => 'DESC',
      'options' => '',
    ),
    array(
      'tablename' => 'node',
      'field' => 'created',
      'sortorder' => 'ASC',
      'options' => 'normal',
    ),
  );
  $flatview->argument = array(
    array(
      'type' => 'comment_target_nid',
      'argdefault' => '1',
      'title' => '',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
  );
  $flatview->field = array(
  );
  $flatview->filter = array(
    array(
      'tablename' => 'node',
      'field' => 'status',
      'operator' => '=',
      'options' => '',
      'value' => '1',
    ),
    array(
      'tablename' => 'node',
      'field' => 'moderate',
      'operator' => '=',
      'options' => '',
      'value' => '0',
    ),
  );
  $flatview->exposed_filter = array();
  $flatview->requires = array('node');
  $flatview->page_type = 'node';
  $views[$flatview->name] = $flatview;

  // Threaded views
  $threadedview = new stdClass();
  $threadedview->name = 'node_comments_threaded';
  $threadedview->description = 'Node comments threaded';
  $threadedview->access = array();
  $threadedview->view_args_php = '';
  $threadedview->sort = array(
    array(
      'tablename' => 'node_comments',
      'field' => 'thread',
      'sortorder' => 'ASC',
      'options' => '',
    ),
  );
  $threadedview->argument = array(
    array(
      'type' => 'comment_target_nid',
      'argdefault' => '1',
      'title' => '',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
  );
  $threadedview->field = array();
  $threadedview->filter = array(
    array(
      'tablename' => 'node',
      'field' => 'status',
      'operator' => '=',
      'options' => '',
      'value' => '1',
    ),
    array(
      'tablename' => 'node',
      'field' => 'moderate',
      'operator' => '=',
      'options' => '',
      'value' => '0',
    ),
  );
  $threadedview->exposed_filter = array();
  $threadedview->requires = array(node_comments, node);

  // Secret sauce for threaded comments - the style plugin
  $threadedview->page_type = 'threaded_comments';

  $views[$threadedview->name] = $threadedview;

  return $views;
}

function nodecomment_views_style_plugins() {
  $items['threaded_comments'] = array(
    'name' => t('Node comment: Threaded comments'),
    'theme' => 'nodecomment_threaded_comments',
    'needs_fields' => true,
  );
  return $items;
}

function views_handler_arg_comment_target_nid($op, &$query, $argtype, $arg = '') {
  switch ($op) {
    case 'summary' :
      // Not supported at present
      break;
    case 'sort':
      // Not supported at present
      break;
    case 'filter' :
      $query->ensure_table("node_comments");
      $query->add_where("node_comments.nid = %d", $arg);
      break;
    case 'link' :
      // Not supported at present
      break;
    case 'title' :
      if ($query) {
        $n = db_fetch_object(db_query("SELECT title FROM {node} WHERE nid = %d", $query));
        return $n->title;
      }
  }
}


// This code is copied shamelessly from views module's views_comment.inc.
// Since comment.module isn't enabled, we lose this functionality. But we
// do, indeed, want it for building forum views and so on.

function views_query_handler_field_last_comment_name($field, $fieldinfo, &$query) {
  $num = $query->add_table('users', false, 1, array(
    'left' => array(
      'table' => $field['tablename'],
      'field' => 'last_comment_uid'
    ),
    'right' => array(
      'field' => 'uid'
    ),
  ));
  $query->add_field('name', $query->get_table_name('users', $num), $field['tablename'] .'_name');
}

function views_handler_field_last_comment_name($fieldinfo, $fielddata, $value, $data) {
  $obj->name = $value;
  $uidfield = $fielddata['tablename'] ."_". $fieldinfo['uid'];
  $obj->uid = $data->$uidfield;
  if ($obj->uid != 0) {
    $fieldname = $fielddata['tablename'] .'_name';
    $obj->name = $data->$fieldname;
  }
  return theme('username', $obj);
}

/*
 * Format a field as a link to a comment.
 */
function views_handler_field_commentlink($fieldinfo, $fielddata, $value, $data) {
  if ($fielddata['options'] == 'nolink') {
    return check_plain($value);
  }
  return l($value, "node/$data->nid", NULL, NULL, "comment-$data->comments_cid");
}

/*
 * Format a field as a number of comments, plus the number of unread comments.
 */
function views_handler_comments_with_new($fieldinfo, $fielddata, $value, $data) {
  $comments = intval($value);
  if ($comments && $new = nodecomment_num_new($data->nid)) {
    $comments .= ' ';
    //comment_page_new_query($data->nid)
    $comments .= l(t('@num new', array('@num' => $new)), "node/$data->nid", NULL, NULL, 'new');
  }
  return $comments;
}

/*
 * Format a field as a link to a 'mark', stating whether or not the comment has
 * updated since it was last viewed by the user.
 */
function views_handler_field_commentlink_with_mark($fieldinfo, $fielddata, $value, $data) {
  if ($fielddata['options'] == 'nolink') {
    $link = check_plain($value);
  }
  else {
    $link = l($value, "node/$data->nid", NULL, NULL, "comment-$data->comments_cid");
  }
  return $link .' '. theme('mark', node_mark($data->nid, $data->comments_timestamp));
}

function nodecomment_views_handler_sort_thread($op, &$query, $sortinfo, $sortdata) {
  // For descending sorts, natural order is ok.
  if ($sortdata['sortorder'] == 'DESC') {
    $query->add_orderby($sortinfo['table'], $sortinfo['field'], 'DESC');
  }
  // For ascending sorts special logic is needed so that only the top level
  // of the thread is sorted ascending, but the sub-threads are sorted DESC.
  // See code comments for comment_render.
  else {
    $alias = $sortdata['field'];
    $query->orderby[] = "SUBSTRING($alias, 1, (LENGTH($alias) - 1))";
  }
}

function theme_nodecomment_threaded_comments($view, $nodes, $type) {
  $divs = 0;
  $last_depth = 0;
  drupal_add_css(drupal_get_path('module', 'comment') .'/comment.css');
  foreach ($nodes as $n) {
    $node = node_load($n->nid);
    $node->depth = count(explode('.', $node->thread)) - 1;


    if ($node->depth > $last_depth) {
      $divs++;
      $output .= '<div class="indented">';
      $last_depth++;
    }
    else {
      while ($node->depth < $last_depth) {
        $divs--;
        $output .= '</div>';
        $last_depth--;
      }
    }

    if ($mode == COMMENT_MODE_THREADED_COLLAPSED) {
      //$output .= theme('comment_thread_collapsed', $node);
    }
    // $mode == COMMENT_MODE_THREADED_EXPANDED
    else {
      $output .= theme('nodecomment_view', $node);
    }
  }
  for ($i = 0; $i < $divs; $i++) {
    $output .= '</div>';
  }

  return $output;
}

function theme_nodecomment_view($comment) {
  static $first_new = TRUE;

  $output = '';
  $comment->new = node_mark($comment->nid, $comment->timestamp);
  if ($first_new && $comment->new != MARK_READ) {
    // Assign the anchor only for the first new comment. This avoids duplicate
    // id attributes on a page.
    $first_new = FALSE;
    $output .= "<a id=\"new\"></a>\n";
  }

  $output .= "<a id=\"comment-$comment->nid\"></a>\n";

  // Switch to folded/unfolded view of the comment
  $output .= node_view($comment, FALSE, FALSE, TRUE);

  return $output;
}
