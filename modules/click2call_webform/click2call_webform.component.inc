<?php

/**
 * @file
 *   Click2Call Webform module Click2Call component.
 */

/**
 * Specify the default properties of a component.
 *
 * @return
 *   An array defining the default structure of a component.
 */
// Define component and its basic capabilities

function _webform_defaults_click2call() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'title_display' => 0,
      'description' => '',
      'private' => FALSE,
      'phone_1_component' => '',
      'phone_2_component' => '',
      'script_name' => '',
      'text_component' => '',
      // Quack like a select component.
      'aslist' => 0,
      'multiple' => 1,
      'optrand' => 0,

      'other_option' => 0,
    ),
  );
}


/**
 * Generate the form for editing a component.
 *
 * Create a set of form elements to be displayed on the form for editing this
 * component. Use care naming the form items, as this correlates directly to the
 * database schema. The component "Name" and "Description" fields are added to
 * every component type and are not necessary to specify here (although they
 * may be overridden if desired).
 *
 * @param $component
 *   A Webform component array.
 * @return
 *   An array of form items to be displayed on the edit component page.
 */
function _webform_edit_click2call($component) {
  $form = array();

  $node = node_load($component['nid']);
  $webform_components = array();
  if (isset($node->webform['components'])) {
    $webform_components = $node->webform['components'];
  }
  $options = array('' => t('- Select -'));
  $phone_options = $options;
  foreach ($webform_components as $sibling_component) {
    switch ($sibling_component['type']) {
      case 'represent':
      case 'phone':
        $phone_options[$sibling_component['cid']] = $sibling_component['name'];
        break;
    }
  }
  $text_options = $options;
  foreach ($webform_components as $sibling_component) {
    switch ($sibling_component['type']) {
      case 'markup':
        $text_options[$sibling_component['cid']] = $sibling_component['name'];
        break;
    }
  }

  $form['extra']['click2call_config'] = array(
    '#title'         => t('Click2Call Configuration'),
    '#type'          => 'fieldset',
//    '#parents' => array('extra', 'click2call_config'),
  );

  $form['extra']['click2call_config']['script_name'] = array(
    '#title'         => t('Script Name'),
    '#type'          => 'select',
    '#options'       => click2call_get_script_options(),
    '#default_value' => $component['extra']['script_name'],
    '#description'   => t('Please select the script to run for this Click2Call component.'),
    '#required'      => TRUE,
    '#parents' => array('extra', 'script_name'),
  );

  $form['extra']['click2call_config']['phone_1_component'] = array(
    '#title'         => t('Phone Number 1 Component'),
    '#type'          => 'select',
    '#options'       => $phone_options,
    '#default_value' => $component['extra']['phone_1_component'],
    '#description'   => t('Please select a component on this webform to use as Phone Number 1.'),
    '#required'      => TRUE,
    '#parents' => array('extra', 'phone_1_component'),
  );

  $form['extra']['click2call_config']['phone_2_component'] = array(
    '#title'         => t('Phone Number 2 Component'),
    '#type'          => 'select',
    '#options'       => $phone_options,
    '#default_value' => $component['extra']['phone_2_component'],
    '#description'   => t('Please select a component on this webform to use as Phone Number 2.'),
    '#required'      => FALSE,
    '#parents' => array('extra', 'phone_2_component'),
  );

  $form['extra']['click2call_config']['text_component'] = array(
    '#title'         => t('Text Component'),
    '#type'          => 'select',
    '#options'       => $text_options,
    '#default_value' => $component['extra']['text_component'],
    '#description'   => t('Please select a component on this webform to use as Text.'),
    '#required'      => FALSE,
    '#parents' => array('extra', 'text_component'),
  );

  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_click2call($component, $value = NULL, $filter = TRUE, $submission = NULL) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $default_value = $filter ? webform_replace_tokens($component['value'], $node) : $component['value'];
  if (isset($value[0])) {
    $default_value = $value[0];
  }

  $delta = $component['nid'] . '-' . $component['cid'];

  $params = array(
    'script_name' => $component['extra']['script_name'],
    'extra' => $component['extra'],
    'submission' => $submission,
  );

//  $call_form_build_id = $call_form['#build_id'];
//  $component['extra']['stash'] = $call_form_build_id;
  $element = array(
    '#type' => 'hidden',
    //    '#title' => $filter ? NULL : $component['name'],
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    //    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    //    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#theme_wrappers' => array('webform_element'),
    //    '#translatable' => array('title'),
    '#translatable' => array('title', 'description'),
    '#default_value' => $default_value,
  );
  if (isset($value) && $value && isset($value[0]) && $value[0]) {
    $element['#suffix'] = 'Call ID: ' . $value[0];
  }
  else {
    $call_form = drupal_get_form('click2call_call_form', 'click2call_webform', $delta, $params);
    $element['#suffix'] = drupal_render($call_form);
  }

  /*
  // Handle disabling.
  if ($component['extra']['disabled']) {
    if ($filter) {
      $element['#attributes']['readonly'] = 'readonly';
    }
    else {
      $element['#disabled'] = TRUE;
    }
  }
  // Set default value.
  if (isset($value) && !empty($value)) {
    // Set the value as an array.
    $element['#default_value'] = array();
    foreach ((array) $value as $key => $option_value) {
      if ($option_value) {
        $element['#default_value'][] = $option_value;
      }
    }
  } else {
    $element['#attributes']['checked'] = 'checked';
  }
  */

  return $element;
}

/**
 * Implements _webform_analysis_component().
 */
 /*
function _webform_analysis_represent($component, $sids = array()) {
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
      ->fields('wsd', array('data'))
      ->condition('nid', $component['nid'])
      ->condition('cid', $component['cid']);

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }

  $nonblanks = 0;
  $submissions = 0;

  // This could possibly be extended to count submissions by
  // country using the postal_code_validation API.
  $result = $query->execute();
  foreach ($result as $data) {
    if (drupal_strlen(trim($data['data'])) > 0) {
      $nonblanks++;
    }
    $submissions++;
  }

  $rows[0] = array(t('Left Blank'), ($submissions - $nonblanks));
  $rows[1] = array(t('User entered value'), $nonblanks);
  return $rows;
}*/

/**
 * Implements _webform_csv_headers_component().
 */
 /*
function _webform_csv_headers_represent($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2][] = t('@name Email', array('@name' => $component['name']));
  return $header;
}
*/

/**
 * Implements _webform_csv_data_component().
 */
 /*
function _webform_csv_data_represent($component, $export_options, $value) {
  return _webform_represent_values_to_string($value);
}
*/

/**
 * Implements _webform_table_component().
 */
/*
function _webform_table_represent($component, $value) {
  return _webform_represent_values_to_string($value);
}
*/

/*
 * We do not need to define:
 * - _webform_attachments_component
 * - _webform_delete_component
 * - _webform_help_component
 * - _webform_submit_component
 */
 /*
function _webform_submit_click2call($component, $value) {
  dsm('well hopefully here we finally are');
  dpm('component');
  dpm($component);
  dsm('value');
  dpm($value);
}
*/

/**
 * Implements _webform_display_component().
 */
function _webform_display_click2call($component, $value, $format = 'html', $submission = array()) {
  $element = array(
    '#title' => $component['name'],
    '#markup' => isset($value[0]) ? _webform_click2call_values_to_string($value) : NULL,
    '#weight' => $component['weight'],
    '#format' => $format,
//    '#theme' => 'webform_display_hidden',
    '#theme_wrappers' => $format == 'text' ? array('webform_element_text') : array('webform_element'),
    '#translatable' => array('title'),
  );

  return $element;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_click2call($component, $value) {
  return _webform_click2call_values_to_string($value);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_click2call($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2][] = t('@name Call ID', array('@name' => $component['name']));
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_click2call($component, $export_options, $value) {
  return isset($value[0]) ? $value[0] : '';
}


/**
 * Creates string from array of Represent values.
 */
function _webform_click2call_values_to_string($value) {
  return voipcall_call_page($value[0]);
}
