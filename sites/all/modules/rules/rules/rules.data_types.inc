<?php
// $Id: rules.data_types.inc,v 1.1.2.15 2008/09/24 16:49:13 fago Exp $

/**
 * @file Defines the data type class
 */

/**
 * Gets the right data object for the given argument information
 * @param $info
 *   Info about the data type.
 * @param $data
 *   Optional data to set. Warning: Only use this, when loosing a reference is ok.
 */
function rules_get_data_object($info, $data = NULL) {
  $class = 'rules_data_type';
  if (($data_type = rules_get_data_types($info['type'])) && class_exists($data_type['class'])) {
    $class = $data_type['class'];
  }
  $object = new $class;
  $object->construct($info['type'], isset($data_type) ? $data_type : array());
  if (isset($data)) {
    $object->init($data);
  }
  return $object;
}


/**
 * Base data type class, from which special data types can be derived.
 * @ingroup rules
 */
class rules_data_type {

  var $_data;
  var $_info;
  var $type;

  /**
   * Constructor
   * @param $type
   *   The data type of this object
   * @param $info
   *   If given, the info about this data type. If it's not given, it will be
   *   retrieved automatically.
   */
  function construct($type, $info = NULL) {
    $this->type = $type;

    if (!isset($info)) {
      $info = rules_get_data_types($type);
    }
    $this->_info = isset($info) ? $info : array();
  }

  /**
   * Inititate the data
   * @param $data If available, the actual argument.
   */
  function init(&$data) {
    $this->_data = &$data;
  }

  /**
   * Replaces the data with the new one
   */
  function update(&$data) {
    $this->_data = &$data;
  }

  /**
   * Gets the data
   */
  function &get() {
    return $this->_data;
  }

  /**
   * Returns whether this data is savable
   */
  function is_savable() {
    $info = $this->get_info();
    return isset($info['savable']) && $info['savable'];
  }

  /**
   * Returns whether this data is identifiable
   */
  function is_identifiable() {
    $info = $this->get_info();
    return isset($info['identifiable']) && $info['identifiable'];
  }

  /**
   * Returns whether the input evaluator should be used for this data
   */
  function eval_input() {
    $info = $this->get_info();
    return isset($info['eval input']) && $info['eval input'];
  }

  /**
   * Gets the information about this data type.
   */
  function get_info() {
    return $this->_info + array(
      'savable' => FALSE,
      'identifiable' => TRUE,
      'eval input' => FALSE,
    );
  }






  /**
   * Makes changes to the data permanent.
   * Implement it, if your data type is savable.
   */
  function save() {

  }

  /**
   * Loads the data identified with an identifier as returned by get_identifier().
   * Return FALSE if loading the data failed.
   *
   * Implement it, if your data type is identifiable.
   */
  function load($identifier) {

  }

  /**
   * Gets the identifier of this data, can be of every php data type - even an array.
   * Implement it, if your data type is identifiable.
   */
  function get_identifier() {

  }

  /**
   * Gets an input form for creating an instance of your data type.
   * Implement it, if your data is not identifiable.
   */
  function get_default_input_form($variable_info, $value) {

  }

  /**
   * Checks the value of your data type. E.g. the number data type
   * uses this to make sure the value is a number.
   *
   * It's only used, if your data type is not identifiable.
   */
  function check_value($variable_info, $value) {
    return $value;
  }
}


