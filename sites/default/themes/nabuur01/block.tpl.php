<?php // $Id: block.tpl.php,v 1.1.1.9 2008/07/28 10:03:15 kester Exp $ ?>
<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="clear-block block block-<?php print $block->module ?>">

<?php if ($block->subject): ?>
  <h2><?php print t($block->subject); ?></h2>
<?php endif;?>

  <div class="content"><?php print $block->content ?></div>
</div>
