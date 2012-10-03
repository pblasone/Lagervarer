<?php
/**
 * @file
 * Auctionflex administration menu items.
 *
 */

/**
 * Generate the settings form for Auctionflex.
 */
function auctionflex_settings_form() {

  $form['auctionflex_buyer_premium'] = array(
  	'#type' => 'textfield',
	'#title' => t("Default buyer's premium % ex. vat"),	
	'#size' => 5,
	'#maxlength' => 5,
	'#default_value' => variable_get('auctionflex_buyer_premium', ''),
  );
  
  $form['auctionflex_min_bid'] = array(
  	'#type' => 'textfield',
	'#title' => t("Minimum bid and increment"),	
	'#size' => 5,
	'#maxlength' => 7,
	'#default_value' => variable_get('auctionflex_min_bid', ''),
  );  
  
  $form['auctionflex_location'] = array(
  	'#type' => 'textarea',
	'#cols' => 50,
	'#rows' => 3,
	'#title' => t("Default auction address"),	
	'#default_value' => variable_get('auctionflex_location', ''),
  );  
  
  $form['auctionflex_terms'] = array(
  	'#type' => 'textarea',
	'#cols' => 40,
	'#rows' => 10,
	'#title' => t("Default auction terms"),	
	'#default_value' => variable_get('auctionflex_terms', ''),
  );
  
  $form['auctionflex_disclaimer'] = array(
  	'#type' => 'textarea',
	'#cols' => 40,
	'#rows' => 3,
	'#title' => t("Bid disclaimer"),	
	'#default_value' => variable_get('auctionflex_disclaimer', ''),
  );    

  return system_settings_form($form);
}
