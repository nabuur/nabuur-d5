<?php // $Id: blocktheme-nabuur-terra-bar.tpl.php,v 1.1.2.1 2008/08/22 13:44:41 fransk Exp $ ?>
<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="clear-block block block-<?php print $block->module ?>">
  <div class="nabuur-box nabuur-terra"><div class="nabuur-content">
    <?php if ($block->subject): ?>
      <div class="title"><h2 class="title"><?php print $block->subject ?></h2></div>
    <?php endif;?>
    <div class="content"><?php print $block->content ?></div>
  </div></div>
</div>
