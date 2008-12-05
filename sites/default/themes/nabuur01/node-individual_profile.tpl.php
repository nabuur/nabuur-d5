<?php // $Id: node-individual_profile.tpl.php,v 1.1.2.5 2008/09/19 15:23:18 fransk Exp $ ?>
<?php phptemplate_comment_wrapper(NULL, $node->type); ?>

<div id="node-<?php print $node->nid; ?>" class="node <?php print $node->type;?><?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">

  <div class="content">
    <?php print $content;
          // @TODO render the user node for my profile in a module or in template.php
          // We need the user for this profile 
          $node_user = user_load(array('uid' => $node->uid));
          print '<p> </p><div class="field"><div class="field-label">Neighbour since:</div><div class="field-items"><div class="field-item">'. format_date($node_user->created) .'<p> </p></div></div>';
          print '<div class="field-label">Last login:</div><div class="field-items"><div class="field-item">'. format_date($node_user->login) .'<p> </p></div></div>';
          $no_posts =  $stats['online'] = db_result(db_query("SELECT COUNT(nid) AS count FROM {node} WHERE uid = %d AND ( type = 'task_discussion' OR type = 'news' OR type = 'comment')", $node->uid));
          print '<div class="field-label">I have posted on NABUUR:</div><div class="field-items"><div class="field-item">'. $no_posts .' Posts<p> </p></div></div></div>';
          //print_r($user);
    ?>
  </div>

  <div class="meta">
  <?php if ($taxonomy): ?>
    <div class="terms"><?php print $terms ?></div>
  <?php endif;?>
  </div>
</div>
