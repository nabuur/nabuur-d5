Source code for website http://www.nabuur.com. Drupal version 5 from
http://drupal.org.

If you would like to develop for nabuur.com you need:
 * in your http-root directory:
   git clone git://github.com/nabuur/nabuur-d5.github nabuur
 * get the database:
   wget http://dl.dropbox.com/u/877259/nabuur-5.x-developer.mysql
 * get the files:
   wget http://dl.dropbox.com/u/877259/nabuur-dev-files.tar

Configuration:
 * create a database in mysql (nabuur)
 * import the database nabuur-5.x-developer.mysql into nabuur
 * copy sites/default/ht_config.example.php sites/default/.ht_config.php
 * edit the database URI in sites/default/.ht_config.php
 * in the nabuur/drupal directory:
   tar -xf /path/to/nabuur-5.x-developer.mysql
 * make files/ read/write for the webserver (for most linuxes is is:)
   sudo chown -R www-data files
 * if you create a site in apache, point the document-root to
   /path/to/nabuur/drupal

Now the site is available on:
   http://localhost/nabuur/drupal
Or http://example.com/




