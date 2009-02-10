<?php
// $Id: nodequeue.workflow_ng.inc,v 1.1.2.2 2008/07/06 16:25:21 ezrag Exp $
/**
 * @file nodequeue_workflow_ng.inc
 * 
 * This include file is enabled when Workflow-ng module is enabled. 
 * It adds some actions that allow nodequeue manipualtion and interaction. 
 */


/*
 * Implementation of hook_action_info().
 */
function nodequeue_action_info() {
   $info = array();
   $info['nodequeue_workflow_ng_action_load_nodequeue'] = array(
      '#label' => t('Load nodequeue'),
      '#new arguments' => array(
        'nodequeue' => array('#entity' => 'nodequeue', '#label' => t('Nodequeue')),
      ),
      '#description' => t('Load a nodequeue as a new argument. This action must be used before any other action on
        a nodequeue can be performed.'),
      '#module' => 'Nodequeue',
   );
   $info['nodequeue_workflow_ng_action_add_node_to_nodequeue'] = array(
      '#label' => t('Add content to nodequeue'),
      '#arguments' => array(
        'node' => array('#entity' => 'node', '#label' => t('Content which will be added to the nodequeue')),
        'nodequeue' => array('#entity' => 'nodequeue', '#label' => t('Nodequeue that content will be added to')),
      ),
      '#description' => t('Add content to a loaded nodequeue.'),
      '#module' => 'Nodequeue',
   );
  $info['nodequeue_workflow_ng_action_remove_node_from_nodequeue'] = array(
      '#label' => t('Remove content from nodequeue'),
      '#arguments' => array(
        'node' => array('#entity' => 'node', '#label' => t('Content which will be removed from the nodequeue')),
        'nodequeue' => array('#entity' => 'nodequeue', '#label' => t('Nodequeue that content will be removed from')),
      ),
      '#description' => t('Remove content from a loaded nodequeue. If a content is in the queue more than once, 
        only the first (closest to 0 position, or the front of the queue) will be removed.'),      
      '#module' => 'Nodequeue',    
  );
  $info['nodequeue_workflow_ng_action_delete_nodequeue'] = array(
      '#label' => t('Delete a nodequeue'),
      '#arguments' => array(
        'nodequeue' => array('#entity' => 'nodequeue', '#label' => t('The nodequeue which will be deleted')),
      ),
      '#description' => t('Delete a nodequeue.'),      
      '#module' => 'Nodequeue',    
  );  
   $info['nodequeue_workflow_ng_action_add_nodequeue'] = array(
      '#label' => t('Add nodequeue'),
      '#new arguments' => array(
        'nodequeue' => array('#entity' => 'nodequeue', '#label' => t('Nodequeue')),
      ),
      '#description' => t('Create a new nodequeue.'),        
      '#module' => 'Nodequeue',
   );  
  return $info;
}


/*
 * Action: Load nodequeue.
 */
function nodequeue_workflow_ng_action_load_nodequeue($settings, &$arguments, &$log) {
  if ($settings['nodequeue_token']) {
    extract( workflow_ng_token_replace_all(array('nodequeue_token'), $settings, $arguments, $log));
    $qid[] = $nodequeue_token;
  }
  else {
    $qid[] = $settings['nodequeue_list'];  
  }
  $queues = nodequeue_load_queues($qid);
  //We load only a single nodequeue.
  $queue = array_shift($queues);
  if ($queue) {
    return array('#new arguments' => array('nodequeue' => $queue));  
  }  
}


function _nodequeue_workflow_ng_action_get_nodequeue() {
  $options = array();
  $queues = nodequeue_load_queues(nodequeue_get_all_qids(500));
  foreach ($queues as $qid => $queue) {
    $options[$qid] = $queue->title;
  }
  return $options;
}


/*
 * Load nodequeue form.
 *
 * @ingroup forms
 * @see nodequeue_workflow_ng_action_load_nodequeue_submit 
 */

function nodequeue_workflow_ng_action_load_nodequeue_form($settings = array(), $argument_info) {
  //Get existing nodequeues.
  $options = _nodequeue_workflow_ng_action_get_nodequeue();
  if ($options) {
    $description = t('Specify the queue into which the node should be submitted. If the queue is a smartqueue, the node shall be placed into every subqueue for which it is eligible.') ;
  } else {
    $description = t('There are no existing queues. Create a nodequeue manually through the !url or using a workflow_ng action.', 
      array('!url' => l('settings', 'admin/content/nodequeue')));
  }
  $form = array();
  $form['nodequeue_list'] = array(
    '#type' => 'select',
    '#title' => t('Select nodequeue'),
    '#description' => $description,
    '#options' => $options,  
    '#default_value' => $settings['nodequeue_list'],
    '#disabled' => empty($options),
  );
  $form['nodequeue_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Nodequeue id'),
    '#description' => t('Enter the queue id (qid) of the nodequeue into which the node should be submitted, or leave blank if you have selected one from the list above.'),
    '#default_value' => $settings['nodequeue_token'],
  ); 
  workflow_ng_token_replacement_help($form, $argument_info);   
  return $form;
}

function nodequeue_workflow_ng_action_load_nodequeue_submit($form_id, $form_values) {
  $token = workflow_ng_token_get_settings(array('nodequeue_token'), $form_values);
  $settings =  array('nodequeue_list' => $form_values['nodequeue_list'], 'nodequeue_token' => $form_values['nodequeue_token']);
  return $token + $settings;
}

/*
 * Action: Add node to nodequeue.
 */
function nodequeue_workflow_ng_action_add_node_to_nodequeue($node, $nodequeue, $settings, &$arguments, &$log) {
  // Get the eligible subqueues.
  $eligible_subqueues = nodequeue_get_subqueues_by_node(array($nodequeue), $node);
    
  //Remove the node from the eligible queues.
  foreach ($eligible_subqueues as $eligible_subqueue) {           
    nodequeue_subqueue_remove_node($eligible_subqueue->sqid, $node->nid);
  }

  // Add node to each subqueue.
  foreach ($eligible_subqueues as $eligible_subqueue) {
    nodequeue_subqueue_add($nodequeue, $eligible_subqueue, $node->nid);
  }
}

/*
 * Action: Remove node from nodequeue.
 */
function nodequeue_workflow_ng_action_remove_node_from_nodequeue($node, $nodequeue, $settings, &$arguments, &$log) {
  // Get the eligible subqueues.
  $eligible_subqueues = nodequeue_get_subqueues_by_node(array($nodequeue), $node);
    
  //Remove the node from the eligible queues.
  foreach ($eligible_subqueues as $eligible_subqueue) {           
    nodequeue_subqueue_remove_node($eligible_subqueue->sqid, $node->nid);
  }
}

/*
 * Action: Delete a nodequeue.
 */
function nodequeue_workflow_ng_action_delete_nodequeue($nodequeue, $settings, &$arguments, &$log) {
  nodequeue_delete($nodequeue->qid);

  // Unload arguments.
  $arguments['data']['nodequeue'] = NULL;
  $arguments['info']['nodequeue'] = NULL;
}

/*
 * Action: Add a nodequeue.
 */
function nodequeue_workflow_ng_action_add_nodequeue($settings, &$arguments, &$log) {
  extract( workflow_ng_token_replace_all(array('title', 'link', 'link_remove'), $settings, $arguments, $log));
  $queue = (object) array();
  //Set the nodequeue object before creating it.
  $queue->roles = array_keys(array_filter($settings['roles']));
  $queue->types = array_keys(array_filter($settings['types']));
  $queue->title = $title;
  $queue->subqueue_title = $title;
  $queue->size = $settings['size'];
  $queue->reverse = $settings['reverse'];
  $queue->add_subqueue = array(TRUE);
  //For consistency with workflow_ng becoming rules module in D6, 
  //set the nodequeue owner accordingly.
  $queue->owner = 'nodequeue_rules';
  
  $queue->link = $link;
  $queue->link_remove = $link_remove;
  $queue->show_in_links = !empty($queue->link) && !empty($queue->link_remove);
  $queue->show_in_tab = $settings['show_in_tab'];
  $queue->show_in_ui = $settings['show_in_ui'];
    
  nodequeue_save($queue);

}

function nodequeue_workflow_ng_action_add_nodequeue_form($settings = array(), $argument_info) {
  $queue = (object)$settings;
  $form = nodequeue_edit_queue_form($queue);
  //Remove redundent fields.
  unset($form['description']);
  unset($form['submit']);
  
  //Add 'show in tab' option, taken from nodequeue admin settings.
  $nodequeue_tab = nodequeue_admin_settings();
  $form['tab_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tab settings'),
    );
  $form['tab_settings']['show_in_tab'] = $nodequeue_tab['nodequeue_use_tab'];
  $form['tab_settings']['show_in_ui'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show in UI'),
    '#description' => t('Determine if the nodequeue should be visible via the nodequeue !url page.', array('!url' => l('settings', 'admin/content/nodequeue'))),
    '#default_value' => $settings['show_in_ui'],
  );  
  workflow_ng_token_replacement_help($form, $argument_info);
  return $form;
}

function nodequeue_workflow_ng_action_add_nodequeue_submit($form_id, $form_values) {
  $form_list = array(
    'title',
    'size',
    'reverse',
    'link',
    'link_remove',
    'roles',
    'types',
    'show_in_tab',
    'show_in_ui',
  );
  $token = workflow_ng_token_get_settings(array('title', 'link', 'link_remove'), $form_values);
  $settings = array();
  foreach ($form_list as $form_item) {
    $settings[$form_item] = $form_values[$form_item];
  }
  return $token + $settings;
}
