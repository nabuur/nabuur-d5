********************************************************************
D R U P A L    M O D U L E
********************************************************************
Name: geonames_cck  
Initial Authors: Peter Brownell and Russell Blakeborough
Sponsors: Code Positive [www.codepositive.com] 
          and school of everything [www.schoolofeverything.com]
Drupal: 5.0.x
********************************************************************
DESCRIPTION:

The geonames_cck module implements a CCK field type that uses geonames 
(http://www.geonames.org) to translate location names into their longditude 
and latitude coordinates. 

This module was designed to work with the geomap (http://drupal.org/project/geomap)
module. Locations are output using Geo microformats, and the geomap 
module will display any geo data on a google map. 

We use the geonames web service module (http://drupal.org/project/geonames) 
to handle the actual communication with geonames. 

-- Limitations
We have built this version of the module to handle basic name lookups and 
disambigation. There are all sorts of things it does not do. 

-- No Multiple Values
Although a lot of work has been done, working multivalue support was out of 
scope for our initial release. 

-- No Locations Module Integration
The current implementation of this module does not integrate with the standard locations system. There are a few reasons for this, but the first was simply implmentation scope. As a result of this lack of integration, we do not currently have working proximity search functionality. 



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

  Geonames CCK fields will now appear as a new field type for CCK
  nodes. Once assigned to a content type, you may want to set the 
  display settings for the field. 
  
  Locations data can be set to hidden so that it is not visible to 
  users, but is still plotted on a map.  
  
  
       
      
********************************************************************
ACKNOWLEDGEMENTS

Thanks to Geonames for a great open resource. 

