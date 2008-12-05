<?php // $Id: blocktheme-nabuur-khaki.tpl.php,v 1.1.2.5 2008/07/09 12:47:26 rolf Exp $ ?>
<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="clear-block block block-<?php print $block->module ?>">
  <div class="nabuur-box nabuur-khaki"><div class="nabuur-content">
    <?php if ($block->subject): ?>
      <div class="title"><h2 class="title"><?php print $block->subject ?></h2></div>
    <?php endif;?>
    <div class="content"><?php print $block->content ?></div>
  </div></div>
</div>
