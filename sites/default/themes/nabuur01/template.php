<?php // $Id: template.php,v 1.1.1.29 2008/12/05 11:24:08 fransk Exp $

/**  = breaking the panels overide sidebars function
function nabuur01_regions() {
  return array(
    'sidebar_right' => t('right sidebar'),
    'content' => t('content'),
    'header' => t('header'),
    'footer' => t('footer'),
    'front_footer' => t('frontpage footer')
  );
}
/*

/**
 * Sets the body-tag class attribute.
 *
 * Adds 'sidebar-left', 'sidebar-right' or 'sidebars' classes as needed.
 */
function phptemplate_body_class($sidebar_right) {
  $path = explode('/', drupal_get_path_alias($_GET['q']));
  if (!$path[3]) {
    if ($path[1] == 'front') {
      $class = 'frontpage';
    }
    else {
      $class = $path[1] .'-frontpage';
    }
  }
  else {
    $class = $path[1] .' '. $path[1] .'-'. $path[3];
  }
  if ($sidebar_right) {
    $class .= ' sidebar-right';
  }
  // Add class .test-site if its not on www.nabuur.com
  if(strpos((url('/',NULL,NULL,TRUE)),'www.nabuur') === FALSE) {
    $class .= ' test-site';
  }
  
  if (isset($class)) {
    print ' class="'. $class .'"';
  }
}

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function phptemplate_breadcrumb($breadcrumb) {
  return nabuur_breadcrumb($breadcrumb);
}

/**
 * Override or insert PHPTemplate variables into the templates.
 */
function _phptemplate_variables($hook, $vars) {
  if ($hook == 'page') {
    if ($secondary = menu_secondary_local_tasks()) {
      $output = '<span class="clear"></span>';
      $output .= "<ul class=\"tabs secondary\">\n". $secondary ."</ul>\n";
      $vars['tabs2'] = $output;
    }

    // Hook into color.module
    if (module_exists('color')) {
      _color_page_alter($vars);
    }
    set_nabuur_login_message($vars);
    set_nabuur_continents($vars);
    return $vars;
  }
  else if ($hook == 'node') {
    $vars['picture'] = theme('nabuur_users_user_picture', $vars['uid']);
    return $vars;
  }
  return $vars;
}


function set_nabuur_login_message(&$vars) {
  global $user;
  
  if ($user->uid > 0) {
    $message = '<div id="login">'. l(t('Logout') .' '. $user->name, 'logout') .'</div>';
  }
  else {
    $message = '<div id="login">'. l(t('Login'), 'user/login') . ' | ' . l(t('Join'), 'user/register') .'</div>';
  }
  $vars['login_message'] = $message;
}

function set_nabuur_continents(&$vars) {
  $result = l(t('Africa'),'village/results/taxonomy%3A8');
  $result .= ' | '. l(t('Asia'), 'village/results/taxonomy%3A9');
  $result .= ' | '. l(t('Latin-America'), 'village/results/taxonomy%3A10');
  $vars['continents'] = $result;
}

/**
 * Returns the rendered local tasks. The default implementation renders
 * them as tabs.
 *
 * @ingroup themeable
 */
function phptemplate_menu_local_tasks() {
  $output = '';

  if ($primary = menu_primary_local_tasks()) {
    $output .= "<ul class=\"tabs primary\">\n". $primary ."</ul>\n";
  }
  if ($secondary = menu_secondary_local_tasks()) {
    $output .= "<ul class=\"tabs secondary\">\n". $secondary ."</ul>\n";
  }
  
  return $output;
}

/**
 * Allow themable wrapping of all comments.
 */
function phptemplate_comment_wrapper($content, $type = null) {
  static $node_type;
  if (isset($type)) $node_type = $type;

  if (!$content || $node_type == 'forum') {
    return '<div id="comments">'. $content . '</div>';
  }
  else {
    return '<div id="comments"><h2 class="comments">'. t('Comments') .'</h2>'. $content .'</div>';
  }
}

/**
* Display the nodes of a view as a table.
* see theme_views_view_list for a definition of $type
*/
function phptemplate_views_view_content_project_forum($view, $nodes, $type) {
  $fields = _views_get_fields();

  foreach ($nodes as $node) {
    $row = array();
    foreach ($view->field as $field) {
      //$cell['data'] = views_theme_field('views_handle_field', $field['queryname'], $fields, $field, $node, $view);
      $cell['data'] = '<pre>'.print_r($field, true).'</pre>';
      $cell['class'] = "view-field view-field-$field[queryname]";
      $row[] = $cell;
    }
    $rows[] = $row;
  }
  return theme('table', $view->table_header, $rows);
}

/**
 * Overide Online Users list
 */
function nabuur01_user_list($users,$title = NULL) {
  if (!empty($users)) {
    foreach ($users as $user) {
      $items[] = theme('username', $user);
    }
  }
  if ($title ==  t('Online users')) {
    $title = ' ';
  }
  return theme('item_list', $items, $title);
}

function nabuur01_links($links, $attributes = array('class' => 'links')) {
  if (is_array($links)) {
    foreach ($links as $type => &$link) {
//      print  '<br>'. $link['title'] .' type '. $type;
      if ($type == 'comment_add' && $link['title'] == t('Add new Task discussion')) {
        $link['title'] = t('Reply to this task');
      }
      if ($type == 'forward_links' && strpos($link['title'],t('Email this page'))) {
        $link['title'] = str_replace ( t('Email this page')  , t('Send to a friend')  , $link['title']);
      }
    }
  }
  return phptemplate_links($links, $attributes);
}

/**
 * Override primary and secondary links to use div's
 */
function phptemplate_links($links, $attributes = array('class' => 'links')) {
  $classes = explode(' ', $attributes['class']);
  if (in_array('primary-links', $classes) || in_array('secondary-links', $classes)) {
    return _phptemplate_links_as_div($links, $attributes);
  }
  else if (in_array('inline', $classes)) {
    return _phptemplate_links_inline($links, $attributes);
  }
  else {
    return $output . theme_links($links, $attributes);
  }
}

function _phptemplate_links_as_div($links, $attributes = array('class' => 'links'))
{
  $output = '';
  if (count($links) > 0) {
    $output = '<div'. drupal_attributes($attributes) .'>';

    $num_links = count($links);
    $i = 1;

    foreach ($links as $key => $link) {
      $class = '';

      // Automatically add a class to each link and also to each LI
      if (isset($link['attributes']) && isset($link['attributes']['class'])) {
        $link['attributes']['class'] .= ' ' . $key;
        $class = $key;
      }
      else {
        $link['attributes']['class'] = $key;
        $class = $key;
      }

      // Add first and last classes to the list of links to help out themers.
      $extra_class = '';
      if ($i == 1) {
        $extra_class .= 'first ';
      }
      if ($i == $num_links) {
        $extra_class .= 'last ';
      }
      // Add active class to the active link
      if ((substr($class, -6) == 'active')) {
        $extra_class .= 'active ';
      }
      elseif (strpos(drupal_get_path_alias($_GET['q']), $link['href']) === 0 || strpos(drupal_get_path_alias($_GET['q']), '/'. trim($link['href'],'/')) == 2) {
        // HACK -- Overview must be exact not in the path
        if ($link['title'] == t('Overview')) {
          if (drupal_get_path_alias($_GET['q']) == i18n_get_lang_prefix(drupal_get_path_alias($_GET['q'])) .'/'. trim($link['href'],'/')) {
            $extra_class .= 'active ';
          }
        }
        else {
          $extra_class .= 'active ';
        }
      }

      $output .= '<div class="tab '. $extra_class . $class ."\">\n";
      $output .= '<div class="left"></div>';
      $output .= '<div class="link">';

      // Is the title HTML?
      $html = isset($link['html']) && $link['html'];

      // Initialize fragment and query variables.
      $link['query'] = isset($link['query']) ? $link['query'] : NULL;
      $link['fragment'] = isset($link['fragment']) ? $link['fragment'] : NULL;
 
      if (isset($link['href'])) {
        $output .= l($link['title'], $link['href'], $link['attributes'], $link['query'], $link['fragment'], FALSE, $html);
      }
      else if ($link['title']) {
        //Some links are actually not links, but we wrap these in <span> for adding title and class attributes
        if (!$html) {
          $link['title'] = check_plain($link['title']);
        }
        $output .= '<span'. drupal_attributes($link['attributes']) .'>'. $link['title'] .'</span>';
      }
  
      $i++;
      $output .= "</div><div class=\"right\"'></div>\n";
      $output .= "</div>\n";
    }

    $output .= '</div>';
  } 
  return $output;
}

function _phptemplate_links_inline($links, $attributes) {
  foreach ($links as $key => $link) {
    $index = strpos($key, '_');
    if (!$index) $index=strpos($key, '-');
    if ($index) {
      $group = substr($key, 0, $index);
    }
    else {
      $group = 'other';
    }
    $link_groups[$group][$key] = $link;
  }
  $output = '';
  if (count($link_groups) == 0) {
    return '';
  }
  else if (count($link_groups) == 1) {
    return theme_links($links, $attributes);
  }
  else {
    $output = '<div'. drupal_attributes($attributes) .">\n";
    foreach ($link_groups as $group => $links) {
      $output .= _phptemplate_links_as_div($links, array('class' => "link-group link-group-$group")) . "\n";
    }
    $output .= '</div>';
    return $output;
  } 
}


function phptemplate_views_more($url) {
  return '<div align="right" class="views_more"><ul><li>' . l(t('more'), $url) . '</li></ul></div>';
}


/**
 * Render a panel pane like a block.
 * from modules/panels/panel.module
 * @see theme_panels_pane()
 *
 * Alterations: t() added to translate title
 *              surround title with <div class="title">
 *
 * A panel pane can have the following fields:
 *
 * $pane->type -- the content type inside this pane
 * $pane->subtype -- The subtype, if applicable. If a view it will be the
 *   view name; if a node it will be the nid, etc.
 * $content->title -- The title of the content
 * $content->content -- The actual content
 * $content->links -- Any links associated with the content
 * $content->more -- An optional 'more' link (destination only)
 * $content->admin_links -- Administrative links associated with the content
 * $content->feeds -- Any feed icons or associated with the content
 * $content->subject -- A legacy setting for block compatibility
 * $content->module -- A legacy setting for block compatibility
 * $content->delta -- A legacy setting for block compatibility
 */
function phptemplate_panels_pane($content, $pane, $display) {
  if (!empty($content->content)) {
    $idstr = $classstr = '';
    if (!empty($content->css_id)) {
      $idstr = ' id="' . $content->css_id . '"';
    }
    $nabuur_boxed=false;
    if (!empty($content->css_class)) {
      if(substr($content->css_class, 0, 7) == 'nabuur-') {
        $nabuur_boxed = true;
      }
      else {
        $classstr = ' ' . $content->css_class;
      }
    }

    $output = "<div class=\"panel-pane$classstr\"$idstr>\n";
    if (user_access('view pane admin links') && !empty($content->admin_links)) {
      $output .= "<div class=\"admin-links panel-hide\">" . theme('links', $content->admin_links) . "</div>\n";
    }
    $box_content='';
    if (!empty($content->title)) {
      /** Row modified by GJI to add title div */
      $box_content .= "<div class=\"title\"><h2 class=\"title\">". t($content->title) ."</h2></div>\n";
    }

    if (!empty($content->feeds)) {
      $box_content .= "<div class=\"feed\">" . implode(' ', $content->feeds) . "</div>\n";
    }

    $box_content .= "<div class=\"content\">$content->content</div>\n";

    if (!empty($content->links)) {
      $box_content .= "<div class=\"links\">" . theme('links', $content->links) . "</div>\n";
    }


    if (!empty($content->more)) {
      if (empty($content->more['title'])) {
        $content->more['title'] = t('more');
      }
      $box_content .= "<div class=\"more-link\">" . l($content->more['title'], $content->more['href']) . "</div>\n";
    }
    if ($nabuur_boxed) {
      $output .= theme('nabuur_boxed', $box_content, $content->css_class);
    }
    else {
      $output .= $box_content;
    }
    $output .= "</div>\n";
    return $output;
  }
}

/**
 * This function uses heredoc notation to make it easier to convert
 * to a template.
 */
function phptemplate_panels_flexible($id, $content, $settings) {
  if (empty($settings)) {
    $settings = panels_flexible_default_panels();
  }

  // Special check for updating.
  if (empty($settings['width_type'])) {
    $settings['width_type'] = '%';
    $settings['percent_width'] = 100;
  }

  if ($id) {
    $idstr = " id='$id'";
    $idcss = "#$id";
  }
  else {
    $idcss = "div.panel-flexible";
  }

  $css = '';
  $output = '';

  for ($row = 1; $row <= intval($settings['rows']); $row++) {
    $output .= "<div class=\"panel-row panel-row-$row clear-block\">\n";
    for ($col = 1; $col <= intval($settings["row_$row"]["columns"]); $col++) {
      // We do a width reduction formula to help IE out a little bit. If width is 100%, we take 1%
      // off the total; by dividing by the # of columns, that gets us the reduction overall.
      $reduce = 0;
      if ($settings['width_type'] == '%' && $settings['percent_width'] == 100) {
        $reduce = 1 / $settings["row_$row"]["columns"];
      }
      if ($col == 1) {
        if (intval($settings["row_$row"]["columns"]) == 1) {
          $class = 'panel-col-only';
        }
        else {
          $class = 'panel-col-first';
        }
      }
      elseif ($col == intval($settings["row_$row"]["columns"])) {
        $class = 'panel-col-last';
      }
      else {
        $class = 'panel-col-inside';
      }
      $output .= "<div class=\"panel-col panel-col-$col $class\">\n";
      $output .= "<div class=\"inside\">" . $content["row_${row}_$col"] . "</div>\n";
      $output .= "</div>\n"; // panel-col-$col
      $css .= "$idcss div.panel-row-$row div.panel-col-$col { width: " . ((intval($settings["row_$row"]["width_$col"])) - $reduce) . $settings["width_type"] ."; }\n";
    }
    $output .= "</div>\n"; // panel-row-$row
  }

  // Add our potential sidebars
  if (!empty($settings['sidebars']['left']) || !empty($settings['sidebars']['right'])) {
    // provide a wrapper if we have a sidebar
    $output = "<div class=\"panel-sidebar-middle panel-sidebar\">\n$output</div>\n";
    if ($settings['sidebars']['width_type'] == '%') {
      $css .= "$idcss div.panel-flexible-sidebars div.panel-sidebar-middle { width: " . (intval($settings['percent_width']) - intval($settings['sidebars']['left_width']) - intval($settings['sidebars']['right_width'])) . "; }\n";
    }
  }

  if (!empty($settings['sidebars']['left'])) {
    $size = intval($settings['sidebars']['left_width']) . $settings['sidebars']['width_type'];
    $output = "<div class=\"panel-sidebar panel-sidebar-left panel-col panel-col-first\"><div class=\"inside\">\n" . $content["sidebar_left"] . "</div>\n</div>\n" . $output;
    $css .= "$idcss div.panel-flexible-sidebars div.panel-sidebar-left { width: $size; margin-left: -$size; }\n";
    $css .= "$idcss div.panel-flexible-sidebars { padding-left: $size; }\n";
    // IE hack
    $css .= "* html $idcss div.panel-flexible-sidebars div.panel-sidebar-left { left: $size; }\n";
  }

  if (!empty($settings['sidebars']['right'])) {
    $size = intval($settings['sidebars']['right_width']) . $settings['sidebars']['width_type'];
    $output .= "<div class=\"panel-sidebar panel-sidebar-right panel-col panel-col-last\"><div class=\"inside\">\n" . $content["sidebar_right"] . "</div>\n</div>\n";
    $css .= "$idcss div.panel-flexible-sidebars div.panel-sidebar-right { width: $size; margin-right: -$size; }\n";
    $css .= "$idcss div.panel-flexible-sidebars { padding-right: $size; }\n";
  }

  // Wrap the whole thing up nice and snug
  if (!$id) {
  	$output = "<div class=\"panel-flexible-sidebars\">\n" . $output . "</div>\n";
  }
  $output = "<div class=\"panel-flexible clear-block\" $idstr>\n" . $output . "</div>\n";
  drupal_set_html_head("<style type=\"text/css\" media=\"all\">\n$css</style>\n");
  return $output;
}



/**
 * Override theme_buddylist_status_block
 *
 * Hide if no buddies
 */
function phptemplate_buddylist_status_block($count, $received_count, $sent_count) {
  if($count>0) {
    return theme_buddylist_status_block($count, $received_count, $sent_count);
  }
  return false;
}

function phptemplate_faceted_search_ui_facet_wrapper($env_id, $facet, $context, $content, $wrap=true) {
  $classes = array(
    'faceted-search-facet', // Note: Tooltips rely on this class.
    'faceted-search-env-'. faceted_search_variable_get($env_id, 'name', $env_id),
    'faceted-search-facet--'. check_plain($facet->get_key() .'--'. $facet->get_id()), // Note: Tooltips rely on this class.
    'faceted-search-facet-'. ($facet->is_active() && $context != 'related' ? 'active' : 'inactive'),
    'faceted-search-'. $context,
  );
  if ($wrap == true) {
    $content = theme('nabuur_boxed', '<div class="content">'. $content .'</div>', 'nabuur-green');
  }
  return '<div class="'. implode(' ', $classes) .'">'. $content .'</div>'."\n";
}

function phptemplate_faceted_search_ui_stage_facet($search, $index, $facet, $categories) {
  $content .= '<p>'. ($search->get_text() ? t('Click a term to refine your current search.') : t('Click a term to initiate a search.')) .'</p>';
  $content .= theme('faceted_search_ui_facet_heading', $search->get_env_id(), $search->ui_state, $search->get_filters(), $index, 'guided');
  $content .= theme('faceted_search_ui_categories', $facet, $categories, $search->ui_state['stage']);
  $content = theme('faceted_search_ui_facet_wrapper', $search->get_env_id(), $facet, 'guided', $content, false);
  return theme('faceted_search_ui_page', $search, $content);
}


function phptemplate_nodeprofile_display_full() {
  return;
}

function phptemplate_nodeprofile_display_add_link() {
  // @todo is it hack the module or test here if the link should be visible?
  return;
}

function nabuur01_block($block) {
  if (module_exists('blocktheme')) {
    if ( $custom_theme = blocktheme_get_theme($block) ) {
      return _phptemplate_callback($custom_theme, array('block' => $block));
    }
  }
  return phptemplate_block($block);
}

/**
 * views template to output a view.
 * This code was generated by the views theming wizard
 * Date: Tue, 05/27/2008 - 17:53
 * View: front_features
 *
 * This function goes in your template.php file
 */
function phptemplate_views_view_list_front_features($view, $nodes, $type) {
  $fields = _views_get_fields();

  $taken = array();

  // Set up the fields in nicely named chunks.
  foreach ($view->field as $id => $field) {
    $field_name = $field['field'];
    if (isset($taken[$field_name])) {
      $field_name = $field['queryname'];
    }
    $taken[$field_name] = true;
    $field_names[$id] = $field_name;
  }

  // Set up some variables that won't change.
  $base_vars = array(
    'view' => $view,
    'view_type' => $type,
  );

  foreach ($nodes as $i => $node) {
    $vars = $base_vars;
    $vars['node'] = $node;
    $vars['count'] = $i;
    $vars['stripe'] = $i % 2 ? 'even' : 'odd';
    $vars['position'] = '';
    foreach ($view->field as $id => $field) {
      $name = $field_names[$id];
      $vars[$name] = views_theme_field('views_handle_field', $field['queryname'], $fields, $field, $node, $view);
      if (isset($field['label'])) {
        $vars[$name . '_label'] = $field['label'];
      }
    }
    if ($i > 0) {
      $output .= '<div class="panel-separator"></div>';
    }
    $output .= _phptemplate_callback('views-list-front_features', $vars);
  }
  return $output;
}

function phptemplate_nodecomment_view($comment) {
  return '<div style="display:none">' . print_r($comment, true) .'</div>' .theme_nodecomment_view($comment);
}

/**
 * Theme the frontpage focus blocks
 */
 
function phptemplate_front_focus($link, $img, $title, $content) {
  $result = l("<img src=\"$img\"/>",$link, null, null, null, null, true). "\n";
  $result .= '<h2>'. l(t($title), $link) ."</h2>\n";
  $result .= '<p>'. t($content) ."</p>\n";
  return $result;
}

/**
 * Override default geomap theming
 * Surround with <div> for proper boxing
 * Use modified jquery
 *
 * The Javascript will use this div to render tha map. 
 * This will be empty if no geo microformats exist on the current page.
 * 
 * @param $delta If more than one map needs to be displayed add an identifier
 * @ingroup themeable
 * @return Map div
 */
function phptemplate_geomap($delta='') {
  if ($google_key = variable_get('googlemaps_key', '')) {
    drupal_set_html_head('<script type="text/javascript" src="http://maps.google.com/maps?file=api&t=h&z=7&amp;v=2&amp;key='. $google_key .'" ></script>');
    drupal_add_js(path_to_theme() .'/jquery.nabuur.googlemaps.js', 'theme');
    drupal_add_css(drupal_get_path('module', 'geomap') .'/geomap.css', 'module');
    //TODO: Allow a different ID one day (use delta)
    $mapid = 'map';
    $output .= '<div class="map-box"><div id="'. $mapid .'" class="googlemap googlemap-'. $delta .'"></div></div>';
    $GLOBALS['geomap'] = true;     
  } 
  else {
    if (user_access('administer site configuration')) $output = l(t('No geomap googleapi key'), 'admin/settings/geomap'); 
  }  
  return $output;
}

/**
 * theme function that renders the HTML for the tags
 * @ingroup themable
 */
function phptemplate_tagadelic_weighted($terms) {
  foreach ($terms as $term) {
    $output .= l(t($term->name), taxonomy_term_path($term), array('class'=>"tagadelic level$term->weight", 'rel'=>'tag')) ." \n";
  }
  return $output;
}

/**
 * function that makes customisation of forms possible
 * @ingroup themable
 */
function nabuur01_task_discussion_node_form($form) {
// firep($form, 'nabuur01_task_discusion_form_before');
  $form1 = $form;
  // add the automatic title
  $arg_2 = arg(2);
  // Only change title for new posts and replies
  if($arg_2 == 'task_discussion') {
    $arg_3 = arg(3);
    $arg_4 = arg(4);
    if(is_numeric($arg_4) && $arg_4 > 0) {
      $node = node_load($arg_4);
    } else {
      if (is_numeric($arg_3)) {
        $node = node_load($arg_3);
      }
    }
    $title = $node->title;
    $pos = (strpos(strtolower($title), 're:')); 
    if($pos === FALSE) { 
      $title = 'Re: '. $title;
    }
    $form1['title']['#value'] = $title;
  }
  
  // change some form attributes
  // @todo join this together with altering the form at og_ level
  $form1['body_filter']['body']['#rows'] = 10;
  $form1['taxonomy']['#collapsible'] = FALSE;
  $form1['og_nodeapi']['visible']['og_public']['#default_value'] = 1;
  if (! user_access('administer organic groups')) {
    $form1['field_external_link']['#attributes'] = array('style' => 'display: none');
    $form1['og_nodeapi']['#attributes'] = array('style' => 'display: none');
  }
  // $form1['body_filter']['format']['#attributes'] = array('style' => 'display: none');
  $form1['body_filter']['format'] = Array ( 'guidelines' => Array ( 
                                            '#title' => 'Formatting guidelines',
                                            '#value' => '<ul class="tips"><li>Web page addresses and e-mail addresses turn into links automatically.</li><li>Allowed HTML tags: &lt;a&gt; &lt;em&gt; &lt;strong&gt; &lt;cite&gt; &lt;code&gt; &lt;ul&gt; &lt;ol&gt; &lt;li&gt; &lt;dl&gt; &lt;dt&gt; &lt;dd&gt; &lt;img&gt;</li><li>Lines and paragraphs break automatically.</li><li>You can use <strong>BBCode</strong> tags in the text.</li></ul>'));
  // add extra preview & submit buttons
  $form1['preview_extra'] = $form1['preview'];
  $form1['submit_extra'] = $form1['submit'];
  // add some more help texts
  $form1['taxonomy']['tags']['10']['#description'] = 'Type slowly and you might find that there are topics people have already created to use.<br> 
  Give your message a tag (keyword) to have it show up when users search for those terms.<br>
  More than 1 tag (keyword) is possible by separating them with a , (comma character).';
  
  // output the items title, body and buttons
  $output = '';
  $output .= drupal_render($form1['title']);
  $output .= drupal_render($form1['body_filter']);
  $output .= drupal_render($form1['taxonomy']);
  $output .= drupal_render($form1['preview_extra']);
  $output .= drupal_render($form1['submit_extra']);
  // unset title and body for next render of form  
  unset($form1['title']);
  unset($form1['body_filter']);
  unset($form1['taxonomy']);
  
  // Add some nice help text
  $output .= '<div class="form-help"><h2>'. t('If you want to add pictures, documents and video, click on one of the links below') .'</h2></div>';
  // Add the advanced functions
  $output .= drupal_render($form1);
  
  
  return $output;   
}
function phptemplate_faceted_search_ui_page($search, $content) {
  $classes = array(
    'faceted-search-page',
    'faceted-search-stage-'. $search->ui_state['stage'],
    'faceted-search-env-'. faceted_search_variable_get($search->get_env_id(), 'name', 'faceted_search'),
  );
  if ( ($search->_env_id == 4 || $search->_env_id == 5) && $search->ui_state['stage'] == 'select') {
    // villages or groups
    return '<div class="'. implode(' ', $classes) .'">'. $content .'</div>'."\n";
  }
  else {
    return '<div class="'. implode(' ', $classes) .'">'. $content .'</div>'."\n";
  }
}

// function phptemplate_faceted_search_ui_stage_facet($search, $index, $facet, $categories) {

function phptemplate_faceted_search_ui_stage_select($search, $keyword_block_content, $guided_block_content, $description_content = '') {
/*
  if ($description_content != '') {
    $form['description'] = array(
      '#type' => 'item',
      '#value' => check_markup($description_content, FILTER_FORMAT_DEFAULT, FALSE),
      '#weight' => -5,
      '#attributes' => array('class' => 'faceted-search-description'),
    );
  }
*/
  if ( ($search->_env_id == 4 || $search->_env_id == 5) && $search->ui_state['stage'] == 'select') {
    if ($search->_env_id == 5) {
      // village
//    $view = views_get_view('recent_active_tasks_'. $gtype);
//      $view = views_get_view('village_news_latest_all');
//      print ' <pre>'. var_export($view,true) .'</pre>';
//      $middle_one = views_build_view('block', $view, NULL, 0, 5);
      $description = check_markup($description_content, FILTER_FORMAT_DEFAULT, FALSE);
      $middle_one = nabuur_og_list_active_villages();
//      $view = views_get_view('og_new_villages');
//      $middle_two = views_build_view('block', $view, NULL, 0, 5);
      $middle_two = nabuur_og_new_active_villages();
      $assist_img_link =l('<img height = "120px" align = "center" src="/files/static/connect-village.jpg"/>', '/assistance-your-community', null, null, null, null, true);
      $assist_link = l(t('Click here to register on NABUUR'), '/assistance-your-community');
      $form['middle']['#value'] = '<div class="search-description">'. $description .'</div>
      <div class="village-assist"><div class="nabuur-box nabuur-beige"><div class="nabuur-content"><div class="content"><div class="picture">'. $assist_img_link .'</div><div style="text-align: center">'. $assist_link .'</div></div></div></div></div>
      <div class="search-middle-first nabuur-box nabuur-border"><h3 class="title">News from the villages</h3><div class="content">'. $middle_one .'</div>
      </div><div class=spacer>&nbsp;</div>
      <div class="search-middle-second nabuur-box nabuur-border"><h3>New Villages on NABUUR.com</h3>'. $middle_two .'</div>
      <div class=spacer>&nbsp;</div>
      <div class="search-middle"><div class="search-top"><div class="search-middle-map" ><img src="/files/furniture/worldmap-s.png" width="350" height="181" border="0" usemap="#map" /><map name="map"><area title="Africa" shape="poly" coords="138,70,165,51,199,60,208,85,219,85,217,103,211,110,223,116,217,139,203,136,190,151,179,151,169,125,171,111,164,100,147,100,138,82,139,76" href="/village/results/taxonomy%3A8" />
      <area title="South America" shape="poly" coords="71,102,84,82,110,94,110,101,126,106,123,120,119,123,107,151,100,163,97,173,86,169,82,127,70,112" href="/village/results/taxonomy%3A10" />
      <area title="North America" shape="poly" coords="66,98,76,93,91,78,118,36,56,7,15,24,14,36,32,31,35,38,30,53,33,68,34,68" href="/village/results/taxonomy%3A11" />
      <area title="Asia" shape="poly" coords="223,86,231,75,251,99,257,91,255,80,262,80,265,92,294,89,286,79,297,61,288,53,263,38,211,40,196,36,198,59,209,74" href="/village/results/taxonomy%3A9" />
      </map></div>
      <div class="search-middle-where nabuur-box nabuur-border"><h3>Where do you want to go?</h3><h4><a href="/village/results/taxonomy:8">Africa</a></h4>
      <p>
      <a href="/village/results/taxonomy:8.12">Burundi</a> | <a href="/village/results/taxonomy:8.13">Cameroon</a> | <a href="/village/results/taxonomy:8.214">DR of the Congo</a> | <a href="/village/results/taxonomy:8.15">Gambia</a> | <a href="/village/results/taxonomy:8.16">Ghana</a> | <a href="/village/results/taxonomy:8.17">Kenya</a> | <a href="/village/results/taxonomy:8.283">Liberia</a> | <a href="/village/results/taxonomy:8.290">Madagascar</a> | <a href="/village/results/taxonomy:8.18">Mali</a> | <a href="/village/results/taxonomy:8.19">Morocco</a> | <a href="/village/results/taxonomy:8.20">Nigeria</a> | <a href="/village/results/taxonomy:8.21">Rwanda</a> | <a href="/village/results/taxonomy:8.22">Senegal</a> | <a href="/village/results/taxonomy:8.23">Sierra Leone</a> | <a href="/village/results/taxonomy:8.24">South Africa</a> | <a href="/village/results/taxonomy:8.25">Tanzania</a> | <a href="/village/results/taxonomy:8.26">Uganda</a> | <a href="/village/results/taxonomy:8.27">Zambia</a> </p>
      <h4><a href="/village/results/taxonomy:9">Asia</a></h4>
      <p>
      <a href="/village/results/taxonomy:9.30">Bangladesh</a> | <a href="/village/results/taxonomy:9.31">Cambodia</a> | <a href="/village/results/taxonomy:9.208">China</a> | <a href="/village/results/taxonomy:9.32">India</a> | <a href="/village/results/taxonomy:9.33">Nepal</a> | <a href="/village/results/taxonomy:9.34">Sri Lanka</a> | <a href="/village/results/taxonomy:9.1215">Tibet</a> 
      </p>
      <h4><a href="/village/results/taxonomy:10">Latin America</a></h4>
      <p>
      <a href="/village/results/taxonomy:10.226">Ecuador</a> | <a href="/village/results/taxonomy:10.29">Peru</a> | <a href="/village/results/taxonomy:11.35">Mexico</a>
      </p></div></div></div>
      <div class=spacer>&nbsp;</div>
      </div>';
    }
    else {
      // id = 4 group
      $view = views_get_view('recent_needshelp_tasks_village');
      $middle_two = views_build_view('block', $view, NULL, 0, 5);
    }
  }
  if ($description_content != '') {
    $form['description'] = array(
      '#type' => 'item',
//      '#value' => check_markup($description_content, FILTER_FORMAT_DEFAULT, FALSE),
      '#weight' => -5,
      '#attributes' => array('class' => 'faceted-search-description'),
    );
  }
  if ($keyword_block_content) {
    $form['keyword'] = array(
      '#type' => 'fieldset',
      '#title' => t('Keyword search'),
      '#value' => $keyword_block_content,
      '#weight' => 0,
      '#attributes' => array('class' => 'faceted-search-keyword'),
    );
  }
  if ($guided_block_content) {
    $form['guided'] = array(
      '#type' => 'fieldset',
      '#title' => t('Guided search'),
      '#value' => $guided_block_content,
      '#weight' => 1,
      '#attributes' => array('class' => 'faceted-search-guided'),
    );
  }
  return drupal_render($form);
}

