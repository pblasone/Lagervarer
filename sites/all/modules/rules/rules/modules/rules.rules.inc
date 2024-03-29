<?php
// $Id: rules.rules.inc,v 1.1.2.23 2008/08/29 12:02:55 fago Exp $
/**
 * @file rules integration for the rules module
 */

define('RULES_DATE_REGEX_LOOSE', '/^(\d{4})-?(\d{2})-?(\d{2})([T\s]?(\d{2}):?(\d{2}):?(\d{2})?)?$/');

/**
 * Implementation of hook_rules_data_type_info().
 * @ingroup rules
 */
function rules_rules_data_type_info() {
  return array(
    'string' => array(
      'label' => t('String'),
      'class' => 'rules_data_type_string',
      'identifiable' => FALSE,
      'eval input' => TRUE,
    ),
    'number' => array(
      'label' => t('Number'),
      'class' => 'rules_data_type_number',
      'identifiable' => FALSE,
      'eval input' => TRUE,
    ),
    'date' => array(
      'label' => t('Date'),
      'class' => 'rules_data_type_date',
      'identifiable' => FALSE,
      'eval input' => TRUE,
    ),
    'boolean' => array(
      'label' => t('Truth value'),
      'class' => 'rules_data_type_boolean',
      'identifiable' => FALSE,
      'eval input' => TRUE,
    ),
    'value' => array(
      'label' => t('A fixed value'),
      'class' => 'rules_data_type',
      'identifiable' => FALSE,
      'hidden' => TRUE,
    ),
  );
}

/**
 * Rules string data type
 * @ingroup rules
 */
class rules_data_type_string extends rules_data_type {

  /**
   * Gets the input form for the data
   * Implement it, if your data is not identifiable.
   */
  function get_default_input_form($info, $value) {
    $info += array('long' => FALSE, 'required' => TRUE, 'description' => '');
    return array(
      '#type' => $info['long'] ? 'textarea' : 'textfield',
      '#title' => $info['label'],
      '#description' => t($info['description']),
      '#required' => $info['required'],
      '#default_value' => $value,
    );
  }
}

/**
 * Rules number data type
 * @ingroup rules
 */
class rules_data_type_number extends rules_data_type_string {

  function check_value($info, $value) {
    return (float)$value;
  }
}

/**
 * Rules date data type
 * @ingroup rules
 */
class rules_data_type_date extends rules_data_type_string {

  function get_default_input_form($info, $value) {
    $info += array('long' => FALSE, 'required' => TRUE, 'description' => '');
    $value = isset($value) ? $value : date('Y-m-d H:i:s', time() + 86400);
    $info['description'] = t($info['description']) .' '. t('Format: %format or other values known by the PHP !strtotime function like "+1 day". ', array('%format' => date('Y-m-d H:i:s', time() + 86400), '!strtotime' => l('strtotime()', 'http://php.net/strtotime')));
    $info['description'] .= '<br />'. t('You may also enter a timestamp in GMT. E.g. use !code together with the PHP input evalutor to specify a date one day after the evaluation time. ', array('!code' => '<pre>'. check_plain('<?php echo time() + 86400 * 1; ?>'). '</pre>'));
    return array(
      '#type' => $info['long'] ? 'textarea' : 'textfield',
      '#title' => $info['label'],
      '#description' => $info['description'],
      '#required' => $info['required'],
      '#default_value' => $value,
    );
  }

  function check_value($info, $value) {
    if (is_numeric($value)) {
      $value = gmdate('Y-m-d H:i:s', $value);
    }
    else if (is_string($value)) {
      $value = gmdate('Y-m-d H:i:s', strtotime($value));
    }
    if (is_string($value) && preg_match(RULES_DATE_REGEX_LOOSE, $value)) {
      return $value;
    }
    rules_log(t('The argument %label is no valid date.', array('%label' => $info['label'])));
  }
}


/**
 * Rules boolean data type
 * @ingroup rules
 */
class rules_data_type_boolean extends rules_data_type_string {

  function check_value($info, $value) {
    return (boolean)$value;
  }

  function get_default_input_form($info, $value) {
    $info += array('long' => FALSE, 'required' => TRUE, 'description' => '');
    $info['description'] = t($info['description']) .' '. t('Just enter 1 for TRUE, 0 for FALSE or make use of an input evaluator.');
    return array(
      '#type' => $info['long'] ? 'textarea' : 'textfield',
      '#title' => $info['label'],
      '#description' => $info['description'],
      '#required' => $info['required'],
      '#default_value' => $value,
    );
  }
}


/**
 * Implementation of hook_rules_condition_info().
 * @ingroup rules
 */
function rules_rules_condition_info() {
  return array(
    'rules_condition_text_compare' => array(
      'label' => t('Textual comparison'),
      'arguments' => array(
        'text1' => array('label' => t('Text 1'), 'type' => 'string'),
        'text2' => array('label' => t('Text 2'), 'type' => 'string'),
      ),
      'help' => 'TRUE is returned, if both texts are equal.',
      'module' => 'Rules',
    ),
    'rules_condition_number_compare' => array(
      'label' => t('Numeric comparison'),
      'arguments' => array(
        'number1' => array('label' => t('Number 1'), 'type' => 'number'),
        'number2' => array('label' => t('Number 2'), 'type' => 'number'),
      ),
      'help' => 'Select greater than, less than or equal to.',
      'module' => 'Rules',
    ),
    'rules_condition_check_boolean' => array(
      'label' => t('Check a truth value'),
      'arguments' => array(
        'boolean' => array('type' => 'boolean', 'label' => t('Truth value')),
      ),
      'module' => 'Rules',
    ),
  );
}


/**
 * Condition Implementation: Textual comparison.
 */
function rules_condition_text_compare($text1, $text2, $settings) {
  return $settings['regex'] ? ereg($text2, $text1) : $text1 == $text2;
}

/**
 * Condition implementation: Numeric comparison.
 */
function rules_condition_number_compare($number1, $number2, $settings) {
  switch ($settings['operation']) {
    case 'greater':
      return $number1 > $number2;
    case 'equal':
      return $number1 == $number2;
    case 'less':
      return $number1 < $number2;
  }
}

/**
 * Condition implementation: Boolean check.
 */
function rules_condition_check_boolean($boolean) {
  return (bool)$boolean;
}




/**
 * Implementation of hook_rules_action_info().
 * @ingroup rules
 */
function rules_rules_action_info() {
  $items = array();
  // Add actions for all rule sets.
  foreach (rules_get_configured_items('rule_sets') as $name => $set_info) {
    $items[ 'rules_action_invoke_set_'. $name ] = $set_info + array(
      'module' => 'Rule Sets',
      'base' => 'rules_action_invoke_set',
      'set' => $name,
    );
  }
  // Add drupal 6 core actions.
  $core_actions = actions_list();
  foreach ($core_actions as $name => $core_action_info) {
    $type_map = rules_gather_data('rules_action_type_map');
    $items[ 'rules_core_'. $name ] = rules_core_action_transform_info($name, $core_action_info, $type_map);
  }
  array_filter($items);

  // Add an action to add a new variable for all not identifiable data types
  foreach (rules_get_data_types() as $name => $type_info) {
    if (isset($type_info['identifiable']) && !$type_info['identifiable'] && (!isset($type_info['hidden']) || !$type_info['hidden'])) {
      $items[ 'rules_add_var_'. $name ] = array(
        'module' => 'Rules',
        'base' => 'rules_action_save_variable',
        'label' => t('Add a new @type variable', array('@type' => strtolower($type_info['label']))),
        'arguments' => array(
          'var_name' => array('type' => 'value', 'default value' => $name),
          $name => array('type' => $name, 'label' => $type_info['label']),
        ),
        'new variables' => array(
          $name => array(
            'type' => $name,
            'label' => t('Added @type', array('@type' => $type_info['label'])),
          ),
        ),
      );
    }
  }
  // Add an action to save savable data types intelligently
  foreach (rules_get_data_types() as $name => $type_info) {
    if (isset($type_info['savable']) && $type_info['savable'] && (!isset($type_info['hidden']) || !$type_info['hidden'])) {
      $items[ 'rules_save_var_'. $name ] = array(
        'module' => isset($type_info['module']) ? $type_info['module'] : 'Rules',
        'base' => 'rules_action_save_variable',
        'label' => t('Save @type', array('@type' => $type_info['label'])),
        'arguments' => array(
          'var_name' => array('type' => 'value', 'default value' => $name),
          $name => array('type' => $name, 'label' => $type_info['label']),
        ),
      );
    }
  }
  return $items;
}

/**
 * Base action implementation for invoking all rule sets
 */
function rules_action_invoke_set() {
  $args = func_get_args();
  $state = array_pop($args);
  $element = array_pop($args);
  $settings = array_pop($args);

  array_unshift($args, $element['#info']['set']);
  call_user_func_array('rules_invoke_rule_set', $args);
}


/**
 * Base action implementation for adding a new variable and saving variables
 */
function rules_action_save_variable($name, $variable) {
  return array($name => $variable);
}



/**
 * Transforms action info written for core to the rules format.
 * If the action should not be used by the rules modules, an empty array is returned.
 */
function rules_core_action_transform_info($name, $core_action_info, $type_map) {

  if (isset($core_action_info['rules_ignore']) && $core_action_info['rules_ignore']) {
    return array();
  }
  $info = $type_map[$core_action_info['type']];
  // If there is no entry in the type map, so we don't use this action.
  if (!isset($info)) {
    return array();
  }
  // Make sure there is a object.
  $info += array('arguments' => array('object' => array('type' => 'value', 'default value' => NULL)));

  $info['label'] = $core_action_info['description'];

  // Special handling of labels for node actions
  // For consistency with the rule naming convention of calling a node "content".
  if ($core_action_info['type'] == 'node') {
    $info['label'] = str_replace('post', t('content'), $info['label']);
  }

  $info['base'] = 'rules_core_action_execute';
  $info['action_name'] = $name;
  $info['configurable'] = $core_action_info['configurable'];
  return $info;
}

/**
 * Action implementation: Execute a core action
 */
function rules_core_action_execute(&$object, $settings, $element, &$state) {
  $info = rules_get_element_info($element);

  $info['action_name']($object, $settings);

  if (isset($settings['auto_save']) && $settings['auto_save']) {
    $argument_name = array_shift(array_keys($info['arguments']));
    return array($argument_name => $object);
  }
}

/**
 * Action form implementation: Get the core actions form
 */
function rules_core_action_execute_form($settings, &$form, &$form_state) {
  $element = $form_state['element'];
  $info = rules_get_element_info($element);
  if ($info['configurable']) {
    $function = $info['action_name']. '_form';
    $form['settings'] = $function($settings);
  }
  if ($info['arguments']) {
    $arg = array_shift($info['arguments']);
    $data_type = rules_get_data_types($arg['type']);

    if (isset($data_type['savable']) && $data_type['savable']) {
      $settings += array('auto_save' => TRUE);
      $form['settings']['auto_save'] = array(
        '#type' => 'checkbox',
        '#title' => t('Permanently apply changes'),
        '#description' => t('If checked, changes to the argument are saved automatically.'),
        '#default_value' => $settings['auto_save'],
        '#weight' => -10,
      );
    }
  }
}

/**
 * Action form submit implementation: Apply the core actions form submit
 */
function rules_core_action_execute_submit(&$settings, &$form, &$form_state) {
  $element = $form_state['element'];
  $info = rules_get_element_info($element);
  if ($info['configurable']) {
    $function = $info['action_name']. '_submit';
    $settings = $function($form['settings'], $form_state) + $settings;
  }
}


/**
 * Implementation of hook_rules_action_type_map().
 *
 * Maps core action types to rules action info. This provides mappings for all
 * action types used in core.
 * @ingroup rules
 */
function rules_rules_action_type_map() {
  return array(
    'node' => array(
      'module' => 'Node',
      'arguments' => array(
        'node' => array(
          'label' => t('Content'),
          'type' => 'node',
        ),
      ),
    ),
    'comment' => array(
      'module' => 'Comment',
      'arguments' => array(
        'comment' => array(
          'label' => t('Comment'),
          'type' => 'comment',
         ),
      ),
    ),
    'user' => array(
      'module' => 'User',
      'arguments' => array(
        'user' => array(
          'label' => t('User'),
          'type' => 'user',
        ),
      ),
    ),
    'system' => array(
      'module' => 'System',
    ),
  );
}
