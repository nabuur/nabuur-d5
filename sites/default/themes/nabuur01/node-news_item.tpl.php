<?php // $Id: node-news_item.tpl.php,v 1.1.1.3 2008/12/05 09:39:01 fransk Exp $ ?>
<?php phptemplate_comment_wrapper(NULL, $node->type); ?>

<div id="node-<?php print $node->nid; ?>" class="node <?php print $node->type;?><?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">

  <?php if (!(arg(0) == 'news' && arg(1) == 'stories')): ?>
  <?php if ($page == 0): ?>
    <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
  <?php endif; ?>
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
  <?php else: // page story telling ?>
  <?php global $user; global $i18n_langpath; if (strlen($i18n_langpath) != 2) {$i18n_langpath = 'en';} ?>
  <div class="story-telling">
  <div class="story-telling-left">
  <?php if ($page == 0): ?>
    <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
  <?php endif; ?>

  <?php if ($submitted): ?>
    <span class="submitted"><?php print t('!date — !username', array('!username' => theme('username', $node), '!date' => format_date($node->created))); ?></span>
  <?php endif; ?>

  <div class="content">
    <?php print $field_picture['0']['view']; print $field_teaser['0']['view']; print $field_body['0']['view']; ?>
  </div>
  </div>
  <div id="sidebar-right" class="sidebar action-box">
  <div class="nabuur-box nabuur-green-bar">
  <div class="title"><h2 class="title">Action Box</h2></div>
  <div class="ab-forward"><?php print theme('links', forward_link('node', $node)); ?></div>
  <?php if ($user->uid): ?>
    <div class="ab-comment"><?php $mytitle = t("Leave a comment");  $mytitle = theme('image', drupal_get_path('theme', 'nabuur01') .'/img/comment.gif', $mytitle, '', array('class' => 'forward-icon forward-icon-margin')) . $mytitle; 
    print '<a href="/en/node/add/comment/'. $node->nid .'">'. $mytitle .'</a>'; ?></div>
    <div class="ab-writeown"><?php $mytitle = t("Write your own story");  $mytitle = theme('image', drupal_get_path('theme', 'nabuur01') .'/img/writeown.gif', $mytitle, '', array('class' => 'forward-icon forward-icon-margin')) . $mytitle; 
    print '<a href="/node/add/news_item?gids[]=29295">'. $mytitle .'</a>';  ?></div>
  <?php endif; ?>
  <div class="ab-10things"><?php $mytitle = t("10 things you can do today");  $mytitle = theme('image', drupal_get_path('theme', 'nabuur01') .'/img/10things.gif', $mytitle, '', array('class' => 'forward-icon forward-icon-margin')) . $mytitle; 
    print '<a href="/10-things-you-can-do">'. $mytitle .'</a>';  ?></div>
  <?php if ($user->uid): ?>
    <div class="ab-fivestar"><?php print $node->content['fivestar_widget']['#value']; ?></div>
  <?php endif; ?>
  <div class="ab-service"><?php print "Bookmark/Search this post:<br>";
             foreach (service_links_render($node, $nodelink = FALSE) as $slink) {
               print $slink;
             }; print '&nbsp;' ?></div>
  </div >
  </div>
  </div>
  
  <?php endif; // if (!$page .... ?>

</div>
