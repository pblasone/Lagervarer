<?php
// $Id: template.php,v 1.17 2008/09/11 10:52:53 johnalbin Exp $

/**
 * @file
 * Contains theme override functions and preprocess functions for the theme.
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 *   The template.php file is one of the most useful files when creating or
 *   modifying Drupal themes. You can add new regions for block content, modify
 *   or override Drupal's theme functions, intercept or make additional
 *   variables available to your theme, and create custom PHP logic. For more
 *   information, please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/theme-guide
 *
 * OVERRIDING THEME FUNCTIONS
 *
 *   The Drupal theme system uses special theme functions to generate HTML
 *   output automatically. Often we wish to customize this HTML output. To do
 *   this, we have to override the theme function. You have to first find the
 *   theme function that generates the output, and then "catch" it and modify it
 *   here. The easiest way to do it is to copy the original function in its
 *   entirety and paste it here, changing the prefix from theme_ to zenlager_.
 *   For example:
 *
 *     original: theme_breadcrumb()
 *     theme override: zenlager_breadcrumb()
 *
 *   where zenlager is the name of your sub-theme. For example, the
 *   zen_classic theme would define a zen_classic_breadcrumb() function.
 *
 *   If you would like to override any of the theme functions used in Zen core,
 *   you should first look at how Zen core implements those functions:
 *     theme_breadcrumbs()      in zen/template.php
 *     theme_menu_item_link()   in zen/template.php
 *     theme_menu_local_tasks() in zen/template.php
 *
 *   For more information, please visit the Theme Developer's Guide on
 *   Drupal.org: http://drupal.org/node/173880
 *
 * CREATE OR MODIFY VARIABLES FOR YOUR THEME
 *
 *   Each tpl.php template file has several variables which hold various pieces
 *   of content. You can modify those variables (or add new ones) before they
 *   are used in the template files by using preprocess functions.
 *
 *   This makes THEME_preprocess_HOOK() functions the most powerful functions
 *   available to themers.
 *
 *   It works by having one preprocess function for each template file or its
 *   derivatives (called template suggestions). For example:
 *     THEME_preprocess_page    alters the variables for page.tpl.php
 *     THEME_preprocess_node    alters the variables for node.tpl.php or
 *                              for node-forum.tpl.php
 *     THEME_preprocess_comment alters the variables for comment.tpl.php
 *     THEME_preprocess_block   alters the variables for block.tpl.php
 *
 *   For more information on preprocess functions, please visit the Theme
 *   Developer's Guide on Drupal.org: http://drupal.org/node/223430
 *   For more information on template suggestions, please visit the Theme
 *   Developer's Guide on Drupal.org: http://drupal.org/node/223440 and
 *   http://drupal.org/node/190815#template-suggestions
 */


/*
 * Add any conditional stylesheets you will need for this sub-theme.
 *
 * To add stylesheets that ALWAYS need to be included, you should add them to
 * your .info file instead. Only use this section if you are including
 * stylesheets based on certain conditions.
 */
/* -- Delete this line if you want to use and modify this code
// Example: optionally add a fixed width CSS file.
if (theme_get_setting('zenlager_fixed')) {
  drupal_add_css(path_to_theme() . '/layout-fixed.css', 'theme', 'all');
}
// */


/**
 * Implementation of HOOK_theme().
 */
function zenlager_theme(&$existing, $type, $theme, $path) {
  $hooks = zen_theme($existing, $type, $theme, $path);
  // Add your theme hooks like this:
  /*
  $hooks['hook_name_here'] = array( // Details go here );
  */
  // @TODO: Needs detailed comments. Patches welcome!
  return $hooks;
}

/**
 * Override or insert variables into all templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered (name of the .tpl.php file.)
 */
/* -- Delete this line if you want to use this function
function zenlager_preprocess(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
function zenlager_preprocess_page(&$vars, $hook) {
  global $user;
  $vars['username'] = $user->name;

  if (in_array('ikke-godkendt', array_values($user->roles))) {
      if (isset($_GET['conf']))
        drupal_goto($path = 'user');
	drupal_set_message(t("Your account has not yet been activated, and therefore many features will be blocked. Please look for our validation e-mail in your inbox and follow the instructions to activate your account.<br /><br />If you haven't received an e-mail from us, please add 'support@!siteurl' to your contact list and <a href=\"/toboggan/revalidate/!user\">click here to re-send the activation e-mail</a>.", array('!siteurl' => 'lagervarer.dk', '!user' => $user->uid)), 'error');
  }
  
  if ($result = db_result(db_query("SELECT dst FROM {url_alias} WHERE src = '%s'", $_GET['q']))) { 
  	$path = $result;
  } else {       
	$path = $_GET['q'];  
  }

  $path = explode('/', $path);    
  if ($path[0] == 'user' && $path[2] && !substr_count($vars['breadcrumb'], 'href="/user"')) {
  	$bread_parts = explode(' › ', $vars['breadcrumb']);
	array_splice($bread_parts,1,0,l(t('Account'), 'user'));    
	$vars['breadcrumb'] = implode(' › ', $bread_parts);  
  }

  switch ($path[0]) {
    	case 'varepartier': case 'vareparti': case 'custom-front-page':
	   	$vars['site_section'] = 'varepartier';
        break;  
	case 'lagersalg': case 'vare':
	   	$vars['site_section'] = 'lagersalg';  
        break;
	case 'forum': case 'brugere': case 'relationships': case 'group': case 'kontakter': case 'beskeder': case 'community':
	   	$vars['site_section'] = 'forum';  
        break;        
	case 'artikler':
	   	$vars['site_section'] = 'artikler';  
        break;                
	case 'information':
	   	$vars['site_section'] = 'information';  
        break;                
	case 'node': case 'user': case 'cart':
	   	$vars['site_section'] = 'user';  
        break;                
	default:
       	$vars['site_section'] = '';  
  }            
}

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
function zenlager_preprocess_node(&$vars, $hook) {
	if ($vars['type'] != 'story' && $vars['type'] != 'blog') { 
    	$vars['submitted'] = ''; 
        $vars['terms'] = '';
    }                    
    
    if ($vars['type'] == 'profile' && !$vars['teaser']) {
		$breadcrumb = array(
        	l(t('Home'), ''),
            l(t('Community'), 'forum'),
            l(t('Users'), 'brugere'),
		);
        drupal_set_breadcrumb($breadcrumb);                     
    }
	elseif ($vars['type'] == 'consumer' && !$vars['teaser']) {
    	require_once(drupal_get_path('module', 'uc_catalog').'/uc_catalog.pages.inc');
        $term = array_shift($vars['node']->taxonomy);
    	$path = explode('/', $vars['path']);                                          
        switch ($path[0]) {
        	case 'vare':
                $breadcrumb = uc_catalog_set_breadcrumb_mod($term->tid, array('consumer'), 'lagersalg', TRUE);
	            break;
		}
        drupal_set_breadcrumb($breadcrumb);
	}        
}             


/**
 * Allow themable wrapping of all comments.
 */
function phptemplate_comment_wrapper($content, $node) {
  if (!$content || $node->type == 'forum') {
    return '<div id="comments">'. $content .'</div>';
  }
  else { 
	if ($node->type == 'vareparti' || $node->type == 'consumer') {
		$caption = t('Questions for the seller');
	} else {
		$caption = t('Comments');
    }        
	if ($node->comment_count > 0) {
    	return '<div id="comments"><h2 class="comments">'. $caption .'</h2>'. $content .'</div>';
	} else {
    	return '<div id="comments">'. $content .'</div>';    
    }        
  }
}

/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
function zenlager_preprocess_comment(&$vars, $hook) {
	$node = content_profile_load('profile', $vars['comment']->uid);

	if (isset($node->field_avatar[0])) {
		$vars['comment']->picture = theme('imagecache', 'thumb50', $node->field_avatar[0]['filepath']);
	} else {
		$vars['comment']->picture = '<img src="/sites/all/themes/zenlager/images/avatar-none-50.png" width="50" height="50" />';    
    }        
}


/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function zenlager_preprocess_block(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */
							 

// SOME UTILITY FUNCTIONS...

/**
 * Format a phone number correctly according to country.
 */
function lv_format_phone($number, $country_code = 'dk') {

	$result = $number;
	$number = lv_clean_number($number);	

	switch ($country_code) {
		case 'dk': case 'no':
			if (strlen($number) == 8) {
				$result = substr($number, 0, 2) . ' ' . substr($number, 2, 2) . ' ' . substr($number, 4, 2) . ' ' . substr($number, 6, 2);
			}
			break;
		case 'uk': case 'gb':
			$country_code = 'uk';

			if (strlen($number == 11)) {
				if (substr($number, 0, 3) == '020') {
					$result = '(' . substr($number, 0, 3) . ') ' . substr($number, 3, 4) . ' ' . substr($number, 7);
				}
				else if (substr($number, 0, 3) == '011') {
					$result = '(' . substr($number, 0, 4) . ') ' . substr($number, 4, 3) . ' ' . substr($number, 7);
				}	
				else {
					$result = '(' . substr($number, 0, 5) . ') ' . substr($number, 5);		
				}
			}

	}

	return $country_code == 'dk' ? $result : '+' . ml_phone_prefix($country) . ' | ' . $result;
}

function lv_clean_number($string, $concat = true) {
    $length = strlen($string);    
    for ($i = 0, $int = '', $concat_flag = true; $i < $length; $i++) {
        if (is_numeric($string[$i]) && $concat_flag) {
            $int .= $string[$i];
        } elseif(!$concat && $concat_flag && strlen($int) > 0) {
            $concat_flag = false;
        }        
    }
    
    return (int) $int;
}


function lv_phone_prefix($country_code) {
	$prefixes = array(
		'dk' => 45,
		'se' => 46,
		'no' => 47,
		'gb' => 44,
		'uk' => 44,
		'th' => 66,
	);	
	if (isset($prefixes[$country_code])) {
		return '+ ' . $prefixes[$country_code] . ' | ';
	} else {
		return false;
	}		
				
}

function lv_city_postcode($city, $postcode, $country_code = 'dk') {
	$formats = array(
		'dk' => '[[postcode]] [[city]]',
		'se' => '[[postcode]] [[city]]',
		'no' => '[[postcode]] [[city]]',
		'gb' => '[[city]] [[postcode]]',
		'uk' => '[[city]] [[postcode]]',
		'th' => '[[city]] [[postcode]]',
	);
	$format = isset($formats[$country_code]) ? $formats[$country_code] : '[[city]] [[postcode]]';	

	$format = str_replace('[[postcode]]', $postcode, $format);		
	$format = str_replace('[[city]]', $city, $format);	
	
	return $format;
}