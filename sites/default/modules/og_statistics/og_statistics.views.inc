<?php
// $Id$

/**
 * @file og_statistics.views.inc
 * Provides support for Views 1 module.
 */

/**
 * Implementation of hook_views_tables().
 */
function og_statistics_views_tables() {
  $tables['og_statistics'] = array(
    // join table
    'name' => 'og_statistics',
    'join' => array(
      'left' => array (
        'table' => 'node',
        'field' => 'nid',
      ),
      'right' => array(
        'field' => 'nid',
      ),
    ),
    // fields
    'fields' => array(
      'user_count' => array(
        'name' => t('Group Statistic: Members Count'),
       // 'handler' => 'views_handler_field_numeric',
        'sortable' => true,
        'help' => t('Display the number of members of group.'),
      ),
      'node_count' => array(
        'name' => t('Group Statistic: Node Count'),
       // 'handler' => 'views_handler_field_numeric',
        'sortable' => true,
        'help' => t('Display the number of nodes posted to group.'),
      ),
      'comment_count' => array(
        'name' => t('Group Statistic: Comment Conut'),
       // 'handler' => 'views_handler_field_numeric',
        'sortable' => true,
        'help' => t('Display the number of comments posted to nodes in group.'),
      ),
      'last_node_timestamp' => array(
        'name' => t('Group Statistic: Last Node Date'),
        'handler' => views_handler_field_dates(),
        'option' => 'string',
        'sortable' => true,
        'help' => t('Display the post time of the last node posted to group.'),
      ),
      'last_comment_timestamp' => array(
        'name' => t('Group Statistic: Last Comment Date'),
        'handler' => views_handler_field_dates(),
        'option' => 'string',
        'help' => t('Display the post time of the last comment posted to group.'),
      ),
      'last_member_timestamp' => array(
        'name' => t('Group Statistic: Last Member Date'),
        'handler' => views_handler_field_dates(),
        'option' =>' string',
        'help' => t('Display the time of the last member joining the group.'),
      ),
    ),
    'filters' => array(
      'user_count' => array(
        'name' => t('Group Statistic: Member Count'),
        'operator' => 'views_handler_operator_gtlt',
        'option' => 'integer',
        'help' => t('Filter by the number of members in a group.'),
      ),
      'node_count' => array(
        'name' => t('Group Statistic: Node Count'),
        'operator' => 'views_handler_operator_gtlt',
        'option' => 'integer',
        'help' => t('Filter by the number of nodes posted to a group.'),
      ),
      'comment_count' => array(
        'name' => t('Group Statistic: Comment Count'),
        'operator' => 'views_handler_operator_gtlt',
        'option' => 'integer',
        'help' => t('Filter by the number of comments posted to nodes in a group.'),
      ),
      'last_node_timestamp' => array(
        'name' => t('Group Statistic: Last Node Time'),
        'operator' => 'views_handler_operator_gtlt',
        'value' => views_handler_filter_date_value_form(),
        'handler' => 'views_handler_filter_timestamp',
        'option' => 'string',
        'help' => t('Filtered by the timestamp of the last node edit in the group.')
          .' '. views_t_strings('filter date'),
      ),
      'last_comment_timestamp' => array(
        'name' => t('Group Statistic: Last Comment Time'),
        'operator' => 'views_handler_operator_gtlt',
        'value' => views_handler_filter_date_value_form(),
        'handler' => 'views_handler_filter_timestamp',
        'option' => 'string',
        'help' => t('Filtered by the timestamp of the last comment in the group.')
          .' '. views_t_strings('filter date'),
      ),
      'last_member_timestamp' => array(
        'name' => t('Group Statistic: Last Member Time'),
        'operator' => 'views_handler_operator_gtlt',
        'value' => views_handler_filter_date_value_form(),
        'handler' => 'views_handler_filter_timestamp',
        'option' => 'string',
        'help' => t('Filtered by the timestamp of the last member to join the group.')
          .' '. views_t_strings('filter date'),
      ),
    ),
    //sorts
    'sorts' => array(
      'user_count' => array(
        'name' => t('Group Statistic: Member Count'),
        'help' => t('Sort by the number of members in a group.'),
      ),
      'node_count' => array(
        'name' => t('Group Statistic: Node Count'),
        'help' => t('Sort by the number of nodes posted to a group.'),
      ),
      'comment_count' => array(
        'name' => t('Group Statistic: Comment Count'),
        'help' => t('Sort by the numeber of comments to nodes in a group.'),
      ),
      'last_node_timestamp' => array(
        'name' => t('Group Statistic: Last Node Date'),
        'handler' => 'views_handler_sort_date',
        'option' => views_handler_sort_date_options(),
        'help' => t('Sort by the date of the most recent node posted to group.'),
      ),
      'last_comment_timestamp' => array(
        'name' => t('Group Statistic: Last Comment Date'),
        'handler' => 'views_handler_sort_date',
        'option' => views_handler_sort_date_options(),
        'help' => t('Sort by the date of the most recent comment posted to a group.'),
      ),
      'last_member_timestamp' => array(
        'name' => t('Group Statistic: Last Member Date'),
        'handler' => 'views_handler_sort_date',
        'option' => views_handler_sort_date_options(),
        'help' => t('Sort by the date of the most recent member joining the group.'),
      ),
    ),
  );
  return $tables;
}
