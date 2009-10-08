<?php // $Id: page.tpl.php,v 1.1.1.15 2008/11/13 09:53:58 fransk Exp $ ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language ?>" lang="<?php print $language ?>">
  <head>
    <title><?php print $head_title ?></title>
    <?php print $head ?>
    <?php print $styles ?>
    <?php print $scripts ?>
    <!--  <style type="text/css" media="print">@import "<?php print base_path() . path_to_theme() ?>/print.css";</style>-->
    <!--[if lt IE 7]>
    <style type="text/css" media="all">@import "<?php print base_path() . path_to_theme() ?>/fix-ie.css";</style>
    <![endif]-->
    <script language="javascript" type="text/javascript">
      $(document).ready(function(){ $(".node-task_discussion .collapsible").addClass("collapsed");
                                    $(".node-news-item .collapsible").addClass("collapsed");}); 
  	</script>

  </head>
  <body<?php print phptemplate_body_class($sidebar_right); ?>>
    <!-- Layout -->
    <div id="page">
      <div id="top">
        <?php 
          print '<a href="'. check_url($base_path) .'" title="nabuur.com - home - volunteer from behind your computer">';
          print '<img src="'. check_url($logo) .'" alt="nabuur.com - the global neighbour network" id="logo" />';
          print $site_html .'</a>'; ?>
        <div id="language">
          <?php $data = translation_block('view'); print $data['content']; ?>
        </div>
        <div id="primary-links">
          <?php print theme('links', $primary_links, array('class' => 'links primary-links')); ?>
        </div>
        <?php print $login_message; ?>
      </div>
      <?php if($secondary_links): ?>
        <div id="tabbar">
          <?php if ($breadcrumb): print $breadcrumb; endif; ?>
          <div id="secondary-links">
            <?php print theme('links', $secondary_links, array('class' => 'links secondary-links')); ?>
          </div>
        </div>
      <?php else: ?>
        <div id="tabbar" class="no-links">
          <?php if ($breadcrumb): print $breadcrumb; endif; ?>
        </div>
      <?php endif; ?>
      <div id="main">
        <div id="content">
          <?php if ($messages): print $messages; endif; ?>
          <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
          <?php if ($title): print '<h2'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h2>'; endif; ?>
          <?php if ($tabs): print $tabs .'</div>'; endif; ?>
           <?php if ($help): print $help; endif; ?>
          <?php print $content ?>
          <?php print $feed_icons ?>
        </div>
        <?php if($sidebar_right): ?>
          <div id="sidebar-right" class="sidebar">
            <?php print $sidebar_right; ?>
          </div>
        <?php endif; ?>
        <div class="spacer">&nbsp;</div>
      </div>
      <div id="footer">
        <p>
          <!-- @todo Move footer PHP to template.php or nabuur module -->
          <?php
            $social_media = array(
              l(t('Follow us on Twitter'), 'http://twitter.com/nabuur', array(), NULL, NULL, TRUE, FALSE),
              l(t('Join our LinkedIn Group'), 'http://www.linkedin.com/groups?gid=81324', array(), NULL, NULL, TRUE, FALSE),
              l(t('Become a fan on Facebook'), 'http://www.facebook.com/nabuur', array(), NULL, NULL, TRUE, FALSE),
            );
            $about = l(t('About NABUUR'), "/about-nabuur");
            $faq = l(t('FAQ'), "/frequently-asked-questions");
            $privacy = l(t('Privacy'), "/privacy");
            $feedback = l(t('Site feedback'), "/group/nabuurcom-website-development/project/task/general-feedback");
            $contact = l(t('Contact'), "/contact");
            $sitemap = l(t('Sitemap'), "/sitemap");
            $volunteering = t('Volunteering in'). ' '. $continents;
            print implode(' | ', $social_media) .'<br /><br />';
            print "$about | $faq | $contact<br />";
            print "$volunteering<br />";
            // Todo: sitemap
            print "$feedback | $privacy<br />";
          ?>
		</p>
      </div>
    </div>
    
    <?php print $closure ?>
  </body>
</html>
