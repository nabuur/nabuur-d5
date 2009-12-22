********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name: Actions Module
Author: John VanDyk <jvandyk at iastate dot edu>
Drupal: 5
********************************************************************

NOTE: This is a backport of the actions/triggers implementation in
      Drupal 6.
      
DESCRIPTION:

This module allows you to configure actions for use by other Drupal
modules.

For example, you can create an action that sends an email message
to someone. Another Drupal module may then execute that action.

If you are familiar with object-oriented programming, the actions
that you create with the action module are like object instances.
Singleton actions are also possible.

********************************************************************
INSTALLATION:

0. Apply node.module.patch to give Drupal 5 a 'presave' op. If
   this sounds scary to you, read
   http://drupal.org/node/310274#comment-1017758

1. Place the entire actions directory into your Drupal modules
   directory (normally sites/all/modules or modules).

2. Enable the action module by navigating to:

     Administer > Site building > Modules

3. If you want anyone besides the administrative user to be able
   to configure actions (usually a bad idea), they must be given
   the "administer actions" access permission:
   
     Administer > User management > Access control

   When the module is enabled and the user has the "administer
   actions" permission, an "actions" menu should appear under 
   Administer > Site configuration in the menu system.

********************************************************************
