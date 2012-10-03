<?php
// $Id: page.tpl.php,v 1.28 2008/01/24 09:42:52 goba Exp $
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
	<head>
		<title><?php print $head_title ?></title>        
		<?php print $head ?>
		<?php print $styles ?>
		<?php print $scripts ?>
		<script type="text/javascript"><?php /* Needed to avoid Flash of Unstyle Content in IE */ ?> </script>        
	</head>
	<body>
<div id="main">
	<div class="backgroundContainer">
		<div class="mainContainer">
			<div id="header">
              LOGO                      
		      <?php print $search_box ?>
			  <div>[<?php print $header ?>]</div>              
            </div>
            <div id="topnav">
		      <?php if (isset($secondary_links)) { ?><?php print theme('links', $secondary_links, array('class' => 'links', 'id' => 'subnavlist')) ?><?php } ?>
		      <?php if (isset($primary_links)) { ?><?php print theme('links', $primary_links, array('class' => 'links', 'id' => 'navlist')) ?><?php } ?>
            </div>
		</div>
	</div>
	
	<div class="backgroundContainer">
		<div class="mainContainer">

<!--			<div class="colmask threecol">
			    <div class="colmid">
					<div class="colleft">
						<div class="col1">
						      <?php if ($mission) { ?><div id="mission"><?php print $mission ?></div><?php } ?>
						      <?php print $breadcrumb ?>
					          <h1 class="title"><?php print $title ?></h1>
						      <div class="tabs"><?php print $tabs ?></div>                        
						      <?php if ($show_messages) { print $messages; } ?>
					          <?php print $help ?>
						      <?php print $content; ?>
						      <?php print $feed_icons; ?>                              
						</div>
			
					    <?php if ($left) { ?><div class="col2"><div id="menuContainer">
					      <?php print $left ?>
					    </div></div><?php } ?>
                        
					    <?php if ($right) { ?><div class="col3"><div id="sideBannerBox">sdfs
					      <?php print $right ?>
					    </div></div><?php } ?>

					</div>
				</div>-->
                       
                	<div id="centercolumn">
						      <?php if ($mission) { ?><div id="mission"><?php print $mission ?></div><?php } ?>
						      <?php print $breadcrumb ?>
					          <h1 class="title"><?php print $title ?></h1>
						      <div class="tabs"><?php print $tabs ?></div>                        
						      <?php if ($show_messages) { print $messages; } ?>
					          <?php print $help ?>
						      <?php print $content; ?>
						      <?php print $feed_icons; ?>                              

                          <?php if ($right) { ?><div id="rightcolumn">
					      <?php print $right ?>
					    </div><?php } ?>
					</div>                
                    
					    <?php if ($left) { ?><div id="leftcolumn">
					      <?php print $left ?>
					    </div><?php } ?>                           
                        
                
            <br style="clear: both;" />    
		    </div>


		<div id="footer">
		  <?php print $footer_message ?>
		  <?php print $footer ?>
		</div>    

		</div>        
</div>
<?php print $closure ?>        
</body>
</html>
