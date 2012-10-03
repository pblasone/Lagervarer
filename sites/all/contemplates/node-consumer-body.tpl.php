<?php
 
    if (isset($node->field_delivery_details) && empty($node->field_delivery_details[0]['value'])) {
		$account = user_load($node->uid);
        profile_load_profile($account);                        
        $delivery_details = $account->profile_delivery;    
    } else {
		$delivery_details = $node->field_delivery_details[0]['value'];    
    }    
 
//  dprint_r($node->content);
?>

  <div>        
  <?php print $node->content['image']['#value']; ?>
	<div class="product-left">
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
        <?php } ?>            

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
								<th><?php print t('Inventory status'); ?></th>                            
                                <td><?php print $node->field_delivery[0]['view'] ?></td>
                            </tr> 
						<?php if ($node->shippable == 1) { ?>
							<?php if ($node->weight > 0) { ?>
							<tr>
								<th><?php print t('Consigment weight'); ?></th>                            
                                <td><?php print $node->weight . ' ' . $node->weight_units ?></td>
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
    
    
