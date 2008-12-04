README.txt
==========
Quicktags adds buttons to your forms, for easy insertation of code (like basic HTML) into a textarea.
It uses on-the-fly generated javascript to do the magick. 
It is not a wysiwyg editor, but something far simpler: It simply inserts the basic HTML into your textarea.


API hook_quicktags
===
The quicktags uses a hook, so that your module can insert its own icons to the line of icons. 
Read the notes on this in the source of the module please. You will find a nice example there too.
Read the manual here: http://webschuur.com/node/628

Notes
=====
 We badly need to get the code optimised. Currently it is too heavy. 
 As of HEAD/4.7 the way of loading modules changes a bit, so we can then to the set_head in the actual textfile hook. That will make sure that the quitcktags JS is only generqted and used when there is a textfield.
 
Bèr Kessels [Drupal Services http://www.webschuur.com]

Feedback will be welcomed, but for support, please create an 'issue' of type 'support request' for the project 'quicktags'.

$Id: README.txt,v 1.6 2007/02/03 10:34:50 ber Exp $