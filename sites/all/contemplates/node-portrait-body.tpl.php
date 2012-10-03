
          
      <?php if (isset($node->field_image[0])) { ?>
		
		<div class="story-image">
	        <div class="story-image-image"><?php print theme('imagecache', 'article', $node->field_image[0]['filepath'], $node->field_image[0]['data']['alt']); ?></div>
            <?php if (isset($node->field_image[0]['data']['description'])) { print '<div class="story-image-description">'.$node->field_image[0]['data']['description'].'</div>'; } ?>
        </div>
      
      <?php } ?>
      <?php print $node->body; ?>    
      <div style="clear: both;"></div>
      <br /><br />
   



