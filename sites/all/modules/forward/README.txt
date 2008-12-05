

        FORWARD MODULE - README
______________________________________________________________________________

NAME:       Forward
AUTHORS:    Sean Robertson <seanr@ngpsoftware.com>
            Peter Feddo
______________________________________________________________________________


DESCRIPTION

Adds a "forward this page" link to each node and a "Send an e-Postcard" link to
the menu. The node link allows users to forward a link to a specific node on 
your site to a friend.  The menu item allows users to send a generic message 
about the site as a whole rather than a specific node; this is useful for an 
"Invite your Friends" feature.  You can customize the default form field values 
and even view a running count of the emails sent so far using the forward module.


INSTALLATION

Step 1) Download latest release from http://drupal.org/project/forward

Step 2)
  Extract the package into your 'modules' directory.


Step 3)
  Enable the forward module.

  Go to "administer >> build >> modules" and put a checkmark in the 'status' column next
  to 'forward'.


Step 4)
  Go to "administer >> site configuration >> forward" to configure the module.

  Edit 'forward.theme' to change the look and feel of your emails.  You can also
  create different themes for different phptemplate themes with by copying the
  contents of 'forward.theme' into your theme's 'template.php' file and
  prepending the function name with 'phptemplate_'.  There is one theme function
  for the "forward this page" email and another for the "Send an e-Postcard" email.
  Then you can customize the function as needed and those changes will only appear
  went sent by a user using that theme.  For more information, see the Drupal
  handbook:

  http://drupal.org/node/11811


Step 5)
  Enable permissions appropriate to your site.

  The forward module provides two permissions:
   - 'access forward': allow user to forward pages.
   - 'administer forward': allow user to configure forward.

  Note that you need to enable 'access forward' for users who should be able to
  send emails using the forward module.


Step 6)
  Go to "administer >> logs >> forward tracking" to view forward usage statistics.


CREDITS & SUPPORT

Special thanks to Jeff Miccolis of developmentseed.org for supplying the
tracking features and various other edits.  Thanks also to Nick White for his
EmailPage module, some code from which was used in this module, as well as the
numerous other users who have submitted issues and patches for forward.

All issues with this module should be reported via the following form:
http://drupal.org/node/add/project_issue/forward

______________________________________________________________________________
http://www.ngpsystems.com