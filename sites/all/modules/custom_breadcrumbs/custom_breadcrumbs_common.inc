<?php
// $Id: custom_breadcrumbs_common.inc,v 1.1.2.1 2009/07/20 12:40:22 mgn Exp $

/**
 * @file
 * Common helper functions used by custom breadcrumbs submodules.
 */

/**
 * Helper function to determine if two paths match, allowing for wildcards and aliases.
 * @param $curpath and $breadcrumb_path are the two paths to compare
 * @return
 *    TRUE (1) if the paths match, FALSE (0) otherwise.
 */
function _custom_breadcrumbs_match_path($curpath, $breadcrumb_path) {
  $path = drupal_get_path_alias($curpath);
  // Compare with the internal and path alias (if any).
  $page_match = drupal_match_path(strtolower($path), strtolower($breadcrumb_path));
  if ($path != $curpath) {
   $page_match = $page_match || drupal_match_path(strtolower($curpath), strtolower($breadcrumb_path));
  }
  return $page_match;
}

/**
 * Helper function to construct the view path replacing wildcards with arguments.
 * @param $display
 *   The view $dipslay variable.
 * @param $viewsargs
 *   The $view->args variable.
 * @return $viewpath
 *   The complete path to the view.
 */
function _custom_breadcrumbs_construct_view_path($display, $viewargs) {
  $bits = explode('/', $display->display_options['path']);
  foreach ($bits as $pos => $bit) {
    if ($bit == '%' && !empty($viewargs)) {
      $bits[$pos] = array_shift($viewargs);
    }
  }
  if (!empty($viewargs)) {
    // Add any additional arguments to end of path.
    $bits = array_merge($bits, $viewargs);
  }
  $viewpath = implode('/', $bits);
  return $viewpath;
}

/**
 *  Helper function to specify allowed views displays for breadcrumb placement.
 * @param $display
 *   The view $dipslay variable.
 * @return
 *    TRUE if the display should be considered for a custom breadcrumb, FALSE otherwise.
 */
function _custom_breadcrumbs_allowed_display($display) {

  if (drupal_substr($display->id, 0, 4)  == 'page' || drupal_substr($display->id, 0, 8)  == 'calendar') {
    if (!(isset($display->handler->view->is_attachment) && $display->handler->view->is_attachment)) {
      if (isset($display->display_options['path']) ) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
