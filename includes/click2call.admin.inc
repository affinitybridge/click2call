<?php

function click2call_admin_settings_form($form, &$form_state) {
  $form = _click2call_settings_fields();
  return system_settings_form($form);
}

function _click2call_settings_fields($type='admin_form', $settings=NULL) {
  $options=array();
  $prefix='';
  /*Set stored values, they can come from admin form or from field form*/
  if ($type=='field_form') {
    $click2call_number=!empty($settings['click2call_number']) ? $settings['click2call_number'] : variable_get('click2call_number', array('type'));
  }
  elseif ($type=='admin_form') {
    $click2call_number=variable_get('click2call_number', array('type'));
  }

  $description = t('Select the options that Click2Call will make available for users to determine the number to be called');

  if (module_exists('voipnumber')) {
    //This options are available only if voipnumber is enabled
    $options['default'] = t("Present the default number associated with the user");
    $options['voipnumber'] = t("Present a list with the different numbers associated with the user");
  }
  else {
    $description .= ' ' . t('Enable !voipnumber module to get more options.', array('!voipnumber' => l('voipnumber', 'http://drupal.org/project/voipnumber')));
  }

  $options['type']=t('Ask user to type the number in');

  $form['click2call_number'] = array(
    '#type' => 'checkboxes',
    '#prefix' => $prefix,
    '#title' => t('Default number to be called'),
    '#options' => $options,
    '#default_value' => $click2call_number,
    '#description' => $description,
  );

  return $form;
}
