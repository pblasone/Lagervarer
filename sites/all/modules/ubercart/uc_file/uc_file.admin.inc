<?php
// $Id: uc_file.admin.inc,v 1.1.2.6 2009/04/02 21:01:45 islandusurper Exp $

/**
 * @file
 * File administration menu items.
 *
 */

// Form step values.
define('UC_FILE_FORM_FILES' , NULL);
define('UC_FILE_FORM_ACTION', 1   );

/**
 * Form builder for file products admin
 */
function uc_file_admin_files_form($form_state) {

  switch ($form_state['storage']['step']) {

    case UC_FILE_FORM_FILES:

      // Refresh our file list before display.
      uc_file_refresh();

      return array(
        '#theme'  => 'uc_file_admin_files_form_show',
        '#validate' => array('uc_file_admin_files_form_show_validate'),
        '#submit' => array('uc_file_admin_files_form_show_submit'),
      ) + uc_file_admin_files_form_show_files($form_state);

    case UC_FILE_FORM_ACTION:

      return array(
        '#validate' => array('uc_file_admin_files_form_action_validate'),
        '#submit'   => array('uc_file_admin_files_form_action_submit'),
      ) + uc_file_admin_files_form_action($form_state);

  }
}

function uc_file_admin_files_form_show_files($form_state) {

  $form['#tree'] = TRUE;

  $form['#header'] = array(
    array(),
    array('data' => t('File'     ), 'field' => 'f.filename', 'sort' => 'asc'),
    array('data' => t('Product'  ), 'field' => 'n.title'                    ),
    array('data' => t('SKU'), 'field' => 'fp.model'                   )
  );

  // Create pager.
  $query = pager_query(
    "SELECT n.nid, f.filename, n.title, fp.model, f.fid, pf.pfid FROM {uc_files} as f ".
      "LEFT JOIN {uc_file_products} as fp ON (f.fid = fp.fid) ".
      "LEFT JOIN {uc_product_features} as pf ON (fp.pfid = pf.pfid) ".
      "LEFT JOIN {node} as n ON (pf.nid = n.nid) ". tablesort_sql($form['#header']),
    UC_FILE_PAGER_SIZE,
    0,
    "SELECT COUNT(*) FROM {uc_files}"
  );

  // Create checkboxes for each file.
  $form['file_select'] = array('#tree' => TRUE);
  while ($file = db_fetch_object($query)) {
    $form['file_select'][$file->fid]['check'] = array('#type' => 'checkbox');
    $form['file_select'][$file->fid]['filename'] = array('#value' => $file->filename);
    $form['file_select'][$file->fid]['title'] = array('#value' => $file->title);
    $form['file_select'][$file->fid]['nid'] = array('#value' => $file->nid);
    $form['file_select'][$file->fid]['model'] = array('#value' => $file->model);
  }

  // Implement a Checkall / Uncheck all deal.
  $check_all = l(t('Check all'), "admin/store/products/files", array('attributes' => array('id' => 'uc_file_select_all'), 'fragment' => 'NULL'));
  $uncheck_all = l(t('Uncheck all'), 'admin/store/products/files', array('attributes' => array('id' => 'uc_file_select_none'), 'fragment' => 'NULL'));
  $form['uc_file_select'] = array(
    '#type' => 'markup',
    '#value' => $check_all .' / '. $uncheck_all,
  );

  $form['uc_file_action'] = array(
    '#type' => 'fieldset',
    '#title' => t('File options'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // Set our default actions.
  $file_actions = array(
    'uc_file_upload' => t('Upload file'),
    'uc_file_delete' => t('Delete file(s)'),
  );

  // Check if any hook_file_action('info', $args) are implemented
  foreach (module_implements('file_action') as $module) {
    $name = $module .'_file_action';
    $result = $name('info', NULL);
    if (is_array($result)) {
      foreach ($result as $key => $action) {
        if ($key != 'uc_file_delete' && $key != 'uc_file_upload') {
          $file_actions[$key] = $action;
        }
      }
    }
  }

  $form['uc_file_action']['action'] = array(
    '#type' => 'select',
    '#title' => t('Action'),
    '#options' => $file_actions,
    '#prefix' => '<div class="duration">',
    '#suffix' => '</div>',
  );

  $form['uc_file_action']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Perform action'),
    '#prefix' => '<div class="duration">',
    '#suffix' => '</div>',
  );

  return $form;
}

function uc_file_admin_files_form_show_validate($form, &$form_state) {

  switch ($form_state['values']['uc_file_action']['action']) {

    case 'uc_file_delete':
     $file_ids = array();
     if (is_array($form_state['values']['file_select'])) {
       foreach ($form_state['values']['file_select'] as $fid => $value) {
         if ($value) {
           $file_ids[] = $fid;
         }
       }
     }

    if (count($file_ids) == 0) {
      form_set_error('', t('You must select at least one file to delete.'));
    }

    break;
  }

}

function uc_file_admin_files_form_show_submit($form, &$form_state) {

  // Increment the form step.
  $form_state['storage']['step'] = UC_FILE_FORM_ACTION;
  $form_state['rebuild'] = TRUE;

}

/**
 * Implementation of theme_form($form)
 */
function theme_uc_file_admin_files_form_show($form) {
  $output = '';

  $header = $form['#header'];

  // Format the file table.
  $rows = array();
  foreach ($form['file_select'] as $fid => $value) {

    // We only want numeric fid's
    if (!is_numeric($fid)) {
      continue;
    }

    $filename = drupal_render($form['file_select'][$fid]['filename']);

    // Make directories bold + italic (or whatever's in the CSS).
    $class = is_dir(uc_file_qualify_file($filename)) ? 'uc-file-directory-view' : '';

    $rows[] = array(
      array('data' => drupal_render($form['file_select'][$fid]['check'])),
      array('data' => $filename, 'class' => $class),
      array('data' => l(drupal_render($form['file_select'][$fid]['title']), 'node/'. drupal_render($form['file_select'][$fid]['nid']))),
      array('data' => drupal_render($form['file_select'][$fid]['model']))
    );
  }

  if (count($rows) == 0) {
    $rows[] = array(
      array(
        'data' => 'No file downloads are available.',
        'colspan' => 4,
      ),
    );
  }

  // Render everything.
  $output .= '<p>'. t('File downloads can be attached to any Ubercart product as a product feature. For security reasons the <a href="!download_url">file downloads directory</a> is separated from the Drupal <a href="!file_url">file system</a>. Here are the list of files (and their associated Ubercart products) that can be used for file downloads.', array('!download_url' => url('admin/store/settings/products/edit/features', array('query' => 'destination=admin/store/products/files')), '!file_url' => url('admin/settings/file-system'))) .'</p>';
  $output .= drupal_render($form['uc_file_action']);
  $output .= drupal_render($form['uc_file_select']);
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, UC_FILE_PAGER_SIZE, 0);
  $output .= drupal_render($form);

  return $output;
}

function uc_file_admin_files_form_action($form_state) {

  // Perform File Action (Upload, Delete, hooked in actions)
  $file_ids = array();
  foreach ((array)$form_state['values']['file_select'] as $fid => $value) {
    $value = $value['check'];
    if ($value) {
      $file_ids[] = $fid;
    }
  }

  $form['file_ids'] = array('#type' => 'value', '#value' => $file_ids);
  $form['action'] = array('#type' => 'value', '#value' => $form_state['values']['uc_file_action']['action']);

  $file_ids = _uc_file_sort_names(_uc_file_get_dir_file_ids($file_ids, FALSE));

  switch ($form_state['values']['uc_file_action']['action']) {

    case 'uc_file_delete':
      $affected_list = _uc_file_build_js_file_display($file_ids);

      $has_directory = FALSE;
      foreach ($file_ids as $file_id) {

        // Gather a list of user-selected filenames.
        $file = uc_file_get_by_id($file_id);
        $filename = $file->filename;
        $file_list[] = (substr($filename, -1) == "/") ? $filename .' ('. t('directory') .')' : $filename;

        // Determine if there are any directories in this list.
        $path = uc_file_qualify_file($filename);
        if (is_dir($path)) {
          $has_directory = TRUE;
        }
      }

      // Base files/dirs the user selected.
      $form['selected_files'] = array(
        '#type' => 'markup',
        '#value' => theme_item_list($file_list, NULL, 'ul', array('class' => 'selected-file-name')),
      );

      $form = confirm_form(
        $form, t('Delete file(s)'),
        'admin/store/products/files',
        t('Deleting a file will remove all its associated file downloads and product features. Removing a directory will remove any files it contains and their associated file downloads and product features.'),
        t('Delete affected files'), t('Cancel')
      );

      // Don't even show the recursion checkbox unless we have any directories.
      if ($has_directory && $affected_list[TRUE] !== FALSE ) {
        $form['recurse_directories'] = array(
          '#type' => 'checkbox',
          '#title' => t('Delete selected directories and their sub directories'),
        );

        // Default to FALSE... although we have the JS behavior to update with the state
        // of the checkbox on load, this should improve the experience of users who
        // don't have JS enabled over not defaulting to any info.
        $form['affected_files'] = array(
          '#type' => 'markup',
          '#value' => theme_item_list($affected_list[FALSE], 'Affected file(s)', 'ul', array('class' => 'affected-file-name')),
        );
      }

      break;

    case 'uc_file_upload': //Upload file

      // Calculate the max size of uploaded files, in bytes.
      $max_bytes = trim(ini_get('post_max_size'));
      switch (strtolower($max_bytes{strlen($max_bytes)-1})) {
        case 'g':
          $max_bytes *= 1024;
        case 'm':
          $max_bytes *= 1024;
        case 'k':
          $max_bytes *= 1024;
      }

      // Gather list of directories under the sleected one(s). '/' is always available.
      $directories = array('' => '/');
      $files = db_query("SELECT * FROM {uc_files}");
      while ($file = db_fetch_object($files)) {
        if (is_dir(variable_get('uc_file_base_dir', NULL) ."/". $file->filename)) {
          $directories[$file->filename] = $file->filename;
        }
      }

      $form['upload_dir'] = array(
        '#type' => 'select',
        '#title' => t('Directory'),
        '#description' => t('The directory to upload the file to. The default directory is the root of the file downloads directory.'),
        '#options' => $directories,
      );
      $form['upload'] = array(
        '#type' => 'file',
        '#title' => t('File'),
        '#description' => t('The maximum file size that can be uploaded is %size bytes. You will need to use a different method to upload the file to the directory (e.g. (S)FTP, SCP) if your file exceeds this size.<br />The module will automatically detect files you upload using an alternate method.', array('%size' => number_format($max_bytes))),
      );

      $form['#attributes']['class'] .= 'foo';
      $form = confirm_form(
        $form, t('Upload file'),
        'admin/store/products/files',
        '',
        t('Upload file'), t('Cancel')
      );

      // Must add this after confirm_form, as it runs over $form['#attributes'].
      // Issue logged at d#319723
      $form['#attributes']['enctype'] = 'multipart/form-data';

      break;

    default:

      // This action isn't handled by us, so check if any hook_file_action('form', $args) are implemented.
      foreach (module_implements('file_action') as $module) {
        $name = $module .'_file_action';
        $result = $name('form', array('action' => $form_state['values']['uc_file_action']['action'], 'file_ids' => $file_ids));
        $form = (is_array($result)) ? array_merge($form, $result) : $form;
      }

      break;
  }

  return $form;
}

function uc_file_admin_files_form_action_validate($form, &$form_state) {

  switch ($form_state['values']['action']) {

    case 'uc_file_upload':

      // Upload the file and get its object.
      if ($temp_file = file_save_upload('upload', array())) {

        // Check if any hook_file_action('upload_validate', $args) are implemented.
        foreach (module_implements('file_action') as $module) {
          $name = $module .'_file_action';
          $result = $name('upload_validate', array('file_object' => $temp_file, 'form_id' => $form_id, 'form_state' => $form_state));
        }

        // Save the uploaded file for later processing.
        $form_state['storage']['temp_file'] = $temp_file;
      }
      else {
        form_set_error('', t('An error occurred while uploading the file'));
      }

      break;

    default:

      // This action isn't handled by us, so check if any hook_file_action('validate', $args) are implemented
      foreach (module_implements('file_action') as $module) {
        $name = $module .'_file_action';
        $result = $name('validate', array('form_id' => $form_id, 'form_state' => $form_state));
      }

      break;
  }
}

function uc_file_admin_files_form_action_submit($form, &$form_state) {

  switch ($form_state['values']['action']) {

    case 'uc_file_delete':

      // File deletion status.
      $status = TRUE;

      // Delete the selected file(s), with recursion if selected.
      $status = uc_file_remove_by_id($form_state['values']['file_ids'], $form_state['values']['recurse_directories']) && $status;

      if ($status) {
        drupal_set_message(t('The selected file(s) have been deleted.'));
      }
      else {
        drupal_set_message(t('One or more files could not be deleted.'));
      }

      break;

    case 'uc_file_upload':

      // Build the destination location. We start with the base directory, then add any
      // directory which was explicitly selected.
      $dir = variable_get('uc_file_base_dir', NULL) .'/';
      $dir = (is_null($form_state['values']['upload_dir'])) ? $dir : $dir . $form_state['values']['upload_dir'];
      if (is_dir($dir)) {

        // Retrieve our uploaded file.
        $file_object = $form_state['storage']['temp_file'];

        // Copy the file to its final location.
        if (copy($file_object->filepath, $dir .'/'. $file_object->filename)) {

          // Check if any hook_file_action('upload', $args) are implemented
          foreach (module_implements('file_action') as $module) {
            $name = $module .'_file_action';
            $result = $name('upload', array('file_object' => $file_object, 'form_id' => $form_id, 'form_state' => $form_state));
          }

          // Update the file list
          uc_file_refresh();

          drupal_set_message(t('The file %file has been uploaded to %dir', array('%file' => $file_object->filename, '%dir' => $dir)));
        }
        else {
          drupal_set_message(t('An error occurred while copying the file to %dir', array('%dir' => $dir)));
        }
      }
      else {
        drupal_set_message(t('Can not move file to %dir', array('%dir' => $dir)));
      }

      break;

    default:

      // This action isn't handled by us, so check if any hook_file_action('submit', $args) are implemented
      foreach (module_implements('file_action') as $module) {
        $name = $module .'_file_action';
        $result = $name('submit', array('form_id' => $form_id, 'form_state' => $form_state));
      }
      break;
  }

  // Return to the original form state.
  $form_state['rebuild'] = FALSE;
  drupal_goto('admin/store/products/files');
}

function _uc_file_display_arrays_equivalent($recur, $no_recur) {

  // Different sizes.
  if (count($recur) != count($no_recur)) {
    return FALSE;
  }

  // Check the elements.
  for ($i = 0; $i < count($recur); $i++) {
    if ($recur[$i] != $no_recur[$i]) {
      return FALSE;
    }
  }

  return TRUE;
}

function _uc_file_build_js_file_display($file_ids) {

  // Gather the files if recursion is selected. Get 'em all ready to be punched into
  // the file list.
  $recursion_file_ids = _uc_file_sort_names(_uc_file_get_dir_file_ids($file_ids, TRUE));
  foreach ($recursion_file_ids as $file_id) {
    $file = uc_file_get_by_id($file_id);
    $recursion[] = '<li>'. $file->filename .'</li>';
  }

  // Gather the files if recursion isn't selected. Get 'em all ready to be punched into
  // the file list.
  $no_recursion_file_ids = $file_ids;
  foreach ($no_recursion_file_ids as $file_id) {
    $file = uc_file_get_by_id($file_id);
    $no_recursion[] = '<li>'. $file->filename .'</li>';
  }

  // We'll disable the recursion checkbox if they're equal.
  $equivalent = _uc_file_display_arrays_equivalent($recursion_file_ids, $no_recursion_file_ids);

  // The list to show if the recursion checkbox is $key.
  $result[TRUE] = $equivalent ? FALSE : $recursion;
  $result[FALSE] = $no_recursion;

  // Set up some JS to dynamically update the list based on the
  // recursion checkbox state.
  drupal_add_js('uc_file_list[false] = '. drupal_to_js('<li>'. implode('</li><li>', $no_recursion) .'</li>') .';', 'inline');
  drupal_add_js('uc_file_list[true] = '. drupal_to_js('<li>'. implode('</li><li>', $recursion) .'</li>') .';', 'inline');

  return $result;
}
