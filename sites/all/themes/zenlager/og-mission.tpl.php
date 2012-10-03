<?php
if (!empty($mission)) {
?>
  <div id="mission" class="og-mission">
  
  <?php if (!empty($group_image)) { ?>
	        <div class="group-image"><?php print $group_image; ?></div>
	<?php } ?>		
  
  <?php print $mission; ?>
	<div style="clear: both;"></div>    
  </div>
<?php } ?>