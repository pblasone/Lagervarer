  
      <div class="profile-description">
	  <?php print $node->field_description[0]['value']; ?>
	  </div>

	<br style="clear: both;" />
      <?php print $node->content['body']['#value']; ?>    


	<?php 
    	require_once(drupal_get_path('module', 'uc_catalog').'/uc_catalog.pages.inc');
        print '<div class="lv-catalog-latest"><h2>'.lv_pages_more_link($node->path.'/varepartier', t("View all !name's bulk lots", array('!name' => $node->name))).t('New bulk lots for sale').'</h2>'.theme('uc_catalog_latest', 0, 'vareparti', '', $node->uid).'</div><br />';         
        print '<div class="lv-catalog-latest"><h2>'.lv_pages_more_link($node->path.'/lagersalg', t("View all !name's single items", array('!name' => $node->name))).t('New single items for sale').'</h2>'.theme('uc_catalog_latest', 0, 'consumer', '', $node->uid).'</div>';         
	    
   ?>
    

    
    

    
    
          <br /><br />

  <?php $links; ?>

