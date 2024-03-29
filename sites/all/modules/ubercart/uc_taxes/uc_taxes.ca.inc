<?php
// $Id: uc_taxes.ca.inc,v 1.1.2.10 2009/03/17 19:19:27 islandusurper Exp $

/**
 * @file
 * This file contains the Conditional Action hooks and functions necessary to
 * make the tax related entity, conditions, events, and actions work.
 */

/******************************************************************************
 * Conditional Action Hooks                                                   *
 ******************************************************************************/

/**
 * Implementation of hook_ca_entity().
 */
function uc_taxes_ca_entity() {
  $entities['tax'] = array(
    '#title' => t('Tax rule'),
    '#type' => 'object',
  );
  return $entities;
}

/**
 * Implementation of hook_ca_trigger().
 *
 * Register an event for each tax rule in {uc_taxes}.
 */
function uc_taxes_ca_trigger() {
  $triggers = array();

  $triggers['calculate_taxes'] = array(
    '#title' => t('Calculate taxes'),
    '#category' => t('Taxes'),
    '#arguments' => array(
      'order' => array('#entity' => 'uc_order', '#title' => t('Order')),
      'tax' => array('#entity' => 'tax', '#title' => t('Tax rule')),
      'user' => array('#entity' => 'user', '#title' => t('User account')),
    ),
  );

  return $triggers;
}

/**
 * Implementation of hook_action_info().
 */
function uc_taxes_ca_action() {
  $actions = array();

  // Loop through all the defined tax rates.
  foreach (uc_taxes_rate_load() as $rate) {
    $actions['uc_taxes_action_apply_tax_'. $rate->id] = array(
      '#title' => t('Charge a tax'),
      '#category' => t('Taxes'),
      '#callback' => 'uc_taxes_action_apply_tax',
      '#arguments' => array(
        'order' => array('#entity' => 'uc_order', '#title' => t('Order')),
        'tax' => array('#entity' => 'tax', '#title' => t('Tax')),
      ),
    );
  }

  return $actions;
}

/**
 * Workflow-ng action callback to calculate a tax.
 *
 * @param $order
 *   The order object being considered.
 * @param $tax
 *   The tax rule calculating the amount.
 * @return
 *   The line item array representing the amount of tax.
 */
function uc_taxes_action_apply_tax($order, $tax) {
  return uc_taxes_apply_tax($order, $tax);
}

/**
 * Implementation of hook_ca_predicate().
 *
 * Create a predicate for each event corresponding to a tax rule.
 */
function uc_taxes_ca_predicate() {
  $predicates = array();

  // Loop through all the defined tax rates.
  foreach (uc_taxes_rate_load() as $rate) {
    $predicates['uc_taxes_'. $rate->id] = array(
      '#title' => $rate->name,
      '#class' => 'taxes',
      '#trigger' => 'calculate_taxes',
      '#status' => 1,
      '#actions' => array(
        array(
          '#name' => 'uc_taxes_action_apply_tax_'. $rate->id,
          '#title' => t('Apply !name', array('!name' => $rate->name)),
          '#argument_map' => array(
            'order' => 'uc_order',
            'tax' => 'tax',
          ),
        ),
      ),
    );
  }

  return $predicates;
}
