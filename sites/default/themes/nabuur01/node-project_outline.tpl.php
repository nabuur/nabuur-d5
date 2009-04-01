<?php // $Id: node.tpl.php,v 1.1.1.8 2008/07/09 12:47:26 rolf Exp $ ?>
<?php phptemplate_comment_wrapper(NULL, $node->type); ?>

<div id="node-<?php print $node->nid; ?>" class="node <?php print $node->type;?><?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">

<?php if ($page == 0): ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>

  <?php  // Add Status image  
    foreach ($node->taxonomy as $tid => $tax) {
      if ($node->taxonomy[$tid]->vid == 6) {
        $tids = $tid;
      }
    }
    print '<div class="pr-status">Status: '. taxonomy_image_display($tids) .'</div>';
  ?>

  <?php if ($submitted): ?>
    <span class="submitted"><?php print t('!date — !username', array('!username' => theme('username', $node), '!date' => format_date($node->created))); ?></span>
  <?php endif; ?>

  <div class="content">
    <?php print $content ?>
  </div>

  <div class="clear-block clear">
    <div class="meta">
    <?php if ($taxonomy): ?>
      <div class="terms"><?php print $terms ?></div>
    <?php endif;?>
    </div>

    <?php if ($links): ?>
      <div class="links"><?php print $links; ?></div>
    <?php endif; ?>
  </div>

</div>
