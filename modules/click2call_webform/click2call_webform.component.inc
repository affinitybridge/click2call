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
  // Get server object to retriev default voice.
  $server = voiptwilio_voipserver_info();

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
      'caller_number' => click2call_webform_get_caller_number_default(),
      'voice_id' => $server->getDefaultVoice()->getVoiceId(),
      'phone_1_component' => '',
      'phone_2_component' => '',
      'script_method' => '',
      'script_name' => '',
      'text_1_component' => '',
      'text_2_component' => '',
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
  $phone_number_types = module_invoke_all('click2call_webform_get_number_types');
  foreach ($webform_components as $sibling_component) {
    if (in_array($sibling_component['type'], $phone_number_types)) {
      $phone_options[$sibling_component['cid']] = $sibling_component['name'];
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

  $form['extra']['click2call_config']['caller_number'] = array(
    '#title'         => t('Caller Number'),
    '#type'          => 'select',
    '#options'       => click2call_webform_get_caller_number_options(),
    '#default_value' => $component['extra']['caller_number'],
    '#description'   => t('The number chosen here will be used to place the initial call to your supporter.'),
    '#required'      => TRUE,
    '#parents' => array('extra', 'caller_number'),
  );

  // Retrieve list of available voices from voipdrupal.
  $server = voiptwilio_voipserver_info();
  $voices = VoipVoice::getVoices(NULL, NULL, NULL, $server->getId());
  $form['extra']['click2call_config']['voice_id'] = array(
    '#title' => t('Default voice'),
    '#type' => 'select',
    '#options' =>  voipcall_get_voices_as_list($voices),
    '#default_value' => $component['extra']['voice_id'],
    '#description' => t('Voice to be used in automatic text-to-speech generation.'),
    '#required'      => TRUE,
    '#parents' => array('extra', 'voice_id'),
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

  $form['extra']['click2call_config']['script_method'] = array(
    '#title'         => t('Script Method'),
    '#type'          => 'radios',
    '#options'       => array(
      'automatic'    => t('Run the script automatically when the webform page holding this component is loaded.'),
      'button'       => t('Allow the user to run the script by clicking a button on the webform'),
      'post_submit'  => t('Do not render any widgets for this component and run the script on webform submit'),
    ),
    '#default_value' => $component['extra']['script_method'],
    '#description'   => t('Please select how the script will be run.'),
    '#required'      => TRUE,
    '#parents' => array('extra', 'script_method'),
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

  $form['extra']['click2call_config']['text_1_component'] = array(
    '#title'         => t('Text 1 Component'),
    '#type'          => 'select',
    '#options'       => $text_options,
    '#default_value' => $component['extra']['text_1_component'],
    '#description'   => t('Please select a component on this webform to use as Text 1.'),
    '#required'      => FALSE,
    '#parents' => array('extra', 'text_1_component'),
  );

  $form['extra']['click2call_config']['text_2_component'] = array(
    '#title'         => t('Text 2 Component'),
    '#type'          => 'select',
    '#options'       => $text_options,
    '#default_value' => $component['extra']['text_2_component'],
    '#description'   => t('Please select a component on this webform to use as Text 2.'),
    '#required'      => FALSE,
    '#parents' => array('extra', 'text_2_component'),
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
    'script_method' => $component['extra']['script_method'],
    'extra' => $component['extra'],
    'submission' => $submission,
  );

//  $call_form_build_id = $call_form['#build_id'];
//  $component['extra']['stash'] = $call_form_build_id;
  $element = array(
    '#type' => 'hidden',
    //    '#title' => $filter ? NULL : $component['name'],
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    //'#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#theme_wrappers' => array('webform_element'),
    //    '#translatable' => array('title'),
    '#translatable' => array('title', 'description'),
    '#default_value' => $default_value,
  );

  if (isset($value) && $value && isset($value[0]) && $value[0]) {
    // Set default value but attempt to resolve with a formatted label
    $element['#suffix'] = 'Call ID: ' . $value[0];
    if (!empty($component['extra']['phone_2_component'])) {
      $phone_2_cid = $component['extra']['phone_2_component'];
      if (!empty($submission->data[$phone_2_cid])) {
        if ($node = node_load($submission->nid)) {
          $phone_2_component = $node->webform['components'][$phone_2_cid];
          $labels = module_invoke_all(
            'click2call_webform_get_label',
            $phone_2_component,
            $submission->data[$phone_2_cid]
          );
          if ($label = array_shift($labels)) {
            $element['#suffix'] = t(
              'Called the office of !contact_label',
              array('!contact_label' => $label)
            );
          }
        }
      }
    }
  }
  else {
    if ('post_submit' == $component['extra']['script_method']) {
      $element['#title_display'] = 'none';
    }
    else {
      $call_form = drupal_get_form('click2call_call_form', 'click2call_webform', $delta, $params);
      $element['#suffix'] = drupal_render($call_form);
    }
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
 * Transform value, which will be a cid, to call log output
 */
function _webform_click2call_values_to_string($value) {
  $call_data = db_select('voipcall', 'vc')

    ->fields('vc')
    ->condition('cid' , $value[0])
    ->execute()
    ->fetchAssoc();
  $output = '';
  if (is_array($call_data)) {
    $output = '<div class="click2call-output">';
    foreach ($call_data as $label => $data) {
      switch ($label) {
        case 'script':
        case 'uid':
        case 'caller_name':
        case 'dest_name':
        case 'max_session_duration':
        case 'answered_by':
        case 'direction':
        case 'source':
        case 'check_machine':
        case 'call_channel':
        case 'call_network':
        case 'call_origin':
        case 'text_status':
        case 'created':
        case 'changed':
          break;
        case 'cid':
          $output .= "<div><strong>" . t('Internal ID') . "</strong>: " . $data . "</div>";
          break;
        case 'call_id':
          $output .= "<div><strong>" . t('VOIP Provider ID') . "</strong>: " . $data . "</div>";
          break;
        case 'duration':
          $output .= "<div><strong>" . t('Duration') . "</strong>: " . $data . " seconds</div>";
          break;
        case 'caller_number':
        case 'dest_number':
          $output .= "<div><strong>" . ucwords(str_replace('_', ' ', $label)) . "</strong>: " . ltrim(preg_replace('/\D/', '', $data), '1') . "</div>";
          break;
        case 'start_time':
        case 'end_time':
          $output .= "<div><strong>" . ucwords(str_replace('_', ' ', $label)) . "</strong>: " . date('F jS, Y \a\t g:ia (e)', $data) . "</div>";
          break;
        case 'error_message':
          $data = empty($data) ? 'none' : $data;
          $output .= "<div><strong>" . t('Error Message') . "</strong>: " . $data . "</div>";
          break;
        default:
          $output .= "<div><strong>" . ucwords(str_replace('_', ' ', $label)) . "</strong>: " . $data . "</div>";
          break;
      }
    }
    $output .= "</div>";
  }
  return $output;
}

/*
 * Provides default value for component's caller number setting.
 */
function click2call_webform_get_caller_number_default() {
  return variable_get('click2call_webform_default_number', variable_get('voipcall_cid_number', ''));
}

/*
 * Provides available options for component's caller number setting.
 */
function click2call_webform_get_caller_number_options() {
  $available_numbers = variable_get('click2call_webform_available_numbers', variable_get('voipcall_cid_number', ''));

  static $option_cache = array();

  // Keep each processed option block in an array indexed by the MD5 hash of
  // the option text.
  $md5 = md5($available_numbers);

  // Check if this option block has been previously processed.
  if (!isset($option_cache[$md5])) {
    $options = array();
    $rows = array_filter(explode("\n", trim($available_numbers)));
    $group = NULL;
    foreach ($rows as $option) {
      $option = trim($option);
      // Split on pipe if possible.
      if (preg_match('/^([^|]+)\|(.*)$/', $option, $matches)) {
        $key = $matches[1];
        $value = $matches[2];
        $options[$key] = $value;
      }
      else {
        $options[$option] = $option;
      }
    }
    // Add option to cache.
    $option_cache[$md5] = $options;
  }
  // Return our options from the option_cache array.
  return $option_cache[$md5];
}
