Imagecache is a dynamic image manipulation and cache tool.  It allows you to 
create a namespace that corresponds to a set of image manipulation actions. It 
generates a derivative image the first time it is requested from a namespace 
until the namespace or the entire imagecache is flushed.

Usage:
Go to Administer -> Site Configuration -> Image cache 
Create a preset,
Add some actions to your preset.  Possible actions include scale, resize, and 
crop.  Multiple actions can be applied to one ruleset. 

Your modified image can be viewed by visiting a URL in this format:
http://example.com/files/imagecache/preset-name/files/image-name.jpg

For example, if your preset is named 'products' and your image is named 
'green-widget.jpg', you could view your modified image at:

http://example.com/files/imagecache/products/files/green-widget.jpg

To automatically display your manipulated image add the following line to your 
desired tpl.php file where you would like the image to appear:
 
<?php
print theme('imagecache', 'preset_namespace', $image['filepath'], $alt, $title, $attributes);
?>

Change 'preset_namespace' to the name of your imagecache preset.

$alt, $title and $attributes are optional parameters.

If you are using Imagecache with the CCK Imagefield module, you might use 
something like the following in your template for an imagecache preset called 'products':

<?php
print theme('imagecache', 'products', $node->field_product_photo[0]['filepath']);
?>

See also this post for more information:
http://drupal.org/node/163561
