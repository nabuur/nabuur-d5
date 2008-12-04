<?php

function similarnodes_views_default_views() {
  $view = new stdClass();
  $view->name = 'similartest';
  $view->description = 'Test';
  $view->access = array (
);
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'Similar Nodes Test';
  $view->page_header = '';
  $view->page_header_format = '1';
  $view->page_footer = '';
  $view->page_footer_format = '1';
  $view->page_empty = 'You must provide a node id as an argument for this view to do anything';
  $view->page_empty_format = '1';
  $view->page_type = 'table';
  $view->url = 'similarnodes_test';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '10';
  $view->block = TRUE;
  $view->block_title = 'Similar Nodes';
  $view->block_header = '';
  $view->block_header_format = '1';
  $view->block_footer = '';
  $view->block_footer_format = '1';
  $view->block_empty = 'No Nodes :( How sad.';
  $view->block_empty_format = '1';
  $view->block_type = 'list';
  $view->nodes_per_block = '5';
  $view->block_more = FALSE;
  $view->block_use_page_header = FALSE;
  $view->block_use_page_footer = FALSE;
  $view->block_use_page_empty = FALSE;
  $view->sort = array (
    array (
      'tablename' => 'similarnodes',
      'field' => 'similar_weight',
      'sortorder' => 'DESC',
      'options' => '',
    ),
  );
  $view->argument = array (
    array (
      'type' => 'similarnode',
      'argdefault' => '7',
      'title' => '',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
  );
  $view->field = array (
    array (
      'tablename' => 'node',
      'field' => 'title',
      'label' => '',
      'handler' => 'views_handler_field_nodelink',
      'options' => 'link',
    ),
    array (
      'tablename' => 'similarnodes',
      'field' => 'similar_weight',
      'label' => '',
    ),
  );
  $view->filter = array (
  );
  $view->exposed_filter = array (
  );
  $view->requires = array(similarnodes, node);
  $views[$view->name] = $view;

  
  return $views;
}
?>