<?php
 
    if (isset($node->field_delivery_details) && empty($node->field_delivery_details[0]['value'])) {
		$account = user_load($node->uid);
        profile_load_profile($account);                        
        $delivery_details = $account->profile_delivery;    
    } else {
		$delivery_details = $node->field_delivery_details[0]['value'];    
    }    

?>

  <div>        
  <?php print $node->content['image']['#value']; ?>
	<div class="product-left">
	    	<div class="product-left-element">    
				<div class="item-description">
					<?php
	        	        if ($node->field_price_per_piece[0]['value']) {
							print '<img width="40" height="40" src="/sites/all/themes/zenlager/images/ikon_valgfritProdPage.png" /><div>' . t('Bulk lot with free choice of quantity. Price is specified per piece. Minimum quantity: <strong>!qty</strong> pcs.', array('!qty' => $node->default_qty));                
        	        	}
	            	    else {
							print '<img width="40" height="41" src="/sites/all/themes/zenlager/images/ikon_papkasseProdPage.png" /><div>' . t('Bulk lot containing <strong>!qty</strong> units. Price is for 1 bulk lot.', array('!qty' => $node->pkg_qty));                                
	    	            }
                        
        			?>    
                      </div>
				</div>        
			</div>                
		<?php if (isset($node->content['uc_auction'])) { ?>
	    	<div class="product-left-element">
            	<h3><img src="/sites/all/themes/zenlager/images/ikon_auktionProdPage.png" width="33" height="18" /><?php print t('Auction') ?></h3>
			    <?php print $node->content['uc_auction']['#value']; ?>
			</div>            
		<?php } 
              if ((isset($node->content['uc_auction']) && $node->uc_auction['buy_now'] && $node->uc_auction['expiry'] > time()) || !isset($node->content['uc_auction']) && $node->sell_price > 0) { ?>
			<div class="product-left-element"> 
            	<h3><img src="/sites/all/themes/zenlager/images/ikon_buynowProdPage.png" width="33" height="18" /><?php print t('Buy now') ?></h3>
                <?php print $node->content['add_to_cart']['#value']; ?></td>
            </div>        
        <?php }
			  if (isset($node->content['uc_bartering']['#value'])) { ?>
   			<div class="product-left-element"> 
            	<h3><img src="/sites/all/themes/zenlager/images/ikon_givetbudProdPage.png" width="33" height="18" /><?php print t('Price negotiation') ?></h3>            
        	<?php if (!isset($node->uc_bartering['first_bid'])) { ?>
			    <?php print $node->content['uc_bartering']['#value']; ?>
	        <?php } elseif ($node->uc_bartering['newest_bid']['status'] === 0) { ?>            	
				<div class="uc-bartering-info-pending"><?php print t('The seller has made you a new offer!').'<br />'.t("<a href=\"#bids\">Click here</a> to view the seller's offer") ?></div>
            <?php } else { ?>                            				            
				<div class="uc-bartering-info"><?php print t('You have previously bid on this item!').'<br />'.t("<a href=\"#bids\">Click here</a> to view your bidding history for this item.") ?></div>            
            <?php } ?>                                                                                                                                                                          
	        </div>        	                
        <?php }
        	  if ($node->partner_product) {            
	   			print '<div class="product-left-element"><div class="item-partnerlink">'.t('The items in this bulk lot can be purchased individually:').'<br />'.l(t('Click here to see per item prices'), 'node/'.$node->partner_product).'</div></div>';               
 			  } 
        ?>                                                                                                                                                                                        
              
              


    </div>
    
	<h2><?php print t('Description'); ?></h2>
	<div>

  <?php 
  	print $node->content['body']['#value'];
	
	if (count($node->files)) {
		echo theme_upload_attachments($node->files);
	}
	
	?>    

    </div>
	<br style="clear: both;" />
  </div>
   
       

	    	<div class="block">
            	<div class="block-inner">
                	<h2 class="title"><?php print t('Product details'); ?></h2>
                    <div class="content">
                    <div style="height: 10px;"></div>
					    <table class="product-details">
							<tr>
								<th><?php print t('Product condition'); ?></th>                            
                                <td><?php print $node->field_condition[0]['view'] ?></td>
                            </tr>                         
							<tr>
								<th><?php print t('Inventory status'); ?></th>                            
                                <td><?php print $node->field_delivery[0]['view'] ?></td>
                            </tr> 
						<?php if ($node->shippable == 1) { ?>
							<?php if ($node->weight > 0) { ?>
							<tr>
								<th><?php print t('Consigment weight'); ?></th>                            
                                <td><?php print $node->weight . ' ' . $node->weight_units ?></td>
                            </tr>                             
                            <?php
                            	}
                            	$dimensions = '';
                            	if ($node->length > 0) { $dimensions .= t(', Length: ') . $node->length . ' ' . $node->length_units; }
                            	if ($node->width > 0) { $dimensions .= t(', Width: ') . $node->width . ' ' . $node->length_units; }
                            	if ($node->height > 0) { $dimensions .= t(', Height: ') . $node->height . ' ' . $node->length_units; }                                                                
                                if ($dimensions <> '') { ?>
								<tr>
    	                        	<th><?php print t('Consignment dimensions'); ?></th>
        	                        <td><?php print substr($dimensions, 2); ?></td>                        
								</tr>
                            <?php } ?>                                    
							<tr>
								<th><?php print t('Terms of delivery'); ?></th>                            
       	                        <td valign="top"><?php print $delivery_details ?></td>
           	                </tr>                            
						<?php } ?>
                        </table>
					</div>                        
				</div>
			</div>       

	    	<div class="block">
            	<div class="block-inner">
                	<h2 class="title"><?php print t('Seller information'); ?></h2>
                    <div class="content">
			            <?php print _lv_profiles_user_info(0, TRUE) ?>
					</div>                        
				</div>
			</div>                   
            
        <?php if (isset($node->uc_bartering) && isset($node->uc_bartering['first_bid'])) { ?>
			<a name="bids"></a>
	    	<div class="block">
            	<div class="block-inner">
                	<h2 class="title"><?php print t('Bids'); ?></h2>
                    <div class="content">
                    <div style="height: 10px;"></div>
					    <?php print $node->content['uc_bartering']['#value']; ?>
					</div>                        
				</div>
			</div>            
        <?php } ?>            
    
                            <br />

                          <?php $links; ?>
               
    
<!--		<table>
			<tbody>
				<tr><td>?php print ?></td><td>?php print ?></td></tr>            
				<tr><td>?php print ?></td><td>?php print ?></td></tr>            
				<tr><td>?php print ?></td><td>?php print ?></td></tr>                                        
            </tbody>        
        </table>    -->
    
    
