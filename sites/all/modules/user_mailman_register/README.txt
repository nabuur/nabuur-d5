User Mailman Register
---------------------

This is a module for mailman subscribing which extends the Mailman manager module features.
The main feature is that, instead of sending user commands in mail format as Mailman Manager does, it sends url requests to the mailman web interface where admins manage lists members.
Other features are:

    * Immediate and completely invisible to the end-user.
    * Correctly syncronized with mailman server.
    * Lists permissions.
    * Display subscriptions in the registration form of new user.
    * Choose what subscription types are available for every list.
    * Integration of notify and invite mailman options for new subscriptions.
    * Retrive the user mail address realname from Profile fields.
    * Optionally, set a subscription as required.


Requirements
------------

1. A mailman server ( http://www.gnu.org/software/mailman/index.html ) with the web interface enbled and accessible from your drupal webserver.

2. Mailman Manager module ( http://drupal.org/project/mailman_manager ) must be installed and enabled.


Installation
------------

1. Activate this module as usual.

2. Go to admin/settings/mailman_manager and add your mailman lists.

3. Go to admin/settings/user_mailman_register:

 * Set the general module preferences.
 * Activate one or more lists added previuosly by configuring its settings. 

4. Go to admin/user/permissions and set permissions for your users.

5. Go to /user/%uid%/user_mailman_register or in the user profile to manage the User Mailman Register subscriptions.


Links
-------------

Project homepage: http://drupal.org/node/195527
Documentation:    http://drupal.org/node/463508


Access control
--------------

With user permissions you'll be able to choose the subscription method (Mailman Manager email and/or User Mailman Register http) for a user of your site, and, in any moment, to switch between the two methods preserving his subscription status.
For example, if you want to deny the Mailman Manager method and use only the User Mailman Register method you have simply to uncheck the "access mailman manager" permission of the Mailman Manager module.

The general "access user_mailman_register" permission controls who can subscribe and manage his own subscriptions for allowed lists by accessing the subscription form.
This permission controls also if the "anonymous user" is allowed to subscribe authorized lists (or to be invited to join them when the "Only invite users" list option is on). In this case, the subscription form will require a valid email address.
 
The "can subscribe to" permission controls what lists are allowed to be displayed and subscribed by a user.
When the "Visible in user registration form" setting is on, apply this permission to the "anonymous user" to control what lists to display in the new user registration form.
Note: When a subscribed user loses this permission because of role changing, his list subscription is also automatically deleted, just to reflect the new state.

Drupal administrator and users with "administer mailman_manager" permission can access and manage whole site subscriptions.


Extra
-----

The User Mailman Register Import Tool submodule can be used to quick import and syncronize subscriptions of existing users (status and passwords) from a mailman dumpdb file.
In most cases you don't need it, because the main module itself is able to syncronize subscriptions status "at request", but it's useful if other modules (like Mailman Manager) needs also mailman passwords or you want provide status information when the mailman server is unreachable by web server for a long time.
After enabling it, you can find the import tab and using info inside the User Mailman Register admin page.
To create the dump file to import, first of all you need a shell access with mailman or greater user permission to your Mailman server.
Then you can redirect into a new file the output of the mailman dumpdb utility, executed with the pck config file path of your list as command line argument.
For example, in Linux Debian:
# /var/lib/mailman/bin/dumpdb /var/lib/mailman/lists/YOUR_LIST/config.pck > mailman.db

Before to import it, it is strongly recommended to MAKE A BACKUP of the mailman_users table so that it can be restored if the import process spolied the table for some unknown bug.


Donation
--------

The user mailman register module is not sponsored by anyone but i develop and support it during my spare time for free. If you gain something thanks to it or you want to support its development, you can consider to make a donation following instructions in the module project page.


Author and Credits
------------------

The User Mailman Register module is developed by Samuele Tognini <samuele@samuele.netsons.org>
