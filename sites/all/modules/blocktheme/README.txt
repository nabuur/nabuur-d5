
== Description ==
BlockTheme allows an admin to define tpl files for standard block templates
and provides a select box on the block configure form so the user can select
a tpl file to use as opposed to having to override the templates by block ID.

== Installation ==

1. Enable the module

2. go to admin/settings/blocktheme and add entries like:
customblocktemplate|My Custom Template
superblock|My SuperTemplate

3. Create tpl files in your theme directory like:
 customblocktemplate.tpl.php
 superblock.tpl.php
 
4. add the following to [yourtheme]_block($block) in template.php:

if (module_exists('blocktheme')) {
  if ( $custom_theme = blocktheme_get_theme($block) ) {
    return _phptemplate_callback($custom_theme,array('block' => $block));
  }
}

return phptemplate_block($block);

== Usage ==

Go to the configure screen for any block and select the appropriate template.