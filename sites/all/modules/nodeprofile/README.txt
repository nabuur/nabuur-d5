$Id: README.txt,v 1.22.2.4 2007/05/06 12:44:10 fago Exp $

Nodeprofile Module
------------------------
by Wolfgang Ziegler, nuppla@zites.net


With this module you can build user profiles with nodes. To achieve this
a couple of other modules are really useful:

Short module overview:
---------------------
nodeprofile.module - Marks content types as profiles

nodefamily.module - Builds node families based on content types and author information, needed by nodeprofile
pageroute.module - Allows the creation of pageroutes, which can be used for building user friendly wizards
usernode.module - Automatic creation of usernodes, which might be useful in particular in conjunction with views
views_fusion.module - Provides fusing of multiple views, which can be used to list users with their profile data



nodeprofile.module
-----------------
This module allows one to mark content types as user profiles, do this
at 'admin/content/types'.

It uses the nodefamily module to restrict the node population to 1, which means
that a user can only create on node of this type - the so called lonely node.


nodefamily.module
-----------------
http://drupal.org/project/nodefamily
This module is required by the nodeprofile module.

Furthermore one may use it to extend your nodeprofile by further content types, but this
is optional. If you want to do that, read the nodefamily README carefully!

Basically this module let one define relations between content types. It
automatically creates a relation between all nodes which have been created
by the same user and have the defined content types.


Pageroute
----------
http://drupal.org/project/pageroute

The pageroute modules can be used to provide an user friendly wizard for creating and
editing several nodes. 
You can use the module to create a "route" which leads users through multiple
pages, e.g. through the process of filling out a nodeprofile.

It also allows you to build a node based user profile which consists of multiple
content types. Then users can easily create and edit their nodes through the
same pageroute.

But it may be also useful if you have a single content-type nodeprofile. E.g. it 
allows you to create two pages, where the first one is used for editing/adding the
profile-node and the second one for viewing the resulting node.

For operationg with nodeprofiles use the lonely node management and the lonely node view
page types. This page types are provided by nodefamily's pageroute integration,
so read more about them in the nodefamily README.


usernode.module
---------------
http://drupal.org/project/usernode

This module tries to make users nodes. It cares for automatic creation and
deletion of a node for each user, the so called usernode.

You don't need to use the usernode module if you want to build nodeprofiles,
however it can be useful if one uses nodeprofiles.

It allows you to
    * Use views to build user listings or even searches. Usernode provides an
      easy customizeable default view.
    * Use it instead of the user account page located at /user. The advantage is
      that you can theme it like any other node and
    * use features, which other modules provide for nodes, with the usernode.
      Think of comments, voting modules, taxonomy...

If the usernode module is installed, the nodeprofile module sets a nodefamily relation
between the content type of usernodes and all nodeprofile content types.


How do usernodes interact with nodeprofiles?
----------------------------------------------
The default display of the usernode shows the nodeprofiles as they are show on the
user page.

However by using the nodefamily relations you are able to directly load all nodeprofiles
and so to create a more customized user representation page. Have a look at the nodefamily
README for further instructions on how to load the nodes.



Links to other useful modules for building nodeprofiles
-------------------------------------------------------
http://drupal.org/project/cck
http://drupal.org/project/views
http://drupal.org/project/views_fusion


Installation
------------
 * Download and install the nodefamily module. (http://drupal.org/project/nodefamily)
 * Copy the nodeprofile module's directory to your modules directory and activate it.
 * Optionally download and install the usernode module too.
 * Optionally activate further modules you want to use for building nodeprofiles
   (cck, views, views_fusion, pageroute)



How to build a user profile with this stuff?
--------------------------------------------

First I describe how a more simple user profile, which consists only of one content type,
can be built:

 * Create your content type you want to use for your profile, e.g. with the CCK.
 * Mark your content type as user profile at 'admin/content/types'
 * If you don't like the usual node forms for editing the profile remove the usual
   'create content' link using the menu module. Use a new link pointing at
   'nodefamily/CONTENT_TYPE' or an url alias of it.
   As an alternative you can build a more flexible pageroute for this. Have a look at the
   pageroute modules.

 * For building user listings and user searches use the views module together with the
   usernode module. If the view should also contain profile information also use the views
   fusion module.


To build a more advanced user profile which consists of several node types:

 * Create the content types that you want to use for your profiles, e.g. with the CCK.

 * Mark a content type as user profile at 'admin/content/types' and set
   appropriate relations between the content types at 'admin/content/nodefamily'.

   To achieve greatest flexibility it is suggested to mark one content-type as nodeprofile.
   Then set nodefamily relations between this type and the other content
   types, which extend the profile. Use the node view of the parent type to build the display
   of your user profile using the nodefamily relations (instructions are provided in the
   nodefamily README).

 * Theme your usernodes

 * Use the pageroute module to provide an user-friendly wizard for filling out the nodeprofile.
   Note: Use the lonely node management page type for adding/editing a nodeprofile content type.

 * For building user listings and user searches use the views module together with the
   usernode module. If the view should also contain profile information also use the views
   fusion module.


Theming with node profiles
---------------------------

The nodeprofile module provides some flexible functions, that can be used for a more customized profile
display.

First of all you can easily print the display of all nodeprofiles like they are displayed on the
user's page. Just use

     $output = nodeprofile_show_profiles($uid);
     echo drupal_render($output);

where $uid has to be the user id of the user, for which we want to display the profiles.

To display a special nodeprofile, you need to know its machine readable type, which you can find
in the content type settings. Then you can do:

      $output = array();
      $output[$type] = array(
        '#type' => '...',
        '#title' => node_get_types('name', $type),
        '#tabs' => array('view', 'edit'),
        '#uid' => $uid,
        '#content_type' => $type,
        '#weight' => 0,
        '#suffix' => '<br />',
      );
      echo drupal_render($output);

  Make sure that $type is the machine readable content type name and $uid the user's id, or just replace
  them with appropriate values.

  #type can be one of 
    * nodeprofile_display_teaser (print the node teaser)
    * nodeprofile_display_full (print the full node view)
    * nodeprofile_display_link (just print a link to the node)

  #tabs allows you to configure the tabs of that are show for the teaser and full node view.
  Set it to array('view') to disable the edit links.
  The link type doesn't display any tabs, however it displays its [edit] link only, if #tabs
  contains 'edit'. To disable tabs, you can set it to FALSE.

  Each tab has a theme function, which could also be overridden. They are called
  theme_nodeprofile_display_tab_view($node) and theme_nodeprofile_display_tab_edit($node). You could
  even add your own tabs, if you also add the proper theme function.

  Note that there are also theme functions for each type
    theme_nodeprofile_display_full
    theme_nodeprofile_display_teaser
    theme_nodeprofile_display_link
  as well as a theme function for the box around the full node view and the node's teaser:
    theme_nodeprofile_display_box
