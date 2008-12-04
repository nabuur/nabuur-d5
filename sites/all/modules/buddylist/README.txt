Readme
------
This module allows users to put each other on a personal 'Buddy List'.

Features include buddy groups and the ability to track your buddies' recent posts.

Send comments to Robert Douglass at: http://drupal.org/user/5449/contact


Requirements
------------
This module requires Drupal 5.x.

This module does not yet offer PostgreSQL support. If you would like to contribute to this module by creating the appropriate PostgreSQL schema, please submit your code at http://drupal.org/project/issues/buddylist



Installation
------------
1. Copy the buddylist.module to the Drupal modules/ directory.

2. Enable buddy list in the "site settings | modules" administration screen.

   Enabling the buddylist module will trigger the creation of the database schema. If you are shown error messages you may have to create the schema by hand. Please see the database definition at the end of this file.

3. Enable buddy list blocks you want in the "blocks" administration screen.
          
4. Optionally add the following theme function to your PHPTemplate's template.php file:

function phptemplate_username($object) {
  global $user;
  /* Use the default theme_username for anonymous users, nodes by this user */
  if ($user->uid == 0 || $object->uid == $user->uid || $object->uid == 0) {
    return theme_username($object);
  }
  if (!user_access('maintain buddy list')) {
  	return theme_username($object);
  }
  
  /* an array, keyed on buddy uids */
  $buddies = buddylist_get_buddies($user->uid);
  /* Find out if this buddy is in the user's buddy list */
  foreach ($buddies as $buddyuid => $buddystructure) {
    if ($buddyuid == $object->uid) {
      $output .= theme_username($object);
      $output .= ' (';
      $output .= theme('remove_from_buddylist_link', $object);
      $output .= ')';
      return $output;
    }
  }
  /* The user is not in the buddylist, give a link to add */
  $output .= theme_username($object);
  $output .= ' (';
  $output .= theme('add_to_buddylist_link', $object);
  $output .= ')';
  return $output;
}


Database Schema
---------------
If the automatic creation of the database tables was unsuccessful you can try creating the tables by hand using the following SQL:

CREATE TABLE `buddylist` (
  `uid` int(10) unsigned NOT NULL default '0',
  `buddy` int(10) unsigned NOT NULL default '0',
  `timestamp` int(11) NOT NULL default '0',
  `received` tinyint(1) NOT NULL default '0',
  UNIQUE KEY `uid-buddy-label` (`uid`,`buddy`),
  KEY `uid` (`uid`)
) TYPE=MyISAM /*!40100 DEFAULT CHARACTER SET utf8 */;

-- 
-- Table structure for table `buddylist_buddy_group`
-- 

CREATE TABLE `buddylist_buddy_group` (
  `uid` int(11) NOT NULL default '0',
  `buddy` int(11) NOT NULL default '0',
  `label_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`uid`,`buddy`,`label_id`)
) TYPE=MyISAM /*!40100 DEFAULT CHARACTER SET utf8 */;

-- 
-- Table structure for table `buddylist_groups`
-- 

CREATE TABLE `buddylist_groups` (
  `uid` int(11) NOT NULL default '0',
  `label_id` int(11) NOT NULL default '0',
  `label` varchar(255) NOT NULL default '',
  `visible` tinyint(1) NOT NULL default '0',
  PRIMARY KEY  (`uid`,`label_id`)
) TYPE=MyISAM /*!40100 DEFAULT CHARACTER SET utf8 */;


Credits
-------
Written by Adrian Rossouw.
Thanks to Ratko Kovacina for the comments/debugging info
Browsing improvements by Doug Sikora

Maintainer: Robert Douglass
Status: maintained (Feb. 2006)

Request/Approval Feature by Ankur Rishi, courtesy of Jewcy.com

TODO
----
1. PGSQL schema (see buddylist.install, buddylist_install())
2. Rework texts so that one is not stuck with "buddy" but could choose "contact", for example
3. Make a workflow whereby a buddy request is sent to the buddy for confirmation which is required before the buddy can be added.
4. Make the notification that someone added you to their buddylist use the privatemsg module, if available.
5. Consider possible Views module integration.