<?php
// $Id: page.tpl.php,v 1.13 2008/09/15 08:31:58 johnalbin Exp $

/**
 * @file page.tpl.php
 *
 * Theme implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $css: An array of CSS files for the current page.
 * - $directory: The directory the theme is located in, e.g. themes/garland or
 *   themes/garland/minelli.
 * - $is_front: TRUE if the current page is the front page. Used to toggle the mission statement.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Page metadata:
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation.
 *   $language->dir contains the language direction. It will either be 'ltr' or 'rtl'.
 * - $head_title: A modified version of the page title, for use in the TITLE tag.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $body_classes: A set of CSS classes for the BODY tag. This contains flags
 *   indicating the current layout (multiple columns, single column), the current
 *   path, whether the user is logged in, and so on.
 *                 
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 * - $mission: The text of the site mission, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $search_box: HTML to display the search box, empty if search has been disabled.
 * - $primary_links (array): An array containing primary navigation links for the
 *   site, if they have been configured.
 * - $secondary_links (array): An array containing secondary navigation links for
 *   the site, if they have been configured.
 *
 * Page content (in order of occurrance in the default page.tpl.php):
 * - $left: The HTML for the left sidebar.
 *
 * - $breadcrumb: The breadcrumb trail for the current page.
 * - $title: The page title, for use in the actual HTML content.
 * - $help: Dynamic help text, mostly for admin pages.
 * - $messages: HTML for status and error messages. Should be displayed prominently.
 * - $tabs: Tabs linking to any sub-pages beneath the current page (e.g., the view
 *   and edit tabs when displaying a node).
 *
 * - $content: The main content of the current Drupal page.
 *
 * - $right: The HTML for the right sidebar.
 *
 * Footer/closing data:
 * - $feed_icons: A string of all feed icons for the current page.
 * - $footer_message: The footer message as defined in the admin settings.
 * - $footer : The footer region.
 * - $closure: Final closing markup from any modules that have altered the page.
 *   This variable should always be output last, after all other dynamic content.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <script type="text/javascript"><?php /* Needed to avoid Flash of Unstyled Content in IE */ ?> </script>
</head>
<body class="<?php print $body_classes; ?>">

  <div id="page"><div id="page-inner">

	<div id="pre-header"><div id="pre-header-inner" class="clear-block">
        <div id="header-blocks" class="region region-header">
          <?php print $header; ?>
        </div> <!-- /#header-blocks -->
    </div></div>

    <a name="top" id="navigation-top"></a>
    <div id="header"><div id="header-inner" class="clear-block">

    	<div id="top-cart">
        
            <?php $cart_count = count(uc_cart_get_contents()); ?>
			<img align="absmiddle" src="/sites/all/themes/zenlager/images/ikon-indkoebskurv.gif" />&nbsp;&nbsp;<a href="/cart"><?php print t('View shopping cart (!count items)', array('!count' => $cart_count)); ?></a>
        </div>
    
		<div id="top-buttons">
	      <?php if ($logged_in): ?>			
			<div><?php print t('You are logged in as:'); ?>&nbsp;<strong><?php print $username; ?></strong><img class="tbbs" src="/sites/all/themes/zenlager/images/top-buttons-bshadow.gif" /><img class="tbrs" src="/sites/all/themes/zenlager/images/top-buttons-rshadow.gif" /></div>
<!--			<div><a href="/user"><?php print t('My Profile'); ?><img class="tbbs" src="/sites/all/themes/zenlager/images/top-buttons-bshadow.gif" /><img class="tbrs" src="/sites/all/themes/zenlager/images/top-buttons-rshadow.gif" /></div>-->
			<div><a href="/logout"><?php print t('Log out'); ?><img class="tbbs" src="/sites/all/themes/zenlager/images/top-buttons-bshadow.gif" /><img class="tbrs" src="/sites/all/themes/zenlager/images/top-buttons-rshadow.gif" /></div>            
          <?php else: ?> 
				
			<div><a id="toboggan-login-link" class="toboggan-login-link" href="/user/login?destination=node"><?php print t('Log in'); ?></a><img class="tbbs" src="/sites/all/themes/zenlager/images/top-buttons-bshadow.gif" /><img class="tbrs" src="/sites/all/themes/zenlager/images/top-buttons-rshadow.gif" /></div>          	
<!--			<div><a href="/user"><?php print t('Log in'); ?></a><img class="tbbs" src="/sites/all/themes/zenlager/images/top-buttons-bshadow.gif" /><img class="tbrs" src="/sites/all/themes/zenlager/images/top-buttons-rshadow.gif" /></div>          	-->
			<div><a href="/user/register"><?php print t('Sign up for free'); ?></a><img class="tbbs" src="/sites/all/themes/zenlager/images/top-buttons-bshadow.gif" /><img class="tbrs" src="/sites/all/themes/zenlager/images/top-buttons-rshadow.gif" /></div>          	            
          <?php endif; ?>            
        </div>



            <?php if ($is_front): ?>          
            <h1 id="logo"><a href="/" title="<?php print $site_name; ?>" rel="home"><img src="/sites/all/themes/zenlager/images/logo.gif" alt="<?php print $site_name; ?>" /></a></h1>
            <?php else: ?> 
            <div id="logo"><a href="/" title="<?php print t('Home'); ?>" rel="home"><img src="/sites/all/themes/zenlager/images/logo.gif" alt="<?php print t('Home'); ?>" /></a></div>            
            <?php endif; ?>            
            <img src="/sites/all/themes/zenlager/images/logo-beta.gif" style="position: absolute; top: 30px; left: 492px;" alt="Lagervarer.dk er under udvikling. Mange funktioner virker ikke endnu!" />
             
           <!-- <div id="beta-info">
				<div>Vigtig information</div>            
                <p>
                    <strong>Lagervarer.dk er under ombygning.</strong> Vi er i øjeblikket i gang med at lave forbedringer her på Lagervarer. Dette kan medføre korte tidsrum med driftsforstyrrelser.
                </p>
            </div>-->


       
        
      <ul id="top-navigation">
          <li><a <?php print $site_section == 'varepartier' ? 'class="active-trail" ' : '' ?>href="/"><img src="/sites/all/themes/zenlager/images/topnav-varepartier<?php print $site_section == 'varepartier' ? '-active' : '' ?>.png" /></a></li>
          <li><a <?php print $site_section == 'lagersalg' ? 'class="active-trail" ' : '' ?> href="/lagersalg"><img alt="Lagersalg" src="/sites/all/themes/zenlager/images/topnav-lager<?php print $site_section == 'lagersalg' ? '-active' : '' ?>.png" /></a></li>
          <li><a <?php print $site_section == 'forum' ? 'class="active-trail" ' : '' ?> href="/community"><img alt="Forum" src="/sites/all/themes/zenlager/images/topnav-forum<?php print $site_section == 'forum' ? '-active' : '' ?>.png" /></a></li>
<!--          <li><a <?php print $site_section == 'artikler' ? 'class="active-trail" ' : '' ?> href="/artikler"><img alt="Artikler" src="/sites/all/themes/zenlager/images/topnav-artikler<?php print $site_section == 'artikler' ? '-active' : '' ?>.png" /></a></li>                              -->
          <li><a <?php print $site_section == 'information' ? 'class="active-trail" ' : '' ?> href="/information"><img alt="Information" src="/sites/all/themes/zenlager/images/topnav-information<?php print $site_section == 'information' ? '-active' : '' ?>.png" /></a></li>                                        
          <li><a <?php print $site_section == 'user' ? 'class="active-trail" ' : '' ?> href="/user"><img alt="Konto" src="/sites/all/themes/zenlager/images/topnav-konto<?php print $site_section == 'user' ? '-active' : '' ?>.png" /></a></li>                                        
      </ul>
      
      <img id="header-inner-right" height="66" width="45" src="/sites/all/themes/zenlager/images/header-inner-right.gif" />
      <img id="header-inner-left" height="66" width="45" src="/sites/all/themes/zenlager/images/header-inner-left.gif" />      

    </div></div> <!-- /#header-inner, /#header -->

    <div id="main"><div id="main-inner" class="clear-block<?php if ($search_box or $primary_links or $secondary_links or $navbar) { print ' with-navbar'; } ?>">

      <div id="content"><div id="content-inner">

        <?php if ($mission): ?>
          <div id="mission"><?php print $mission; ?></div>
        <?php endif; ?>

        <?php if ($content_top): ?>
          <div id="content-top" class="region region-content_top">
            <?php print $content_top; ?>
          </div> <!-- /#content-top -->
        <?php endif; ?>

        <?php if ($breadcrumb or $title or $tabs or $help or $messages): ?>
          <div id="content-header">
            <?php print $breadcrumb; ?>
            <?php if ($title): ?>
              <h1 class="title"><?php print $title; ?></h1>
            <?php endif; ?>
            <?php print $messages; ?>
            <?php if ($tabs): ?>
              <div class="tabs"><?php print $tabs; ?></div>
            <?php endif; ?>
            <?php print $help; ?>
          </div> <!-- /#content-header -->
        <?php endif; ?>

        <div id="content-area">
          <?php print $content; ?>
        </div>

        <?php if ($feed_icons): ?>
          <div class="feed-icons"><?php print $feed_icons; ?></div>
        <?php endif; ?>

        <?php if ($content_bottom): ?>
          <div id="content-bottom" class="region region-content_bottom">
            <?php print $content_bottom; ?>
          </div> <!-- /#content-bottom -->
        <?php endif; ?>

      </div></div> <!-- /#content-inner, /#content -->


      <?php if ($left): ?>
        <div id="sidebar-left"><div id="sidebar-left-inner" class="region region-left">
          <?php print $left; ?>
        </div></div> <!-- /#sidebar-left-inner, /#sidebar-left -->
      <?php endif; ?>

      <?php if ($right): ?>
        <div id="sidebar-right"><div id="sidebar-right-inner" class="region region-right">
          <?php print $right; ?>
        </div></div> <!-- /#sidebar-right-inner, /#sidebar-right -->
      <?php endif; ?>

    </div></div> <!-- /#main-inner, /#main -->


      <div id="footer"><div id="footer-inner" class="region region-footer">

		<div class="footer-row footer-row-border">
			<h2><?php print variable_get('sitename', 'Lagervarer.dk'); ?></h2>
			<ul>
				<li><?php print l(t('Help'), 'information/faq'); ?></li>			
				<li><?php print l(t('About !sitename', array('!sitename' => variable_get('sitename', 'Lagervarer.dk'))), 'information/om-os'); ?></li>
				<li><?php print l(t('Contact us'), 'information/kontakt'); ?></li>													
			</ul>		
		</div>

		<div class="footer-row footer-row-border">
	<?php
	if (user_access('access feedback form') && $_GET['q'] != 'admin/reports/feedback') {
    	$block = (object)module_invoke('feedback', 'block', 'view', 'form');
	    $block->module = 'feedback';
    	$block->delta = 'form';
	    $block->region = 'footer';
    	print theme('block', $block);
	  }		
	?>	  		
		</div>
		
		
		<div style="clear: both;"></div>
        <?php print $footer; ?>

      </div></div> <!-- /#footer-inner, /#footer -->


  </div></div> <!-- /#page-inner, /#page -->

  <?php if ($closure_region): ?>
    <div id="closure-blocks" class="region region-closure"><?php print $closure_region; ?></div>
  <?php endif; ?>

  <?php print $closure; ?>

</body>
</html>
