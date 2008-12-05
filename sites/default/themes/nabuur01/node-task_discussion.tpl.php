<?php // $Id: node-task_discussion.tpl.php,v 1.1.1.7 2008/08/14 16:14:49 admin Exp $ ?>
<?php
/**
 * For comments as nodes to work, we need to know in advance what node types
 * will be used as comments and theme them differently. In particular, pay
 * attention to the way the title is output. This tpl.php assumes a content
 * type called "comment".
 */
 ?>

<div id="node-<?php print $node->nid; ?>" class="node comment<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">
  <div class="comment-header">
    <div class="comment-left">   
      <?php print theme('username', $node); ?>
    </div>
    <div class="comment-right">
      <div class="subject">
      <a name="comment-<?php print $node->nid; ?>"></a>
      <?php if ($node->title != '{{no-title}}') { 
        $query = ($_GET['page']) ? 'page='. (int)$_GET['page'] : NULL;
        $query .= ($_GET['page'] && $_GET['sort']) ? '&' : NULL;
        $query .= ($_GET['sort'] == 'asc' || $_GET['sort'] == 'desc' || $_GET['sort'] == 'threaded') ? 'sort='. $_GET['sort'] : NULL;
        print l($node->title, $_GET['q'], NULL, $query, "comment-$node->nid") . ' ' . theme('mark', $node->new); 
      } ?>
      </div>
    </div>
    <div style="clear:both"></div>
  </div>
  <div class="comment-main">
    <div class="comment-left">
      <?php print $picture ?>
    </div>
    <div class="comment-right">
      <div class="content">
        <?php if ($submitted): ?>
          <span class="submitted"><?php print format_date($node->created); ?></span>
        <?php endif; ?>
        <?php print $content ?>
      </div>
    </div>
    <div style="clear:both"></div>
  </div>
  <div class="comment-footer">
    <div class="clear-block clear">
      <div class="meta">
      <?php if ($taxonomy): ?>
        <div class="terms"><?php print $terms ?></div>
      <?php endif;?>
      </div>
    </div>

    <?php if ($links): ?>
      <div class="links clear-block"><?php print $links; ?></div>
    <?php endif; ?>
  </div>
</div>
