<?php // $Id: views-list-front_features.tpl.php,v 1.1.2.6 2008/07/21 14:28:55 rolf Exp $ ?>
<?php 
/**
 * views template to output one 'row' of a view.
 * This code was generated by the views theming wizard
 * Date: Tue, 05/27/2008 - 17:53
 * View: front_features
 *
 * Variables available:
 * $view -- the entire view object. Important parts of this object are
 *   front_features, .
 * $view_type -- The type of the view. Probably 'page' or 'block' but could
 *   also be 'embed' or other string passed in from a custom view creator.
 * $node -- the raw data. This is not a real node object, but will contain
 *   the nid as well as other support fields that might be necessary.
 * $count -- the current row in the view (not TOTAL but for this page) starting
 *   from 0.
 * $stripe -- 'odd' or 'even', alternating. * $title -- Display the title of the node.
 * $title_label -- The assigned label for $title
 * $field_picture_fid -- 
 * $field_picture_fid_label -- The assigned label for $field_picture_fid
 * $field_teaser_value -- 
 * $field_teaser_value_label -- The assigned label for $field_teaser_value
 *
 * This function goes in your views-list-front_features.tpl.php file
 */
 
 
 //now we add the stylesheet...
  
  ?>
<div class="front-feature<?php echo $position; ?> front-pi">
    <h2><?php echo t('STORY FROM A VILLAGE'); ?></h2>

    <div class="view-field view-data-title">
      <?php print $title?>
    </div>

    <div class="view-field view-data-field-picture-fid">
      <?php print $field_picture_fid?>
    </div>

    <div class="view-field view-data-field-teaser-value">
      <?php print $field_teaser_value?>
    </div>

    <div class="views_more" align="right">
    <br><ul><li>
      <?php print l(t('more stories'), '/news/stories', array('title' => t('Read more stories on NABUUR.com'))); ?>
    </li></ul>
    </div>
</div>
