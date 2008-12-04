********************************************************************
D R U P A L    M O D U L E
********************************************************************
Name: Geomap 
Initial Author: Peter Brownell
Sponsors: Code Positive [www.codepositive.com] 
          and school of everything [www.schoolofeverything.com]
Drupal: 5.0.x
********************************************************************
DESCRIPTION:

  The Geomap module will render a Google map in a block.
  
  The locations placed on the google map are obtained by 
  analysing the current page for GEO microformat informaton.
  When this info exists, a map will be rendered, when there 
  is no location information, no map will appear. 
  
  This module has been disigned to work with the geonames_cck
  module, which outputs it's data in the correct format. 
  
  More information on GEO Microformats: 
    http://microformats.org/wiki/geo 
    
  There have been a few updates to the GEO microformat since
  this was originally written, so not every form of the microformat
  is currently supported.
  
  Example data: 
  <div class="geo" title="Canterbury">
    Canterbury, United Kingdom
    <span class="latitude" title="51.2667"/>
    <span class="longitude" title="1.08333"/>
  </div> 
  

********************************************************************
INSTALLATION:

  Note: It is assumed that you have Drupal up and running.  Be sure to
    check the Drupal web site if you need assistance.  If you run into
    problems, you should always read the INSTALL.txt that comes with the
    Drupal package and read the online documentation.

  1. Place the module directory in to Drupal
        modules/directory.

  2. Enable the module by navigating to:

     Administer > Site building > Modules

  Click the 'Save configuration' button at the bottom to commit your
    changes.
    


********************************************************************
CONFIGURATION

  1. Go to Administer > Settings > Geomap 
      
      Set your google map key. 
      You can obtain this key (for free) from 
        http://code.google.com/apis/maps/signup.html
  
  2. Go to Administer > Site Building > Blocks
       Assign the "Geo Microformat Googlemap" to a region
       
  3. Add some geo microformats to your pages. 
     The best way to do this is to use the geonames_cck module, 
     and add a location field to a node. 
     
      
********************************************************************
ACKNOWLEDGEMENTS

Javascrpt code: 
  Based on jQuery googleMap by Dylan Verheul <dylan@dyve.net>
