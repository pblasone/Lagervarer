<?php
// $Id: custom_breadcrumbs_taxonomy.inc,v 1.1.2.7 2009/07/20 22:37:32 mgn Exp $

/**
 * @file
 * Helper functions for custom_breadcrumbs_taxonomy.
 */

/**
 * Set a Taxonomy breadcrumb and call the original taxonomy/term/% callback.
 *
 * @param $callback
 *   A callback function as defined in hook_menu().
 * @param $file
 *   A callback file as defined in hook_menu().
 * @param $filepath
 *   A callback file path as defined in hook_menu().
 * @param $str_tids
 *   A term selector string, e.g. "1,3,8" or "4+9".
 * @param ...
 *   Additional arguments to pass on to the callback.
 *
 * @ return
 *   The return value of the original callback function.
 */
function custom_breadcrumbs_taxonomy_term_page() {
  $args = func_get_args();
  $callback = array_shift($args);
  $file = array_shift($args);
  $filepath = array_shift($args);
  $str_tids = array_shift($args);
  // Use first term to generate breadcrumb trail.
  $tids = taxonomy_terms_parse_string($str_tids);
  $terms = array();
  foreach ($tids['tids'] as $tid) {
    $term = taxonomy_get_term($tid);
    if ($term) {
      $terms[$term->tid] = $term;
    }
  }
  require_once($filepath .'/'. $file);
  $output = call_user_func_array($callback, $args);
  _custom_breadcrumbs_taxonomy_set_breadcrumb($tids['tids'][0], NULL, TRUE, array(), $terms);

  return $output;
}
/**
 *  Helper function to set the breadcrumb using a node's taxonomy.
 */
function _custom_breadcrumbs_taxonomy_set_breadcrumb($tid, $vid = NULL, $is_term_page = FALSE, $objs = array(), $terms = array()) {
  if (variable_get('custom_breadcrumbs_taxonomy_use_hierarchy', TRUE)) {
    $breadcrumb = custom_breadcrumbs_taxonomy_generate_breadcrumb($tid, $vid, $is_term_page, $objs, $terms);
    if (variable_get('custom_breadcrumbs_taxonomy_include_node_title', FALSE) && isset($objs['node'])) {
      $breadcrumb[] = $objs['node']->title;
    }
    drupal_set_breadcrumb($breadcrumb);
  }
  else {
    global $language;
    $languages = array('language' => $language->language, 'all' => '');
    // Check each term to see if it has a custom breadcrumb.
    $vids = array();
    if (!empty($terms)) {
      foreach ($terms as $term) {
        $breadcrumbs = custom_breadcrumbs_load_breadcrumbs('custom_breadcrumbs_taxonomy', 'custom_breadcrumbs_taxonomy_term', array('tid' => $term->tid), $languages);
        $objs['taxonomy'] = $term;
        if ($breadcrumb = custom_breadcrumbs_select_breadcrumb($breadcrumbs, $objs)) {
          custom_breadcrumbs_set_breadcrumb($breadcrumb, $objs);
          return;
        }
        if (!isset($vids[$term->vid])) {
          $vids[$term->vid] = $term;
        }
      }
    }
    if (empty($vids) && !is_null($vid)) {
      $vids[$vid] = NULL;
    }
    // No terms match, look for a match on the taxonomy vocabulary.
    foreach ($vids as $vid => $term) {
      $breadcrumbs = custom_breadcrumbs_load_breadcrumbs('custom_breadcrumbs_taxonomy', 'custom_breadcrumbs_taxonomy_vocabulary', array('vid' => $vid), $languages);
      $objs['taxonomy'] = $term;
      if ($breadcrumb = custom_breadcrumbs_select_breadcrumb($breadcrumbs, $objs)) {
        custom_breadcrumbs_set_breadcrumb($breadcrumb, $objs);
        return;
      }
    }
  }
}

/**
 * Return lightest term for given node ($nid).
 * Similar to taxonomy_node_get_terms, but only return the lightest term in the
 * lightest vocab for the node.
 */
function custom_breadcrumbs_taxonomy_node_get_lightest_term($nid) {

  /* We only want the first row of the result--this is the lightest term of the
   * lightest vocab.
   */
  $result = db_query(db_rewrite_sql('SELECT t.* FROM {term_node} r INNER JOIN {term_data} t ON r.tid = t.tid INNER JOIN {term_hierarchy} h ON t.tid = h.tid INNER JOIN {vocabulary} v ON t.vid = v.vid WHERE r.nid = %d ORDER BY v.weight, h.parent DESC, t.weight, t.name', 't', 'tid'), $nid);

  // Extract the first row of query.
  $term = db_fetch_object($result);
  return $term;
}

/**
 * Generate a breadcrumb from the taxonomy hierarchy of the tid. This is a taxonomy_breadcrumbs type
 * breadcrumb, and will only be used on the taxonomy term page and on nodes if
 * custom_breadcrumbs_taxonomy_use_hierarchy has been enabled. 
 */
function custom_breadcrumbs_taxonomy_generate_breadcrumb($tid, $vid = NULL, $is_term_page = FALSE, $objs = NULL, $terms = array()) {
  $term = (!empty($terms)) ? array_pop($terms) : NULL;
  $vocabid = (!is_null($vid)) ? $vid : (is_null($term) ? NULL : $term->vid);
  $types = NULL;
  if (module_exists('token')) {
    $objs['taxonomy'] = $term;
    $types = custom_breadcrumbs_token_types($objs);
  }
  $trail = array();
  $trail = _custom_breadcrumbs_taxonomy_home_trail();

  if (!is_null($vocabid)) {
    $vocab_trail = _custom_breadcrumbs_taxonomy_vocabulary_trail($vocabid, $is_term_page, $objs, $types);
    $trail = array_merge($trail, $vocab_trail);
  }

  if (!is_null($tid)) {
    $term_trail = _custom_breadcrumbs_taxonomy_term_trail($tid, $is_term_page, $objs, $types);
    $trail = array_merge($trail, $term_trail);
    // Optionally remove the current TERM from end of breadcrumb trail.
    if (!variable_get('custom_breadcrumbs_taxonomy_show_current_term', TRUE) && (count($trail) > 1)) {
      array_pop($trail);
    }
  }
  return $trail;
}

/**
 * Helper function for when i18ntaxonomy module is not installed.
 */

function _custom_breadcrumbs_taxonomy_tt($string_id, $default, $language = NULL) {
  return function_exists('tt') ? tt($string_id, $default, $language) : $default;
}

/**
 * Helper function to generate the home breadcrumb trail.
 */

function _custom_breadcrumbs_taxonomy_home_trail() {
  $trail = array();
  if (variable_get('custom_breadcrumbs_taxonomy_append_breadcrumb', FALSE)) {
    // Check to see if a breadcrumb has already been defined
    $trail = drupal_get_breadcrumb();
  } 
  else {
    $trail = custom_breadcrumbs_home_crumb();
  }
  return $trail;
}

/**
 * Helper function to generate the vocabulary trail.
 */
function _custom_breadcrumbs_taxonomy_vocabulary_trail($vid, $is_term_page = FALSE, $objs = array(), $types = array('global' => NULL)) {
  // Generate the VOCABULARY breadcrumb.
  $trail = array();
  // Check to see if a vocabulary breadcrumb exists.
  $breadcrumbs = custom_breadcrumbs_load_breadcrumbs('custom_breadcrumbs_taxonomy', 'custom_breadcrumbs_taxonomy_vocabulary', array('vid' => $vid));
  $vocabulary_path = NULL;
  if ($breadcrumb = custom_breadcrumbs_select_breadcrumb($breadcrumbs, $objs)) {
    $vocabulary_path = $breadcrumb->paths;
    if (module_exists('token')) {
      $vocabulary_path = token_replace_multiple($vocabulary_path, $types);
    }
  }
  if ($vocabulary = taxonomy_vocabulary_load($vid)) {
    if ($vocabulary_path != NULL) {
      $trail = array(l(_custom_breadcrumbs_taxonomy_tt("taxonomy:vocabulary:$term->tid:name", $vocabulary->name), $vocabulary_path));
    }
    elseif (variable_get('custom_breadcrumbs_taxonomy_show_vocabulary', FALSE)) {
      $trail = array(_custom_breadcrumbs_taxonomy_tt("taxonomy:vocabulary:$term->tid:name", $vocabulary->name));
    }
  }
  return $trail;
}

/**
 * Helper function to generate the taxonomy term trail.
 */
function _custom_breadcrumbs_taxonomy_term_trail($tid, $is_term_page = FALSE, $objs = array(), $types = array('global' => NULL)) {
  // Generate the TERM breadcrumb.
  $trail = array();
  $parent_terms = array_reverse(taxonomy_get_parents_all($tid));
  foreach ($parent_terms as $parent_term) {
    $breadcrumbs = custom_breadcrumbs_load_breadcrumbs('custom_breadcrumbs_taxonomy', 'custom_breadcrumbs_taxonomy_term', array('tid' => $parent_term->tid));
    $term_path = NULL;
    if ($breadcrumb = custom_breadcrumbs_select_breadcrumb($breadcrumbs, $objs)) {
      $term_path = $breadcrumb->paths;
      if (module_exists('token')) {
        $term_path = token_replace_multiple($term_path, $types);
      }
    }
    if ($term_path == NULL) {
      $term_path = taxonomy_term_path(taxonomy_get_term($parent_term->tid));
    }
    // Do not create links to own self if we are on a taxonomy/term page.
    if ($is_term_page && $parent_term->tid == $tid) {
      $trail[] = _custom_breadcrumbs_taxonomy_tt("taxonomy:term:$parent_term->tid:name", $parent_term->name);
    }
    else {
      $trail[] = l(_custom_breadcrumbs_taxonomy_tt("taxonomy:term:$parent_term->tid:name", $parent_term->name), $term_path);
    }
  }
  return $trail;
}
