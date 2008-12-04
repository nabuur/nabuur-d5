$Id: README.txt,v 1.5.2.2 2007/03/22 16:03:30 fago Exp $

Node Family Module
------------------------
by Wolfgang Ziegler, nuppla@zites.net



Description
-----------
With this module you can define relations between content types. It
automatically creates a relation between all nodes which have been created
by the same user and have the defined content types.

It is a simple node relationship module which doesn't provide any end user
interface. Relations are set automatically (based on admin defined content
type relations) or programmatically through its API.
It also allows to restrict the population of a content type (the number of
creatable nodes of a content type per user).

The module doesn't display related nodes when viewing a node or anything
similar. Currently it's up to the user to use the relation, e.g. use it to
display related nodes during viewing a node (example provided) or use it for
generating views in conjunction with views fusion.


Installation
------------
Simple copy the files in your drupal modules directory and activate the module.


How to use it
-------------
Go to 'admin/content/nodefamily' to manually add relations between content types
or set relations programmatically through the API.


The module doesn't display related nodes or anything similar when viewing a
node. Currently it's up to the user to use the relation, e.g.:

Short instructions to display child nodes inside a parent node:

 * theme the default display of your parent node type,
   instructions are available in the handbook: http://drupal.org/node/11816

 * load the children's nodes at the top of your theme:
   <?php $children = nodefamily_relation_load($nid); ?>

 * get the node views and print them

   e.g. use this to print the teaser of the first child node:
   <?php print node_view($children[0], TRUE, FALSE, TRUE); ?>

   or this to print the full node view of all children:
   <?php
      foreach ($children as $childnode) {
        print node_view($childnode);
      }
   ?>
   
 * The above example would just print all children. If you want to print
   a child node of a special content type only, you can use the function
   nodefamily_relation_load_by_type($nid) instead.
   E.g.:

   <?php $children = nodefamily_relation_load_by_type($nid);
         if (isset($children['your_content_type'])) {
           foreach ($children['your_content_type'] as $childnode) {
             print node_view($childnode);
           }
         }
   ?>
   Replace 'your_content_type' with your machine-readable content-type name!




The module also allows to restrict the number of creatable nodes of a content
type per user (population).
You can find this setting at 'admin/content/types'. It might be useful
to set the population to one for a parent content type.

If the population of a content type is restricted to 1,
nodefamily provides a unique URL for adding/editing nodes of this type:

Go to 'nodefamily/CONTENT_TYPE' for adding/editing your node of the content type.
Go to 'nodefamily/CONTENT_TYPE/USER_ID' to edit nodes of this type of other users.

Replace CONTENT_TYPE with the machine readable content type name.


Pageroute integration
---------------------
This module offers integration to the pageroute module. It provides two further
page types:

 * lonely node management page
 * lonely node display

Both page types may be used only with content types which are restricted
to a maximum population of one. Then you can use the lonely node management page
to add/edit the "lonely node" as it might be useful e.g. for user profiles.

The lonely node display page can be used to view this lonely node. This might be 
useful for showing the resulting node. However, keep in mind that
there will be a (themeable) "There is no node of this type." message, if there
is no node that can be displayed.

This type are in particular useful, if you want to use pageroute for building
nodeprofiles!


Views integration
------------------
This module offers some basic Views integration. Currently it adds filters for
filtering by parent or children node ids which can be restricted to filter only
by nodes with a certain content type. This is useful in conjunction with the NOT
EQUAL operator, e.g. use NOT EQUAL node id 0 and a special content type to filter
by nodes that have children of a special content type.
There are also some argument handlers: for filtering by the node id of grandparents,
parents, children and grandchildren. Useful for e.g. building a view that lists all
children of the node with the argument id.

For building views that contain data of more related content types or if you need more
complex filtering you can use the views fusion module.

The views fusion module allows you to build tabular views, which list data of several
related nodes in one line!
E.g. if you use the usernode module together with the nodeprofile module, it allows you
to build a user listing which also contains data of your profile node. This is working,
because there is a nodefamily relation between this nodes and nodefamily offers views
fusion support!
