<?php
// $Id: uc_catalog.pages.inc,v 1.1.2.8 2009/02/04 13:44:30 islandusurper Exp $

/**
 * @file
 * Catalog menu items.
 *
 */

/**
 * Display a formatted catalog page.
 *
 * If the category has products in it, display them in a TAPIr table. Subcategories
 *   are linked along the top of the page. If it does not have products, display
 *   subcategories in a grid with their images and subcategories.
 *
 * @param $tid
 *   Catalog term id from URL.
 * @return
 *   Formatted HTML of the catalog page.
 */
function theme_uc_catalog_browse($tid = 0, $base_path = '', $lv_type = 'vareparti', $sale_method = '', $account = 0, $promoted = false) {
  drupal_add_css(drupal_get_path('module', 'uc_catalog') .'/uc_catalog.css'); 
  global $user;
  
 	$uid = is_object($account) ? $account->uid : 0;
    $base_path = str_replace('[uid]', $uid, $base_path);
    $base_path = str_replace('[alias]', $account->alias, $base_path);    

  /* Make our own menu_local_task lookalikes */
	$path = drupal_get_path_alias($_GET['q']); 
    $path = str_replace('/auktioner', '', $path);
    $path = str_replace('/prisforhandling', '', $path);    
	$active_tab = array('' => '', 'auktioner' => '', 'prisforhandling' => '');
  	$active_tab[$sale_method] = ' class="active"';
	$tabs = '<div class="tabs"><ul class="tabs primary clear-block">';
    $tabs .= '<li'.$active_tab[''].'><a'.$active_tab[''].' href="/'.$path.'">'.t('All').'</a></li>';
    $tabs .= '<li'.$active_tab['auktioner'].'><a'.$active_tab['auktioner'].' href="/'.$path.'/auktioner">'.t('Auctions').'</a></li>';
    $tabs .= '<li'.$active_tab['prisforhandling'].'><a'.$active_tab['prisforhandling'].' href="/'.$path.'/prisforhandling">'.t('Negotiable price').'</a></li>';
    $tabs .= '</ul></div>';        
  
  
  $child_tids = array($tid);                 
  $types = array($lv_type);
  $output = '';
  $catalog = uc_catalog_get_page_mod((int)$tid, array($lv_type), $uid);
  drupal_set_title(check_plain($catalog->name));
  drupal_set_breadcrumb(uc_catalog_set_breadcrumb_mod($catalog->tid, $types, $base_path));
  $links = array();
  $child_list = array();
  $sql_args = array();
  foreach ($catalog->children as $child) {
	$child_tids[] = $child->tid;
    if (!empty($child->nodes)) {
      $links[] = array('title' => $child->name . (variable_get('uc_catalog_breadcrumb_nodecount', FALSE) ? ' ('. $child->nodes .')' : ''), 'href' => lv_term_get_path($child, $base_path),
        'attributes' => array('rel' => 'tag'),
      );
    }
    if (!empty($child->image)) {
      $image = '<div>';
      if (module_exists('imagecache')) {
        $image .= l(theme('imagecache', 'uc_category', $child->image['filepath']), lv_term_get_path($child, $base_path), array('html' => TRUE));
      }
      else {
        $image .= l(theme('image', $child->image['filepath']), lv_term_get_path($child, $base_path), array('html' => TRUE));
      }
      $image .= '</div>';
    }
    else {
      $image = '<div></div>';
    }
    $grandchildren = array();
    $j = 0;
    $max_gc_display = 3;
    foreach ($child->children as $i => $grandchild) {
      if ($j > $max_gc_display) {
        break;
      }
      $g_child_nodes = 0;
      foreach ($types as $type) {
        $g_child_nodes += taxonomy_term_count_nodes_mod($grandchild->tid, $type);
      }
      if ($g_child_nodes) {
        $grandchildren[$i] = l($grandchild->name, lv_term_get_path($grandchild, $base_path), array('class' => 'subcategory'));
        $j++;
      }
    }
    //$grandchildren = array_slice($grandchildren, 0, intval(count($grandchildren) / 2) + 1, TRUE);
    if ($j > $max_gc_display) {
      array_push($grandchildren, l(t('More...'), lv_term_get_path($child, $base_path), array('class' => 'subcategory')));
    }
    if ($child->nodes) {
      $cell_link = $image .'<strong>'. l($child->name, lv_term_get_path($child, $base_path)) .'</strong>';
      if (variable_get('uc_catalog_show_subcategories', TRUE)) {
        $cell_link .= "<br/><span>". implode(', ', $grandchildren) ."</span>\n";
      }
      $child_list[] = $cell_link;
    }
  }
  if (!empty($catalog->image)) {
    $output .= theme('imagecache', 'uc_thumbnail', $catalog->image['filepath'], $catalog->name, $catalog->name, array('class' => 'category'));
  }

  // Build an ORDER BY clause for the SELECT query based on table sort info.
/*  if (empty($_REQUEST['order'])) {
    $order = 'ORDER BY p.ordering, n.title, n.nid';
  }
  else {
    $order = tapirsort_sql(uc_product_table_header());
  }  */
  
  $sql_join = '';

  foreach ($types as $type) {
    $sql_args[] = $type;
  }
    
  $sql_where = '';  
  if (is_object($account)) {
	$sql_where = ' AND n.uid = %d';  
	$sql_args[] = $account->uid;
  }
  if ($user->uid == 0 || $account->uid != $user->uid) {
	$sql_where .= ' AND n.status = 1';
  } 
  		   
  if ($query = lv_search_get_query()) {	 
	  lv_search_prepare_sql($query, $lv_type, $sql_where, $sql_args, $sql_join);
  }
  else {
  	if ($tid > 0) {
    	$sql_where .= ' AND tn.tid IN ('.db_placeholders($child_tids) .')';
		foreach ($child_tids as $child_tid) {
	    	$sql_args[] = $child_tid;
		}
	}
	
	if ($sale_method == 'auktioner') {
		$sql_where .= ' AND (uca.nid = n.nid AND uca.expiry > UNIX_TIMESTAMP())';
	}                                                                 
	elseif ($sale_method == 'prisforhandling') {
		$sql_where .= ' AND (ucb.nid = n.nid AND (uca.nid IS NULL OR uca.expiry < UNIX_TIMESTAMP()))';    		      	
	}			
  }
  
  if ($promoted) $sql_where .= ' AND n.promote = 1';

  																										
  $sql = "SELECT DISTINCT(n.nid), n.sticky, n.title, n.created, p.model, p.sell_price, p.ordering, p.pkg_qty
    FROM {node} n
      INNER JOIN {term_node} tn ON n.vid = tn.vid
      INNER JOIN {uc_products} AS p ON n.vid = p.vid 
	  LEFT OUTER JOIN {uc_bartering} ucb ON n.nid = ucb.nid 
	  LEFT OUTER JOIN {uc_auction} uca ON n.nid = uca.nid 
	  LEFT OUTER JOIN {uc_auction_now} ucn ON n.nid = ucn.nid "	  
      . $sql_join .
    "WHERE n.type IN (". db_placeholders($types, 'varchar') .")".$sql_where .
      " ORDER BY n.created DESC";      

      $sql_count = "SELECT COUNT(DISTINCT(n.nid))
        FROM {node} n
          INNER JOIN {term_node} tn ON n.vid = tn.vid
      INNER JOIN {uc_products} AS p ON n.vid = p.vid 
	  LEFT OUTER JOIN {uc_bartering} ucb ON n.nid = ucb.nid 
	  LEFT OUTER JOIN {uc_auction} uca ON n.nid = uca.nid 
	  LEFT OUTER JOIN {uc_auction_now} ucn ON n.nid = ucn.nid "
      . $sql_join .
    "WHERE n.type IN (". db_placeholders($types, 'varchar') .")".$sql_where;          

  $sql = db_rewrite_sql($sql);
  $sql_count = db_rewrite_sql($sql_count);
  $catalog->products = array();
  $result = pager_query($sql, variable_get('uc_product_nodes_per_page', 12), 0, $sql_count, $sql_args);
  while ($node = db_fetch_object($result)) {
    $catalog->products[] = $node->nid;  
  }

//  if (count($catalog->products)) {
    if (count($links) && ($uid > 0 || $tid > 0)) {
      $output .= theme('links', $links, array('class' => 'links inline')) ."<br />\n";
    }
	
	$squeeze = $account > 0 ? TRUE : FALSE;
	
	if (!$promoted) {
	    $output .= $catalog->description;				
		$output .= drupal_get_form('lv_search_search_form', $lv_type, $tid, $uid, $squeeze);	
		$output .= $tabs;  
	}		
    $output .= uc_catalog_products_mod($catalog->products, TRUE, $squeeze);
    $output .= theme('pager');
 /* }
  else {
    // Display table of child categories similar to an osCommerce site's front page.
    $columns = variable_get('uc_catalog_category_columns', 3);
    $cat_rows = array();
    $row = array();
    $i = 1;
    foreach ($child_list as $cell) {
      $row[] = array('data' => $cell, 'class' => 'category');
      if ($i % $columns == 0) {
        $cat_rows[] = $row;
        $row = array();
      }
      $i++;
    }
    if (count($row) > 0 && count($row) < $columns) {
      if (count($cat_rows) >= 1) {
        $row = array_merge($row, array_fill(count($row), $columns - count($row), array('data' => '&nbsp;', 'class' => 'category')));
      }
      $cat_rows[] = $row;
    }
    $output .= $catalog->description;
    $output .= theme('table', array(), $cat_rows, array('class' => 'category'));
  }    */

  return $output;
}


// + FUNCTION (Copy of theme_uc_catalog_products() in uc_catalog.module
/**
 * Display a formatted list of products.
 *
 * @param $products
 *   An array of product nids.
 * @return
 *   A TAPIr table.
 * @ingroup themeable
 */
function uc_catalog_products_mod($products, $show_header = TRUE, $squeeze = FALSE) {
  if (!$products) {
    $output .= '<div class="no-products">'. t('No products are available in this category.') .'</div>';
    return $output;
  }
  else {
    if (variable_get('uc_catalog_grid_display', FALSE)) {
      return theme('uc_catalog_product_grid', $products);
    }
    else {
      return uc_catalog_product_table($products, $show_header, $squeeze);
    }
  }
}       


function theme_uc_catalog_latest($tid = 0, $lv_type = 'vareparti', $sale_method = '', $account = 0, $limit = 10) {

  $types = array($lv_type);
  $sql_args = array();
  foreach ($types as $type) {
   	$sql_args[] = $type;
  }  

  if ($tid > 0) {
	  $child_tids = $tid;                
	  $catalog = uc_catalog_get_page_mod((int)$tid);
	  foreach ($catalog->children as $child) {
	  	$child_tids .= ', '.$child->tid; 
	  }  
  }	      

  $sql_join = '';
  if ($sale_method == 'auktioner') {
	$sql_join = 'INNER JOIN {uc_auction} AS uca ON n.nid = uca.nid ';
  }                                                                 
  elseif ($sale_method == 'prisforhandling') {
	$sql_join = 'INNER JOIN {uc_bartering} AS ucb ON n.nid = ucb.nid ';  
  }

  $sql_where = '';  
  if ($child_tids) {
	$sql_where = " AND tn.tid IN ('".$child_tids."')";  
  }  
  if ($account > 0) {
	$sql_where = ' AND n.uid = '.$account;  
  }        
  
  
  $sql = "SELECT DISTINCT(n.nid), n.sticky, n.title, n.created, p.model, p.sell_price, p.ordering
    FROM {node} n
      INNER JOIN {term_node} tn ON n.vid = tn.vid
      INNER JOIN {uc_products} AS p ON n.vid = p.vid "
      . $sql_join .
    "WHERE n.status = 1 AND n.promote = 0".$sql_where .
      " AND n.type IN (". db_placeholders($types, 'varchar') .") ORDER BY n.created DESC LIMIT 0,".$limit;      
      
  $products = array();
  $result = db_query($sql, $sql_args);
  while ($node = db_fetch_object($result)) {
    $products[] = $node->nid;  
  }
  
  return uc_catalog_products_mod($products, FALSE, TRUE);   
  

}


// + FUNCTION (Copy of uc_product_table and uc_product_table_header in uc_product.module
/**
 * Display product fields in a TAPIr table.
 *
 * Display image, name, price, and add to cart form.
 */
function uc_catalog_product_table($args = array(), $show_header = TRUE, $squeeze = FALSE) {
  $time = time();
  $rows = array();
  foreach ($args as $nid) {
	$node = node_load($nid);
    $data = array();
   
      if (module_exists('imagecache')) {
        if (($field = variable_get('uc_image_'. $node->type, '')) && isset($node->$field) && file_exists($node->{$field}[0]['filepath'])) {
          $image = $node->{$field}[0];
          $cell = array('data' => l(theme('imagecache', 'product_list', $image['filepath'], $image['data']['alt'], $image['data']['title']) . theme('imagecache', 'hoverbox', $image['filepath'], '', '', array('class' => 'preview')), 'node/'. $node->nid, array('html' => TRUE)), 'class' => 'product-image hoverbox');
        }
        else {
          $cell = array('data' => t('n/a'), 'class' => 'product-image');
        }
      }               
      if ($node->type == 'vareparti') { $cell['rowspan'] = 2; }
      $data[] = $cell;

    $cell = array(
      'data' => l($node->title, 'node/'. $node->nid),
      'class' => 'product-name',
      'width' => '100%',
    );
    if ($node->type == 'vareparti') { $cell['class'] .= ' name-lot'; }
    $data[] = $cell;
    
    $price = '';                              
    $tr_class = 'product-fixed-price';
	if (isset($node->uc_auction)) {
    	$tr_class = 'product-auction';
        $hb_title = (int)$node->uc_auction['bid_count'] ? t('Highest bid') : t('Starting price');
    	$price = '<div class="price-container"><div class="label"><img src="/sites/all/themes/zenlager/images/ikon_auktion.png" alt="'.t('This item is on auction.').'" />'.t('Auction').'</div>'.t('!count bids', array('!count' => $node->uc_auction['bid_count'])).'</div>
        	      <div class="auction-container"><div class="highbid"><div class="auction-label">'.$hb_title.'</div>'.uc_currency_format($node->uc_auction['high_bid_amt']).'</div>';
		if ($node->uc_auction['expiry'] < $time) {
        	$price .= '<div class="timeleft expired">'.t('Auction ended').'</div>';
		}
        elseif ($node->uc_auction['expiry'] < strtotime("+1 hour", $time)) {
			$price .= '<div class="timeleft expiring"><div class="auction-label">'.t('Expires').'</div>'.format_interval($node->uc_auction['expiry'] - $time).'</div>';				                       
        }
        else {
			$price .= '<div class="timeleft"><div class="auction-label">'.t('Expires').'</div>'.format_interval($node->uc_auction['expiry'] - $time).'</div>';				                               
        }            
        $price .= '<br /></div>';   
		if ($node->sell_price > 0 && $node->uc_auction['buy_now']) {
			$price .= '<div class="price-container"><div class="label"><img src="/sites/all/themes/zenlager/images/ikon_buynow.png" alt="'.t('You can buy this item at a fixed price.').'" />'.t('Buy Now').'</div>'.uc_currency_format($node->display_price).'</div>';
        }

    } 
    elseif (isset($node->uc_bartering)) {
    	$tr_class = 'product-bartering';    
		$price = '<div class="price-container"><div class="label"><img src="/sites/all/themes/zenlager/images/ikon_givetbud.png" alt="'.t('You can negotiate a price for this item.').'" />'.t('Place a bid').'</div>'.t('!count bids', array('!count' => $node->uc_bartering['bid_count'])).'</div>';
		if ($node->sell_price > 0) {
			$price .= '<div class="price-container"><div class="label"><img src="/sites/all/themes/zenlager/images/ikon_buynow.png" alt="'.t('You can buy this item at a fixed price.').'" />'.t('Buy Now').'</div>'.uc_currency_format($node->display_price).'</div>';
        }
    }
    else {
		$price = theme('uc_product_price', $node->display_price, 'sell-price', TRUE);    
    }

    $cell = array(
      'data' => $price,
      'class' => 'product-price',
      'valign' => 'top',
    );   
    if ($node->type == 'vareparti' && !$squeeze) { $cell['rowspan'] = 2; }
    $data[] = $cell;

    $rows[] = array('data' => $data, 'class' => $tr_class);
    if ($node->type == 'vareparti') {
		if ($node->field_price_per_piece[0]['value']) {
			$inforow = '<div><img align="absmiddle" src="/sites/all/themes/zenlager/images/ikonSingle20.gif" /> ' . t('Free quantity, price is per item.');
			if ($node->use_stock) {
				 $inforow .= t(' Max. qty: <strong>@antal</strong> pcs.', array('@antal' => number_format($node->stock_level, 0, ',', '.')));			
			}
		} elseif ($node->use_stock && (int)$node->stock_level === 1) {
			$inforow = '<div><img align="absmiddle" src="/sites/all/themes/zenlager/images/ikonLot20.gif" /> ' . t('One lot of <strong>@antal</strong> pcs. Price is for the whole lot.', array('@antal' => number_format($node->pkg_qty, 0, ',', '.')));
		} else {
			$inforow = '<div><img align="absmiddle" src="/sites/all/themes/zenlager/images/ikonLot20.gif" /> ' . t('Price per lot of <strong>@antal</strong> pcs.', array('@antal' => number_format($node->pkg_qty, 0, ',', '.')));
			if ($node->use_stock) {
				$inforow .= t(' <strong>@antal</strong> lots available.', array('@antal' => number_format($node->stock_level, 0, ',', '.')));			
			}
		}
		$colspan = $squeeze ? 2 : 1;
	    $rows[] = array('data' => array(array('data' => $inforow.'</div>', 'class' => 'lot-info', 'colspan' => $colspan)), 'class' => 'lot-info');
		
	}        
  }

  if (count($args) == 0) {
    $rows[] = array(
        'data' => t('No products available.'),
        'attributes' => array(
          'colspan' => 'full',
        ),
    );
  }
 
  if ($show_header) {
	  $header = array(t('Image'), t('Name'), t('Price'));
  }      
  
  return theme('table', $header, $rows, array('class' => 'category-products'));
}            

// + FUNCTION (Copy of uc_catalog_get_page
// We have to copy it in here to modify loading of child nodes to include grand children in the count and to filter by i.e. username
/**
 * Load catalog information for display.
 *
 * Retrieve image, product, and subcategory information for the current term id.
 *
 * @param $tid
 *   Taxonomy term id.
 * @return
 *   A catalog object containing all the information needed to display a catalog page.
 */
function uc_catalog_get_page_mod($tid, $types = array(), $uid = 0) {
  $catalog = new stdClass();
  $vid = variable_get('uc_catalog_vid', 0);

  if ($tid) {
    $term = taxonomy_get_term($tid);
    $name = $term->name;
    $description = $term->description;
  }
  else {
    $tid = 0;
    $name = variable_get('uc_catalog_name', t('Catalog'));
    $description = variable_get('uc_catalog_description', '');
  }
  $catalog->tid = $tid;
  $catalog->vid = $vid;
  $catalog->name = $name;
  $catalog->description = $description;
  $catalog->children = array();
  if ($file = uc_catalog_image_load($catalog->tid)) {
    if (module_exists('imagecache')) {
      $file_path =  file_create_url(file_directory_path() .'/imagecache/uc_category/'. $file->filepath);
    }
    else {
      $file_path = $file->filepath;
    }
    $info = image_get_info($file_path);
    $catalog->image = $info;
    $catalog->image['filepath'] = $file->filepath;
  }
  if (!count($types)) {
	  $types = uc_product_types(); 
  }            

  $links = array();
  $child_list = array();
  $children = taxonomy_get_children($tid, $vid);
  foreach ($children as $child) {
    $n = 0;
    foreach ($types as $type) {
      $n += taxonomy_term_count_nodes_mod($child->tid, $type, $uid);
    }
    $child->nodes = $n;
    
    // Display child category's image.
    if (module_exists('imagecache') && $file = uc_catalog_image_load($child->tid)) {
      $imagecache_path =  file_create_url(file_directory_path() .'/imagecache/uc_category/'. $file->filepath);
      $info = image_get_info($imagecache_path);
      $child->image = $info;
      $child->image['filepath'] = $file->filepath;
    }
    // Display list of child category's children categories.
    // If more than $max_gc_display, show "More..." link to child.
    $grandchildren_list = taxonomy_get_children($child->tid, $vid);
    $child->children = $grandchildren_list;
    $catalog->children[] = $child;
  }
  //$node_resource = taxonomy_select_nodes(array($tid));
  return $catalog;
}   


// + FUNCTION copy of taxonomy_term_count_nodes
/**
 * Count the number of published nodes classified by a term.
 *
 * @param $tid
 *   The term's ID
 *
 * @param $type
 *   The $node->type. If given, taxonomy_term_count_nodes only counts
 *   nodes of $type that are classified with the term $tid.
 *
 * @return int
 *   An integer representing a number of nodes.
 *   Results are statically cached.
 */
function taxonomy_term_count_nodes_mod($tid, $type = 0, $uid = 0) {
  static $count;  

  $sql_where = '';  
  if ($uid > 0) {
  	$sql_where = ' AND n.uid = '.$uid;
  }

  if (!isset($count[$type])) {
    // $type == 0 always evaluates TRUE if $type is a string
    if (is_numeric($type)) {
      $result = db_query(db_rewrite_sql('SELECT t.tid, COUNT(n.nid) AS c FROM {term_node} t INNER JOIN {node} n ON t.vid = n.vid WHERE n.status = 1'.$sql_where.' GROUP BY t.tid'));
    }
    else {
      $result = db_query(db_rewrite_sql("SELECT t.tid, COUNT(n.nid) AS c FROM {term_node} t INNER JOIN {node} n ON t.vid = n.vid WHERE n.status = 1".$sql_where." AND n.type = '%s' GROUP BY t.tid"), $type);
    }
    $count[$type] = array();
    while ($term = db_fetch_object($result)) {
      $count[$type][$term->tid] = $term->c;
    }
  }
  $children_count = 0;
  foreach (_taxonomy_term_children($tid) as $c) {
    $children_count += taxonomy_term_count_nodes_mod($c, $type, $uid);
  }
  return $children_count + (isset($count[$type][$tid]) ? $count[$type][$tid] : 0);
}     

// + FUNCTION Copy of uc_catalog_set_breadcrumb()
/**
 * Formats the breadcrumb to the current term's ancestry.
 *
 * @param $tid
 *   The currently viewed catalog term's id.
 * @param $is_node
 *   If true, include the current category page in the breadcrumb.
 * @return
 *   An array of breadcrumb links.
 */
function uc_catalog_set_breadcrumb_mod($tid, $types = array('vareparti'), $path = '', $include_term = FALSE) {
  static $breadcrumbs = array();
  static $terms = array();
  
  if ($path == '') { $path = 'catalog'; }
  $path_parts = explode('/', $path);                               

  $names = array('vareparti' => t('Bulk lots'), 'consumer' => t('Warehouse Sale'));
      
  if (variable_get('uc_catalog_breadcrumb', TRUE)) {
    if (empty($breadcrumbs)) {
      if (variable_get('site_frontpage', 'node') != 'catalog') {
        $breadcrumbs[] = l(t('Home'), '');
      }  
      if ($path_parts[0] == 'brugere') {
      	$account = lv_user_alias_load($path_parts[1]);
        
		$breadcrumbs[] = l(t('Community'), 'forum');
        $breadcrumbs[] = l(t('Users'), 'brugere');
        $breadcrumbs[] = l($account->name, 'brugere/'.$path_parts[1]);
      }                                 
      elseif ($path_parts[0] == 'user') {
		$breadcrumbs[] = l(t('Account'), 'user');		      
      }      
      if ($tid != 0) {
        $breadcrumbs[] = l($names[$types[0]], $path);
      }
    }
    $parents = taxonomy_get_parents_all($tid);

	if (!$include_term) {
	    // Remove current term from the breadcrumb.
    	array_shift($parents);
	}        

    while (count($parents)) {
      $current = array_pop($parents);
      if (!in_array($current->tid, $terms)) {
        $breadcrumbs[] = l($current->name, lv_term_get_path($current, $path));
        $terms[] = $current->tid;
      }
    }
 
    return $breadcrumbs;
  }
  else {
    return NULL;
  }
}
      


